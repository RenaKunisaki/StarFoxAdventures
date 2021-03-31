/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXGet.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXGet.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    9     2002/07/24 9:07 Hirose
    Resolved version conflicts.
    
    9     2002/04/09 15:15 Hirose
    const type specifier support. (worked by hiratsu@IRD)
    
    8     2001/06/25 7:06p Hirose
    Removed prototype of GXGetArray.
    
    7     2001/01/25 2:22p Carl
    Added GXGetTexObjTlut.
    
    6     2001/01/24 7:19p Carl
    Fixes for GXGetTexRegionAll.
    
    5     2001/01/24 4:28p Carl
    New texture init and get functions added.
    
    4     2000/10/18 4:05p Carl
    Latest round of HW2 modifications and minor updates.
    
    3     2000/05/12 6:05p Hirose
    Changed definition of GXGetLight*v macros
    
    2     2000/04/11 1:03p Alligator
    Added GXGetLight* functions
    
    8     1999/11/18 2:50p Hirose
    Added GXGetTlutObj* functions
    
    7     1999/11/17 10:26p Hirose
    Added GXGetTexObj* functions
    
    6     1999/11/12 1:22p Alligator
    Added GXProject, GXGetViewportv, GXGetProjectionv
    
    5     1999/11/08 9:14a Alligator
    added GXGetTexObj
    
    4     1999/09/30 10:40p Yasu
    Renamed some GX functions and enums
    
    3     1999/09/14 10:36a Ryan
    Changed GXGetViewport to take all floats
    
    2     1999/07/20 6:10p Alligator
    Added GXGetVtxDescv, GXGetVtxAttrFmtv
    
    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXGET_H__
#define __GXGET_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

/*---------------------------------------------------------------------------* 
      GEOMETRY AND VERTEX FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetVtxDesc            ( GXAttr attr, GXAttrType *type );
void GXGetVtxDescv           ( GXVtxDescList *vcd );
void GXGetVtxAttrFmtv        ( GXVtxFmt fmt, GXVtxAttrFmtList *vat );

void GXGetLineWidth          ( u8 *width, GXTexOffset *texOffsets );
void GXGetPointSize          ( u8 *pointSize, GXTexOffset *texOffsets );

void GXGetVtxAttrFmt( 
    GXVtxFmt       idx, 
    GXAttr         attr, 
    GXCompCnt*     compCnt, 
    GXCompType*    compType, 
    u8*            shift );

/*---------------------------------------------------------------------------* 
      TRANSFORMATION AND MATRIX FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetViewportv( f32* viewport );  // used by GXProject
void GXGetProjectionv( f32* p );       // used by GXProject

/*---------------------------------------------------------------------------* 
      CLIPPING AND CULLING FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetScissor( 
    u32*    left, 
    u32*    top, 
    u32*    width, 
    u32*    height );

void GXGetCullMode( GXCullMode *mode );

/*---------------------------------------------------------------------------* 
      LIGHTING OBJECT ACCESS FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetLightAttnA(
        const GXLightObj* lt_obj,
        f32*        a0,
        f32*        a1,
        f32*        a2 );

void GXGetLightAttnK(
        const GXLightObj* lt_obj,
        f32*        k0,
        f32*        k1,
        f32*        k2 );

void GXGetLightPos(
        const GXLightObj* lt_obj,
        f32*        x,
        f32*        y,
        f32*        z );

#define GXGetLightPosv(lo, vec) \
 (GXGetLightPos((lo), (f32*)(vec), (f32*)(vec)+1, (f32*)(vec)+2))

void GXGetLightDir(
        const GXLightObj* lt_obj,
        f32*        nx,
        f32*        ny,
        f32*        nz );

#define GXGetLightDirv(lo, vec) \
 (GXGetLightDir((lo), (f32*)(vec), (f32*)(vec)+1, (f32*)(vec)+2))

void GXGetLightColor(
        const GXLightObj* lt_obj,
        GXColor*    color );

/*---------------------------------------------------------------------------* 
      TEXTURE OBJECT ACCESS FUNCTIONS
 *---------------------------------------------------------------------------*/
void*         GXGetTexObjData  ( const GXTexObj* tex_obj );
u16           GXGetTexObjWidth ( const GXTexObj* tex_obj );
u16           GXGetTexObjHeight( const GXTexObj* tex_obj );
GXTexFmt      GXGetTexObjFmt   ( const GXTexObj* tex_obj );
GXTexWrapMode GXGetTexObjWrapS ( const GXTexObj* tex_obj );
GXTexWrapMode GXGetTexObjWrapT ( const GXTexObj* tex_obj );
GXBool        GXGetTexObjMipMap( const GXTexObj* tex_obj );     

void GXGetTexObjAll(
        const GXTexObj* tex_obj, 
        void**          data, 
        u16*            width, 
        u16*            height, 
        GXTexFmt*       format,
        GXTexWrapMode*  wrapS, 
        GXTexWrapMode*  wrapT,
        GXBool*         mipMap );

GXTexFilter   GXGetTexObjMinFilt  ( const GXTexObj* tex_obj );
GXTexFilter   GXGetTexObjMagFilt  ( const GXTexObj* tex_obj );
f32           GXGetTexObjMinLOD   ( const GXTexObj* tex_obj );
f32           GXGetTexObjMaxLOD   ( const GXTexObj* tex_obj );
f32           GXGetTexObjLODBias  ( const GXTexObj* tex_obj );
GXBool        GXGetTexObjBiasClamp( const GXTexObj* tex_obj );
GXBool        GXGetTexObjEdgeLOD  ( const GXTexObj* tex_obj );
GXAnisotropy  GXGetTexObjMaxAniso ( const GXTexObj* tex_obj );

void GXGetTexObjLODAll (
        const GXTexObj* tex_obj,
        GXTexFilter*    min_filt,
        GXTexFilter*    mag_filt,
        f32*            min_lod,
        f32*            max_lod,
        f32*            lod_bias,
        GXBool*         bias_clamp,
        GXBool*         do_edge_lod,
        GXAnisotropy*   max_aniso );

u32 GXGetTexObjTlut( const GXTexObj* tex_obj );

void*         GXGetTlutObjData      ( const GXTlutObj* tlut_obj );
GXTlutFmt     GXGetTlutObjFmt       ( const GXTlutObj* tlut_obj );
u16           GXGetTlutObjNumEntries( const GXTlutObj* tlut_obj );

void GXGetTlutObjAll (
        const GXTlutObj*      tlut_obj,
        void**          data,
        GXTlutFmt*      format,
        u16*            numEntries );

void GXGetTexRegionAll (
        const GXTexRegion*    region,
        GXBool*         is_cached,
        GXBool*         is_32b_mipmap,
        u32*            tmem_even,
        u32*            size_even,
        u32*            tmem_odd,
        u32*            size_odd );

void GXGetTlutRegionAll (
        const GXTlutRegion*   region,
        u32*            tmem_addr,
        GXTlutSize*     tlut_size );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXGET_H__
