/*---------------------------------------------------------------------------*
  Project: OS - Low Level Locked Cache Operations Library
  File:    OSLC.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSLC.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     1999/12/22 5:16p Tian
    Cleanup
    
    3     1999/12/07 5:54p Tian
    Changed LCAlloc* to invalidate instead of flush
    
    2     1999/12/02 12:04p Tian
    Added defines for max DMA transaction sizes
    
    4     1999/07/27 5:31p Shiki
    Cleanup.

    3     1999/05/11 4:43p Shiki
    Refreshed include tree.

    1     1999/4/30 12:49p Tianli01

    1     1999/03/04 2:23p Tianli01
    Initial check-in to new tree

    1     1999/02/04 6:02p Tianli01
    Initial check-in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
    This header file defines the low level locked cache operations.
    Most should be macros to the underlying assembly routines, but
    others will manipulate hardware setup registers.

    Note that there are some restrictions on arguments that must
    be adhered to.  Read implementation notes (OSCache.c) or
    man pages for details.
 */

#ifndef __OSLC_H__
#define __OSLC_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// maximum size of a single DMA transaction
#define LC_MAX_DMA_BLOCKS   (128)
#define LC_MAX_DMA_BYTES    (LC_MAX_DMA_BLOCKS * 32)

void LCAlloc            ( void* addr, u32 nBytes );
void LCAllocNoInvalidate( void* addr, u32 nBytes );
void LCAllocOneTag  ( BOOL invalidate, void* tag );
void LCAllocTags    ( BOOL invalidate, void* startTag, u32 numBlocks );
u32  LCQueueLength  ( void );

#ifdef __cplusplus
}
#endif

#endif  // __OSLC_H__
