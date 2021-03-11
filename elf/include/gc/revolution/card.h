/*---------------------------------------------------------------------------*
  Project:  Dolphin Memory Card API
  File:     card.h

  Copyright 2000-2004 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: card.h,v $
  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Ported from dolphin source tree.

    
    39    2004/03/08 19:50 Shiki
    Added CARDGetCurrentMode().

    38    2004/01/06 11:27 Shiki
    Added CARDGetFastMode() and CARDSetFastMode().

    37    2003/02/12 16:26 Shiki
    Defined CARD_ATTR_GLOBAL and CARD_ATTR_COMPANY.

    36    2002/08/12 15:30 Shiki
    Added __padding field to CARDFileInfo{} and CARDStat{}.

    35    2002/08/05 15:49 Shiki
    Added const keywords to relevant function prototypes.

    34    2001/11/26 11:52 Shiki
    Added CARD_XFER_SETATTRIBUTES and CARD_XFER_WRITE.

    33    2001/10/18 14:16 Shiki
    Added CARDGetAttributes(),CARDGetSerialNo(),CARDSetAttributes[Async]().

    32    2001/07/19 22:09 Shiki
    Added CARDCheckEx[Async].

    31    2001/06/19 11:55a Dante
    Added CARDSetIconAddress & CARDSetCommentAddress

    30    2001/06/18 10:16p Dante
    Added CARDSetIconAnim() & CARDSetBannerFormat()

    29    2001/05/18 4:03p Shiki
    Added CARDProbeEx().

    28    2001/05/18 1:49p Shiki
    Added CARD_XFER_*.

    27    2001/05/18 1:16p Shiki
    Added CARDGetXferredBytes().

    26    2001/05/10 11:07a Shiki
    Modified CARD_ENCODE_* to sync with OS_FONT_ENCODE_*.

    25    2001/04/23 15:28 Shiki
    Fixed CARDGetIcon*() macros.

    24    2001/04/23 14:45 Shiki
    Removed CARDStat.gameVersion.

    23    2001/04/19 9:10 Shiki
    Added CARD_COMMENT_SIZE.

    22    2001/02/27 16:46 Shiki
    Added CARDCancel().

    21    2001/02/22 15:54 Shiki
    Added CARDGetIconAnim().

    20    2001/02/22 11:48 Shiki
    Changed the type of fileName from u8[] to char[].

    19    2001/02/22 9:37 Shiki
    Added support for multiple sector sizes.

    18    2001/01/23 9:30 Shiki
    Fixed typo.

    17    2000/12/14 7:17p Shiki
    Added CARDGetMemSize().

    16    2000/12/11 11:05p Shiki
    Added icon stuff.

    15    2000/10/25 11:50p Shiki
    Added CARDCheckAsync().

    14    2000/09/07 6:53p Shiki
    Added CARD_FILENAME_MAX.

    13    2000/09/05 2:46p Shiki
    Fixed to be C++ ready.

    12    2000/09/01 5:28p Shiki
    Added CARDRename() and CARDRenameAsync().

    11    2000/08/25 6:46p Shiki
    Revised CARD_ENCODE_*.

    10    2000/08/25 3:54p Shiki
    Added CARD_RESULT_ENCODING.

    9     2000/08/24 6:32p Shiki
    Added CARDGetEncoding().

    8     2000/08/24 4:16p Shiki
    Added CARDProbe().

    7     2000/08/10 4:39p Shiki
    Modified CARDStat.length from u8 to u32.

    6     2000/08/10 4:27p Shiki
    Added CARD_BLOCK_SIZE.

    5     2000/08/09 6:19p Shiki
    Cleanup.

    4     2000/08/09 5:10p Shiki
    Added CARD_RESULT_NAMETOOLONG.

    3     2000/08/08 7:51p Shiki
    Added CARD_RESULT_LIMIT.

    2     2000/07/19 7:44p Shiki
    Added CARD_RESULT_NOPERM.

    1     2000/07/14 3:58p Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __CARD_H__
#define __CARD_H__

#include <revolution/types.h>
#include <revolution/os/OSFont.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CARD_WORKAREA_SIZE  (5 * 8 * 1024)
#define CARD_READ_SIZE      512
#define CARD_MAX_FILE       127
#define CARD_COMMENT_SIZE   64

// internal API command xfer bytes
#define CARD_XFER_CREATE        (2 * 8 * 1024)  // CARDCreate[Async]
#define CARD_XFER_DELETE        (2 * 8 * 1024)  // CARD[Fast]Delete[Async]
#define CARD_XFER_MOUNT         (5 * 8 * 1024)  // CARDMount[Async]
#define CARD_XFER_FORMAT        (5 * 8 * 1024)  // CARDFormat[Async]
#define CARD_XFER_RENAME        (1 * 8 * 1024)  // CARDRename[Async]
#define CARD_XFER_SETSTATUS     (1 * 8 * 1024)  // CARDSetStatus[Async]
#define CARD_XFER_SETATTRIBUTES (1 * 8 * 1024)  // CARDSetAttributes[Async]
#define CARD_XFER_WRITE         (1 * 8 * 1024)  // CARDWrite[Async]

#define CARD_ENCODE_ANSI    OS_FONT_ENCODE_ANSI
#define CARD_ENCODE_SJIS    OS_FONT_ENCODE_SJIS

// longest file name string excluding terminating zero
#define CARD_FILENAME_MAX   32

#define CARD_ICON_MAX       8
#define CARD_ICON_WIDTH     32
#define CARD_ICON_HEIGHT    32
#define CARD_BANNER_WIDTH   96
#define CARD_BANNER_HEIGHT  32

#define CARD_MODE_NORMAL    0
#define CARD_MODE_FAST      1

typedef struct CARDFileInfo
{
    s32 chan;
    s32 fileNo;

    s32 offset;
    s32 length;
    u16 iBlock;
    u16 __padding;
} CARDFileInfo;

typedef struct CARDStat
{
    // read-only (Set by CARDGetStatus)
    char fileName[CARD_FILENAME_MAX];
    u32  length;
    u32  time;           // seconds since 2000/01/01 midnight
    u8   gameName[4];
    u8   company[2];

    // read/write (Set by CARDGetStatus/CARDSetStatus)
    u8   bannerFormat;
    u8   __padding;
    u32  iconAddr;      // offset to the banner, bannerTlut, icon, iconTlut data set.
    u16  iconFormat;
    u16  iconSpeed;
    u32  commentAddr;   // offset to the pair of 32 byte character strings.

    // read-only (Set by CARDGetStatus)
    u32  offsetBanner;
    u32  offsetBannerTlut;
    u32  offsetIcon[CARD_ICON_MAX];
    u32  offsetIconTlut;
    u32  offsetData;
} CARDStat;

#define CARDGetBannerFormat(stat)          (((stat)->bannerFormat) & CARD_STAT_BANNER_MASK)
#define CARDGetIconAnim(stat)              (((stat)->bannerFormat) & CARD_STAT_ANIM_MASK)
#define CARDGetIconFormat(stat, n)         (((stat)->iconFormat >> (2 * (n))) & CARD_STAT_ICON_MASK)
#define CARDGetIconSpeed(stat, n)          (((stat)->iconSpeed  >> (2 * (n))) & CARD_STAT_SPEED_MASK)
#define CARDSetBannerFormat(stat, f)       ((stat)->bannerFormat = (u8) (((stat)->bannerFormat & ~CARD_STAT_BANNER_MASK)|(f)))
#define CARDSetIconAnim(stat, f)           ((stat)->bannerFormat = (u8) (((stat)->bannerFormat & ~CARD_STAT_ANIM_MASK  )|(f)))
#define CARDSetIconFormat(stat, n, f)      ((stat)->iconFormat = (u16) (((stat)->iconFormat & ~(CARD_STAT_ICON_MASK  << (2 * (n)))) | ((f) << (2 * (n)))))
#define CARDSetIconSpeed(stat, n, f)       ((stat)->iconSpeed  = (u16) (((stat)->iconSpeed  & ~(CARD_STAT_SPEED_MASK << (2 * (n)))) | ((f) << (2 * (n)))))
#define CARDSetIconAddress(stat, addr)     ((stat)->iconAddr =     (u32)(addr))
#define CARDSetCommentAddress(stat, addr)  ((stat)->commentAddr =  (u32)(addr))

#define CARD_STAT_ICON_NONE     0
#define CARD_STAT_ICON_C8       1
#define CARD_STAT_ICON_RGB5A3   2
#define CARD_STAT_ICON_MASK     3

#define CARD_STAT_BANNER_NONE   0
#define CARD_STAT_BANNER_C8     1
#define CARD_STAT_BANNER_RGB5A3 2
#define CARD_STAT_BANNER_MASK   3

#define CARD_STAT_ANIM_LOOP     0x00
#define CARD_STAT_ANIM_BOUNCE   0x04
#define CARD_STAT_ANIM_MASK     0x04

#define CARD_STAT_SPEED_END     0
#define CARD_STAT_SPEED_FAST    1
#define CARD_STAT_SPEED_MIDDLE  2
#define CARD_STAT_SPEED_SLOW    3
#define CARD_STAT_SPEED_MASK    3

#define CARD_ATTR_RESERVED_0    0x01u
#define CARD_ATTR_RESERVED_1    0x02u
#define CARD_ATTR_PUBLIC        0x04u
#define CARD_ATTR_NO_COPY       0x08u
#define CARD_ATTR_NO_MOVE       0x10u
#define CARD_ATTR_GLOBAL        0x20u
#define CARD_ATTR_COMPANY       0x40u
#define CARD_ATTR_RESERVED_7    0x80u

// For backward compatibility only...
#define CARD_ATTR_RESERVED_5    0x20u
#define CARD_ATTR_RESERVED_6    0x40u

typedef void (*CARDCallback)( s32 chan, s32 result );

void CARDInit              ( void );
BOOL CARDGetFastMode       ( void );
BOOL CARDSetFastMode       ( BOOL enable );

s32  CARDCheck             ( s32 chan );
s32  CARDCheckAsync        ( s32 chan, CARDCallback callback );
s32  CARDCheckEx           ( s32 chan, s32* xferBytes );
s32  CARDCheckExAsync      ( s32 chan, s32* xferBytes, CARDCallback callback );
s32  CARDCreate            ( s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo );
s32  CARDCreateAsync       ( s32 chan, const char* fileName, u32 size, CARDFileInfo* fileInfo, CARDCallback callback );
s32  CARDDelete            ( s32 chan, const char* fileName );
s32  CARDDeleteAsync       ( s32 chan, const char* fileName, CARDCallback callback );
s32  CARDFastDelete        ( s32 chan, s32 fileNo );
s32  CARDFastDeleteAsync   ( s32 chan, s32 fileNo, CARDCallback callback );
s32  CARDFastOpen          ( s32 chan, s32 fileNo, CARDFileInfo* fileInfo );
s32  CARDFormat            ( s32 chan );
s32  CARDFormatAsync       ( s32 chan, CARDCallback callback );
s32  CARDFreeBlocks        ( s32 chan, s32* byteNotUsed, s32* filesNotUsed );
s32  CARDGetAttributes     ( s32 chan, s32 fileNo, u8* attr );
s32  CARDGetEncoding       ( s32 chan, u16* encode );
s32  CARDGetMemSize        ( s32 chan, u16* size );
s32  CARDGetResultCode     ( s32 chan );
s32  CARDGetSectorSize     ( s32 chan, u32* size );
s32  CARDGetSerialNo       ( s32 chan, u64* serialNo );
s32  CARDGetStatus         ( s32 chan, s32 fileNo, CARDStat* stat );
s32  CARDGetXferredBytes   ( s32 chan );
s32  CARDMount             ( s32 chan, void* workArea, CARDCallback detachCallback );
s32  CARDMountAsync        ( s32 chan, void* workArea, CARDCallback detachCallback, CARDCallback attachCallback );
s32  CARDOpen              ( s32 chan, const char* fileName, CARDFileInfo* fileInfo );
BOOL CARDProbe             ( s32 chan );
s32  CARDProbeEx           ( s32 chan, s32* memSize, s32* sectorSize );
s32  CARDRename            ( s32 chan, const char* oldName, const char* newName );
s32  CARDRenameAsync       ( s32 chan, const char* oldName, const char* newName, CARDCallback callback );
s32  CARDSetAttributesAsync( s32 chan, s32 fileNo, u8 attr, CARDCallback callback);
s32  CARDSetAttributes     ( s32 chan, s32 fileNo, u8 attr );
s32  CARDSetStatus         ( s32 chan, s32 fileNo, CARDStat* stat );
s32  CARDSetStatusAsync    ( s32 chan, s32 fileNo, CARDStat* stat, CARDCallback callback );
s32  CARDUnmount           ( s32 chan );
s32  CARDGetCurrentMode    ( s32 chan, u32* mode );

s32  CARDCancel            ( CARDFileInfo* fileInfo );
s32  CARDClose             ( CARDFileInfo* fileInfo );
s32  CARDRead              ( CARDFileInfo* fileInfo, void* addr, s32 length, s32 offset );
s32  CARDReadAsync         ( CARDFileInfo* fileInfo, void* addr, s32 length, s32 offset, CARDCallback callback );
s32  CARDWrite             ( CARDFileInfo* fileInfo, const void* addr, s32 length, s32 offset );
s32  CARDWriteAsync        ( CARDFileInfo* fileInfo, const void* addr, s32 length, s32 offset, CARDCallback callback );

#define CARDGetFileNo(fileInfo)     ((fileInfo)->fileNo)

#define CARD_RESULT_UNLOCKED        1
#define CARD_RESULT_READY           0
#define CARD_RESULT_BUSY            -1
#define CARD_RESULT_WRONGDEVICE     -2
#define CARD_RESULT_NOCARD          -3
#define CARD_RESULT_NOFILE          -4
#define CARD_RESULT_IOERROR         -5
#define CARD_RESULT_BROKEN          -6
#define CARD_RESULT_EXIST           -7
#define CARD_RESULT_NOENT           -8
#define CARD_RESULT_INSSPACE        -9
#define CARD_RESULT_NOPERM          -10
#define CARD_RESULT_LIMIT           -11
#define CARD_RESULT_NAMETOOLONG     -12
#define CARD_RESULT_ENCODING        -13
#define CARD_RESULT_CANCELED        -14
#define CARD_RESULT_FATAL_ERROR     -128

#ifdef __cplusplus
}
#endif

#endif  // __CARD_H__
