/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDPIXEL.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDPixel.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2001/10/13 2:22a Hirose
    Added GDSetBlendMode().
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDPIXEL_H__
#define __GDPIXEL_H__

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

void GDSetFog(
    GXFogType   type,
    f32         startz,
    f32         endz,
    f32         nearz,
    f32         farz,
    GXColor     color );

void GDSetBlendMode(
    GXBlendMode     type,
    GXBlendFactor   src_factor, 
    GXBlendFactor   dst_factor, 
    GXLogicOp       logic_op );

void GDSetBlendModeEtc(
    GXBlendMode     type,
    GXBlendFactor   src_factor, 
    GXBlendFactor   dst_factor, 
    GXLogicOp       logic_op,
    GXBool          color_update_enable,
    GXBool          alpha_update_enable,
    GXBool          dither_enable );

void GDSetZMode(
    GXBool          compare_enable,
    GXCompare       func,
    GXBool          update_enable );

void GDSetDstAlpha( GXBool enable, u8 alpha );

void GDSetDrawSync( u16 token ); 

/*---------------------------------------------------------------------------*/

// Defines for setting PE (and some TEV) registers...

/*
 *  tev_fog_param_0 struct
 */
#define TEV_FOG_PARAM_0_A_MANT_SHIFT    0
#define TEV_FOG_PARAM_0_A_EXPN_SHIFT    11
#define TEV_FOG_PARAM_0_A_SIGN_SHIFT    19
#define TEV_FOG_PARAM_0_RID_SHIFT   24
#define TEV_FOG_PARAM_0(a_mant, a_expn, a_sign, rid) \
    ((((unsigned long)(a_mant)) << TEV_FOG_PARAM_0_A_MANT_SHIFT) | \
     (((unsigned long)(a_expn)) << TEV_FOG_PARAM_0_A_EXPN_SHIFT) | \
     (((unsigned long)(a_sign)) << TEV_FOG_PARAM_0_A_SIGN_SHIFT) | \
     (((unsigned long)(rid)) << TEV_FOG_PARAM_0_RID_SHIFT))

#define TEV_FOG_PARAM_0_PS(a_preshifted, rid) \
    ( ((unsigned long)(a_preshifted)) | \
     (((unsigned long)(rid)) << TEV_FOG_PARAM_0_RID_SHIFT))

/*
 *  tev_fog_param_1 struct
 */
#define TEV_FOG_PARAM_1_B_MAG_SHIFT 0
#define TEV_FOG_PARAM_1_RID_SHIFT   24
#define TEV_FOG_PARAM_1(b_mag, rid) \
    ((((unsigned long)(b_mag)) << TEV_FOG_PARAM_1_B_MAG_SHIFT) | \
     (((unsigned long)(rid)) << TEV_FOG_PARAM_1_RID_SHIFT))

/*
 *  tev_fog_param_2 struct
 */
#define TEV_FOG_PARAM_2_B_SHF_SHIFT 0
#define TEV_FOG_PARAM_2_RID_SHIFT   24
#define TEV_FOG_PARAM_2(b_shf, rid) \
    ((((unsigned long)(b_shf)) << TEV_FOG_PARAM_2_B_SHF_SHIFT) | \
     (((unsigned long)(rid)) << TEV_FOG_PARAM_2_RID_SHIFT))

/*
 *  tev_fog_param_3 struct
 */
#define TEV_FOG_PARAM_3_C_MANT_SHIFT    0
#define TEV_FOG_PARAM_3_C_EXPN_SHIFT    11
#define TEV_FOG_PARAM_3_C_SIGN_SHIFT    19
#define TEV_FOG_PARAM_3_PROJ_SHIFT  20
#define TEV_FOG_PARAM_3_FSEL_SHIFT  21
#define TEV_FOG_PARAM_3_RID_SHIFT   24
#define TEV_FOG_PARAM_3(c_mant, c_expn, c_sign, proj, fsel, rid) \
    ((((unsigned long)(c_mant)) << TEV_FOG_PARAM_3_C_MANT_SHIFT) | \
     (((unsigned long)(c_expn)) << TEV_FOG_PARAM_3_C_EXPN_SHIFT) | \
     (((unsigned long)(c_sign)) << TEV_FOG_PARAM_3_C_SIGN_SHIFT) | \
     (((unsigned long)(proj)) << TEV_FOG_PARAM_3_PROJ_SHIFT) | \
     (((unsigned long)(fsel)) << TEV_FOG_PARAM_3_FSEL_SHIFT) | \
     (((unsigned long)(rid)) << TEV_FOG_PARAM_3_RID_SHIFT))

#define TEV_FOG_PARAM_3_PS(c_preshifted, proj, fsel, rid) \
    ( ((unsigned long)(c_preshifted)) | \
     (((unsigned long)(proj)) << TEV_FOG_PARAM_3_PROJ_SHIFT) | \
     (((unsigned long)(fsel)) << TEV_FOG_PARAM_3_FSEL_SHIFT) | \
     (((unsigned long)(rid)) << TEV_FOG_PARAM_3_RID_SHIFT))

/*
 *  tev_fog_color struct
 */
#define TEV_FOG_COLOR_B_SHIFT   0
#define TEV_FOG_COLOR_G_SHIFT   8
#define TEV_FOG_COLOR_R_SHIFT   16
#define TEV_FOG_COLOR_RID_SHIFT 24
#define TEV_FOG_COLOR(b, g, r, rid) \
    ((((unsigned long)(b)) << TEV_FOG_COLOR_B_SHIFT) | \
     (((unsigned long)(g)) << TEV_FOG_COLOR_G_SHIFT) | \
     (((unsigned long)(r)) << TEV_FOG_COLOR_R_SHIFT) | \
     (((unsigned long)(rid)) << TEV_FOG_COLOR_RID_SHIFT))

/*
 *  pe_zmode struct
 */
#define PE_ZMODE_ENABLE_SHIFT   0
#define PE_ZMODE_FUNC_SHIFT 1
#define PE_ZMODE_MASK_SHIFT 4
#define PE_ZMODE_RID_SHIFT  24
#define PE_ZMODE(enable, func, mask, rid) \
    ((((unsigned long)(enable)) << PE_ZMODE_ENABLE_SHIFT) | \
     (((unsigned long)(func)) << PE_ZMODE_FUNC_SHIFT) | \
     (((unsigned long)(mask)) << PE_ZMODE_MASK_SHIFT) | \
     (((unsigned long)(rid)) << PE_ZMODE_RID_SHIFT))

/*
 *  pe_cmode0 struct
 */
#define PE_CMODE0_BLEND_ENABLE_SHIFT    0
#define PE_CMODE0_LOGICOP_ENABLE_SHIFT  1
#define PE_CMODE0_DITHER_ENABLE_SHIFT   2
#define PE_CMODE0_COLOR_MASK_SHIFT  3
#define PE_CMODE0_ALPHA_MASK_SHIFT  4
#define PE_CMODE0_DFACTOR_SHIFT 5
#define PE_CMODE0_SFACTOR_SHIFT 8
#define PE_CMODE0_BLENDOP_SHIFT 11
#define PE_CMODE0_LOGICOP_SHIFT 12
#define PE_CMODE0_RID_SHIFT 24
#define PE_CMODE0(blend_enable, logicop_enable, dither_enable, color_mask, alpha_mask, dfactor, sfactor, blendop, logicop, rid) \
    ((((unsigned long)(blend_enable)) << PE_CMODE0_BLEND_ENABLE_SHIFT) | \
     (((unsigned long)(logicop_enable)) << PE_CMODE0_LOGICOP_ENABLE_SHIFT) | \
     (((unsigned long)(dither_enable)) << PE_CMODE0_DITHER_ENABLE_SHIFT) | \
     (((unsigned long)(color_mask)) << PE_CMODE0_COLOR_MASK_SHIFT) | \
     (((unsigned long)(alpha_mask)) << PE_CMODE0_ALPHA_MASK_SHIFT) | \
     (((unsigned long)(dfactor)) << PE_CMODE0_DFACTOR_SHIFT) | \
     (((unsigned long)(sfactor)) << PE_CMODE0_SFACTOR_SHIFT) | \
     (((unsigned long)(blendop)) << PE_CMODE0_BLENDOP_SHIFT) | \
     (((unsigned long)(logicop)) << PE_CMODE0_LOGICOP_SHIFT) | \
     (((unsigned long)(rid)) << PE_CMODE0_RID_SHIFT))

/*
 *  pe_cmode1 struct
 */
#define PE_CMODE1_CONSTANT_ALPHA_SHIFT  0
#define PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT   8
#define PE_CMODE1_RID_SHIFT 24
#define PE_CMODE1(constant_alpha, constant_alpha_enable, rid) \
    ((((unsigned long)(constant_alpha)) << PE_CMODE1_CONSTANT_ALPHA_SHIFT) | \
     (((unsigned long)(constant_alpha_enable)) << PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT) | \
     (((unsigned long)(rid)) << PE_CMODE1_RID_SHIFT))

/*
 *  pe_token struct
 */
#define PE_TOKEN_TOKEN_SHIFT    0
#define PE_TOKEN_RID_SHIFT  24
#define PE_TOKEN(token, rid) \
    ((((unsigned long)(token)) << PE_TOKEN_TOKEN_SHIFT) | \
     (((unsigned long)(rid)) << PE_TOKEN_RID_SHIFT))

/*
 *  PE (and some TEV) register addresses (BP address space)
 */
#define PE_ZMODE_ID         0x00000040
#define PE_CMODE0_ID        0x00000041
#define PE_CMODE1_ID        0x00000042
#define PE_CONTROL_ID       0x00000043
#define PE_TOKEN_ID         0x00000047
#define PE_TOKEN_INT_ID     0x00000048

#define TEV_FOG_PARAM_0_ID  0x000000ee
#define TEV_FOG_PARAM_1_ID  0x000000ef
#define TEV_FOG_PARAM_2_ID  0x000000f0
#define TEV_FOG_PARAM_3_ID  0x000000f1
#define TEV_FOG_COLOR_ID    0x000000f2

/*
 *  TEV register field definitions
 */

//  tev_proj enum
#define TEV_PROJ_PERSP      0x00000000
#define TEV_PROJ_ORTHO      0x00000001

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GDPIXEL_H__
