/*---------------------------------------------------------------------------*
  Project:  SC library
  File:     sc.h

  Copyright 2006 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __SC_H__
#define __SC_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void SCInit(void);

#define SC_STATUS_OK        0
#define SC_STATUS_BUSY      1
#define SC_STATUS_ERROR     2
#define SC_STATUS_INTERNAL3 3 // Not returned by SCCheckStatus().

extern u32 SCCheckStatus(void);

typedef void (*SCFlushCallback)(u32 result);

extern u32  SCFlush(void);
extern void SCFlushAsync(SCFlushCallback callback);

#define SC_SOUND_MODE_MONO          0u
#define SC_SOUND_MODE_STEREO        1u
#define SC_SOUND_MODE_SURROUND      2u
#define SC_SOUND_MODE_DEFAULT       SC_SOUND_MODE_STEREO

extern u8 SCGetSoundMode(void);

#define SC_LANG_JAPANESE            0u
#define SC_LANG_ENGLISH             1u
#define SC_LANG_GERMAN              2u
#define SC_LANG_FRENCH              3u
#define SC_LANG_SPANISH             4u
#define SC_LANG_ITALIAN             5u
#define SC_LANG_DUTCH               6u
#define SC_LANG_SIMP_CHINESE        7u
#define SC_LANG_TRAD_CHINESE        8u
#define SC_LANG_KOREAN              9u

extern u8 SCGetLanguage(void);

#define SC_ASPECT_RATIO_4x3         0u
#define SC_ASPECT_RATIO_16x9        1u
#define SC_ASPECT_RATIO_DEFAULT     SC_ASPECT_RATIO_4x3

extern u8 SCGetAspectRatio(void);

#define SC_PROGRESSIVE_MODE_OFF     0u
#define SC_PROGRESSIVE_MODE_ON      1u
#define SC_PROGRESSIVE_MODE_DEFAULT SC_PROGRESSIVE_MODE_OFF

extern u8 SCGetProgressiveMode(void);

#define SC_EURGB60_MODE_OFF           0u
#define SC_EURGB60_MODE_ON            1u
#define SC_EURGB60_MODE_DEFAULT       SC_EURGB60_MODE_OFF // Temporary

extern u8 SCGetEuRgb60Mode(void);

extern BOOL SCCheckPCMessageRestriction(void);
extern BOOL SCCheckPCPassword(char *password);

//////////////////////////////////////////////////////////////////////
// Emulated API (OBSOLETE; they do nothing and return FALSE).
//////////////////////////////////////////////////////////////////////

extern BOOL SCEmuSetAspectRatio(u8 ratio);
extern BOOL SCEmuSetLanguage(u8 language);

#ifdef __cplusplus
}
#endif

#endif // __SC_H__
