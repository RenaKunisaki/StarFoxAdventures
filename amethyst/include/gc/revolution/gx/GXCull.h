/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXCull.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXCull.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2002/07/24 9:06 Hirose
    Resolved version conflicts.
    
    5     1999/10/13 4:32p Alligator
    Changed GXSetViewport, GXSetScissor to use xorig, yorig, wd, ht
    
    4     1999/09/30 10:40p Yasu
    Renamed some GX functions and enums
    
    3     1998/01/05 2:48p Ryan
    Routine updated of header files to match man pages
    
    2     1999/07/28 4:07p Alligator
    Updated header files and emulator for hw changes
    
    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXCULL_H__
#define __GXCULL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

/*---------------------------------------------------------------------------*/
void GXSetScissor 	( u32 left, u32 top, u32 wd, u32 ht );
void GXSetCullMode	( GXCullMode mode );
void GXSetCoPlanar	( GXBool enable );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXCULL_H__
