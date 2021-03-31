/*---------------------------------------------------------------------------*
  Project:  Revolution KPR Key Processing library
  File:     kpr.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: kpr.h,v $
  Revision 1.6  2007/05/31 00:25:55  carlmu
  Added KPRRemoveChar.

  Revision 1.5  2007/05/17 22:16:13  carlmu
  Misc API changes for 0.10.

  Revision 1.4  2007/05/11 23:21:49  carlmu
  Added extern "C".
  Added region-specific initializers for dead-code stripping.

  Revision 1.3  2007/05/05 01:58:13  carlmu
  Changes for 0.8 API

  Revision 1.2  2007/05/01 21:36:27  carlmu
  Added KPRClearQueue, modified various API return codes,
  limited Alt+keypad entries to 8 bits, changed overflow to fatal error.

  Revision 1.1  2007/04/25 19:02:01  carlmu
  Initial version.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __KPR_H__
#define __KPR_H__

#ifdef __cplusplus
extern "C" {
#endif

// KPR_FLUSH_AKP_CHAR is the character used to finish an Alt+keypad sequence.
// KPR_FLUSH_ALL_CHAR will "flush" all unprocessed characters in a queue.

#define KPR_FLUSH_AKP_CHAR 0x0000
#define KPR_FLUSH_ALL_CHAR 0xffff

typedef enum _KPRMode
{
    KPR_MODE_NONE               = 0x00,
    KPR_MODE_ALT_KEYPAD         = 0x01,
    KPR_MODE_DEADKEY            = 0x02,
    KPR_MODE_JP_ROMAJI_HIRAGANA = 0x04, // this and the next
    KPR_MODE_JP_ROMAJI_KATAKANA = 0x08  // are mutually exclusive
} KPRMode;

// Note: 5 characters is the longest queue length necessary.
// Do not make this bigger; KPR is not a general-purpose queue.
#define KPR_MAX_QUEUE_LEN  5

typedef struct _KPRQueue
{
    wchar_t  text[ KPR_MAX_QUEUE_LEN ];
    KPRMode mode;
    u8       oCount; // number of characters available for output
    u8       iCount; // number of characters that are input
    u32      altVal; // for building up Alt-keypad value
} KPRQueue;

void    KPRInitRegionUS ( void );
void    KPRInitRegionJP ( void );
void    KPRInitRegionEU ( void );

void    KPRInitQueue    ( KPRQueue *queue );
void    KPRClearQueue   ( KPRQueue *queue );
void    KPRSetMode      ( KPRQueue *queue, KPRMode mode );
KPRMode KPRGetMode      ( KPRQueue *queue );
u8      KPRPutChar      ( KPRQueue *queue, wchar_t inChar );
wchar_t KPRGetChar      ( KPRQueue *queue );
wchar_t KPRRemoveChar   ( KPRQueue *queue );
u8      KPRLookAhead    ( KPRQueue *queue, wchar_t *string, u32 maxSize );

#ifdef __cplusplus
}
#endif

#endif // __KPR_H__
