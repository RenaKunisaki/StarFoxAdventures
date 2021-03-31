/*---------------------------------------------------------------------------*
  Project:      NAND flash library
  File:         nand.h
  Programmer:   HIRATSU Daisuke

  Copyright (C) 2005-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: nand.h,v $
  Revision 1.53  2007/04/20 08:30:37  hiratsu
  Added NANDSimpleSafeCancel[Async]().

  Revision 1.52  2007/04/09 03:01:13  hiratsu
  Added NANDSimpleSafe* and NANDGet*DirUsage API series.

  Revision 1.51  2007/02/21 11:55:24  hiratsu
  Added new result code: NAND_RESULT_MAXDEPTH.

  Revision 1.50  2006/11/23 07:07:24  hiratsu
  Renamed member variable.

  Revision 1.49  2006/11/20 08:25:04  hiratsu
  Added new member to NANDFileInfo structure in order to detect
  multiple NANDClose[Async]() calls for same NANDFileInfo.

  Revision 1.48  2006/10/23 10:50:44  hiratsu
  Removed SafeCancel APIs becase they are not mature.

  Revision 1.47  2006/10/19 00:50:37  hiratsu
  Prepared NANDSafeCancel[Async]().

  Revision 1.46  2006/10/07 09:29:55  hiratsu
  Added new API, NANDGetAvailableArea[Async].

  Revision 1.45  2006/10/05 13:26:25  hiratsu
  Icon animation speed macros were wrong.  Fixed.

  Revision 1.44  2006/09/15 14:00:22  hiratsu
  Modified comment.

  Revision 1.43  2006/09/14 13:05:23  hiratsu
  Added banner API.

  Revision 1.42  2006/09/05 12:50:47  hiratsu
  Added NAND_FSBLOCK_SIZE macro.

  Revision 1.41  2006/08/22 23:59:24  hiratsu
  Added new members for NANDCheckAsync().

  Revision 1.40  2006/08/12 12:06:51  hiratsu
  Added new member to command block.

  Revision 1.39  2006/08/11 12:03:21  hiratsu
  Revised NANDCheck[Async] interface.

  Revision 1.38  2006/08/11 10:19:23  yasumoto_yoshitaka
  surpress padding warning

  Revision 1.37  2006/08/09 01:56:16  hiratsu
  Added NANDCheck().
  But its implementation is not sufficient.

  Revision 1.36  2006/07/31 13:56:07  hiratsu
  Fixed NANDSafeOpen/Close bug.

  Revision 1.35  2006/07/27 05:49:07  hiratsu
  Removed outdated error code: NAND_RESULT_INIT_FAILED.

  Revision 1.34  2006/07/24 11:43:41  hiratsu
  Added NANDGetHomeDir().

  Revision 1.33  2006/07/03 02:23:10  hiratsu
  Added AUTHENTICATION result code.

  Revision 1.32  2006/06/30 08:52:34  hiratsu
  Added NANDSafeOpen/Close() series.

  Revision 1.31  2006/06/30 04:13:27  hiratsu
  Renamed macro.  NAND_TYPE_DIRECTORY -> NAND_TYPE_DIR

  Revision 1.30  2006/06/27 12:45:16  hiratsu
  Removed NANDCheckInit().

  Revision 1.29  2006/06/26 11:41:35  hiratsu
  Added NANDCheckInit(), and NANDGetType[Async]().

  Revision 1.28  2006/06/17 10:04:14  hiratsu
  Added new error code: NAND_RESULT_ALLOC_FAILED because async function may fail to alloc memory.

  Revision 1.27  2006/06/12 08:14:07  hiratsu
  NAND_MAX_FD -> 15.  Because system use one file descriptor.

  Revision 1.26  2006/06/08 11:09:17  hiratsu
  Removed attribute macro.

  Revision 1.25  2006/06/06 08:45:35  hiratsu
  Removed NANDFinalize().
  New members are added to NANDCommandBlock.

  Revision 1.24  2006/06/01 12:38:25  hiratsu
  Added new member to NANDCommandBlock.

  Revision 1.23  2006/05/25 09:27:28  hiratsu
  Added new member to NANDFileInfo to implement safe file access.

  Revision 1.22  2006/05/15 09:34:28  hiratsu
  Added NANDTellAsync(), NANDGetLengthAsync(), NANDChangeDirAsync().

  Revision 1.21  2006/05/09 09:10:25  hiratsu
  Modified error code.

  Revision 1.20  2006/05/04 11:34:17  hiratsu
  Implemented NANDGet/SetStatusAsync().

  Revision 1.19  2006/05/04 09:09:53  hiratsu
  Renamed API.
  NANDGetSize -> NANDGetLength
  NANDGetOffset -> NANDTell

  Revision 1.18  2006/05/04 04:37:48  hiratsu
  Added new APIs.  NANDSetUserData(), NANDGetUserData().
  Renamed argument. cmdBlk -> block

  Revision 1.17  2006/05/03 08:50:20  hiratsu
  NANDCommandBlock is installed for callback.

  Revision 1.16  2006/04/27 17:58:45  orest
  Added NANDFinalize() api.

  Revision 1.15  2006/04/25 05:14:43  hiratsu
  Added NANDSafeOpen/Close(). But these APIs wiil be changed soon.
  Updated NANDFileInfo for above APIs.

  Revision 1.14  2006/03/20 10:55:35  hiratsu
  Implemented NANDMove[Async] instead of NANDRename[Async].

  Revision 1.13  2006/03/17 04:41:56  hiratsu
  Added NAND_MAX_NAME macro.
  File and directory name length is limited by this value.

  Revision 1.12  2006/03/16 11:42:36  hiratsu
  New error code is added.

  Revision 1.11  2006/03/11 09:45:13  hiratsu
  Changed value of NAND_RESULT_UNKNOWN.

  Revision 1.10  2006/03/02 05:17:39  hiratsu
  Added macro for maximum file descriptor.

  Revision 1.9  2006/02/16 13:03:35  hiratsu
  Refined API names.

  Revision 1.8  2006/02/13 12:38:26  hiratsu
  Some macro definitions are modified in order to match ISFS API.
  Sorted APIs.
  Misc APIs are added, but not implemented.

  Revision 1.7  2006/02/11 08:52:51  hiratsu
  Renamed APIs.

  Revision 1.6  2006/02/08 12:57:47  hiratsu
  Directory APIs are added.  But it is not tested very well.

  Revision 1.5  2006/02/07 06:56:50  hiratsu
  Added directory API.

  Revision 1.4  2006/02/06 00:47:51  hiratsu
  Added Async APIs and seek API.

 *---------------------------------------------------------------------------*/

#ifndef __NAND_H__
#define __NAND_H__


#include <revolution/types.h>


#ifdef __cplusplus
extern "C" {
#endif


#define NAND_RESULT_OK                0 
#define NAND_RESULT_ACCESS         ( -1)
#define NAND_RESULT_ALLOC_FAILED   ( -2)
#define NAND_RESULT_BUSY           ( -3)
#define NAND_RESULT_CORRUPT        ( -4)
#define NAND_RESULT_ECC_CRIT       ( -5)
#define NAND_RESULT_EXISTS         ( -6)
//#define NAND_RESULT_INIT_FAILED    ( -7)
#define NAND_RESULT_INVALID        ( -8)
#define NAND_RESULT_MAXBLOCKS      ( -9)
#define NAND_RESULT_MAXFD          (-10)
#define NAND_RESULT_MAXFILES       (-11)
#define NAND_RESULT_NOEXISTS       (-12)
#define NAND_RESULT_NOTEMPTY       (-13)
#define NAND_RESULT_OPENFD         (-14)
#define NAND_RESULT_AUTHENTICATION (-15)
#define NAND_RESULT_MAXDEPTH       (-16)
#define NAND_RESULT_UNKNOWN        (-64)
#define NAND_RESULT_FATAL_ERROR   (-128)

#define NAND_PERM_OTHER_READ   0x01
#define NAND_PERM_OTHER_WRITE  0x02
#define NAND_PERM_GROUP_READ   0x04
#define NAND_PERM_GROUP_WRITE  0x08
#define NAND_PERM_OWNER_READ   0x10
#define NAND_PERM_OWNER_WRITE  0x20

#define NAND_PERM_OWNER_MASK   (NAND_PERM_OWNER_READ | NAND_PERM_OWNER_WRITE)
#define NAND_PERM_GROUP_MASK   (NAND_PERM_GROUP_READ | NAND_PERM_GROUP_WRITE)
#define NAND_PERM_OTHER_MASK   (NAND_PERM_OTHER_READ | NAND_PERM_OTHER_WRITE)

#define NAND_ACCESS_READ       0x01
#define NAND_ACCESS_WRITE      0x02
#define NAND_ACCESS_RW         (NAND_ACCESS_READ | NAND_ACCESS_WRITE)

#define NAND_SEEK_SET          0
#define NAND_SEEK_CUR          1
#define NAND_SEEK_END          2

#define NAND_MAX_CAPACITY      (512*1024*1024)
#define NAND_MAX_INODES        6143
#define NAND_MAX_PATH          64
#define NAND_MAX_FD            15
#define NAND_MAX_NAME          12        // Not including the NULL character

#define NAND_FSBLOCK_SIZE      (16*1024) // 16KB

#define NAND_TYPE_FILE         0x01
#define NAND_TYPE_DIR          0x02

#define NAND_CHECK_HOME_INSSPACE    0x00000001
#define NAND_CHECK_HOME_INSINODE    0x00000002
#define NAND_CHECK_SYS_INSSPACE     0x00000004
#define NAND_CHECK_SYS_INSINODE     0x00000008



#ifdef  __MWERKS__
#pragma warn_padding off
#endif
typedef struct NANDCommandBlock
{
    void *userData;

    // Do not access below members.
    void *callback;
    void *fileInfo;
    void *bytes;
    void *inodes;
    void *status;
    u32 ownerId;
    u16 groupId;
    u8  nextStage;
    // 1 bytes padding
    u32 attr;
    u32 ownerAcc;
    u32 groupAcc;
    u32 othersAcc;
    u32 num;
    char absPath[NAND_MAX_PATH];
    u32 *length;
    u32 *pos;
    int state;
    void *copyBuf;
    u32 bufLength;
    u8 *type;
    u32 uniqNo;
    u32 reqBlocks;
    u32 reqInodes;
    u32 *answer;
    u32 homeBlocks;
    u32 homeInodes;
    u32 userBlocks;
    u32 userInodes;
    u32 workBlocks;
    u32 workInodes;
    const char **dir;
    BOOL simpleFlag;
} NANDCommandBlock;
#ifdef  __MWERKS__
#pragma warn_padding reset
#endif

typedef void (*NANDCallback)( s32 result, NANDCommandBlock *block );

#ifdef  __MWERKS__
#pragma warn_padding off
#endif
typedef struct NANDFileInfo
{
    // Do not access below members.
    s32  fileDescriptor;
    s32  origFd;
    char origPath[NAND_MAX_PATH];
    char tmpPath[NAND_MAX_PATH];
    u8   accType;
    u8   stage;
    u8   mark;
    // 2 bytes padding
} NANDFileInfo;
#ifdef  __MWERKS__
#pragma warn_padding reset
#endif


typedef struct NANDStatus
{
    u32 ownerId;
    u16 groupId;
    u8  attribute;
    u8  permission;
} NANDStatus;


s32   NANDInit           (void);

//    File access API
s32   NANDCreate         (const char *path, u8 perm, u8 attr);
s32   NANDCreateAsync    (const char *path, u8 perm, u8 attr, NANDCallback cb, NANDCommandBlock *block);
s32   NANDOpen           (const char *path, NANDFileInfo *info, u8 accType);
s32   NANDOpenAsync      (const char *path, NANDFileInfo *info, u8 accType, NANDCallback cb, NANDCommandBlock *block);
s32   NANDClose          (NANDFileInfo *info);
s32   NANDCloseAsync     (NANDFileInfo *info, NANDCallback cb, NANDCommandBlock *block);
s32   NANDRead           (NANDFileInfo *info,       void *buf, u32 length);
s32   NANDReadAsync      (NANDFileInfo *info,       void *buf, u32 length, NANDCallback cb, NANDCommandBlock *block);
s32   NANDWrite          (NANDFileInfo *info, const void *buf, u32 length);
s32   NANDWriteAsync     (NANDFileInfo *info, const void *buf, u32 length, NANDCallback cb, NANDCommandBlock *block);
s32   NANDSeek           (NANDFileInfo *info, s32 offset, s32 whence);
s32   NANDSeekAsync      (NANDFileInfo *info, s32 offset, s32 whence, NANDCallback cb, NANDCommandBlock *block);
s32   NANDTell           (NANDFileInfo *info, u32 *pos);
s32   NANDTellAsync      (NANDFileInfo *info, u32 *pos, NANDCallback cb, NANDCommandBlock *block);
s32   NANDGetLength      (NANDFileInfo *info, u32 *length);
s32   NANDGetLengthAsync (NANDFileInfo *info, u32 *length, NANDCallback cb, NANDCommandBlock *block);
s32   NANDSafeOpen       (const char *path, NANDFileInfo *info, u8 accType, void *buf, u32 length);
s32   NANDSafeOpenAsync  (const char *path, NANDFileInfo *info, u8 accType, void *buf, u32 length, NANDCallback cb, NANDCommandBlock *block);
s32   NANDSafeClose      (NANDFileInfo *info);
s32   NANDSafeCloseAsync (NANDFileInfo *info, NANDCallback cb, NANDCommandBlock *block);

s32   NANDSimpleSafeOpen       (const char *path, NANDFileInfo *info, const u8 accType, void *buf, const u32 length);
s32   NANDSimpleSafeOpenAsync  (const char *path, NANDFileInfo *info, const u8 accType, void *buf, const u32 length, NANDCallback cb, NANDCommandBlock *block);
s32   NANDSimpleSafeClose      (NANDFileInfo *info);
s32   NANDSimpleSafeCloseAsync (NANDFileInfo *info, NANDCallback cb, NANDCommandBlock *block);
s32   NANDSimpleSafeCancel     (NANDFileInfo *info);
s32   NANDSimpleSafeCancelAsync(NANDFileInfo *info, NANDCallback cb, NANDCommandBlock *block);


//    Directory access API
s32   NANDReadDir        (const char *path, char *nameList, u32 *num);
s32   NANDReadDirAsync   (const char *path, char *nameList, u32 *num, NANDCallback cb, NANDCommandBlock *block);
s32   NANDCreateDir      (const char *path, u8 perm, u8 attr);
s32   NANDCreateDirAsync (const char *path, u8 perm, u8 attr, NANDCallback cb, NANDCommandBlock *block);
s32   NANDChangeDir      (const char *path);
s32   NANDChangeDirAsync (const char *path, NANDCallback cb, NANDCommandBlock *block);
s32   NANDGetCurrentDir  (      char  path[NAND_MAX_PATH]);
s32   NANDGetHomeDir     (      char  path[NAND_MAX_PATH]);

//    File or Directory access API
s32   NANDDelete         (const char *path);
s32   NANDDeleteAsync    (const char *path, NANDCallback cb, NANDCommandBlock *block);
s32   NANDMove           (const char *path, const char *destDir);
s32   NANDMoveAsync      (const char *path, const char *destDir, NANDCallback cb, NANDCommandBlock *block);
s32   NANDGetStatus      (const char *path,       NANDStatus *stat);
s32   NANDGetStatusAsync (const char *path,       NANDStatus *stat, NANDCallback cb, NANDCommandBlock *block);
s32   NANDSetStatus      (const char *path, const NANDStatus *stat);
s32   NANDSetStatusAsync (const char *path, const NANDStatus *stat, NANDCallback cb, NANDCommandBlock *block); 
s32   NANDGetType        (const char *path, u8 *type);
s32   NANDGetTypeAsync   (const char *path, u8 *type, NANDCallback cb, NANDCommandBlock *block);

//    Misc APIs.
s32   NANDFreeBlocks     (u32 *bytes, u32 *inodes);
s32   NANDFreeBlocksAsync(u32 *bytes, u32 *inodes, NANDCallback cb, NANDCommandBlock *block);

s32   NANDCheck          (u32 fsBlock, u32 inode, u32 *answer);
s32   NANDCheckAsync     (u32 fsBlock, u32 inode, u32 *answer, NANDCallback cb, NANDCommandBlock *block);
s32   NANDGetAvailableArea     (u32 *fsBlock, u32 *inode);
s32   NANDGetAvailableAreaAsync(u32 *fsBlock, u32 *inode, NANDCallback cb, NANDCommandBlock *block);

void  NANDSetUserData   (      NANDCommandBlock *block, void *data);
void* NANDGetUserData   (const NANDCommandBlock *block);

s32   NANDGetTmpDirUsage      (u32 *fsBlock, u32 *inode);
s32   NANDGetTmpDirUsageAsync (u32 *fsBlock, u32 *inode, NANDCallback cb, NANDCommandBlock *block);
s32   NANDGetHomeDirUsage     (u32 *fsBlock, u32 *inode);
s32   NANDGetHomeDirUsageAsync(u32 *fsBlock, u32 *inode, NANDCallback cb, NANDCommandBlock *block);



/********************** Banner ***********************/

#define NAND_BANNER_TEXTURE_SIZE (192 * 64 * 2)
#define NAND_BANNER_ICON_SIZE    ( 48 * 48 * 2)
#define NAND_BANNER_COMMENT_SIZE 32

#define NAND_BANNER_ICON_ANIM_SPEED_END    0
#define NAND_BANNER_ICON_ANIM_SPEED_FAST   1
#define NAND_BANNER_ICON_ANIM_SPEED_NORMAL 2
#define NAND_BANNER_ICON_ANIM_SPEED_SLOW   3
#define NAND_BANNER_ICON_ANIM_SPEED_MASK   3

#define NAND_BANNER_FLAG_NOTCOPY 0x00000001

#define NAND_BANNER_FLAG_ANIM_BOUNCE 0x00000010
#define NAND_BANNER_FLAG_ANIM_LOOP   0x00000000
#define NAND_BANNER_FLAG_ANIM_MASK   0x00000010

#define NAND_BANNER_SIGNATURE 0x5749424E

#define NANDGetIconSpeed(stat, n)          (((stat)->iconSpeed  >> (2 * (n))) & NAND_BANNER_ICON_ANIM_SPEED_MASK)
#define NANDSetIconSpeed(stat, n, f)       ((stat)->iconSpeed  = (u16) (((stat)->iconSpeed  & ~(NAND_BANNER_ICON_ANIM_SPEED_MASK << (2 * (n)))) | ((f) << (2 * (n)))))

#define NAND_BANNER_SIZE( i ) ( 32 + NAND_BANNER_COMMENT_SIZE * sizeof(u16) * 2 + NAND_BANNER_TEXTURE_SIZE + NAND_BANNER_ICON_SIZE * (i) )

typedef struct
{
    u32 signature;            // Signature: 0x5749424E

    u32 flag;                 // Flags
    u16 iconSpeed;            // Icon animation speed
    u8  reserved[22];         // for 32B align

    u16 comment[2][NAND_BANNER_COMMENT_SIZE];    // title and comment
    u8  bannerTexture[NAND_BANNER_TEXTURE_SIZE]; // Banner texture
    u8  iconTexture[8][NAND_BANNER_ICON_SIZE];   // Icon texture 0-7
} NANDBanner;


void NANDInitBanner( NANDBanner *bnr, u32 flag, const u16 *title, const u16 *comment );


#ifdef __cplusplus
}
#endif


#endif // end of __NAND_H__
