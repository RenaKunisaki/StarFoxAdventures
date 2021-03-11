/*---------------------------------------------------------------------------*
  Project:  header for archiver for Revolution dvd
  File:     arc.h

  Copyright (C) 2001-2006 Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: arc.h,v $
  Revision 1.3  2006/09/29 08:08:09  shirakae
  Added ARCEntrynumIsDir

  Revision 1.2  2006/07/04 09:08:46  hiratsu
  Added "const" to arguments.

  Revision 1.1  2006/04/19 10:50:09  hiratsu
  Initial check-in.  Ported from Dolphin.

    
    4     06/04/17 10:10 Hiratsu
    Fixed comments.
    
    3     06/04/14 16:18 Hiratsu
    Added include guard.
    
    2     02/10/07 9:39 Hashida
    C++ support.
    
    1     7/02/01 11:37p Hashida
    Initial revision.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __ARC_H__
#define __ARC_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned int    magic;
    int             fstStart;
    int             fstSize;
    int             fileStart;
    int             reserve[4];
    
} ARCHeader;

#define DARCH_MAGIC         0x55aa382d

typedef struct
{
    void*       archiveStartAddr;
    void*       FSTStart;
    void*       fileStart;
    u32         entryNum;
    char*       FSTStringStart;
    u32         FSTLength;
    u32         currDir;
    
} ARCHandle;

typedef struct
{
    ARCHandle*  handle;
    u32         startOffset;
    u32         length;

} ARCFileInfo;

typedef struct
{
    ARCHandle*  handle;
    u32         entryNum;
    u32         location;
    u32         next;
} ARCDir;

typedef struct 
{
    ARCHandle*  handle;
    u32         entryNum;
    BOOL        isDir;
    char*       name;
} ARCDirEntry;


BOOL  ARCInitHandle(void* arcStart, ARCHandle* handle);
BOOL  ARCOpen(ARCHandle* handle, const char* fileName, ARCFileInfo* af);
BOOL  ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* af);
s32   ARCConvertPathToEntrynum(ARCHandle* handle, const char* pathPtr);
BOOL  ARCEntrynumIsDir( const ARCHandle * handle, s32 entrynum );
void* ARCGetStartAddrInMem(ARCFileInfo* af);
u32   ARCGetStartOffset(ARCFileInfo* af);
u32   ARCGetLength(ARCFileInfo* af);
BOOL  ARCClose(ARCFileInfo* af);

BOOL  ARCChangeDir(ARCHandle* handle, const char* dirName);
BOOL  ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen);

BOOL  ARCOpenDir(ARCHandle* handle, const char* dirName, ARCDir* dir);
BOOL  ARCReadDir(ARCDir* dir, ARCDirEntry* dirent);
BOOL  ARCCloseDir(ARCDir* dir);

/*---------------------------------------------------------------------------*
  Name:         ARCTellDir

  Description:  Returns the current location associated with the directory

  Arguments:    dir         Pre-opened ARCDir* structure

  Returns:      current location
 *---------------------------------------------------------------------------*/
#define ARCTellDir(dir)             ((dir)->location)

/*---------------------------------------------------------------------------*
  Name:         ARCSeekDir

  Description:  Sets the position of the next ARCReadDir on the directory

  Arguments:    dir         Pre-opened ARCDir* structure
                loc         location to set

  Returns:      None
 *---------------------------------------------------------------------------*/
#define ARCSeekDir(dir, loc)        ((dir)->location = loc)

/*---------------------------------------------------------------------------*
  Name:         ARCRewindDir

  Description:  Resets the position of the directory to the beginning

  Arguments:    dir         Pre-opened ARCDir* structure

  Returns:      None
 *---------------------------------------------------------------------------*/
#define ARCRewindDir(dir)           ((dir)->location = (dir)->entryNum + 1)

#define ARCGetDirEntryName(dirent)  ((dirent)->name)
#define ARCDirEntryIsDir(dirent)    ((dirent)->isDir)


#ifdef __cplusplus
}
#endif

#endif  // __ARC_H__
