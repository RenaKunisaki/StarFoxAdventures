/*---------------------------------------------------------------------------*
  Project: disk banner define names and definition
  File:    DVDBanner.h

  Copyright 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DVDBanner.h,v $
  Revision 1.1  2006/01/31 08:10:02  ooizumi
  Definitions for banner file "opening.bnr".

    
    2     11/19/01 14:49 Shiki
    Added DVDBanner2.

    1     01/04/24 17:36 Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __DVDBANNER_H__
#define __DVDBANNER_H__

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// Game banner file must be placed in the root directory
#define DVD_BANNER_FILENAME         "opening.bnr"

#define DVD_BANNER_WIDTH            96
#define DVD_BANNER_HEIGHT           32

//
// "opening.bnr" file format for JP/US console
//
typedef struct DVDBanner
{
    u32 id;                 // 'BNR1'
    u32 padding[7];
    u8  image[2 * DVD_BANNER_WIDTH * DVD_BANNER_HEIGHT];    // RGB5A3 96x32 texture image
    u8  shortTitle[32];     // Short game title shown in IPL menu
    u8  shortMaker[32];     // Short developer, publisher names shown in IPL menu
    u8  longTitle[64];      // Long game title shown in IPL game start screen
    u8  longMaker[64];      // Long developer, publisher names shown in IPL game start screen
    u8  comment[128];       // Game description shown in IPL game start screen in two lines.
                            // Comment can include a '\n'.
} DVDBanner;

typedef struct DVDBannerComment
{
    u8  shortTitle[32];     // Short game title shown in IPL menu
    u8  shortMaker[32];     // Short developer, publisher names shown in IPL menu
    u8  longTitle[64];      // Long game title shown in IPL game start screen
    u8  longMaker[64];      // Long developer, publisher names shown in IPL game start screen
    u8  comment[128];       // Game description shown in IPL game start screen in two lines.
                            // Comment can include a '\n'.
} DVDBannerComment;

//
// "opening.bnr" file format for EU console
//
typedef struct DVDBanner2
{
    u32 id;                 // 'BNR2'
    u32 padding[7];
    u8  image[2 * DVD_BANNER_WIDTH * DVD_BANNER_HEIGHT];    // RGB5A3 96x32 texture image
    DVDBannerComment comment[6];    // Comments in six languages
} DVDBanner2;

// DVDBanner.id
#define DVD_BANNER_ID               'BNR1'  // US/JP
#define DVD_BANNER_ID2              'BNR2'  // EU

#ifdef __cplusplus
}
#endif

#endif  // __DVDBANNER_H__
