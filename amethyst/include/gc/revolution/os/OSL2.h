/*---------------------------------------------------------------------------*
  Project: OS - Low Level L2 Cache Operations Library
  File:    OSL2.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSL2.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    3     1999/05/11 4:43p Shiki
    Refreshed include tree.

    1     1999/04/30 12:49p Tianli01

    1     1999/03/04 2:23p Tianli01
    Initial check-in to new tree
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*
    This header file defines the low level L2 cache operations.
    Most should be macros to the underlying assembly routines, but
    others will manipulate hardware setup registers.
 */

#ifndef __OSL2_H__
#define __OSL2_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void L2Enable           ( void );
void L2Disable          ( void );
void L2GlobalInvalidate ( void );
void L2SetDataOnly      ( BOOL dataOnly );
void L2SetWriteThrough  ( BOOL writeThrough );

#ifdef __cplusplus
}
#endif

#endif  // __OSL2_H__
