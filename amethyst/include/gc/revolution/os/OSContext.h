/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Context API
  File:     OSContext.h

  Copyright 1998-2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSContext.h,v $
  Revision 1.3  2006/08/23 10:40:51  kawaset
  Added OSSwitchFiberEx().

  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    10    2002/08/12 15:17 Shiki
    Added psf_pad field to OSContext{}.

    9     2001/03/16 16:16 Shiki
    Clean up.

    8     2000/08/23 6:40p Shiki
    Added OSGetStackPointer().

    7     2000/07/20 7:52p Shiki
    Added OSSwitchStack() and OSSwitchFiber().

    6     2000/02/22 3:10p Tian
    Removed ifdef GEKKO.  Full gekko context structure is always used.

    5     2000/01/17 6:23p Shiki
    Replaced OSContext.fpSaved with .state for future thread support.

    4     2000/01/14 2:31p Tian
    Fixed bugs in the #defines for the Gekko registers.

    3     1999/11/29 5:51p Tian
    Added PSF defines, and increased size of context frame for Gekko

    2     1999/11/29 2:22p Tian
    Added Gekko GQRs (graphics quantization registers).  Will need to
    update frame size if we really need extra FPR array for save/restore of
    paired singles.

    8     1999/08/03 4:13p Shiki
    Removed #ifdef _DEBUG for OSDumpContext().

    7     1999/08/03 3:54p Shiki
    Removed dsisr and dar from OSContext.

    6     1999/07/16 3:17p Shiki
    Removed the definition of OS_CONTEXT_MODE_MORIBUND.

    5     1999/07/13 9:37p Shiki
    Added fpscr_pad member in OSContext so that FPSCR register can be
    saved/restored properly.

    4     1999/07/12 10:01p Shiki
    Added OSFillFPUContext().

    3     1999/05/11 4:43p Shiki
    Refreshed include tree.

    1     1999/04/30 12:49p Tianli01

    7     1999/04/21 8:06p Shiki
    Moved to _DEBUG (avoid DEBUG)

    6     1999/04/13 5:49p Tianli01
    Fixed FPU saving/loading APIs to conform to spec.

    5     1999/04/06 2:58p Tianli01
    Added FPU state saving constants and routines.  Note that FPU saving
    API is not yet to spec.

    4     1999/04/02 5:29p Tianli01
    Changed OSContext to use array.  Added DumpNVRegisters.

    3     1999/04/01 7:53p Tianli01
    Added floating point saved bit in OSContext

    2     1999/03/31 6:09p Tianli01
    Fixed bugs in field offsets.  Added PPC APIs and OSDumpContext for
    exceptions.

    1     1999/03/26 2:08p Tianli01
    Broken up from previous OS.h.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSCONTEXT_H__
#define __OSCONTEXT_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Context API
 *---------------------------------------------------------------------------*/

// Floating point context modes
#define     OS_CONTEXT_MODE_FPU         0x01u
#define     OS_CONTEXT_MODE_PSFP        0x02u

// Context status
#define     OS_CONTEXT_STATE_FPSAVED    0x01u   // set if FPU is saved
#define     OS_CONTEXT_STATE_EXC        0x02u   // set if saved by exception

typedef struct OSContext
{
    // General-purpose registers
    u32 gpr[32];

    u32 cr;
    u32 lr;
    u32 ctr;
    u32 xer;

    // Floating-point registers
    f64 fpr[32];

    u32 fpscr_pad;
    u32 fpscr;

    // Exception handling registers
    u32 srr0;
    u32 srr1;

    // Context mode
    u16 mode;           // since UIMM is 16 bits in PPC
    u16 state;          // OR-ed OS_CONTEXT_STATE_*

    // Place Gekko regs at the end so we have minimal changes to
    // existing code
    u32 gqr[8];
    u32 psf_pad;
    f64 psf[32];

} OSContext;

// Size of context frame on stack.
// This constant should reflect a large enough number to hold
// an entire context and padding for the stack frame header.
#define __OS_CONTEXT_FRAME  768

#define OS_CONTEXT_R0       0
#define OS_CONTEXT_R1       4
#define OS_CONTEXT_R2       8
#define OS_CONTEXT_R3       12
#define OS_CONTEXT_R4       16
#define OS_CONTEXT_R5       20
#define OS_CONTEXT_R6       24
#define OS_CONTEXT_R7       28
#define OS_CONTEXT_R8       32
#define OS_CONTEXT_R9       36
#define OS_CONTEXT_R10      40
#define OS_CONTEXT_R11      44
#define OS_CONTEXT_R12      48
#define OS_CONTEXT_R13      52
#define OS_CONTEXT_R14      56
#define OS_CONTEXT_R15      60
#define OS_CONTEXT_R16      64
#define OS_CONTEXT_R17      68
#define OS_CONTEXT_R18      72
#define OS_CONTEXT_R19      76
#define OS_CONTEXT_R20      80
#define OS_CONTEXT_R21      84
#define OS_CONTEXT_R22      88
#define OS_CONTEXT_R23      92
#define OS_CONTEXT_R24      96
#define OS_CONTEXT_R25      100
#define OS_CONTEXT_R26      104
#define OS_CONTEXT_R27      108
#define OS_CONTEXT_R28      112
#define OS_CONTEXT_R29      116
#define OS_CONTEXT_R30      120
#define OS_CONTEXT_R31      124

#define OS_CONTEXT_CR       128
#define OS_CONTEXT_LR       132
#define OS_CONTEXT_CTR      136
#define OS_CONTEXT_XER      140

#define OS_CONTEXT_FPR0     144
#define OS_CONTEXT_FPR1     152
#define OS_CONTEXT_FPR2     160
#define OS_CONTEXT_FPR3     168
#define OS_CONTEXT_FPR4     176
#define OS_CONTEXT_FPR5     184
#define OS_CONTEXT_FPR6     192
#define OS_CONTEXT_FPR7     200
#define OS_CONTEXT_FPR8     208
#define OS_CONTEXT_FPR9     216
#define OS_CONTEXT_FPR10    224
#define OS_CONTEXT_FPR11    232
#define OS_CONTEXT_FPR12    240
#define OS_CONTEXT_FPR13    248
#define OS_CONTEXT_FPR14    256
#define OS_CONTEXT_FPR15    264
#define OS_CONTEXT_FPR16    272
#define OS_CONTEXT_FPR17    280
#define OS_CONTEXT_FPR18    288
#define OS_CONTEXT_FPR19    296
#define OS_CONTEXT_FPR20    304
#define OS_CONTEXT_FPR21    312
#define OS_CONTEXT_FPR22    320
#define OS_CONTEXT_FPR23    328
#define OS_CONTEXT_FPR24    336
#define OS_CONTEXT_FPR25    344
#define OS_CONTEXT_FPR26    352
#define OS_CONTEXT_FPR27    360
#define OS_CONTEXT_FPR28    368
#define OS_CONTEXT_FPR29    376
#define OS_CONTEXT_FPR30    384
#define OS_CONTEXT_FPR31    392

#define OS_CONTEXT_FPSCR    400     // 8 bytes including padding

#define OS_CONTEXT_SRR0     408
#define OS_CONTEXT_SRR1     412

#define OS_CONTEXT_MODE     416     // only 2 bytes
#define OS_CONTEXT_STATE    418     // only 2 bytes

#define OS_CONTEXT_GQR0     420
#define OS_CONTEXT_GQR1     424
#define OS_CONTEXT_GQR2     428
#define OS_CONTEXT_GQR3     432
#define OS_CONTEXT_GQR4     436
#define OS_CONTEXT_GQR5     440
#define OS_CONTEXT_GQR6     444
#define OS_CONTEXT_GQR7     448
#define __OSCONTEXT_PADDING 452     // double word alignment for the 64 bit psf

#define OS_CONTEXT_PSF0     456
#define OS_CONTEXT_PSF1     464
#define OS_CONTEXT_PSF2     472
#define OS_CONTEXT_PSF3     480
#define OS_CONTEXT_PSF4     488
#define OS_CONTEXT_PSF5     496
#define OS_CONTEXT_PSF6     504
#define OS_CONTEXT_PSF7     512
#define OS_CONTEXT_PSF8     520
#define OS_CONTEXT_PSF9     528
#define OS_CONTEXT_PSF10    536
#define OS_CONTEXT_PSF11    544
#define OS_CONTEXT_PSF12    552
#define OS_CONTEXT_PSF13    560
#define OS_CONTEXT_PSF14    568
#define OS_CONTEXT_PSF15    576
#define OS_CONTEXT_PSF16    584
#define OS_CONTEXT_PSF17    592
#define OS_CONTEXT_PSF18    600
#define OS_CONTEXT_PSF19    608
#define OS_CONTEXT_PSF20    616
#define OS_CONTEXT_PSF21    624
#define OS_CONTEXT_PSF22    632
#define OS_CONTEXT_PSF23    640
#define OS_CONTEXT_PSF24    648
#define OS_CONTEXT_PSF25    656
#define OS_CONTEXT_PSF26    664
#define OS_CONTEXT_PSF27    672
#define OS_CONTEXT_PSF28    680
#define OS_CONTEXT_PSF29    688
#define OS_CONTEXT_PSF30    696
#define OS_CONTEXT_PSF31    704

u32         OSGetStackPointer   ( void );
u32         OSSwitchStack       ( u32 newsp );
int         OSSwitchFiber       ( u32 pc, u32 newsp );
int         OSSwitchFiberEx     ( u32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 pc, u32 newsp );

void        OSSetCurrentContext ( OSContext* context );
OSContext*  OSGetCurrentContext ( void );
u32         OSSaveContext       ( OSContext* context );

void        OSLoadContext       ( OSContext* context );
void        OSClearContext      ( OSContext* context );
void        OSInitContext       ( OSContext* context, u32 pc, u32 sp );

void        OSLoadFPUContext    ( OSContext* context );
void        OSSaveFPUContext    ( OSContext* context );

void        OSFillFPUContext    ( OSContext* context );

void        OSDumpContext       ( OSContext* context );

#ifdef __cplusplus
}
#endif

#endif  // __OSCONTEXT_H__
