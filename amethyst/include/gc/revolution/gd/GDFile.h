/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDFile.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDFile.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    3     2002/08/05 19:49 Hirose
    Const type specifier support.
    
    2     2001/09/14 2:13p Carl
    Fixed some name consistency.
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDFile_H__
#define __GDFile_H__

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

// Element of a general-purpose data list.
// This is used to keep track of multiple DL's or multiple patch lists.
typedef struct _GDGList
{
    void *ptr;        // Points to either a DL or an array of GDPatch's
    u32   byteLength; // Length of DL or number of GDPatch's * sizeof(GDPatch)
} GDGList;

typedef struct _GDFileHeader
{
    u32 versionNumber;    // This kind of thing always seems to be useful
    u32 numDLs;           // How many DL's are in this file?
    u32 numPLs;           // How many patch tables are in this file?
    GDGList *DLDescArray; // Pointer (in file it's an offset) to list of DL's
    GDGList *PLDescArray; // Pointer (in file it's an offset) to list of PL's
} GDLFileHeader;

enum { GDFileVersionNumber = 0x11223344 };

/*---------------------------------------------------------------------------*/

#ifndef EPPC
s32 GDWriteDLFile(char *fName, u32 numDLs, u32 numPLs,
                  GDGList *DLDescArray, GDGList *PLDescArray);
#endif

s32 GDReadDLFile(const char *fName, u32 *numDLs, u32 *numPLs,
                 GDGList **DLDescArray, GDGList **PLDescArray);

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GDFile_H__

