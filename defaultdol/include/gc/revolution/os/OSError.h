/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Error Handler API
  File:     OSError.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSError.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2002/08/26 22:17 Shiki
    Added __OSFpscrEnableBits.

    3     2002/08/19 21:42 Shiki
    Added OS_ERROR_FPE.

    2     2001/08/15 17:55 Shiki
    Added OS_ERROR_PROTECTION.

    4     1999/10/08 1:46p Shiki
    Fixed OSSetErrorHandler() parameter declaration.

    3     1999/08/03 3:54p Shiki
    Revised OSErrorHandler() declaration.

    2     1999/07/13 6:09p Shiki
    Fixed OSSetErrorHandler() declaration.

    1     1999/07/12 9:34p Shiki
    Initial check-in

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSERROR_H__
#define __OSERROR_H__

#include <revolution/os.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSException.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Error Handler API
 *---------------------------------------------------------------------------*/

typedef u16    OSError;

typedef void (*OSErrorHandler)( OSError error, OSContext* context, ... );
/*
 * Note: for most errors, OSErrorHandler takes third and fourth arguments
 * /dsisr/ and /dar/, which are of type /u32/ like below:
 *
 * void (*OSErrorHandler)( OSError error, OSContext* context,
 *                         u32 dsisr, u32 dar );
 *
 */

#define OS_ERROR_SYSTEM_RESET         0
#define OS_ERROR_MACHINE_CHECK        1
#define OS_ERROR_DSI                  2
#define OS_ERROR_ISI                  3
#define OS_ERROR_EXTERNAL_INTERRUPT   4
#define OS_ERROR_ALIGNMENT            5
#define OS_ERROR_PROGRAM              6
#define OS_ERROR_FLOATING_POINT       7     // Floating-point unavailable
#define OS_ERROR_DECREMENTER          8
#define OS_ERROR_SYSTEM_CALL          9
#define OS_ERROR_TRACE                10
#define OS_ERROR_PERFORMACE_MONITOR   11
#define OS_ERROR_BREAKPOINT           12
#define OS_ERROR_SYSTEM_INTERRUPT     13
#define OS_ERROR_THERMAL_INTERRUPT    14
#define OS_ERROR_PROTECTION           15
#define OS_ERROR_FPE                  16    // Floating-point exception

#define OS_ERROR_MAX                  (OS_ERROR_FPE+1)

OSErrorHandler OSSetErrorHandler( OSError error, OSErrorHandler handler );

// Default FPSCR enable bits set when OS_ERROR_FPE handler is set
extern u32 __OSFpscrEnableBits;     // OR-ed FPSCR_*E bits

#ifdef __cplusplus
}
#endif

#endif  // __OSERROR_H__
