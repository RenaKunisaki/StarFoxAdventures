/*---------------------------------------------------------------------------*
  Project: OS - High Level Cache Operations Library
  File:    OSCache.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSCache.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    10    2000/02/16 3:13p Tian
    Fixed locked cache region at 0xEnnn nnnn
    
    9     2000/02/15 6:14p Tian
    Corrected LCGetBase
    
    8     1999/12/23 2:41p Tian
    Added NoSync versions of DCFlushRange and DCStoreRange
    
    7     1999/12/22 5:16p Tian
    Cleanup
    
    6     1999/12/22 4:11p Tian
    Changed return type of LCGetBase
    
    5     1999/12/22 3:17p Tian
    Added LCGetBase and LC_BASE
    
    4     1999/12/07 5:55p Tian
    Changed LCAlloc* to invalidate instead of flush
    
    3     1999/12/02 7:10p Tian
    Added LCQueueWait
    
    2     1999/12/02 12:04p Tian
    Updated the locked cache APIs to return transactions added.  Added
    LCFlushQueue
    
    4     1999/07/27 5:27p Shiki
    Cleanup.

    3     1999/05/11 4:43p Shiki
    Refreshed include tree.

    1     1999/04/30 12:49p Tianli01

    4     1999/04/21 4:12p Tianli01
    moved ICSync to OSIC.h

    3     1999/04/12 7:45p Tianli01
    Added ICSync

    2     1999/03/26 2:07p Tianli01
    Included the low level cache APIs for convenience

    1     1999/03/04 2:22p Tianli01
    Initial check-in to new tree

    2     1999/02/12 5:39p Tianli01
    Updated argument names

    1     1999/02/04 6:02p Tianli01
    Initial check-in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
    This header file defines the most common high level cache operations.
    For finer grain control over the caches, use OSDC.h, OSIC.h, and OSLC.h
    for data cache, instruction cache, and locked cache routines respectively.
 */

#ifndef __OSCACHE_H__
#define __OSCACHE_H__

#include <revolution/types.h>
#include <revolution/os/OSDC.h>
#include <revolution/os/OSIC.h>
#include <revolution/os/OSL2.h>
#include <revolution/os/OSLC.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    L1 Data Cache Operations
 *---------------------------------------------------------------------------*/

void DCInvalidateRange  ( void* addr, u32 nBytes );
void DCFlushRange       ( void* addr, u32 nBytes );
void DCStoreRange       ( void* addr, u32 nBytes );
void DCFlushRangeNoSync ( void* addr, u32 nBytes );
void DCStoreRangeNoSync ( void* addr, u32 nBytes );
void DCZeroRange        ( void* addr, u32 nBytes );
void DCTouchRange       ( void* addr, u32 nBytes );

/*---------------------------------------------------------------------------*
    L1 Instruction Cache Operations
 *---------------------------------------------------------------------------*/

void ICInvalidateRange  ( void* addr, u32 nBytes );

/*---------------------------------------------------------------------------*
    Locked Cache Operations
 *---------------------------------------------------------------------------*/
// 0xEnnn nnnn addresses will be mapped in by LCEnable (DBAT3 will be used)
#define LC_BASE_PREFIX  0xE000
#define LC_BASE         (LC_BASE_PREFIX << 16)

void LCEnable           ( void );
void LCDisable          ( void );
void LCLoadBlocks       ( void* destTag,  void* srcAddr, u32 numBlocks );
void LCStoreBlocks      ( void* destAddr, void* srcTag,  u32 numBlocks );
u32  LCLoadData         ( void* destAddr, void* srcAddr, u32 nBytes );
u32  LCStoreData        ( void* destAddr, void* srcAddr, u32 nBytes );
u32  LCQueueLength      ( void );
void LCQueueWait        ( u32 len );
void LCFlushQueue       ( void );

#define LCGetBase()     ((void*)LC_BASE)
    
#ifdef __cplusplus
}
#endif

#endif  // __OSCACHE_H__
