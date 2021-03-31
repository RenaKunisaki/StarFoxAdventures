/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:	    GXCpu2Efb.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXCpu2Efb.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2002/07/24 9:06 Hirose
    Resolved version conflicts.
    
    3     2000/12/04 6:22p Hirose
    updated prototypes for GXPeekZ/GXPokeZ.
    added GXCompressZ/GXDecompressZ
    
    2     2000/06/08 3:59p Carl
    Made GXPokeZMode similar to GXSetZMode.
    Eliminated GXPokeZUpdate.
    
    1     1999/10/06 2:24a Yasu
    Initial Version
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __GXCPU2EFB_H__
#define __GXCPU2EFB_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

/*---------------------------------------------------------------------------*/
void GXPokeAlphaMode    ( GXCompare func, u8 threshold );
void GXPokeAlphaRead    ( GXAlphaReadMode mode );
void GXPokeAlphaUpdate  ( GXBool update_enable );
void GXPokeBlendMode    ( GXBlendMode type, GXBlendFactor src_factor,
                          GXBlendFactor dst_factor, GXLogicOp op );
void GXPokeColorUpdate  ( GXBool update_enable );
void GXPokeDstAlpha     ( GXBool enable, u8 alpha );
void GXPokeDither       ( GXBool dither );
void GXPokeZMode        ( GXBool compare_enable, GXCompare func,
                          GXBool update_enable );

void GXPokeZ    ( u16 x, u16 y, u32 z );
void GXPeekZ    ( u16 x, u16 y, u32* z );
void GXPokeARGB ( u16 x, u16 y, u32 color );
void GXPeekARGB ( u16 x, u16 y, u32* color );

u32  GXCompressZ16      ( u32 z24, GXZFmt16 zfmt );
u32  GXDecompressZ16    ( u32 z16, GXZFmt16 zfmt );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXCPU2EFB_H__
