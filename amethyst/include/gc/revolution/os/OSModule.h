/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Module API
  File:     OSModule.h

  Copyright 1999-2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSModule.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    11    2002/09/27 13:59 Shiki
    Added R_DOLPHIN_MRKREF.

    10    2002/09/25 16:46 Shiki
    Added support for fixSize and OSLinkFixed (version 3).

    9     2002/09/02 11:12 Shiki
    Added bssSection field to OSModuleHeader{} using previous padding0
    field.

    8     2002/08/30 9:00 Shiki
    Changed OSNotify*() interface for speeding up the loads and unloads
    under the debugger.

    7     2002/08/05 18:04 Shiki
    Added const keywords to relevant function prototypes.

    6     2001/11/27 21:23 Shiki
    Added OSSearchModule().

    5     2001/09/04 20:32 Shiki
    Added support for align and bssAlign (version 2).

    4     2001/04/02 13:06 Shiki
    Separated OSModuleInfo into OSModuleInfo and OSModuleHeader.

    3     2000/10/31 4:41p Shiki
    Fixed OSUnlink() declaration.

    2     2000/04/13 11:14p Shiki
    Fixed copyright.

    1     2000/04/13 11:13p Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSMODULE_H__
#define __OSMODULE_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_MODULE_VERSION       3
typedef struct OSModuleHeader   OSModuleHeader;

typedef u32                     OSModuleID;
typedef struct OSModuleQueue    OSModuleQueue;
typedef struct OSModuleLink     OSModuleLink;
typedef struct OSModuleInfo     OSModuleInfo;
typedef struct OSSectionInfo    OSSectionInfo;
typedef struct OSImportInfo     OSImportInfo;
typedef struct OSRel            OSRel;

struct OSModuleQueue
{
    OSModuleInfo*   head;
    OSModuleInfo*   tail;
};

struct OSModuleLink
{
    OSModuleInfo*   next;
    OSModuleInfo*   prev;
};

struct OSModuleInfo
{
    OSModuleID      id;                 // unique identifier for the module
    OSModuleLink    link;               // doubly linked list of modules
    u32             numSections;        // # of sections
    u32             sectionInfoOffset;  // offset to section info table
    u32             nameOffset;         // offset to module name
    u32             nameSize;           // size of module name
    u32             version;            // version number
};

struct OSModuleHeader
{
    // CAUTION: info must be the 1st member
    OSModuleInfo    info;

    // OS_MODULE_VERSION == 1
    u32 bssSize;            // total size of bss sections in bytes
    u32 relOffset;
    u32 impOffset;
    u32 impSize;            // size in bytes
    u8  prologSection;      // section # for prolog function
    u8  epilogSection;      // section # for epilog function
    u8  unresolvedSection;  // section # for unresolved function
    u8  bssSection;         // section # for bss section (set at run-time)
    u32 prolog;             // prolog function offset
    u32 epilog;             // epilog function offset
    u32 unresolved;         // unresolved function offset

    // OS_MODULE_VERSION == 2
#if (2 <= OS_MODULE_VERSION)
    u32 align;              // module alignment constraint
    u32 bssAlign;           // bss alignment constraint
#endif

    // OS_MODULE_VERSION == 3
#if (3 <= OS_MODULE_VERSION)
    u32 fixSize;
#endif
};

#define OSGetSectionInfo(module)    \
        ((OSSectionInfo*) (((OSModuleInfo*) (module))->sectionInfoOffset))

/*---------------------------------------------------------------------------*
  Name:         OSSectionInfo

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
struct OSSectionInfo
{
    u32             offset;
    u32             size;
};

// OSSectionInfo.offset bit
#define OS_SECTIONINFO_EXEC             0x1
#define OS_SECTIONINFO_OFFSET(offset)   ((offset) & ~0x1)

struct OSImportInfo
{
    OSModuleID      id;                 // external module id
    u32             offset;             // offset to OSRel instructions
};

struct OSRel
{
    u16             offset;             // byte offset from the previous entry
    u8              type;
    u8              section;
    u32             addend;
};

#define R_DOLPHIN_NOP           201     //  C9h current offset += OSRel.offset
#define R_DOLPHIN_SECTION       202     //  CAh current section = OSRel.section
#define R_DOLPHIN_END           203     //  CBh
#define R_DOLPHIN_MRKREF        204     //  CCh

/*---------------------------------------------------------------------------*
    Function Prototypes
 *---------------------------------------------------------------------------*/

void OSSetStringTable( const void* stringTable );
BOOL OSLink          ( OSModuleInfo* newModule, void* bss );
#if (3 <= OS_MODULE_VERSION)
BOOL OSLinkFixed     ( OSModuleInfo* newModule, void* bss );
#endif
BOOL OSUnlink        ( OSModuleInfo* oldModule );

OSModuleInfo* OSSearchModule(void* ptr, u32* section, u32* offset);

// debugger notification
void OSNotifyLink    ( OSModuleInfo* module );
void OSNotifyUnlink  ( OSModuleInfo* module );

#ifdef __cplusplus
}
#endif

#endif  // __OSMODULE_H__
