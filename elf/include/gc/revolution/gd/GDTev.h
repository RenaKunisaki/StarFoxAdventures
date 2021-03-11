/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDTEV.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDTev.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2001/09/21 1:48p Carl
    Changed order of args for GDSetKonstantSel.
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDTEV_H__
#define __GDTEV_H__

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

void GDSetTevOp(GXTevStageID stage, GXTevMode mode);

void GDSetTevColorCalc(
    GXTevStageID    stage,
    GXTevColorArg   a,
    GXTevColorArg   b,
    GXTevColorArg   c,
    GXTevColorArg   d,
    GXTevOp         op,
    GXTevBias       bias,
    GXTevScale      scale,
    GXBool          clamp,
    GXTevRegID      out_reg );

void GDSetTevAlphaCalcAndSwap(
    GXTevStageID    stage,
    GXTevAlphaArg   a,
    GXTevAlphaArg   b,
    GXTevAlphaArg   c,
    GXTevAlphaArg   d,
    GXTevOp         op,
    GXTevBias       bias,
    GXTevScale      scale,
    GXBool          clamp,
    GXTevRegID      out_reg,
    GXTevSwapSel    ras_sel,
    GXTevSwapSel    tex_sel );

void GDSetTevColor   ( GXTevRegID reg, GXColor color );
void GDSetTevColorS10( GXTevRegID reg, GXColorS10 color );
void GDSetTevKColor  ( GXTevKColorID reg, GXColor color );

// This function sets two stages at once: the one you specify
// must be an even stage number.
void GDSetTevKonstantSel( GXTevStageID evenStage, 
                          GXTevKColorSel kcsel0, GXTevKAlphaSel kasel0,
                          GXTevKColorSel kcsel1, GXTevKAlphaSel kasel1 );

void GDSetTevSwapModeTable(
    GXTevSwapSel    table,
    GXTevColorChan  red,
    GXTevColorChan  green,
    GXTevColorChan  blue,
    GXTevColorChan  alpha );

// Alpha compare and Z texture select are part of the TEV unit
void GDSetAlphaCompare(
    GXCompare       comp0,
    u8              ref0,
    GXAlphaOp       op,
    GXCompare       comp1,
    u8              ref1 );

void GDSetZTexture ( GXZTexOp op, GXTexFmt fmt, u32 bias );

// This function sets two stages at once: the one you specify
// must be an even stage number.
void GDSetTevOrder(
    GXTevStageID    evenStage, 
    GXTexCoordID    coord0,
    GXTexMapID      map0,
    GXChannelID     color0,
    GXTexCoordID    coord1,
    GXTexMapID      map1,
    GXChannelID     color1 );

/*---------------------------------------------------------------------------*/

// Defines for setting TEV registers...

/*
 *  tev_color_env struct
 */
#define TEV_COLOR_ENV_SELD_SHIFT	0
#define TEV_COLOR_ENV_SELC_SHIFT	4
#define TEV_COLOR_ENV_SELB_SHIFT	8
#define TEV_COLOR_ENV_SELA_SHIFT	12
#define TEV_COLOR_ENV_BIAS_SHIFT	16
#define TEV_COLOR_ENV_SUB_SHIFT	18
#define TEV_COLOR_ENV_CLAMP_SHIFT	19
#define TEV_COLOR_ENV_SHIFT_SHIFT	20
#define TEV_COLOR_ENV_DEST_SHIFT	22
#define TEV_COLOR_ENV_RID_SHIFT	24
#define TEV_COLOR_ENV(seld, selc, selb, sela, bias, sub, clamp, shift, dest, rid) \
	((((unsigned long)(seld)) << TEV_COLOR_ENV_SELD_SHIFT) | \
	 (((unsigned long)(selc)) << TEV_COLOR_ENV_SELC_SHIFT) | \
	 (((unsigned long)(selb)) << TEV_COLOR_ENV_SELB_SHIFT) | \
	 (((unsigned long)(sela)) << TEV_COLOR_ENV_SELA_SHIFT) | \
	 (((unsigned long)(bias)) << TEV_COLOR_ENV_BIAS_SHIFT) | \
	 (((unsigned long)(sub)) << TEV_COLOR_ENV_SUB_SHIFT) | \
	 (((unsigned long)(clamp)) << TEV_COLOR_ENV_CLAMP_SHIFT) | \
	 (((unsigned long)(shift)) << TEV_COLOR_ENV_SHIFT_SHIFT) | \
	 (((unsigned long)(dest)) << TEV_COLOR_ENV_DEST_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_COLOR_ENV_RID_SHIFT))

/*
 *  tev_alpha_env struct
 */
#define TEV_ALPHA_ENV_RSWAP_SHIFT	0
#define TEV_ALPHA_ENV_TSWAP_SHIFT	2
#define TEV_ALPHA_ENV_SELD_SHIFT	4
#define TEV_ALPHA_ENV_SELC_SHIFT	7
#define TEV_ALPHA_ENV_SELB_SHIFT	10
#define TEV_ALPHA_ENV_SELA_SHIFT	13
#define TEV_ALPHA_ENV_BIAS_SHIFT	16
#define TEV_ALPHA_ENV_SUB_SHIFT	18
#define TEV_ALPHA_ENV_CLAMP_SHIFT	19
#define TEV_ALPHA_ENV_SHIFT_SHIFT	20
#define TEV_ALPHA_ENV_DEST_SHIFT	22
#define TEV_ALPHA_ENV_RID_SHIFT	24
#define TEV_ALPHA_ENV(rswap, tswap, seld, selc, selb, sela, bias, sub, clamp, shift, dest, rid) \
	((((unsigned long)(rswap)) << TEV_ALPHA_ENV_RSWAP_SHIFT) | \
	 (((unsigned long)(tswap)) << TEV_ALPHA_ENV_TSWAP_SHIFT) | \
	 (((unsigned long)(seld)) << TEV_ALPHA_ENV_SELD_SHIFT) | \
	 (((unsigned long)(selc)) << TEV_ALPHA_ENV_SELC_SHIFT) | \
	 (((unsigned long)(selb)) << TEV_ALPHA_ENV_SELB_SHIFT) | \
	 (((unsigned long)(sela)) << TEV_ALPHA_ENV_SELA_SHIFT) | \
	 (((unsigned long)(bias)) << TEV_ALPHA_ENV_BIAS_SHIFT) | \
	 (((unsigned long)(sub)) << TEV_ALPHA_ENV_SUB_SHIFT) | \
	 (((unsigned long)(clamp)) << TEV_ALPHA_ENV_CLAMP_SHIFT) | \
	 (((unsigned long)(shift)) << TEV_ALPHA_ENV_SHIFT_SHIFT) | \
	 (((unsigned long)(dest)) << TEV_ALPHA_ENV_DEST_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_ALPHA_ENV_RID_SHIFT))

/*
 *  tev_registerl struct
 */
#define TEV_REGISTERL_R_SHIFT	0
#define TEV_REGISTERL_A_SHIFT	12
#define TEV_REGISTERL_TYPE_SHIFT	23
#define TEV_REGISTERL_RID_SHIFT	24
#define TEV_REGISTERL(r, a, t, rid) \
	((((unsigned long)(r)) << TEV_REGISTERL_R_SHIFT) | \
	 (((unsigned long)(a)) << TEV_REGISTERL_A_SHIFT) | \
	 (((unsigned long)(t)) << TEV_REGISTERL_TYPE_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_REGISTERL_RID_SHIFT))

/*
 *  tev_registerh struct
 */
#define TEV_REGISTERH_B_SHIFT	0
#define TEV_REGISTERH_G_SHIFT	12
#define TEV_REGISTERH_TYPE_SHIFT	23
#define TEV_REGISTERH_RID_SHIFT	24
#define TEV_REGISTERH(b, g, t, rid) \
	((((unsigned long)(b)) << TEV_REGISTERH_B_SHIFT) | \
	 (((unsigned long)(g)) << TEV_REGISTERH_G_SHIFT) | \
	 (((unsigned long)(t)) << TEV_REGISTERH_TYPE_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_REGISTERH_RID_SHIFT))

/*
 *  tev_alphafunc struct
 */
#define TEV_ALPHAFUNC_A0_SHIFT	0
#define TEV_ALPHAFUNC_A1_SHIFT	8
#define TEV_ALPHAFUNC_OP0_SHIFT	16
#define TEV_ALPHAFUNC_OP1_SHIFT	19
#define TEV_ALPHAFUNC_LOGIC_SHIFT	22
#define TEV_ALPHAFUNC_RID_SHIFT	24
#define TEV_ALPHAFUNC(a0, a1, op0, op1, logic, rid) \
	((((unsigned long)(a0)) << TEV_ALPHAFUNC_A0_SHIFT) | \
	 (((unsigned long)(a1)) << TEV_ALPHAFUNC_A1_SHIFT) | \
	 (((unsigned long)(op0)) << TEV_ALPHAFUNC_OP0_SHIFT) | \
	 (((unsigned long)(op1)) << TEV_ALPHAFUNC_OP1_SHIFT) | \
	 (((unsigned long)(logic)) << TEV_ALPHAFUNC_LOGIC_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_ALPHAFUNC_RID_SHIFT))

/*
 *  tev_z_env_0 struct
 */
#define TEV_Z_ENV_0_ZOFF_SHIFT	0
#define TEV_Z_ENV_0_RID_SHIFT	24
#define TEV_Z_ENV_0(zoff, rid) \
	((((unsigned long)(zoff)) << TEV_Z_ENV_0_ZOFF_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_Z_ENV_0_RID_SHIFT))

/*
 *  tev_z_env_1 struct
 */
#define TEV_Z_ENV_1_TYPE_SHIFT	0
#define TEV_Z_ENV_1_OP_SHIFT	2
#define TEV_Z_ENV_1_RID_SHIFT	24
#define TEV_Z_ENV_1(type, op, rid) \
	((((unsigned long)(type)) << TEV_Z_ENV_1_TYPE_SHIFT) | \
	 (((unsigned long)(op)) << TEV_Z_ENV_1_OP_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_Z_ENV_1_RID_SHIFT))

/*
 *  tev_ksel struct
 */
#define TEV_KSEL_XRB_SHIFT	0
#define TEV_KSEL_XGA_SHIFT	2
#define TEV_KSEL_KCSEL0_SHIFT	4
#define TEV_KSEL_KASEL0_SHIFT	9
#define TEV_KSEL_KCSEL1_SHIFT	14
#define TEV_KSEL_KASEL1_SHIFT	19
#define TEV_KSEL_RID_SHIFT	24
#define TEV_KSEL(xrb, xga, kcsel0, kasel0, kcsel1, kasel1, rid) \
	((((unsigned long)(xrb)) << TEV_KSEL_XRB_SHIFT) | \
	 (((unsigned long)(xga)) << TEV_KSEL_XGA_SHIFT) | \
	 (((unsigned long)(kcsel0)) << TEV_KSEL_KCSEL0_SHIFT) | \
	 (((unsigned long)(kasel0)) << TEV_KSEL_KASEL0_SHIFT) | \
	 (((unsigned long)(kcsel1)) << TEV_KSEL_KCSEL1_SHIFT) | \
	 (((unsigned long)(kasel1)) << TEV_KSEL_KASEL1_SHIFT) | \
	 (((unsigned long)(rid)) << TEV_KSEL_RID_SHIFT))

/*
 *  ras1_tref struct
 */
#define RAS1_TREF_TI0_SHIFT	0
#define RAS1_TREF_TC0_SHIFT	3
#define RAS1_TREF_TE0_SHIFT	6
#define RAS1_TREF_CC0_SHIFT	7
#define RAS1_TREF_PAD0_SHIFT	10
#define RAS1_TREF_TI1_SHIFT	12
#define RAS1_TREF_TC1_SHIFT	15
#define RAS1_TREF_TE1_SHIFT	18
#define RAS1_TREF_CC1_SHIFT	19
#define RAS1_TREF_PAD1_SHIFT	22
#define RAS1_TREF_RID_SHIFT	24
#define RAS1_TREF(ti0, tc0, te0, cc0, ti1, tc1, te1, cc1, rid) \
	((((unsigned long)(ti0)) << RAS1_TREF_TI0_SHIFT) | \
	 (((unsigned long)(tc0)) << RAS1_TREF_TC0_SHIFT) | \
	 (((unsigned long)(te0)) << RAS1_TREF_TE0_SHIFT) | \
	 (((unsigned long)(cc0)) << RAS1_TREF_CC0_SHIFT) | \
	 (((unsigned long)(ti1)) << RAS1_TREF_TI1_SHIFT) | \
	 (((unsigned long)(tc1)) << RAS1_TREF_TC1_SHIFT) | \
	 (((unsigned long)(te1)) << RAS1_TREF_TE1_SHIFT) | \
	 (((unsigned long)(cc1)) << RAS1_TREF_CC1_SHIFT) | \
	 (((unsigned long)(rid)) << RAS1_TREF_RID_SHIFT))

/*
 *  TEV register addresses (BP address space)
 */
#define TEV_COLOR_ENV_0_ID	0x000000c0
#define TEV_ALPHA_ENV_0_ID	0x000000c1
#define TEV_COLOR_ENV_1_ID	0x000000c2
#define TEV_ALPHA_ENV_1_ID	0x000000c3
#define TEV_COLOR_ENV_2_ID	0x000000c4
#define TEV_ALPHA_ENV_2_ID	0x000000c5
#define TEV_COLOR_ENV_3_ID	0x000000c6
#define TEV_ALPHA_ENV_3_ID	0x000000c7
#define TEV_COLOR_ENV_4_ID	0x000000c8
#define TEV_ALPHA_ENV_4_ID	0x000000c9
#define TEV_COLOR_ENV_5_ID	0x000000ca
#define TEV_ALPHA_ENV_5_ID	0x000000cb
#define TEV_COLOR_ENV_6_ID	0x000000cc
#define TEV_ALPHA_ENV_6_ID	0x000000cd
#define TEV_COLOR_ENV_7_ID	0x000000ce
#define TEV_ALPHA_ENV_7_ID	0x000000cf
#define TEV_COLOR_ENV_8_ID	0x000000d0
#define TEV_ALPHA_ENV_8_ID	0x000000d1
#define TEV_COLOR_ENV_9_ID	0x000000d2
#define TEV_ALPHA_ENV_9_ID	0x000000d3
#define TEV_COLOR_ENV_A_ID	0x000000d4
#define TEV_ALPHA_ENV_A_ID	0x000000d5
#define TEV_COLOR_ENV_B_ID	0x000000d6
#define TEV_ALPHA_ENV_B_ID	0x000000d7
#define TEV_COLOR_ENV_C_ID	0x000000d8
#define TEV_ALPHA_ENV_C_ID	0x000000d9
#define TEV_COLOR_ENV_D_ID	0x000000da
#define TEV_ALPHA_ENV_D_ID	0x000000db
#define TEV_COLOR_ENV_E_ID	0x000000dc
#define TEV_ALPHA_ENV_E_ID	0x000000dd
#define TEV_COLOR_ENV_F_ID	0x000000de
#define TEV_ALPHA_ENV_F_ID	0x000000df
#define TEV_REGISTERL_0_ID	0x000000e0
#define TEV_REGISTERH_0_ID	0x000000e1
#define TEV_REGISTERL_1_ID	0x000000e2
#define TEV_REGISTERH_1_ID	0x000000e3
#define TEV_REGISTERL_2_ID	0x000000e4
#define TEV_REGISTERH_2_ID	0x000000e5
#define TEV_REGISTERL_3_ID	0x000000e6
#define TEV_REGISTERH_3_ID	0x000000e7
// Fog registers are in GDPixel.h
#define TEV_ALPHAFUNC_ID	0x000000f3
#define TEV_Z_ENV_0_ID	0x000000f4
#define TEV_Z_ENV_1_ID	0x000000f5
#define TEV_KSEL_0_ID	0x000000f6
#define TEV_KSEL_1_ID	0x000000f7
#define TEV_KSEL_2_ID	0x000000f8
#define TEV_KSEL_3_ID	0x000000f9
#define TEV_KSEL_4_ID	0x000000fa
#define TEV_KSEL_5_ID	0x000000fb
#define TEV_KSEL_6_ID	0x000000fc
#define TEV_KSEL_7_ID	0x000000fd

#define RAS1_TREF0_ID	0x00000028
#define RAS1_TREF1_ID	0x00000029
#define RAS1_TREF2_ID	0x0000002a
#define RAS1_TREF3_ID	0x0000002b
#define RAS1_TREF4_ID	0x0000002c
#define RAS1_TREF5_ID	0x0000002d
#define RAS1_TREF6_ID	0x0000002e
#define RAS1_TREF7_ID	0x0000002f

/*
 *  TEV register field definitions
 */

//  TEV register type field
#define TEV_COLOR_REG    0
#define TEV_KONSTANT_REG 1

//  Z-texture types (formats)
#define TEV_Z_TYPE_U8	0x00000000
#define TEV_Z_TYPE_U16	0x00000001
#define TEV_Z_TYPE_U24	0x00000002

//  Rasterized color selections
#define RAS1_CC_0	0x00000000 /* Color channel 0 */
#define RAS1_CC_1	0x00000001 /* Color channel 1 */
#define RAS1_CC_B	0x00000005 /* Indirect texture bump alpha */
#define RAS1_CC_BN	0x00000006 /* ind tex bump alpha, normalized 0-255 */
#define RAS1_CC_Z	0x00000007 /* Set color value to zero */

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GDTEV_H__
