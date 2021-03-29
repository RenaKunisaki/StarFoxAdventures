/*---------------------------------------------------------------------------*
  Project: OS - memory protection API
  File:    OSMemory.h

  Copyright 2001-2006 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSMemory.h,v $
  Revision 1.4  2006/02/06 10:21:30  hirose
  Changed definition of OSGetPhysicalMemSize/OSGetConsoleSimulatedMemSize
  as aliases to Mem1 size functions.

  Revision 1.3  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.2  2006/01/28 09:34:06  hirose
  Moved memory size APIs from os.h. Added MEM2 related functions.

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    1     2001/08/15 17:56 Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSMEMORY_H__
#define __OSMEMORY_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Memory size APIs
 *---------------------------------------------------------------------------*/
u32 OSGetPhysicalMem1Size(void);            // 24Mbyte typical
u32 OSGetPhysicalMem2Size(void);            // 64Mbyte or 128Mbyte
u32 OSGetConsoleSimulatedMem1Size(void);    // 24Mbyte typical
u32 OSGetConsoleSimulatedMem2Size(void);    // 64Mbyte or 128Mbyte

// Conventional definitions
#define OSGetPhysicalMemSize            OSGetPhysicalMem1Size
#define OSGetConsoleSimulatedMemSize    OSGetConsoleSimulatedMem1Size

/*---------------------------------------------------------------------------*
    Memory protection APIs
 *---------------------------------------------------------------------------*/
#define OS_PROTECT_CHAN0            0
#define OS_PROTECT_CHAN1            1
#define OS_PROTECT_CHAN2            2
#define OS_PROTECT_CHAN3            3

// Capability bits
#define OS_PROTECT_CONTROL_NONE     0x00
#define OS_PROTECT_CONTROL_READ     0x01    // OK to read  [addr, addr + nBytes)
#define OS_PROTECT_CONTROL_WRITE    0x02    // OK to write [addr, addr + nBytes)
#define OS_PROTECT_CONTROL_RDWR     (OS_PROTECT_CONTROL_READ | OS_PROTECT_CONTROL_WRITE)

// dsisr bits for memory protection error handler, which tells
// from which region the error was reported
#define OS_PROTECT0_BIT             0x00000001  // by OS_PROTECT_CHAN0 range
#define OS_PROTECT1_BIT             0x00000002  // by OS_PROTECT_CHAN1 range
#define OS_PROTECT2_BIT             0x00000004  // by OS_PROTECT_CHAN2 range
#define OS_PROTECT3_BIT             0x00000008  // by OS_PROTECT_CHAN3 range
#define OS_PROTECT_ADDRERR_BIT      0x00000010  // by [24M or 48M, 64M)

void OSProtectRange( u32 chan, void* addr, u32 nBytes, u32 control );
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  // __OSMEMORY_H__
