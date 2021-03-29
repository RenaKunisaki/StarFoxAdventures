/*---------------------------------------------------------------------------*
  Project: OS - Low Level Data Cache Operations Library
  File:    OSDC.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSDC.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    3     1999/05/11 4:43p Shiki
    Refreshed include tree.

    1     1999/4/30 12:49p Tianli01

    1     1999/03/04 2:22p Tianli01
    Initial check-in to new tree

    1     1999/02/04 6:02p Tianli01
    Initial check-in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
    This header file defines the low level data cache operations.  Most should
    be macros to the underlying assembly routines, but others will manipulate
    hardware setup registers.
 */

#ifndef __OSDC_H__
#define __OSDC_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void DCFlashInvalidate  ( void );
void DCEnable           ( void );
void DCDisable          ( void );
void DCFreeze           ( void );
void DCUnfreeze         ( void );
void DCTouchLoad        ( void* addr );
void DCBlockZero        ( void* addr );
void DCBlockStore       ( void* addr );
void DCBlockFlush       ( void* addr );
void DCBlockInvalidate  ( void* addr );

#ifdef __cplusplus
}
#endif

#endif  // __OSDC_H__
