/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDGEOMETRY.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDGeometry.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2002/08/05 19:49 Hirose
    Const type specifier support.
    
    3     2001/10/13 2:24a Hirose
    Added GDSetCullMode().
    
    2     2001/09/19 6:35p Carl
    Added GDSetLPSize.
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDGEOMETRY_H__
#define __GDGEOMETRY_H__

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/

void GDSetVtxDescv    ( const GXVtxDescList *attrPtr );

void GDSetVtxAttrFmtv ( GXVtxFmt vtxfmt, const GXVtxAttrFmtList *list );

void GDSetArray       ( GXAttr attr, const void *base_ptr, u8 stride );

void GDSetArrayRaw    ( GXAttr attr, u32 base_ptr_raw, u8 stride );

void GDPatchArrayPtr  ( const void *base_ptr );

void GDSetTexCoordGen ( GXTexCoordID  dst_coord,
                        GXTexGenType  func,
                        GXTexGenSrc   src_param,
                        GXBool        normalize,
                        u32           postmtx );

void GDSetCullMode( GXCullMode cm );

void GDSetGenMode( u8 nTexGens, u8 nChans, u8 nTevs );

void GDSetGenMode2( u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm );



void GDSetLPSize( u8 lineWidth, u8 pointSize,
                  GXTexOffset lineOffset, GXTexOffset pointOffset,
                  GXBool lineHalfAspect );

void GDSetCoPlanar( GXBool enable );

// For GDBegin and GDEnd we have declarations and definitions

static inline void GDBegin ( GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts )
{
    GDWrite_u8((u8) (vtxfmt | type));
    GDWrite_u16(nverts);
}

static inline void GDEnd ( void )
{
}

/*---------------------------------------------------------------------------*/

// Defines for setting CP (and XF and GEN_MODE) registers...

/*
 *  cp_vcd_reg_lo struct
 */
#define CP_VCD_REG_LO_PMIDX_SHIFT   0
#define CP_VCD_REG_LO_T0MIDX_SHIFT  1
#define CP_VCD_REG_LO_T1MIDX_SHIFT  2
#define CP_VCD_REG_LO_T2MIDX_SHIFT  3
#define CP_VCD_REG_LO_T3MIDX_SHIFT  4
#define CP_VCD_REG_LO_T4MIDX_SHIFT  5
#define CP_VCD_REG_LO_T5MIDX_SHIFT  6
#define CP_VCD_REG_LO_T6MIDX_SHIFT  7
#define CP_VCD_REG_LO_T7MIDX_SHIFT  8
#define CP_VCD_REG_LO_POS_SHIFT 9
#define CP_VCD_REG_LO_NRM_SHIFT 11
#define CP_VCD_REG_LO_COL0_SHIFT    13
#define CP_VCD_REG_LO_COL1_SHIFT    15
#define CP_VCD_REG_LO(pmidx, t0midx, t1midx, t2midx, t3midx, t4midx, t5midx, t6midx, t7midx, pos, nrm, col0, col1) \
    ((((unsigned long)(pmidx)) << CP_VCD_REG_LO_PMIDX_SHIFT) | \
     (((unsigned long)(t0midx)) << CP_VCD_REG_LO_T0MIDX_SHIFT) | \
     (((unsigned long)(t1midx)) << CP_VCD_REG_LO_T1MIDX_SHIFT) | \
     (((unsigned long)(t2midx)) << CP_VCD_REG_LO_T2MIDX_SHIFT) | \
     (((unsigned long)(t3midx)) << CP_VCD_REG_LO_T3MIDX_SHIFT) | \
     (((unsigned long)(t4midx)) << CP_VCD_REG_LO_T4MIDX_SHIFT) | \
     (((unsigned long)(t5midx)) << CP_VCD_REG_LO_T5MIDX_SHIFT) | \
     (((unsigned long)(t6midx)) << CP_VCD_REG_LO_T6MIDX_SHIFT) | \
     (((unsigned long)(t7midx)) << CP_VCD_REG_LO_T7MIDX_SHIFT) | \
     (((unsigned long)(pos)) << CP_VCD_REG_LO_POS_SHIFT) | \
     (((unsigned long)(nrm)) << CP_VCD_REG_LO_NRM_SHIFT) | \
     (((unsigned long)(col0)) << CP_VCD_REG_LO_COL0_SHIFT) | \
     (((unsigned long)(col1)) << CP_VCD_REG_LO_COL1_SHIFT))

#define CP_VCD_REG_LO_PS(pmidx, t76543210midx, pos, nrm, col0, col1) \
    ((((unsigned long)(pmidx)) << CP_VCD_REG_LO_PMIDX_SHIFT) | \
     (((unsigned long)(t76543210midx)) << CP_VCD_REG_LO_T0MIDX_SHIFT) | \
     (((unsigned long)(pos)) << CP_VCD_REG_LO_POS_SHIFT) | \
     (((unsigned long)(nrm)) << CP_VCD_REG_LO_NRM_SHIFT) | \
     (((unsigned long)(col0)) << CP_VCD_REG_LO_COL0_SHIFT) | \
     (((unsigned long)(col1)) << CP_VCD_REG_LO_COL1_SHIFT))

/*
 *  cp_vcd_reg_hi struct
 */
#define CP_VCD_REG_HI_TEX0_SHIFT    0
#define CP_VCD_REG_HI_TEX1_SHIFT    2
#define CP_VCD_REG_HI_TEX2_SHIFT    4
#define CP_VCD_REG_HI_TEX3_SHIFT    6
#define CP_VCD_REG_HI_TEX4_SHIFT    8
#define CP_VCD_REG_HI_TEX5_SHIFT    10
#define CP_VCD_REG_HI_TEX6_SHIFT    12
#define CP_VCD_REG_HI_TEX7_SHIFT    14
#define CP_VCD_REG_HI(tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7) \
    ((((unsigned long)(tex0)) << CP_VCD_REG_HI_TEX0_SHIFT) | \
     (((unsigned long)(tex1)) << CP_VCD_REG_HI_TEX1_SHIFT) | \
     (((unsigned long)(tex2)) << CP_VCD_REG_HI_TEX2_SHIFT) | \
     (((unsigned long)(tex3)) << CP_VCD_REG_HI_TEX3_SHIFT) | \
     (((unsigned long)(tex4)) << CP_VCD_REG_HI_TEX4_SHIFT) | \
     (((unsigned long)(tex5)) << CP_VCD_REG_HI_TEX5_SHIFT) | \
     (((unsigned long)(tex6)) << CP_VCD_REG_HI_TEX6_SHIFT) | \
     (((unsigned long)(tex7)) << CP_VCD_REG_HI_TEX7_SHIFT))

/*
 *  cp_vat_reg_a struct
 */
#define CP_VAT_REG_A_POSCNT_SHIFT   0
#define CP_VAT_REG_A_POSFMT_SHIFT   1
#define CP_VAT_REG_A_POSSHFT_SHIFT  4
#define CP_VAT_REG_A_NRMCNT_SHIFT   9
#define CP_VAT_REG_A_NRMFMT_SHIFT   10
#define CP_VAT_REG_A_COL0CNT_SHIFT  13
#define CP_VAT_REG_A_COL0FMT_SHIFT  14
#define CP_VAT_REG_A_COL1CNT_SHIFT  17
#define CP_VAT_REG_A_COL1FMT_SHIFT  18
#define CP_VAT_REG_A_TEX0CNT_SHIFT  21
#define CP_VAT_REG_A_TEX0FMT_SHIFT  22
#define CP_VAT_REG_A_TEX0SHFT_SHIFT 25
#define CP_VAT_REG_A_BYTEDEQUANT_SHIFT  30 /* Should always be 1 */
#define CP_VAT_REG_A_NORMALINDEX3_SHIFT 31
#define CP_VAT_REG_A(posCnt, posFmt, posShft, nrmCnt, nrmFmt, Col0Cnt, Col0Fmt, Col1Cnt, Col1Fmt, tex0Cnt, tex0Fmt, tex0Shft, normalIndex3) \
    ((((unsigned long)(posCnt)) << CP_VAT_REG_A_POSCNT_SHIFT) | \
     (((unsigned long)(posFmt)) << CP_VAT_REG_A_POSFMT_SHIFT) | \
     (((unsigned long)(posShft)) << CP_VAT_REG_A_POSSHFT_SHIFT) | \
     (((unsigned long)(nrmCnt)) << CP_VAT_REG_A_NRMCNT_SHIFT) | \
     (((unsigned long)(nrmFmt)) << CP_VAT_REG_A_NRMFMT_SHIFT) | \
     (((unsigned long)(Col0Cnt)) << CP_VAT_REG_A_COL0CNT_SHIFT) | \
     (((unsigned long)(Col0Fmt)) << CP_VAT_REG_A_COL0FMT_SHIFT) | \
     (((unsigned long)(Col1Cnt)) << CP_VAT_REG_A_COL1CNT_SHIFT) | \
     (((unsigned long)(Col1Fmt)) << CP_VAT_REG_A_COL1FMT_SHIFT) | \
     (((unsigned long)(tex0Cnt)) << CP_VAT_REG_A_TEX0CNT_SHIFT) | \
     (((unsigned long)(tex0Fmt)) << CP_VAT_REG_A_TEX0FMT_SHIFT) | \
     (((unsigned long)(tex0Shft)) << CP_VAT_REG_A_TEX0SHFT_SHIFT) | \
     (((unsigned long)(1))        << CP_VAT_REG_A_BYTEDEQUANT_SHIFT) | \
     (((unsigned long)(normalIndex3)) << CP_VAT_REG_A_NORMALINDEX3_SHIFT))

/*
 *  cp_vat_reg_b struct
 */
#define CP_VAT_REG_B_TEX1CNT_SHIFT  0
#define CP_VAT_REG_B_TEX1FMT_SHIFT  1
#define CP_VAT_REG_B_TEX1SHFT_SHIFT 4
#define CP_VAT_REG_B_TEX2CNT_SHIFT  9
#define CP_VAT_REG_B_TEX2FMT_SHIFT  10
#define CP_VAT_REG_B_TEX2SHFT_SHIFT 13
#define CP_VAT_REG_B_TEX3CNT_SHIFT  18
#define CP_VAT_REG_B_TEX3FMT_SHIFT  19
#define CP_VAT_REG_B_TEX3SHFT_SHIFT 22
#define CP_VAT_REG_B_TEX4CNT_SHIFT  27
#define CP_VAT_REG_B_TEX4FMT_SHIFT  28
#define CP_VAT_REG_B_VCACHE_ENHANCE_SHIFT   31 /* Must always be 1 */
#define CP_VAT_REG_B(tex1Cnt, tex1Fmt, tex1Shft, tex2Cnt, tex2Fmt, tex2Shft, tex3Cnt, tex3Fmt, tex3Shft, tex4Cnt, tex4Fmt) \
    ((((unsigned long)(tex1Cnt)) << CP_VAT_REG_B_TEX1CNT_SHIFT) | \
     (((unsigned long)(tex1Fmt)) << CP_VAT_REG_B_TEX1FMT_SHIFT) | \
     (((unsigned long)(tex1Shft)) << CP_VAT_REG_B_TEX1SHFT_SHIFT) | \
     (((unsigned long)(tex2Cnt)) << CP_VAT_REG_B_TEX2CNT_SHIFT) | \
     (((unsigned long)(tex2Fmt)) << CP_VAT_REG_B_TEX2FMT_SHIFT) | \
     (((unsigned long)(tex2Shft)) << CP_VAT_REG_B_TEX2SHFT_SHIFT) | \
     (((unsigned long)(tex3Cnt)) << CP_VAT_REG_B_TEX3CNT_SHIFT) | \
     (((unsigned long)(tex3Fmt)) << CP_VAT_REG_B_TEX3FMT_SHIFT) | \
     (((unsigned long)(tex3Shft)) << CP_VAT_REG_B_TEX3SHFT_SHIFT) | \
     (((unsigned long)(tex4Cnt)) << CP_VAT_REG_B_TEX4CNT_SHIFT) | \
     (((unsigned long)(tex4Fmt)) << CP_VAT_REG_B_TEX4FMT_SHIFT) | \
     (((unsigned long)(1)) << CP_VAT_REG_B_VCACHE_ENHANCE_SHIFT))

/*
 *  cp_vat_reg_c struct
 */
#define CP_VAT_REG_C_TEX4SHFT_SHIFT 0
#define CP_VAT_REG_C_TEX5CNT_SHIFT  5
#define CP_VAT_REG_C_TEX5FMT_SHIFT  6
#define CP_VAT_REG_C_TEX5SHFT_SHIFT 9
#define CP_VAT_REG_C_TEX6CNT_SHIFT  14
#define CP_VAT_REG_C_TEX6FMT_SHIFT  15
#define CP_VAT_REG_C_TEX6SHFT_SHIFT 18
#define CP_VAT_REG_C_TEX7CNT_SHIFT  23
#define CP_VAT_REG_C_TEX7FMT_SHIFT  24
#define CP_VAT_REG_C_TEX7SHFT_SHIFT 27
#define CP_VAT_REG_C(tex4Shft, tex5Cnt, tex5Fmt, tex5Shft, tex6Cnt, tex6Fmt, tex6Shft, tex7Cnt, tex7Fmt, tex7Shft) \
    ((((unsigned long)(tex4Shft)) << CP_VAT_REG_C_TEX4SHFT_SHIFT) | \
     (((unsigned long)(tex5Cnt)) << CP_VAT_REG_C_TEX5CNT_SHIFT) | \
     (((unsigned long)(tex5Fmt)) << CP_VAT_REG_C_TEX5FMT_SHIFT) | \
     (((unsigned long)(tex5Shft)) << CP_VAT_REG_C_TEX5SHFT_SHIFT) | \
     (((unsigned long)(tex6Cnt)) << CP_VAT_REG_C_TEX6CNT_SHIFT) | \
     (((unsigned long)(tex6Fmt)) << CP_VAT_REG_C_TEX6FMT_SHIFT) | \
     (((unsigned long)(tex6Shft)) << CP_VAT_REG_C_TEX6SHFT_SHIFT) | \
     (((unsigned long)(tex7Cnt)) << CP_VAT_REG_C_TEX7CNT_SHIFT) | \
     (((unsigned long)(tex7Fmt)) << CP_VAT_REG_C_TEX7FMT_SHIFT) | \
     (((unsigned long)(tex7Shft)) << CP_VAT_REG_C_TEX7SHFT_SHIFT))

/*
 *  cp_array_base_reg struct
 */
#define CP_ARRAY_BASE_REG_BASE_SHIFT    0
#define CP_ARRAY_BASE_REG(base) \
    ((((unsigned long)(base)) << CP_ARRAY_BASE_REG_BASE_SHIFT))

/*
 *  cp_array_stride_reg struct
 */
#define CP_ARRAY_STRIDE_REG_STRIDE_SHIFT    0
#define CP_ARRAY_STRIDE_REG(stride) \
    ((((unsigned long)(stride)) << CP_ARRAY_STRIDE_REG_STRIDE_SHIFT))

/*
 *  xf_invtxspec_f struct
 */
#define XF_INVTXSPEC_HOST_COLORS_SHIFT  0
#define XF_INVTXSPEC_HOST_NORMAL_SHIFT  2
#define XF_INVTXSPEC_HOST_TEXTURES_SHIFT    4
#define XF_INVTXSPEC(host_colors, host_normal, host_textures) \
    ((((unsigned long)(host_colors)) << XF_INVTXSPEC_HOST_COLORS_SHIFT) | \
     (((unsigned long)(host_normal)) << XF_INVTXSPEC_HOST_NORMAL_SHIFT) | \
     (((unsigned long)(host_textures)) << XF_INVTXSPEC_HOST_TEXTURES_SHIFT))

/*
 *  xf_numcolors_f struct
 */
#define XF_NUMCOLORS_SHIFT  0
#define XF_NUMCOLORS(numcolors) \
    ((((unsigned long)(numcolors)) << XF_NUMCOLORS_SHIFT))

/*
 *  xf_numtex_f struct
 */
#define XF_NUMTEX_SHIFT 0
#define XF_NUMTEX(numtex) \
    ((((unsigned long)(numtex)) << XF_NUMTEX_SHIFT))

/*
 *  xf_tex struct (texgen)
 */
#define XF_TEX_PROJECTION_SHIFT 1
#define XF_TEX_INPUT_FORM_SHIFT 2
#define XF_TEX_TEXGEN_TYPE_SHIFT    4
#define XF_TEX_SOURCE_ROW_SHIFT 7
#define XF_TEX_EMBOSS_SOURCE_SHIFT  12
#define XF_TEX_EMBOSS_LIGHT_SHIFT   15
#define XF_TEX(projection, input_form, texgen_type, source_row, emboss_source, emboss_light) \
    ((((unsigned long)(projection)) << XF_TEX_PROJECTION_SHIFT) | \
     (((unsigned long)(input_form)) << XF_TEX_INPUT_FORM_SHIFT) | \
     (((unsigned long)(texgen_type)) << XF_TEX_TEXGEN_TYPE_SHIFT) | \
     (((unsigned long)(source_row)) << XF_TEX_SOURCE_ROW_SHIFT) | \
     (((unsigned long)(emboss_source)) << XF_TEX_EMBOSS_SOURCE_SHIFT) | \
     (((unsigned long)(emboss_light)) << XF_TEX_EMBOSS_LIGHT_SHIFT))

/*
 *  xf_dualtex_f struct
 */
#define XF_DUALTEX_DUALMTX_SHIFT    0
#define XF_DUALTEX_NORMAL_ENABLE_SHIFT  8
#define XF_DUALTEX(dualmtx, normal_enable) \
    ((((unsigned long)(dualmtx)) << XF_DUALTEX_DUALMTX_SHIFT) | \
     (((unsigned long)(normal_enable)) << XF_DUALTEX_NORMAL_ENABLE_SHIFT))

/*
 *  gen_mode struct
 */
#define GEN_MODE_NTEX_SHIFT 0
#define GEN_MODE_NCOL_SHIFT 4
#define GEN_MODE_MS_EN_SHIFT    9
#define GEN_MODE_NTEV_SHIFT 10
#define GEN_MODE_REJECT_EN_SHIFT    14
#define GEN_MODE_NBMP_SHIFT 16
#define GEN_MODE_ZFREEZE_SHIFT  19
#define GEN_MODE_RID_SHIFT  24
#define GEN_MODE(ntex, ncol, ms_en, ntev, reject_en, nbmp, zfreeze, rid) \
    ((((unsigned long)(ntex)) << GEN_MODE_NTEX_SHIFT) | \
     (((unsigned long)(ncol)) << GEN_MODE_NCOL_SHIFT) | \
     (((unsigned long)(ms_en)) << GEN_MODE_MS_EN_SHIFT) | \
     (((unsigned long)(ntev)) << GEN_MODE_NTEV_SHIFT) | \
     (((unsigned long)(reject_en)) << GEN_MODE_REJECT_EN_SHIFT) | \
     (((unsigned long)(nbmp)) << GEN_MODE_NBMP_SHIFT) | \
     (((unsigned long)(zfreeze)) << GEN_MODE_ZFREEZE_SHIFT) | \
     (((unsigned long)(rid)) << GEN_MODE_RID_SHIFT))

/*
 *  su_lpsize struct
 */
#define SU_LPSIZE_LSIZE_SHIFT   0
#define SU_LPSIZE_PSIZE_SHIFT   8
#define SU_LPSIZE_LTOFF_SHIFT   16
#define SU_LPSIZE_PTOFF_SHIFT   19
#define SU_LPSIZE_LINEASPECT_SHIFT  22
#define SU_LPSIZE_PAD0_SHIFT    23
#define SU_LPSIZE_RID_SHIFT 24
#define SU_LPSIZE(lsize, psize, ltoff, ptoff, lineaspect, rid) \
    ((((unsigned long)(lsize)) << SU_LPSIZE_LSIZE_SHIFT) | \
     (((unsigned long)(psize)) << SU_LPSIZE_PSIZE_SHIFT) | \
     (((unsigned long)(ltoff)) << SU_LPSIZE_LTOFF_SHIFT) | \
     (((unsigned long)(ptoff)) << SU_LPSIZE_PTOFF_SHIFT) | \
     (((unsigned long)(lineaspect)) << SU_LPSIZE_LINEASPECT_SHIFT) | \
     (((unsigned long)(rid)) << SU_LPSIZE_RID_SHIFT))

/*
 *  CP register addresses (CP address space)
 */
#define CP_MATINDEX_A_ID    0x30
#define CP_MATINDEX_B_ID    0x40
#define CP_VCD_LO_ID    0x50
#define CP_VCD_HI_ID    0x60
#define CP_VAT_A_ID 0x70
#define CP_VAT_B_ID 0x80
#define CP_VAT_C_ID 0x90
#define CP_ARRAY_BASE_ID        0xa0
#define CP_ARRAY_STRIDE_ID  0xb0

/*
 *  XF register addresses (XF address space)
 */
#define XF_INVTXSPEC_ID 0x1008
#define XF_NUMCOLORS_ID 0x1009
#define XF_NUMTEX_ID    0x103f
#define XF_TEX0_ID  0x1040
#define XF_TEX1_ID  0x1041
#define XF_TEX2_ID  0x1042
#define XF_TEX3_ID  0x1043
#define XF_TEX4_ID  0x1044
#define XF_TEX5_ID  0x1045
#define XF_TEX6_ID  0x1046
#define XF_TEX7_ID  0x1047
#define XF_DUALTEX0_ID  0x1050
#define XF_DUALTEX1_ID  0x1051
#define XF_DUALTEX2_ID  0x1052
#define XF_DUALTEX3_ID  0x1053
#define XF_DUALTEX4_ID  0x1054
#define XF_DUALTEX5_ID  0x1055
#define XF_DUALTEX6_ID  0x1056
#define XF_DUALTEX7_ID  0x1057

/*
 *  GEN_MODE register address (BP address space)
 */
#define GEN_MODE_ID 0x00000000

/*
 *  SU register addresses (BP address space)
 */
#define SU_LPSIZE_ID    0x00000022

/*
 *  XF register field definitions
 */

//  xf_tex_projection_f enum
#define XF_TEX_ST   0x00000000
#define XF_TEX_STQ  0x00000001

//  xf_tex_input_form_f enum
#define XF_TEX_AB11 0x00000000
#define XF_TEX_ABC1 0x00000001

//  xf_tex_texgen_type_f enum
#define XF_TEXGEN_REGULAR   0x00000000
#define XF_TEXGEN_EMBOSS_MAP    0x00000001
#define XF_TEXGEN_COLOR_STRGBC0 0x00000002
#define XF_TEXGEN_COLOR_STRGBC1 0x00000003

//  xf_tex_source_row_f enum
#define XF_GEOM_INROW   0x00000000
#define XF_NORMAL_INROW 0x00000001
#define XF_COLORS_INROW 0x00000002
#define XF_BINORMAL_T_INROW 0x00000003
#define XF_BINORMAL_B_INROW 0x00000004
#define XF_TEX0_INROW   0x00000005
#define XF_TEX1_INROW   0x00000006
#define XF_TEX2_INROW   0x00000007
#define XF_TEX3_INROW   0x00000008
#define XF_TEX4_INROW   0x00000009
#define XF_TEX5_INROW   0x0000000a
#define XF_TEX6_INROW   0x0000000b
#define XF_TEX7_INROW   0x0000000c

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GDGEOMETRY_H__
