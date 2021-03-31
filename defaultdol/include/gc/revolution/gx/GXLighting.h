/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXLighting.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXLighting.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    10    2002/07/24 9:07 Hirose
    Resolved version conflicts.
    
    10    2002/04/09 15:15 Hirose
    const type specifier support. (worked by hiratsu@IRD)
    
    9     2001/10/03 9:33a Hirose
    Fixed GXInitSpecularDirHAv macro again.
    
    8     2000/08/21 4:44p Hirose
    Fixed GXInitSpecularDirHAv macro
    
    7     2000/08/21 11:36a Hirose
    Deleted GXInitSpecularHA / added GXInitSpecluarDirHA instead
    
    6     2000/05/12 6:04p Hirose
    Changed definition of GXInit*v macros
    
    5     2000/04/10 3:09p Alligator
    Removed GXInitSpecularHAv macro
    
    4     2000/02/12 5:16p Alligator
    Integrated ArtX source tree changes
    
    3     2000/02/01 7:22p Alligator
    Second GX update from ArtX
    
    2     1999/12/13 4:54p Hirose
    Added GXInitLightShininess macro
    
    9     1999/11/17 4:14p Hirose
    Added macros for vector type
    
    8     1999/10/29 3:35p Hirose
    Replaced GXSetChannels(GXChannelID) to GXSetNumChans(u8)
    
    7     1999/10/28 4:20p Hirose
    Added GXSetChannels
    
    6     1999/10/18 3:49p Hirose
    Changed the type of light_mask on GXSetChanCtrl to u32
    
    5     1999/09/30 10:40p Yasu
    Renamed some GX functions and enums
    
    4     1999/09/09 1:22p Hirose
    Added GXInitLightSpot() and GXInitLightDistAttn()
    
    3     1998/01/05 3:02p Ryan
    Routine update of header file to match man pages
    
    2     1999/07/28 4:07p Alligator
    Updated header files and emulator for hw changes
    
    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXLIGHTING_H__
#define __GXLIGHTING_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

/*---------------------------------------------------------------------------*/
void GXInitLightAttn(
         GXLightObj*   lt_obj,
         f32           a0,
         f32           a1,
         f32           a2,
         f32           k0,
         f32           k1,
         f32           k2 );

void GXInitLightAttnA ( GXLightObj *lt_obj, f32 a0, f32 a1, f32 a2);
void GXInitLightAttnK ( GXLightObj *lt_obj, f32 k0, f32 k1, f32 k2 );

void GXInitLightSpot(
         GXLightObj*   lt_obj,
         f32           cutoff,
         GXSpotFn      spot_func );

void GXInitLightDistAttn(
         GXLightObj*   lt_obj,
         f32           ref_distance,
         f32           ref_brightness,
         GXDistAttnFn  dist_func );


void GXInitLightPos 	( GXLightObj* lt_obj, f32 x, f32 y, f32 z );
void GXInitLightColor 	( GXLightObj* lt_obj, GXColor color );
void GXLoadLightObjImm 	( const GXLightObj* lt_obj, GXLightID light );
void GXLoadLightObjIndx	( u32 lt_obj_indx, GXLightID light );
void GXSetChanAmbColor 	( GXChannelID chan, GXColor amb_color );
void GXSetChanMatColor	( GXChannelID chan, GXColor mat_color );
void GXSetNumChans      ( u8 nChans );

void GXInitLightDir 	( GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz );

// New functions to set light direction and half-angle.
void GXInitSpecularDir 	( GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz );
void GXInitSpecularDirHA(
        GXLightObj* lt_obj,
        f32         nx,
        f32         ny,
        f32         nz,
        f32         hx,
        f32         hy,
        f32         hz );

void GXSetChanCtrl(
        GXChannelID   chan,
        GXBool        enable,
        GXColorSrc    amb_src,
        GXColorSrc    mat_src,
        u32           light_mask,
        GXDiffuseFn   diff_fn,
        GXAttnFn      attn_fn );

/*---------------------------------------------------------------------------*/
// Convenient Macros

#define GXInitLightPosv(lo,vec) \
    (GXInitLightPos((lo), *(f32*)(vec), *((f32*)(vec)+1), *((f32*)(vec)+2)))

#define GXInitLightDirv(lo,vec) \
    (GXInitLightDir((lo), *(f32*)(vec), *((f32*)(vec)+1), *((f32*)(vec)+2)))

#define GXInitSpecularDirv(lo,vec) \
    (GXInitSpecularDir((lo), *(f32*)(vec), *((f32*)(vec)+1), *((f32*)(vec)+2)))

#define GXInitSpecularDirHAv(lo,vec0,vec1) \
    (GXInitSpecularDirHA((lo), \
    *(f32*)(vec0), *((f32*)(vec0)+1), *((f32*)(vec0)+2), \
    *(f32*)(vec1), *((f32*)(vec1)+1), *((f32*)(vec1)+2)))

#define GXInitLightShininess(lobj, shininess) \
    (GXInitLightAttn(lobj, 0.0F, 0.0F, 1.0F,  \
                    (shininess)/2.0F, 0.0F,   \
                    1.0F-(shininess)/2.0F ))


/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXLIGHTING_H__
