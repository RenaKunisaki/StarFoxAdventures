/*---------------------------------------------------------------------------*
  Project:  Dolphin OS
  File:     dvd.h

  Copyright 1998- 2004 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dvd.h,v $
  Revision 1.10  2006/09/28 04:50:20  shirakae
  Added DVDEntrynumIsDir

  Revision 1.9  2006/06/07 02:21:23  ooizumi
  Deleted DVDCheckDisk.

  Revision 1.8  2006/06/05 00:21:27  ooizumi
  Enabled DVDCheckDiskAsync for RVL0 target.

  Revision 1.7  2006/05/17 22:16:20  ooizumi
  Added DVD_STATE_NO_INPUT state.

  Revision 1.6  2006/04/24 05:23:16  ooizumi
  Changed a header file for WIN32 target.

  Revision 1.5  2006/03/20 10:24:33  ooizumi
  Deleted APIs for hardware streaming.

  Revision 1.4  2006/03/12 03:03:29  craig
  Added GC and RVL magic number locations to disk id structure.

  Revision 1.3  2006/03/10 22:44:58  ooizumi
  Changed DVDGetStartAddr's return value u32 to u64.

  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Ported from dolphin source tree.

    
    54    2004/03/31 20:27 Hashida
    Updated copyright info.
    
    53    2003/09/10 14:37 Ooshima
    Modified DVDDirEntry and DVDRewindDir to remove DVDETH definition.
    
    51    2003/04/22 11:29 Hashida
    Modified DVDGetFileInfoStatus to refer fileinfo's member instead of
    just casting to DVDCommandBlock structure.
    
    50    2003/02/28 5:30p Ooshima
    Changed DVDCommandBlock and DVDDirEntry structure for DVDETH again
    
    49    2003/01/09 10:34a Ooshima
    Changed DVDCommandBlock and DVDDirEntry structure for DVDETH
    
    47    2002/08/22 12:00 Shiki
    Clean up.

    46    2002/08/20 21:38 Shiki
    Added DVDSetAutoFatalMessaging().

    45    2002/08/05 16:19 Shiki
    Added const keywords to relevant function prototypes.

    44    2002/02/07 16:04 Hashida
    Changed the return value of DVDGenerateDiskID from void to DVDDiskID*

    43    2002/01/02 5:00p Hashida
    Added DVDCompareDiskID and DVDGenerateDiskID.

    42    2001/08/10 3:04p Hashida
    Added DVDFastOpenDir

    41    2001/07/06 1:20a Hashida
    Resurrected function version of DVDGetFileInfoStatus().

    40    2001/06/29 11:43a Hashida
    Added DVDCheckDisk().

    39    2001/06/26 9:18a Hashida
    Modified so that DVDGetFileInfoStatus is properly prototype'd for MAC
    build.

    38    2001/06/15 10:35p Hashida
    Deleted obsolete result codes.

    37    2001/06/15 9:01p Hashida
    Changed DVDGetFileInfoStatus to a macro.

    36    2001/05/29 11:52a Hashida
    Removed duplicated DVDPause and DVDResume prototypes.

    35    2001/05/10 2:24p Hashida
    Added DVDDumpWaitingQueue.

    34    2001/05/01 2:49p Hashida
    Removed DVDGetStreamStartAddr & DVDGetStreamLength.

    33    2001/04/19 5:40p Hashida
    Added DVDGetCurrentDiskID

    32    2001/03/22 6:10p Hashida
    Added DVDCancelAll and DVDCancelAllAsync.
    Moved DVDCommandBlock structure in DVDFileInfo to the first member.

    31    2001/03/02 5:48p Hashida
    Modified error code handling (fatal error, internal retry and retry)

    30    2001/03/02 11:35a Hashida
    Added DVDCancel, DVDPause, DVDResume, DVDGetTransferredSize.

    29    2001/02/08 3:46p Hashida
    Added DVDPause and DVDResume prototypes.

    28    2001/01/08 7:02p Hashida
    Added DVDSet/GetUserData macros.

    27    2000/11/13 5:30p Hashida
    Added streamingBufSize member to diskid.


    26    2000/10/27 2:24p Hashida
    Added a check code so that audio streaming command will not be issued
    while a track is auto finishing.

    25    2000/10/10 11:28a Hashida
    Fixed a problem that MAC target won't be built.

    24    2000/10/09 6:53p Hashida
    Added prioritized queuing.

    23    2000/09/29 9:51p Hirose
    Replaced "MAC" flag by better alternatives

    22    2000/09/26 3:38p Hashida
    Added DVDChangeDisk (synchronous version)

    21    2000/09/25 3:32p Hashida
    Changed API names that don't follow the convention (sync, async)

    20    2000/07/21 11:15a Hashida
    Added DVD_STATE_PAUSING.

    19    2000/07/20 12:32p Hashida
    Removed DVDConvertEntrynumToPath

    18    2000/07/20 9:46a Hashida
    Added DVDFastOpen(), DVDConvertPathToEntrynum and
    DVDConvertEntrynumToPath.

    17    2000/07/07 6:00p Dante
    PC Compatibility

    16    2000/05/14 8:06p Hashida
    Added DVDGetCurrentDir

    15    2000/05/10 5:24p Hashida
    Removed relocate FST function.

    14    2000/04/13 5:21p Hashida
    Added DVD_RESULT_* definitions for streaming commands.
    Added more streaming APIs' prototypes.

    13    2000/04/12 3:58p Hashida
    Added audio streaming stuffs.

    12    2000/04/12 1:31p Hashida
    Misc. change.

    11    2000/03/29 2:29p Hashida
    Added DVDGetDirEntryName and DVDDirEntryIsDir

    10    2000/03/28 3:42p Hashida
    Added directory accessing functions to MAC build.

    9     2000/03/13 2:46p Hashida
    Added DVDGetFSTLocation.

    8     2000/03/10 2:38p Hashida
    Added DVDRelocateFST

    7     2000/03/02 3:00p Ryan
    Added DVDChangeDir to MAC build

    6     2000/03/01 1:13p Hashida
    Renamed DVDDirEntry->filename to name.

    5     2000/02/29 7:57p Hashida
    Added directory access support.

    4     2000/02/02 6:28p Tian
    Halved size of DVD_FST_MAX_SIZE

    3     2000/01/13 12:14p Hashida
    Added directory access functions for future use.

    21    1999/10/06 10:33a Hashida
    Changed the filename length limit to 200 chars.

    20    1999/09/09 5:55p Hashida
    Removed warnings.

    19    1999/08/26 5:37p Hashida
    Added DVDSetAutoInvalidation

    18    1999/07/21 11:52p Hashida
    Added DVD_MIN_TRANSFER_SIZE

    17    1999/07/21 9:47p Hashida
    Added DVDSeek(), DVDChangeDir(), DVDChangeDisk()

    16    1999/07/20 2:37p Hashida
    Changed DVDFileInfo structure for EPPC
    Removed DVD_RESULT_GOOD

    15    1999/07/19 11:45p Hashida
    Changed the value for fatal error to -1

    14    1999/07/19 4:29p Hashida
    Added DVDCommandBlockStatus and DVDDriveStatus.

    13    1999/07/19 2:58p Hashida
    Added DVD_FST_MAX_SIZE, DVD_STATE*, DVD_RESULT* and DVDCommandBlock.

    12    1999/07/09 9:59p Hashida
    Added DVDDiskID structure

    9     1999/06/12 12:11a Hashida
    Added DVDSetRoot()

    8     1999/06/11 3:04p Shiki
    Added DVDGetFileInfoStatus().

    7     1999/06/04 3:14p Hashida
    Fixed small mistakes

    6     1999/05/28 1:34p Shiki
    Fixed DVDReadAll() and DVDReadAllAsync() macros.

    5     1999/05/11 4:42p Shiki
    Refreshed include tree.

    3     1999/04/30 4:22p Shiki
    Added DVDInit() declaration.

    2     1999/04/30 4:02p Shiki
    Supported DVDReadAsync().

    1     1999/04/29 2:51p Shiki

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __DVD_H__
#define __DVD_H__

#ifdef  MACOS
#include <OSUtils.h>
#include <Gestalt.h>
#include <Files.h>
#endif  // MACOS

#ifdef WIN32
#include <windows.h>
#endif

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MACOS
// The following is a restriction that only exists on Mac emulator
#define DVD_PATH_MAX    200  // The number of bytes in an absolute pathname
                             // Excluding terminating '\0'.
#endif

typedef struct DVDDiskID DVDDiskID;

struct DVDDiskID
{
    char      gameName[4];
    char      company[2];
    u8        diskNumber;
    u8        gameVersion;
    u8        streaming;
    u8        streamingBufSize; // 0 = default
    u8        padding[14];      // 0's are stored
    u32       rvlMagic;         // Revolution disk magic number
    u32       gcMagic;          // GC magic number is here
};

typedef struct DVDCommandBlock DVDCommandBlock;

typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);

struct DVDCommandBlock
{
    DVDCommandBlock* next;
    DVDCommandBlock* prev;
    u32              command;
    s32              state;
    u32              offset;
    u32              length;
    void*            addr;
    u32              currTransferSize;
    u32              transferredSize;
    DVDDiskID*       id;
    DVDCBCallback    callback;
    void*            userData;
};

typedef struct DVDFileInfo  DVDFileInfo;

/*---------------------------------------------------------------------------*
  Name:         DVDCallback

  Description:  DVD callback function

  Arguments:    result:      	If the function that set the callback succeeds,
                            it is the number of bytes transferred. If that
                            function fails, it is -1.

  Returns:      None.
 *---------------------------------------------------------------------------*/
typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);

struct DVDFileInfo
{
#ifndef MACOS
    DVDCommandBlock cb;
#endif

    u32             startAddr;      // Disk address of file
    u32             length;         // File size in bytes

#ifdef MACOS
    DVDFileInfo*    next;
#endif  // MACOS
    DVDCallback     callback;

#ifdef WIN32
    FILE * file;
#endif

#ifdef  MACOS
    IOParam         pb;
#endif  // MACOS

};

typedef struct
{
    u32         entryNum;
    u32         location;
    u32         next;
} DVDDir;

typedef struct
{
    u32         entryNum;
    BOOL        isDir;
    char*       name;
} DVDDirEntry;


// FST maximum size
#ifndef MINNOW
#define DVD_FST_MAX_SIZE          0x00100000
#else // MINNOW
#define DVD_FST_MAX_SIZE          0x00080000
#endif

#define DVD_MIN_TRANSFER_SIZE     32

#define DVD_STATE_FATAL_ERROR     -1
#define DVD_STATE_END             0
#define DVD_STATE_BUSY            1
#define DVD_STATE_WAITING         2
#define DVD_STATE_COVER_CLOSED    3         // Internal state. never returns.
#define DVD_STATE_NO_DISK         4
#define DVD_STATE_COVER_OPEN      5
#define DVD_STATE_WRONG_DISK      6
#define DVD_STATE_MOTOR_STOPPED   7
#define DVD_STATE_PAUSING         8
#define DVD_STATE_IGNORED         9
#define DVD_STATE_CANCELED        10
#define DVD_STATE_RETRY           11
#define DVD_STATE_NO_INPUT        12

#define DVD_FILEINFO_READY        0
#define DVD_FILEINFO_BUSY         1

#define DVD_RESULT_GOOD           0
#define DVD_RESULT_FATAL_ERROR    -1
#define DVD_RESULT_IGNORED        -2
#define DVD_RESULT_CANCELED       -3

#define DVD_AIS_SUCCESS           0x0

#define DVDGetStartAddr(fileInfo)                               \
        ((u64)((u64)(fileInfo)->startAddr) << 2)

#define DVDGetLength(fileInfo)                                  \
        ((fileInfo)->length)

void DVDInit             ( void );

BOOL DVDOpen             ( const char* fileName, DVDFileInfo* fileInfo );
BOOL DVDFastOpen         ( s32 entrynum, DVDFileInfo* fileInfo );

#ifndef MACOS
s32  DVDReadPrio         ( DVDFileInfo* fileInfo, void* addr, s32 length,
                           s32 offset, s32 prio );

BOOL DVDReadAsyncPrio    ( DVDFileInfo* fileInfo, void* addr, s32 length,
                           s32 offset,
                           DVDCallback callback, s32 prio );

#define DVDRead(fileInfo, addr, length, offset)                 \
                         DVDReadPrio((fileInfo), (addr), (length), (offset), 2)
#define DVDReadAsync(fileInfo, addr, length, offset, callback)  \
                         DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)
#else
s32  DVDRead             ( DVDFileInfo* fileInfo, void* addr, s32 length,
                           s32 offset );

BOOL DVDReadAsync        ( DVDFileInfo* fileInfo, void* addr, s32 length,
                           s32 offset,
                           DVDCallback callback );
#endif

BOOL DVDClose            ( DVDFileInfo* fileInfo );

#ifndef MACOS
BOOL DVDGetCurrentDir    ( char* path, u32 maxlen );
#endif

BOOL DVDChangeDir        ( const char* dirName );

#ifdef MACOS
BOOL DVDMovePickup       ( DVDFileInfo* fileInfo,
                           s32 offset,
                           DVDCallback callback );
#else  // EPPC
s32  DVDSeekPrio         ( DVDFileInfo* fileInfo,
                           s32 offset, s32 prio );

BOOL DVDSeekAsyncPrio    ( DVDFileInfo* fileInfo,
                           s32 offset,
                           DVDCallback callback, s32 prio );

#define DVDSeek(fileInfo, offset)                               \
                         DVDSeekPrio((fileInfo), (offset), 2)
#define DVDSeekAsync(fileInfo, offset, callback)                \
                         DVDSeekAsyncPrio((fileInfo), (offset), (callback), 2)

s32  DVDChangeDisk       ( DVDCommandBlock* block, DVDDiskID* id );

BOOL DVDChangeDiskAsync  ( DVDCommandBlock* block, DVDDiskID* id,
                           DVDCBCallback callback );
#endif

s32  DVDGetCommandBlockStatus
                         ( const DVDCommandBlock* block );

s32  DVDGetFileInfoStatus( const DVDFileInfo* fileInfo );

#define DVDGetFileInfoStatus(fileinfo)                          \
                         DVDGetCommandBlockStatus(&(fileinfo)->cb)

s32  DVDGetDriveStatus   ( void );

BOOL DVDSetAutoInvalidation
                         ( BOOL autoInval );

void* DVDGetFSTLocation  ( void );

#ifdef MACOS
void DVDSetRoot          ( const char *rootPath );
#endif
#ifdef WIN32
void DVDSetRoot          ( const char *rootPath );
#endif

s32  DVDConvertPathToEntrynum
                         ( const char* pathPtr );

BOOL DVDEntrynumIsDir    ( s32 entrynum );

// Directory access functions
BOOL DVDOpenDir          ( const char* dirName, DVDDir* dir );
BOOL DVDFastOpenDir      ( s32 entrynum, DVDDir* dir );
BOOL DVDReadDir          ( DVDDir* dir, DVDDirEntry* dirent );
BOOL DVDCloseDir         ( DVDDir* dir );

void DVDPause            ( void );
void DVDResume           ( void );

/*---------------------------------------------------------------------------*
  Name:         DVDTellDir

  Description:  Returns the current location associated with the directory

  Arguments:    dir:         	Pre-opened DVDDir* structure

  Returns:      current location
 *---------------------------------------------------------------------------*/
#define DVDTellDir(dir)             ((dir)->location)

/*---------------------------------------------------------------------------*
  Name:         DVDSeekDir

  Description:  Sets the position of the next DVDReadDir on the directory

  Arguments:    dir:         	Pre-opened DVDDir* structure
                loc:         	location to set

  Returns:      None
 *---------------------------------------------------------------------------*/
#define DVDSeekDir(dir, loc)        ((dir)->location = loc)

/*---------------------------------------------------------------------------*
  Name:         DVDRewindDir

  Description:  Resets the position of the directory to the beginning

  Arguments:    dir:         	Pre-opened DVDDir* structure

  Returns:      None
 *---------------------------------------------------------------------------*/
void DVDRewindDir(DVDDir* dir);

#define DVDGetDirEntryName(dirent)  ((dirent)->name)
#define DVDDirEntryIsDir(dirent)    ((dirent)->isDir)

/*---------------------------------------------------------------------------*
  Name:         DVDSetUserData

  Description:  Set user data in command block

  Arguments:    block:       	Command block
                data:        	Data to set

  Returns:      None
 *---------------------------------------------------------------------------*/
#define DVDSetUserData(block, data) ((block)->userData = (data))

/*---------------------------------------------------------------------------*
  Name:         DVDGetUserData

  Description:  Get user data in command block

  Arguments:    block:       	Command block

  Returns:      data (void * type)
 *---------------------------------------------------------------------------*/
#define DVDGetUserData(block)       ((block)->userData)

#ifndef EMU

BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDCancel(DVDCommandBlock* block);
BOOL DVDCancelAllAsync(DVDCBCallback callback);
s32 DVDCancelAll(void);

s32 DVDGetTransferredSize(DVDFileInfo* fileinfo);

DVDDiskID* DVDGetCurrentDiskID(void);

void DVDDumpWaitingQueue(void);

BOOL DVDCheckDiskAsync(DVDCommandBlock* block, DVDCBCallback callback);

BOOL DVDCompareDiskID(const DVDDiskID* id1, const DVDDiskID* id2);

DVDDiskID* DVDGenerateDiskID(DVDDiskID* id, const char* game, const char* company,
                             u8 diskNum, u8 version);

BOOL DVDSetAutoFatalMessaging(BOOL enable);

#endif

#ifdef __cplusplus
}
#endif

#endif  // __DVD_H__
