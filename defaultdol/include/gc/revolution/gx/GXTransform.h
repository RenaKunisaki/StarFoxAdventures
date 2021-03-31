/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXTransform.h

  Copyright 1998-2002 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXTransform.h,v $
  Revision 1.3  2007/01/11 00:47:44  aka
  Removed win32.h.

  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.


    10    2003/01/10 17:30 Hirose
    Added GXSetZScaleOffset.

    9    2002/07/24 9:07 Hirose
    Resolved version conflicts.

    9    2002/04/09 15:15 Hirose
    const type specifier support. (worked by hiratsu@IRD)

    8     2001/05/08 5:56p Carl
    GXSetScissorBoxOffset now takes signed arguments.

    7    2001/04/17 5:29p Tian
    Changed all inlines to static inline

    6     2000/12/11 5:54p Stevera
    Added conditional #include "win32/win32.h" for pcemu

    5     2000/11/06 4:06p Carl
    Added GXSetClipMode.

    4    2000/10/18 4:05p Carl
    Latest round of HW2 modifications and minor updates.

    3     2000/06/15 1:17p Carl
    Fixed indexed normal matrix load issues.

    2     2000/03/06 11:54a Alligator
    Changed GXSetDefaultMtx to GXSetCurrentMtx

    8    1999/11/18 2:32p Alligator
    Added defines for projection and viewport vector sizes

    7     1999/11/12 1:22p Alligator
    Added GXProject, GXGetViewportv, GXGetProjectionv

    6    1999/10/13 4:32p Alligator
    Changed GXSetViewport, GXSetScissor to use xorig, yorig, wd, ht

    5    1999/09/30 10:40p Yasu
    Renamed some GX functions and enums

    4     1999/09/17 11:24a Ryan
    Changed GXJitterViewport to GXSetViewportJitter

    3     1999/09/01 4:27p Ryan
    Added GXJitterViewport

    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXTRANSFORM_H__
#define __GXTRANSFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

#define GX_PROJECTION_SZ  7
#define GX_VIEWPORT_SZ    6

/*---------------------------------------------------------------------------*/
void GXSetProjection ( const f32 mtx[4][4], GXProjectionType type );
void GXSetProjectionv ( const f32 *ptr );
void GXLoadPosMtxImm ( const f32 mtx[3][4], u32 id );
void GXLoadPosMtxIndx ( u16 mtx_indx, u32 id );
void GXLoadNrmMtxImm ( const f32 mtx[3][4], u32 id );
void GXLoadNrmMtxImm3x3 ( const f32 mtx[3][3], u32 id );
void GXLoadNrmMtxIndx3x3 ( u16 mtx_indx, u32 id );
void GXSetCurrentMtx ( u32 id );
void GXLoadTexMtxImm ( const f32 mtx[][4], u32 id, GXTexMtxType type );
void GXLoadTexMtxIndx ( u16 mtx_indx, u32 id, GXTexMtxType type );

void GXProject ( 
	f32  x,          // Model coordinates
	f32  y,
	f32  z,
	const f32  mtx[3][4],  // Model-view matrix
	const f32* pm,         // Projection matrix, as returned by GXGetProjectionv
	const f32* vp,         // Viewport, as returned by GXGetViewportv
	f32* sx,         // Screen coordinates
	f32* sy,
	f32* sz );

void GXSetViewport(
         f32 left, 
         f32 top, 
         f32 wd, 
         f32 ht, 
         f32 nearz, 
         f32 farz );

static inline void GXSetViewportv( const f32 *vp )
{   // Note: Doesn't check for NULL ptr
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

void GXSetViewportJitter(
         f32 left, 
         f32 top, 
         f32 wd, 
         f32 ht, 
         f32 nearz, 
         f32 farz,
	 u32 field );

void GXSetZScaleOffset  ( f32 scale, f32 offset );

#if ( GX_REV != 1 )
void GXSetScissorBoxOffset( s32 x_off, s32 y_off );
#endif

void GXSetClipMode( GXClipMode mode );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXTRANSFORM_H__
