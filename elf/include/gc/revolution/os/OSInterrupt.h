/*---------------------------------------------------------------------------*
  Project:  Revolution OS Interrupt API
  File:     OSInterrupt.h
  Programmers: Shiki Okasaka
               Hashida Takayuki
               HIRATSU Daisuke
               Kazuki Hirose

  Copyright 1998-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

    
    5     11/09/01 12:54 Shiki
    Added __OSLastInterrupt and other declarations.

    4     01/02/14 21:07 Shiki
    Added __OS_INTERRUPT_PI_HSP.

    3     5/13/00 7:38p Shiki
    Added __OSSpuriousInterrupts().

    2     4/20/00 5:43p Shiki
    Minor revision.

    12    11/10/99 4:36p Shiki
    Revised CP and PE interrupt handling method.

    11    10/04/99 10:19a Hashida
    Changed VI to be handled using PI_VI

    10    9/08/99 4:26p Shiki
    Removed CP_IDLE and CP_BREAKPOINT definitions.

    9     9/03/99 5:56p Shiki
    Changed __OS_INTERRUPT_SI_TC to __OS_INTERRUPT_PI_SI.

    8     8/27/99 4:32p Shiki
    Revised __OS_INTERRUPT definitions.

    7     8/26/99 5:11p Shiki
    Added __OS_INTERRUPT_MEM_ADDRESS.

    6     8/19/99 3:48p Shiki
    Added __OSDispatchInterrupt().

    5     8/17/99 3:31p Shiki
    Revised OS_INTERRUPTMASK_* definitions.

    4     8/16/99 11:04p Shiki
    Added __OSSetInterruptMask() and __OSGetInterruptMask().

    3     8/16/99 10:06p Shiki
    Added OS_INTERRUPTMASK_* definitions.

    2     8/13/99 5:22p Shiki
    Revised __OS_INTERRUPT definitions.

    1     8/02/99 2:17p Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSINTERRUPT_H__
#define __OSINTERRUPT_H__

#include <revolution/types.h>
#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef s16  __OSInterrupt;
typedef void (*__OSInterruptHandler)(
    __OSInterrupt   interrupt,
    OSContext*      context
);

typedef u32  OSInterruptMask;

#define __OS_INTERRUPT_MEM_0                0
#define __OS_INTERRUPT_MEM_1                1
#define __OS_INTERRUPT_MEM_2                2
#define __OS_INTERRUPT_MEM_3                3
#define __OS_INTERRUPT_MEM_ADDRESS          4
#define __OS_INTERRUPT_DSP_AI               5
#define __OS_INTERRUPT_DSP_ARAM             6
#define __OS_INTERRUPT_DSP_DSP              7
#define __OS_INTERRUPT_AI_AI                8
#define __OS_INTERRUPT_EXI_0_EXI            9
#define __OS_INTERRUPT_EXI_0_TC             10
#define __OS_INTERRUPT_EXI_0_EXT            11
#define __OS_INTERRUPT_EXI_1_EXI            12
#define __OS_INTERRUPT_EXI_1_TC             13
#define __OS_INTERRUPT_EXI_1_EXT            14
#define __OS_INTERRUPT_EXI_2_EXI            15
#define __OS_INTERRUPT_EXI_2_TC             16
#define __OS_INTERRUPT_PI_CP                17
#define __OS_INTERRUPT_PI_PE_TOKEN          18
#define __OS_INTERRUPT_PI_PE_FINISH         19
#define __OS_INTERRUPT_PI_SI                20
#define __OS_INTERRUPT_PI_DI                21
#define __OS_INTERRUPT_PI_RSW               22
#define __OS_INTERRUPT_PI_ERROR             23
#define __OS_INTERRUPT_PI_VI                24
#define __OS_INTERRUPT_PI_DEBUG             25
#define __OS_INTERRUPT_PI_HSP               26
#define __OS_INTERRUPT_PI_ACR               27   // for Revolution
#define __OS_INTERRUPT_MAX                  32


#define OS_INTERRUPTMASK(interrupt)         (0x80000000u >> (interrupt))        // for cntlzw

#define OS_INTERRUPTMASK_MEM_0              OS_INTERRUPTMASK(__OS_INTERRUPT_MEM_0)
#define OS_INTERRUPTMASK_MEM_1              OS_INTERRUPTMASK(__OS_INTERRUPT_MEM_1)
#define OS_INTERRUPTMASK_MEM_2              OS_INTERRUPTMASK(__OS_INTERRUPT_MEM_2)
#define OS_INTERRUPTMASK_MEM_3              OS_INTERRUPTMASK(__OS_INTERRUPT_MEM_3)
#define OS_INTERRUPTMASK_MEM_ADDRESS        OS_INTERRUPTMASK(__OS_INTERRUPT_MEM_ADDRESS)
#define OS_INTERRUPTMASK_MEM                (OS_INTERRUPTMASK_MEM_0             | \
                                                OS_INTERRUPTMASK_MEM_1          | \
                                                OS_INTERRUPTMASK_MEM_2          | \
                                                OS_INTERRUPTMASK_MEM_3          | \
                                                OS_INTERRUPTMASK_MEM_ADDRESS)
#define OS_INTERRUPTMASK_DSP_AI             OS_INTERRUPTMASK(__OS_INTERRUPT_DSP_AI)
#define OS_INTERRUPTMASK_DSP_ARAM           OS_INTERRUPTMASK(__OS_INTERRUPT_DSP_ARAM)
#define OS_INTERRUPTMASK_DSP_DSP            OS_INTERRUPTMASK(__OS_INTERRUPT_DSP_DSP)
#define OS_INTERRUPTMASK_DSP                (OS_INTERRUPTMASK_DSP_AI            | \
                                                OS_INTERRUPTMASK_DSP_ARAM       | \
                                                OS_INTERRUPTMASK_DSP_DSP)
#define OS_INTERRUPTMASK_AI_AI              OS_INTERRUPTMASK(__OS_INTERRUPT_AI_AI)
#define OS_INTERRUPTMASK_AI                 (OS_INTERRUPTMASK_AI_AI)
#define OS_INTERRUPTMASK_EXI_0_EXI          OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_0_EXI)
#define OS_INTERRUPTMASK_EXI_0_TC           OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_0_TC)
#define OS_INTERRUPTMASK_EXI_0_EXT          OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_0_EXT)
#define OS_INTERRUPTMASK_EXI_0              (OS_INTERRUPTMASK_EXI_0_EXI         | \
                                                OS_INTERRUPTMASK_EXI_0_TC       | \
                                                OS_INTERRUPTMASK_EXI_0_EXT)
#define OS_INTERRUPTMASK_EXI_1_EXI          OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_1_EXI)
#define OS_INTERRUPTMASK_EXI_1_TC           OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_1_TC)
#define OS_INTERRUPTMASK_EXI_1_EXT          OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_1_EXT)
#define OS_INTERRUPTMASK_EXI_1              (OS_INTERRUPTMASK_EXI_1_EXI         | \
                                                OS_INTERRUPTMASK_EXI_1_TC       | \
                                                OS_INTERRUPTMASK_EXI_1_EXT)
#define OS_INTERRUPTMASK_EXI_2_EXI          OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_2_EXI)
#define OS_INTERRUPTMASK_EXI_2_TC           OS_INTERRUPTMASK(__OS_INTERRUPT_EXI_2_TC)
#define OS_INTERRUPTMASK_EXI_2              (OS_INTERRUPTMASK_EXI_2_EXI         | \
                                                OS_INTERRUPTMASK_EXI_2_TC)
#define OS_INTERRUPTMASK_EXI                (OS_INTERRUPTMASK_EXI_0_EXI         | \
                                                OS_INTERRUPTMASK_EXI_0_TC       | \
                                                OS_INTERRUPTMASK_EXI_0_EXT      | \
                                                OS_INTERRUPTMASK_EXI_1_EXI      | \
                                                OS_INTERRUPTMASK_EXI_1_TC       | \
                                                OS_INTERRUPTMASK_EXI_1_EXT      | \
                                                OS_INTERRUPTMASK_EXI_2_EXI      | \
                                                OS_INTERRUPTMASK_EXI_2_TC)
#define OS_INTERRUPTMASK_PI_PE_TOKEN        OS_INTERRUPTMASK(__OS_INTERRUPT_PI_PE_TOKEN)
#define OS_INTERRUPTMASK_PI_PE_FINISH       OS_INTERRUPTMASK(__OS_INTERRUPT_PI_PE_FINISH)
#define OS_INTERRUPTMASK_PI_PE              (OS_INTERRUPTMASK_PI_PE_TOKEN       | \
                                                OS_INTERRUPTMASK_PI_PE_FINISH)
#define OS_INTERRUPTMASK_PI_CP              OS_INTERRUPTMASK(__OS_INTERRUPT_PI_CP)
#define OS_INTERRUPTMASK_PI_SI              OS_INTERRUPTMASK(__OS_INTERRUPT_PI_SI)
#define OS_INTERRUPTMASK_PI_DI              OS_INTERRUPTMASK(__OS_INTERRUPT_PI_DI)
#define OS_INTERRUPTMASK_PI_RSW             OS_INTERRUPTMASK(__OS_INTERRUPT_PI_RSW)
#define OS_INTERRUPTMASK_PI_ERROR           OS_INTERRUPTMASK(__OS_INTERRUPT_PI_ERROR)
#define OS_INTERRUPTMASK_PI_VI              OS_INTERRUPTMASK(__OS_INTERRUPT_PI_VI)
#define OS_INTERRUPTMASK_PI_DEBUG           OS_INTERRUPTMASK(__OS_INTERRUPT_PI_DEBUG)
#define OS_INTERRUPTMASK_PI_HSP             OS_INTERRUPTMASK(__OS_INTERRUPT_PI_HSP)
#define OS_INTERRUPTMASK_PI_ACR             OS_INTERRUPTMASK(__OS_INTERRUPT_PI_ACR)
#define OS_INTERRUPTMASK_PI                 (OS_INTERRUPTMASK_PI_CP             | \
                                                OS_INTERRUPTMASK_PI_SI          | \
                                                OS_INTERRUPTMASK_PI_DI          | \
                                                OS_INTERRUPTMASK_PI_RSW         | \
                                                OS_INTERRUPTMASK_PI_ERROR       | \
                                                OS_INTERRUPTMASK_PI_VI          | \
                                                OS_INTERRUPTMASK_PI_PE_TOKEN    | \
                                                OS_INTERRUPTMASK_PI_PE_FINISH   | \
                                                OS_INTERRUPTMASK_PI_DEBUG       | \
                                                OS_INTERRUPTMASK_PI_HSP         | \
                                                OS_INTERRUPTMASK_PI_ACR           )

__OSInterruptHandler __OSSetInterruptHandler(
    __OSInterrupt         interrupt,
    __OSInterruptHandler  handler
);

__OSInterruptHandler __OSGetInterruptHandler(
    __OSInterrupt         interrupt
);

void __OSDispatchInterrupt(
    __OSException         exception,
    OSContext*            context);

OSInterruptMask OSGetInterruptMask  ( void );
OSInterruptMask OSSetInterruptMask  ( OSInterruptMask mask );
OSInterruptMask __OSMaskInterrupts  ( OSInterruptMask mask );
OSInterruptMask __OSUnmaskInterrupts( OSInterruptMask mask );

#ifdef  _DEBUG
extern u64 __OSSpuriousInterrupts;  // # of spurious interrupts Gekko has taken
#endif  // _DEBUG

// The record of the last interrupt Gekko has taken except __OS_INTERRUPT_MEM_*.
extern volatile __OSInterrupt __OSLastInterrupt;        // One of __OS_INTERRUPT_*
extern volatile u32           __OSLastInterruptSrr0;    // SRR0 value at the interrupt
extern volatile OSTime        __OSLastInterruptTime;    // OSGetTime() value at the interrupt


/*-----------------------------------------------------------------------------*/
// ACR interrupt emulation for non-RVL target (tentative)
#ifndef HOLLYWOOD_REV
#define EMU_PI_ACRINT
#endif

/*-----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif  // __OSINTERRUPT_H__
