/*---------------------------------------------------------------------------*
  Project: Write gather pipe definitions
  File:    PPCWGPipe.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: PPCWGPipe.h,v $
  Revision 1.2  2006/02/04 11:56:45  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    3     2001/05/07 6:45p Tian
    Fixes for SN compiler.
    
    2     2000/11/01 3:55p Shiki
    Removed #ifdef EPPC.

    1     1999/07/14 6:59p Yoshya01
    Initial version.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __PPCWGPIPE_H__
#define __PPCWGPIPE_H__

#ifdef  __cplusplus
extern  "C" {
#endif

#include <revolution/types.h>

/*---------------------------------------------------------------------------*
    PPC Write Gather Pipe

    Write Gather Pipe is defined as:
        PPCWGPipe wgpipe : <Write Gathered Address>;

    Then, used as:
        wgpipe.u8  = 0xff;
        wgpipe.s16 = -5;
        wgpipe.f32 = 0.10f;
 *---------------------------------------------------------------------------*/
typedef union uPPCWGPipe
{
    u8  u8;
    u16 u16;
    u32 u32;
    u64 u64;
    s8  s8;
    s16 s16;
    s32 s32;
    s64 s64;
    f32 f32;
    f64 f64;
} PPCWGPipe;

#ifdef  __cplusplus
}
#endif

#endif  //__PPCWGPIPE_H__
