/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDTexture.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDTexture.h,v $
  Revision 1.3  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.2  2006/02/03 05:26:28  hirose
  Updated for longer memory support.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    3     2001/10/13 2:22a Hirose
    Changed GDSetTexCoordScale function.
    
    2     2001/10/04 4:46p Carl
    Added GDLoadTlut, etc.
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDTEXTURE_H__
#define __GDTEXTURE_H__

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

#define GX_TMEM_HALF  524288
#define GX_TMEM_MAX  1048576

/*---------------------------------------------------------------------------*/

void GDSetTexLookupMode (
    GXTexMapID      id,
    GXTexWrapMode   wrap_s, 
    GXTexWrapMode   wrap_t,
    GXTexFilter     min_filt, 
    GXTexFilter     mag_filt, 
    f32             min_lod, 
    f32             max_lod,
    f32             lod_bias,
    GXBool          bias_clamp,
    GXBool          do_edge_lod, 
    GXAnisotropy    max_aniso );

void GDSetTexImgAttr   ( GXTexMapID id, u16 width, u16 height, GXTexFmt format );
void GDSetTexImgPtr    ( GXTexMapID id, void *image_ptr );
void GDSetTexImgPtrRaw ( GXTexMapID id, u32   image_ptr_raw );
void GDPatchTexImgPtr  ( void *image_ptr );

void GDSetTexTlut ( GXTexMapID id, u32 tmem_addr, GXTlutFmt format );

void GDSetTexCached ( GXTexMapID id,
                      u32 tmem_even, GXTexCacheSize size_even,
                      u32 tmem_odd,  GXTexCacheSize size_odd );

void GDSetTexPreLoaded ( GXTexMapID id, u32 tmem_even, u32 tmem_odd );

void GDSetTexCoordScale ( GXTexCoordID coord, u16 s_scale, u16 t_scale );

void GDSetTexCoordScale2 ( GXTexCoordID coord,
                           u16 s_scale, GXBool s_bias, GXBool s_wrap,
                           u16 t_scale, GXBool t_bias, GXBool t_wrap);

void GDSetTexCoordScaleAndTOEs ( GXTexCoordID coord,
                                 u16 s_scale, GXBool s_bias, GXBool s_wrap,
                                 u16 t_scale, GXBool t_bias, GXBool t_wrap,
                                 GXBool line_offset, GXBool point_offset);

void GDLoadTlut     ( void *tlut_ptr,     u32 tmem_addr, GXTlutSize size );
void GDLoadTlutRaw  ( u32   tlut_ptr_raw, u32 tmem_addr, GXTlutSize size );
static inline void GDPatchTlutPtr ( void *tlut_ptr );

/*---------------------------------------------------------------------------*/

// This uses the same code as GDPatchTexImgPtr:
static inline void GDPatchTlutPtr ( void *tlut_ptr )
{
    GDPatchTexImgPtr( tlut_ptr );
}

/*---------------------------------------------------------------------------*/

// Defines for setting texture registers...

//
//  tx_setmode0 register
//
#define TX_SETMODE0_WRAP_S_SHIFT        0
#define TX_SETMODE0_WRAP_T_SHIFT        2
#define TX_SETMODE0_MAG_FILTER_SHIFT    4
#define TX_SETMODE0_MIN_FILTER_SHIFT    5
#define TX_SETMODE0_DIAGLOD_SHIFT       8
#define TX_SETMODE0_LODBIAS_SHIFT       9
#define TX_SETMODE0_MAXANISO_SHIFT      19
#define TX_SETMODE0_LODCLAMP_SHIFT      21
#define TX_SETMODE0_RID_SHIFT           24
#define TX_SETMODE0(wrap_s, wrap_t, mag_filter, min_filter, diaglod, lodbias, maxaniso, lodclamp, rid) \
        ((((u32)(wrap_s)) << TX_SETMODE0_WRAP_S_SHIFT) | \
         (((u32)(wrap_t)) << TX_SETMODE0_WRAP_T_SHIFT) | \
         (((u32)(mag_filter)) << TX_SETMODE0_MAG_FILTER_SHIFT) | \
         (((u32)(min_filter)) << TX_SETMODE0_MIN_FILTER_SHIFT) | \
         (((u32)(diaglod)) << TX_SETMODE0_DIAGLOD_SHIFT) | \
         (((u32)(lodbias)) << TX_SETMODE0_LODBIAS_SHIFT) | \
         (((u32)(maxaniso)) << TX_SETMODE0_MAXANISO_SHIFT) | \
         (((u32)(lodclamp)) << TX_SETMODE0_LODCLAMP_SHIFT) | \
         (((u32)(rid)) << TX_SETMODE0_RID_SHIFT))

//
//  tx_setmode1 register
//
#define TX_SETMODE1_MINLOD_SHIFT        0
#define TX_SETMODE1_MAXLOD_SHIFT        8
#define TX_SETMODE1_RID_SHIFT           24
#define TX_SETMODE1(minlod, maxlod, rid) \
        ((((u32)(minlod)) << TX_SETMODE1_MINLOD_SHIFT) | \
         (((u32)(maxlod)) << TX_SETMODE1_MAXLOD_SHIFT) | \
         (((u32)(rid)) << TX_SETMODE1_RID_SHIFT))

//
//  tx_setimage0 register
//
#define TX_SETIMAGE0_WIDTH_SHIFT        0
#define TX_SETIMAGE0_HEIGHT_SHIFT       10
#define TX_SETIMAGE0_FORMAT_SHIFT       20
#define TX_SETIMAGE0_RID_SHIFT          24
#define TX_SETIMAGE0(width, height, format, rid) \
        ((((u32)(width)) << TX_SETIMAGE0_WIDTH_SHIFT) | \
         (((u32)(height)) << TX_SETIMAGE0_HEIGHT_SHIFT) | \
         (((u32)(format)) << TX_SETIMAGE0_FORMAT_SHIFT) | \
         (((u32)(rid)) << TX_SETIMAGE0_RID_SHIFT))

//
//  tx_setimage1 register
//
#define TX_SETIMAGE1_TMEM_OFFSET_SHIFT  0
#define TX_SETIMAGE1_CACHE_WIDTH_SHIFT  15
#define TX_SETIMAGE1_CACHE_HEIGHT_SHIFT 18
#define TX_SETIMAGE1_IMAGE_TYPE_SHIFT   21
#define TX_SETIMAGE1_RID_SHIFT          24
#define TX_SETIMAGE1(tmem_offset, cache_width, cache_height, image_type, rid) \
        ((((u32)(tmem_offset)) << TX_SETIMAGE1_TMEM_OFFSET_SHIFT) | \
         (((u32)(cache_width)) << TX_SETIMAGE1_CACHE_WIDTH_SHIFT) | \
         (((u32)(cache_height)) << TX_SETIMAGE1_CACHE_HEIGHT_SHIFT) | \
         (((u32)(image_type)) << TX_SETIMAGE1_IMAGE_TYPE_SHIFT) | \
         (((u32)(rid)) << TX_SETIMAGE1_RID_SHIFT))

//
//  tx_setimage2 register
//
#define TX_SETIMAGE2_TMEM_OFFSET_SHIFT  0
#define TX_SETIMAGE2_CACHE_WIDTH_SHIFT  15
#define TX_SETIMAGE2_CACHE_HEIGHT_SHIFT 18
#define TX_SETIMAGE2_RID_SHIFT          24
#define TX_SETIMAGE2(tmem_offset, cache_width, cache_height, rid) \
        ((((u32)(tmem_offset)) << TX_SETIMAGE2_TMEM_OFFSET_SHIFT) | \
         (((u32)(cache_width)) << TX_SETIMAGE2_CACHE_WIDTH_SHIFT) | \
         (((u32)(cache_height)) << TX_SETIMAGE2_CACHE_HEIGHT_SHIFT) | \
         (((u32)(rid)) << TX_SETIMAGE2_RID_SHIFT))

//
//  tx_setimage3 register
//
#define TX_SETIMAGE3_IMAGE_BASE_SHIFT   0
#define TX_SETIMAGE3_RID_SHIFT          24
#define TX_SETIMAGE3(image_base, rid) \
        ((((u32)(image_base)) << TX_SETIMAGE3_IMAGE_BASE_SHIFT) | \
         (((u32)(rid)) << TX_SETIMAGE3_RID_SHIFT))

//
//  tx_settlut register
//
#define TX_SETTLUT_TMEM_OFFSET_SHIFT    0
#define TX_SETTLUT_FORMAT_SHIFT         10
#define TX_SETTLUT_RID_SHIFT            24
#define TX_SETTLUT(tmem_offset, format, rid) \
        ((((u32)(tmem_offset)) << TX_SETTLUT_TMEM_OFFSET_SHIFT) | \
         (((u32)(format)) << TX_SETTLUT_FORMAT_SHIFT) | \
         (((u32)(rid)) << TX_SETTLUT_RID_SHIFT))

//
//  su_ts0 struct
//
#define SU_TS0_SSIZE_SHIFT              0
#define SU_TS0_BS_SHIFT                 16
#define SU_TS0_WS_SHIFT                 17
#define SU_TS0_LF_SHIFT                 18
#define SU_TS0_PF_SHIFT                 19
#define SU_TS0_RID_SHIFT                24
#define SU_TS0(ssize, bs, ws, lf, pf, rid) \
        ((((u32)(ssize)) << SU_TS0_SSIZE_SHIFT) | \
         (((u32)(bs)) << SU_TS0_BS_SHIFT) | \
         (((u32)(ws)) << SU_TS0_WS_SHIFT) | \
         (((u32)(lf)) << SU_TS0_LF_SHIFT) | \
         (((u32)(pf)) << SU_TS0_PF_SHIFT) | \
         (((u32)(rid)) << SU_TS0_RID_SHIFT))

//
//  su_ts1 struct
//
#define SU_TS1_TSIZE_SHIFT              0
#define SU_TS1_BT_SHIFT                 16
#define SU_TS1_WT_SHIFT                 17
#define SU_TS1_RID_SHIFT                24
#define SU_TS1(tsize, bt, wt, rid) \
        ((((u32)(tsize)) << SU_TS1_TSIZE_SHIFT) | \
         (((u32)(bt)) << SU_TS1_BT_SHIFT) | \
         (((u32)(wt)) << SU_TS1_WT_SHIFT) | \
         (((u32)(rid)) << SU_TS1_RID_SHIFT))

//
//  tx_loadtlut0 struct
//
#define TX_LOADTLUT0_TLUT_BASE_SHIFT    0
#define TX_LOADTLUT0_RID_SHIFT  24
#define TX_LOADTLUT0(tlut_base, rid) \
        ((((unsigned long)(tlut_base)) << TX_LOADTLUT0_TLUT_BASE_SHIFT) | \
         (((unsigned long)(rid)) << TX_LOADTLUT0_RID_SHIFT))

//
//  tx_loadtlut1 struct
//
#define TX_LOADTLUT1_TMEM_OFFSET_SHIFT  0
#define TX_LOADTLUT1_COUNT_SHIFT        10
#define TX_LOADTLUT1_PAD0_SHIFT 21
#define TX_LOADTLUT1_RID_SHIFT  24
#define TX_LOADTLUT1(tmem_offset, count, rid) \
        ((((unsigned long)(tmem_offset)) << TX_LOADTLUT1_TMEM_OFFSET_SHIFT) | \
         (((unsigned long)(count)) << TX_LOADTLUT1_COUNT_SHIFT) | \
         (((unsigned long)(rid)) << TX_LOADTLUT1_RID_SHIFT))


// These register ID's can also be found in GD or GX run-time arrays.
// However, we need these #define's here for compile-time access.
//
// The following are defined per texture ID
//
#define TX_SETMODE0_I0_ID       0x80
#define TX_SETMODE0_I1_ID       0x81
#define TX_SETMODE0_I2_ID       0x82
#define TX_SETMODE0_I3_ID       0x83
#define TX_SETMODE0_I4_ID       0xa0
#define TX_SETMODE0_I5_ID       0xa1
#define TX_SETMODE0_I6_ID       0xa2
#define TX_SETMODE0_I7_ID       0xa3
                                  
#define TX_SETMODE1_I0_ID       0x84
#define TX_SETMODE1_I1_ID       0x85
#define TX_SETMODE1_I2_ID       0x86
#define TX_SETMODE1_I3_ID       0x87
#define TX_SETMODE1_I4_ID       0xa4
#define TX_SETMODE1_I5_ID       0xa5
#define TX_SETMODE1_I6_ID       0xa6
#define TX_SETMODE1_I7_ID       0xa7
                                  
#define TX_SETIMAGE0_I0_ID      0x88
#define TX_SETIMAGE0_I1_ID      0x89
#define TX_SETIMAGE0_I2_ID      0x8a
#define TX_SETIMAGE0_I3_ID      0x8b
#define TX_SETIMAGE0_I4_ID      0xa8
#define TX_SETIMAGE0_I5_ID      0xa9
#define TX_SETIMAGE0_I6_ID      0xaa
#define TX_SETIMAGE0_I7_ID      0xab
                                  
#define TX_SETIMAGE1_I0_ID      0x8c
#define TX_SETIMAGE1_I1_ID      0x8d
#define TX_SETIMAGE1_I2_ID      0x8e
#define TX_SETIMAGE1_I3_ID      0x8f
#define TX_SETIMAGE1_I4_ID      0xac
#define TX_SETIMAGE1_I5_ID      0xad
#define TX_SETIMAGE1_I6_ID      0xae
#define TX_SETIMAGE1_I7_ID      0xaf
                                  
#define TX_SETIMAGE2_I0_ID      0x90
#define TX_SETIMAGE2_I1_ID      0x91
#define TX_SETIMAGE2_I2_ID      0x92
#define TX_SETIMAGE2_I3_ID      0x93
#define TX_SETIMAGE2_I4_ID      0xb0
#define TX_SETIMAGE2_I5_ID      0xb1
#define TX_SETIMAGE2_I6_ID      0xb2
#define TX_SETIMAGE2_I7_ID      0xb3
                                  
#define TX_SETIMAGE3_I0_ID      0x94
#define TX_SETIMAGE3_I1_ID      0x95
#define TX_SETIMAGE3_I2_ID      0x96
#define TX_SETIMAGE3_I3_ID      0x97
#define TX_SETIMAGE3_I4_ID      0xb4
#define TX_SETIMAGE3_I5_ID      0xb5
#define TX_SETIMAGE3_I6_ID      0xb6
#define TX_SETIMAGE3_I7_ID      0xb7
                                  
#define TX_SETTLUT_I0_ID        0x98
#define TX_SETTLUT_I1_ID        0x99
#define TX_SETTLUT_I2_ID        0x9a
#define TX_SETTLUT_I3_ID        0x9b
#define TX_SETTLUT_I4_ID        0xb8
#define TX_SETTLUT_I5_ID        0xb9
#define TX_SETTLUT_I6_ID        0xba
#define TX_SETTLUT_I7_ID        0xbb

// The following are defined per texcoord

#define SU_SSIZE0_ID            0x30
#define SU_TSIZE0_ID            0x31
#define SU_SSIZE1_ID            0x32
#define SU_TSIZE1_ID            0x33
#define SU_SSIZE2_ID            0x34
#define SU_TSIZE2_ID            0x35
#define SU_SSIZE3_ID            0x36
#define SU_TSIZE3_ID            0x37
#define SU_SSIZE4_ID            0x38
#define SU_TSIZE4_ID            0x39
#define SU_SSIZE5_ID            0x3a
#define SU_TSIZE5_ID            0x3b
#define SU_SSIZE6_ID            0x3c
#define SU_TSIZE6_ID            0x3d
#define SU_SSIZE7_ID            0x3e
#define SU_TSIZE7_ID            0x3f

// The following are non-ID specific:

#define TX_LOADTLUT0_ID         0x64
#define TX_LOADTLUT1_ID         0x65
#define BU_IMASK_ID             0x0f

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GDTEXTURE_H__
