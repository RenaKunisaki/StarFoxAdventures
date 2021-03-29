/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXPixel.h

  Copyright 1998- 2003 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXPixel.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    6     2003/09/26 16:56 Hirose
    Added GXSetFogColor().
    
    5     2002/07/24 9:07 Hirose
    Resolved version conflicts.
    
    5     2002/04/09 15:15 Hirose
    const type specifier support. (worked by hiratsu@IRD)
    
    4     2000/04/26 4:09p Alligator
    Added parameter to GXSetFieldMode
    
    3     2000/04/19 2:38p Carl
    Added GXSetFieldMode (ArtX update).
    
    2     1999/12/10 4:53p Carl
    Fixed Z compare stuff.
    
    10    1999/10/13 5:19p Alligator
    Changed GXSetMotionComp to GXSetFieldMask
    
    9     1999/10/04 2:42p Yasu
    Added GXInitFogAdjTable
    
    8     1999/09/30 10:40p Yasu
    Renamed some GX functions and enums
    
    7     1999/09/29 6:29p Alligator
    Added width parameter to GXSetFogRangeAdj
    
    6     1999/09/02 10:57a Ryan
    
    5     1999/09/01 5:45p Ryan
    
    4     1999/08/27 2:25p Yasu
    Changed a parameter of GXSetBlendMode, GXSetZMode.
    Appended GXSetZUpdate, GXSetAlphaUpdate and GXSetDither.
    
    3     1999/08/26 2:37p Yasu
    Deletef=d a parameter of density in GXSetFog().
    Changed a function name GXSetColorMode -> GXSetBlendMode
    
    2     1999/07/28 4:07p Alligator
    Updated header files and emulator for hw changes
    
    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXPIXEL_H__
#define __GXPIXEL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

/*---------------------------------------------------------------------------*/
void GXSetFog(	GXFogType	type,
		f32		startz,
		f32		endz,
		f32		nearz,
		f32		farz,
		GXColor		color );

void GXSetFogColor( GXColor color );

void GXInitFogAdjTable( GXFogAdjTable* table, u16 width, const f32 projmtx[4][4] );

void GXSetFogRangeAdj( GXBool enable, u16 center, const GXFogAdjTable* table );

void GXSetBlendMode (	GXBlendMode	type,
			GXBlendFactor	src_factor, 
			GXBlendFactor	dst_factor, 
			GXLogicOp	op );

void GXSetColorUpdate( GXBool update_enable );
void GXSetAlphaUpdate( GXBool update_enable );
void GXSetZMode(	GXBool		compare_enable,
			GXCompare	func,
			GXBool		update_enable );

void GXSetZCompLoc( GXBool before_tex );
void GXSetPixelFmt ( GXPixelFmt pix_fmt, GXZFmt16 z_fmt );
void GXSetDither( GXBool dither );
void GXSetDstAlpha( GXBool enable, u8 alpha );
void GXSetFieldMask( GXBool odd_mask, GXBool even_mask );
void GXSetFieldMode( GXBool field_mode, GXBool half_aspect_ratio );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif // __GXPIXEL_H__

#endif
