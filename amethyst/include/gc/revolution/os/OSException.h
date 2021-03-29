/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Exception table
  File:     OSException.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSException.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2001/03/16 17:41 Shiki
    Moved GQR1 to GQR7 to general register context.
    
    9     1999/08/02 2:36p Shiki
    Added __OSGetExceptionHandler().

    8     1999/07/26 4:01p Shiki
    Removed #include <dolphin/base/PPCArch.h>.

    7     1999/07/21 2:29p Shiki
    Removed OS_EXCEPTION_CALLBACK_EXIT macro since it is not necessary any
    more.

    6     1999/07/20 10:13p Shiki
    Cleanup.

    5     1999/07/08 8:08p Tian
    All exceptions save and restore LR now.

    4     1999/05/11 4:43p Shiki
    Refreshed include tree.

    2     1999/05/03 5:26p Tianli01
    PPC->EPPC

    1     1999/04/30 12:49p Tianli01

    8     1999/04/21 8:11p Shiki
    Moved to _DEBUG (avoid DEBUG).

    7     1999/04/21 8:10p Shiki
    Detabbed.

    6     1999/04/13 5:50p Tianli01
    Added PPCArch.h, added MSR[RI] management to exception handling.

    5     1999/04/12 7:46p Tianli01
    Added OSExceptionNames for debugging

    4     1999/04/02 5:30p Tianli01
    Fixed minor bug in OS_EXCEPTION_CALLBACK_EXIT.  It now assumes that R4
    is the context pointer and does not clobber it.

    3     1999/04/01 7:54p Tianli01
    Added OS_EXCEPTION_CALLBACK_EXIT for easy returns from exceptions that
    invoked callbacks.

    2     1999/03/31 6:08p Tianli01
    Changed OSException to __OSException.  Added basic context saving
    helper macros.

    1     1999/03/26 2:08p Tianli01
    Broken up from previous OS.h.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSEXCEPTION_H__
#define __OSEXCEPTION_H__

#include <revolution/types.h>
#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Exception Handler API
 *---------------------------------------------------------------------------*/

typedef u8   __OSException;
typedef void (*__OSExceptionHandler)(
    __OSException           exception,
    OSContext*              context
);

#define __OS_EXCEPTION_SYSTEM_RESET         0
#define __OS_EXCEPTION_MACHINE_CHECK        1
#define __OS_EXCEPTION_DSI                  2
#define __OS_EXCEPTION_ISI                  3
#define __OS_EXCEPTION_EXTERNAL_INTERRUPT   4
#define __OS_EXCEPTION_ALIGNMENT            5
#define __OS_EXCEPTION_PROGRAM              6
#define __OS_EXCEPTION_FLOATING_POINT       7
#define __OS_EXCEPTION_DECREMENTER          8
#define __OS_EXCEPTION_SYSTEM_CALL          9
#define __OS_EXCEPTION_TRACE                10
#define __OS_EXCEPTION_PERFORMACE_MONITOR   11
#define __OS_EXCEPTION_BREAKPOINT           12
#define __OS_EXCEPTION_SYSTEM_INTERRUPT     13
#define __OS_EXCEPTION_THERMAL_INTERRUPT    14
#define __OS_EXCEPTION_MAX                  \
        (__OS_EXCEPTION_THERMAL_INTERRUPT+1)

// Updates OS exception table, NOT first-level exception vector.
__OSExceptionHandler    __OSSetExceptionHandler(
    __OSException           exception,
    __OSExceptionHandler    handler
);

__OSExceptionHandler    __OSGetExceptionHandler(
    __OSException           exception
);

// Context saving helpers
// For now, use stmw.  Theoretically on 750 it has the same latency
// as pipelined stores.
#ifdef GEKKO
#define OS_EXCEPTION_SAVE_GPRS(context)                 \
    stw     r0, OS_CONTEXT_R0(context)              ;   \
    stw     r1, OS_CONTEXT_R1(context)              ;   \
    stw     r2, OS_CONTEXT_R2(context)              ;   \
    stmw    r6, OS_CONTEXT_R6(context)              ;   \
    /* Save GQR1 to GQR7. GQR0 must always be zero */   \
    mfspr   r0, GQR1                                ;   \
    stw     r0, OS_CONTEXT_GQR1(context)            ;   \
    mfspr   r0, GQR2                                ;   \
    stw     r0, OS_CONTEXT_GQR2(context)            ;   \
    mfspr   r0, GQR3                                ;   \
    stw     r0, OS_CONTEXT_GQR3(context)            ;   \
    mfspr   r0, GQR4                                ;   \
    stw     r0, OS_CONTEXT_GQR4(context)            ;   \
    mfspr   r0, GQR5                                ;   \
    stw     r0, OS_CONTEXT_GQR5(context)            ;   \
    mfspr   r0, GQR6                                ;   \
    stw     r0, OS_CONTEXT_GQR6(context)            ;   \
    mfspr   r0, GQR7                                ;   \
    stw     r0, OS_CONTEXT_GQR7(context)            ;
#else   // !GEKKO                  
#define OS_EXCEPTION_SAVE_GPRS(context)                 \
    stw     r0, OS_CONTEXT_R0(context)              ;   \
    stw     r1, OS_CONTEXT_R1(context)              ;   \
    stw     r2, OS_CONTEXT_R2(context)              ;   \
    stmw    r6, OS_CONTEXT_R6(context)              ;
#endif  // GEKKO

#ifdef __cplusplus
}
#endif

#endif  // __OSEXCEPTION_H__
