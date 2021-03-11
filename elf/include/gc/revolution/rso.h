/*---------------------------------------------------------------------------*
  Project:  RSO
  File:     rso.h

  Copyright 2006 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  @version $Id: rso.h,v 1.17 2006/12/18 06:23:39 srd_nisiwaki Exp $

 *---------------------------------------------------------------------------*/

#ifndef __RSO_H__
#define __RSO_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RSO_VERSION       1
//version 0.10
typedef struct RSOObjectHeader   RSOObjectHeader;

typedef u32    RSOHash;
typedef struct RSOObjectList		RSOObjectList;
typedef struct RSOObjectLink		RSOObjectLink;
typedef struct RSOObjectInfo		RSOObjectInfo;
typedef struct RSOSymbolHeader		RSOSymbolHeader;
typedef struct RSOSectionInfo		RSOSectionInfo;
typedef struct RSOExportTable		RSOExportTable;
typedef struct RSOImportTable		RSOImportTable;
typedef struct RSORel			RSORel;
typedef struct RSOExportFuncTable       RSOExportFuncTable;

#define RSO_NAME_ADR(moduleP)    (((RSOObjectHeader *)moduleP)->info.nameOffset)
#define RSO_NAME_SIZE(moduleP)   (((RSOObjectHeader *)moduleP)->info.nameSize)

struct RSOExportFuncTable
{
    const char*  symbol_name;
    u32*  symbol_ptr;
};
    
struct RSOObjectList
{
    RSOObjectInfo*   head;
    RSOObjectInfo*   tail;
};

struct RSOObjectLink
{
    RSOObjectInfo*   next;
    RSOObjectInfo*   prev;
};
    
struct RSOObjectInfo
{
    RSOObjectLink   link;
    u32             numSections;        // # of sections
    u32             sectionInfoOffset;  // Offset to section info table
    u32             nameOffset;         // Offset to module name
    u32             nameSize;           // Size of module name    
    u32             version;            // Version number
};

struct RSOSymbolHeader
{
	u32				tableOffset;		// Offset to symbol table
	u32				tableSize;			// Symbol table size
	u32				stringOffset;		// Offset to symbol name table
};

struct RSOObjectHeader
{
    // CAUTION: Info must be the 1st member
    RSOObjectInfo    info;

    u32 bssSize;            // Total size of bss sections in bytes
    u8  prologSection;
    u8  epilogSection;
    u8  unresolvedSection;
    u8  bssSection;
    u32 prolog;
    u32 epilog;
    u32 unresolved;
    // Data ordering (Be careful when changing)
    // (Deletes the latter half data after linking module)
    // Information necessary for referencing from outside
    // expHeader.tableOffset -> expHeader.stringOffset ->
    // Information necessary for referencing outside
    // externalRelOffset -> impHeader.tableOffset -> impHeader.stringOffset ->
    // Information necessary for internal referencing
    // internalRelOffset

    // Internal referencing (self-contained)
    u32 internalRelOffset;
    u32 internalRelSize;
    
    // External referencing (reference other modules)
    u32 externalRelOffset;
    u32 externalRelSize;
    // Information to be referenced from outside
    RSOSymbolHeader     expHeader;
    // Information referencing outside
    RSOSymbolHeader     impHeader;    
};

#define RSOGetSectionInfo(module)    \
        ((RSOSectionInfo*) (((RSOObjectInfo*) (module))->sectionInfoOffset))

// Fixed level
#define RSO_FL_NON              0
#define RSO_FL_INTERNAL         1       // Up to the internal reference information
#define RSO_FL_EXTERNAL         2       // Up to the information necessary for referencing outside
//#define RSO_FL_EXPORT           3       // Up to the information necessary for referencing from outside

// RSO_FL_EXPORT includes RSO_FL_EXTERNAL and RSO_FL_INTERNAL
// RSO_FL_EXTERNAL includes RSO_FL_INTERNAL

// Size of a single intermediate code export.
#define RSO_FAR_JUMP_SIZE     24

/*---------------------------------------------------------------------------*
  Name:         RSOSectionInfo

  Description:  Section information that corresponds to Elf32_Shdr.

  Members:      offset      Byte offset from the beginning of the file.

                            The OS_SECTIONINFO_EXEC bit is set for a
                            section that contains executable machine
                            instructions.

                            The offset can be zero for unused sections and
                            also for BSS sections. BSS sections have non-
                            zero size but unused sections do not.

                size        Section size in bytes. For BSS sections, the
                            sections occupy no space in the file.

  Note:         Unused sections are required to use same section indices
                as the original ELF file. (for debugging purpose)

 *---------------------------------------------------------------------------*/
struct RSOSectionInfo
{
    u32             offset;
    u32             size;
};

//Use the following numbers for a static rso RSOExportTable.section
#define RSO_EXPSEC_INIT		1
#define RSO_EXPSEC_TEXT		2
#define RSO_EXPSEC_CTORS	3
#define RSO_EXPSEC_DTORS	4
#define RSO_EXPSEC_RODATA	5
#define RSO_EXPSEC_DATA		6
#define RSO_EXPSEC_BSS		7
#define RSO_EXPSEC_SDATA	8
#define RSO_EXPSEC_SDATA2	9
#define RSO_EXPSEC_SDATA0	10
#define RSO_EXPSEC_SBSS		11
#define RSO_EXPSEC_SBSS2	12
#define RSO_EXPSEC_SBSS0	13
#define RSO_EXPSEC_UNKNOWN  0xf1

struct RSOExportTable
{
    u32         strOffset;
    u32         value;
    u32         section;
    RSOHash     hash;
};

struct RSOImportTable
{
    u32         strOffset;
    u32         value;
    u32         relOffset;
};

struct RSORel
{
    u32 offset;
    u32 info;
    u32 addend;
};

//The lower 8 bit is 'type' (relocation type) is RSORel.info
//The upper 24 bit is 'section' (section number of referenced symbol) when internally referenced, and 'importIndex' (index number of import symbol table) when externally referenced
//
//Use the macro below to translate. The operation of RSO_R_SECTION and RSO_R_IMPIDX is exactly the same.
#define RSO_R_SECTION(i)	((i)>>8)
#define RSO_R_IMPIDX(i)		((i)>>8)
#define RSO_R_TYPE(i)		((unsigned char)(i))
#define RSO_R_INFO(s,t)		(((s)<<8)+(unsigned char)(t))

#define R_REVOLUTION_NOP			201     //  C9h current offset += OSRel.offset
#define R_REVOLUTION_SECTION		202     //  CAh current section = OSRel.section
#define R_REVOLUTION_END			203     //  CBh
#define R_REVOLUTION_MRKREF			204     //  CCh

#ifdef __cplusplus
}
#endif

#endif  // __OSMODULE_H__
