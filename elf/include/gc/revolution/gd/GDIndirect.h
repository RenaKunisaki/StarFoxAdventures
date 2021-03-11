/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDIndirect.h

  Copyright 2003 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDIndirect.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2003/02/19 17:37 Hirose
    
    1     2003/02/03 14:04 Hirose
    Initial check in.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDINDIRECT_H__
#define __GDINDIRECT_H__

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
void GDSetTevIndirect (
    GXTevStageID        tev_stage,
    GXIndTexStageID     ind_stage,
    GXIndTexFormat      format,
    GXIndTexBiasSel     bias_sel,
    GXIndTexMtxID       matrix_sel,
    GXIndTexWrap        wrap_s,
    GXIndTexWrap        wrap_t,
    GXBool              add_prev,
    GXBool              utc_lod,
    GXIndTexAlphaSel    alpha_sel );

void GDSetIndTexMtx (
    GXIndTexMtxID       mtx_id,
    const f32           offset[2][3],
    s8                  scale_exp );

void GDSetIndTexCoordScale (
    GXIndTexStageID     indStageEven,
    GXIndTexScale       scaleS0,
    GXIndTexScale       scaleT0,
    GXIndTexScale       scaleS1,
    GXIndTexScale       scaleT1 );

void GDSetIndTexOrder (
    GXTexCoordID        texCoord0,
    GXTexMapID          texMap0, 
    GXTexCoordID        texCoord1,
    GXTexMapID          texMap1, 
    GXTexCoordID        texCoord2,
    GXTexMapID          texMap2, 
    GXTexCoordID        texCoord3,
    GXTexMapID          texMap3 );

/*---------------------------------------------------------------------------*/
void GDSetTevDirect ( GXTevStageID tev_stage );

void GDSetTevIndWarp (
    GXTevStageID        tev_stage,
    GXIndTexStageID     ind_stage,
    GXBool              signed_offset,
    GXBool              replace_mode,
    GXIndTexMtxID       matrix_sel );

void GDSetTevIndTile (
    GXTevStageID        tev_stage,
    GXIndTexStageID     ind_stage,
    u16                 tilesize_s,
    u16                 tilesize_t,
    u16                 tilespacing_s,
    u16                 tilespacing_t,
    GXIndTexFormat      format,
    GXIndTexMtxID       matrix_sel,
    GXIndTexBiasSel     bias_sel,
    GXIndTexAlphaSel    alpha_sel );

void GDSetTevIndBumpST (
    GXTevStageID        tev_stage,
    GXIndTexStageID     ind_stage,
    GXIndTexMtxID       matrix_sel );

void GDSetTevIndBumpXYZ (
    GXTevStageID        tev_stage,
    GXIndTexStageID     ind_stage,
    GXIndTexMtxID       matrix_sel );

void GDSetTevIndRepeat ( GXTevStageID tev_stage );

/*---------------------------------------------------------------------------*/
void __GDSetIndTexMask ( u32 mask );

/*---------------------------------------------------------------------------*/

// Defines for setting BUMP(Indirect Texture) registers...

/*
 *  ind_mtxa struct
 */
#define IND_MTXA_MA_SHIFT	0
#define IND_MTXA_MB_SHIFT	11
#define IND_MTXA_S_SHIFT	22
#define IND_MTXA_RID_SHIFT	24
#define IND_MTXA(ma, mb, s, rid) \
	((((unsigned long)(ma)) << IND_MTXA_MA_SHIFT) | \
	 (((unsigned long)(mb)) << IND_MTXA_MB_SHIFT) | \
	 (((unsigned long)(s)) << IND_MTXA_S_SHIFT) | \
	 (((unsigned long)(rid)) << IND_MTXA_RID_SHIFT))

/*
 *  ind_mtxb struct
 */
#define IND_MTXB_MC_SHIFT	0
#define IND_MTXB_MD_SHIFT	11
#define IND_MTXB_S_SHIFT	22
#define IND_MTXB_RID_SHIFT	24
#define IND_MTXB(mc, md, s, rid) \
	((((unsigned long)(mc)) << IND_MTXB_MC_SHIFT) | \
	 (((unsigned long)(md)) << IND_MTXB_MD_SHIFT) | \
	 (((unsigned long)(s)) << IND_MTXB_S_SHIFT) | \
	 (((unsigned long)(rid)) << IND_MTXB_RID_SHIFT))

/*
 *  ind_mtxc struct
 */
#define IND_MTXC_ME_SHIFT	0
#define IND_MTXC_MF_SHIFT	11
#define IND_MTXC_S_SHIFT	22
#define IND_MTXC_RID_SHIFT	24
#define IND_MTXC(me, mf, s, rid) \
	((((unsigned long)(me)) << IND_MTXC_ME_SHIFT) | \
	 (((unsigned long)(mf)) << IND_MTXC_MF_SHIFT) | \
	 (((unsigned long)(s)) << IND_MTXC_S_SHIFT) | \
	 (((unsigned long)(rid)) << IND_MTXC_RID_SHIFT))

/*
 *  ind_cmd struct
 */
#define IND_CMD_BT_SHIFT	0
#define IND_CMD_FMT_SHIFT	2
#define IND_CMD_BIAS_SHIFT	4
#define IND_CMD_BS_SHIFT	7
#define IND_CMD_M_SHIFT	9
#define IND_CMD_SW_SHIFT	13
#define IND_CMD_TW_SHIFT	16
#define IND_CMD_LB_SHIFT	19
#define IND_CMD_FB_SHIFT	20
#define IND_CMD_PAD0_SHIFT	21
#define IND_CMD_RID_SHIFT	24
#define IND_CMD(bt, fmt, bias, bs, m, sw, tw, lb, fb, rid) \
	((((unsigned long)(bt)) << IND_CMD_BT_SHIFT) | \
	 (((unsigned long)(fmt)) << IND_CMD_FMT_SHIFT) | \
	 (((unsigned long)(bias)) << IND_CMD_BIAS_SHIFT) | \
	 (((unsigned long)(bs)) << IND_CMD_BS_SHIFT) | \
	 (((unsigned long)(m)) << IND_CMD_M_SHIFT) | \
	 (((unsigned long)(sw)) << IND_CMD_SW_SHIFT) | \
	 (((unsigned long)(tw)) << IND_CMD_TW_SHIFT) | \
	 (((unsigned long)(lb)) << IND_CMD_LB_SHIFT) | \
	 (((unsigned long)(fb)) << IND_CMD_FB_SHIFT) | \
	 (((unsigned long)(rid)) << IND_CMD_RID_SHIFT))

/*
 *  ind_imask struct
 */
#define IND_IMASK_IMASK_SHIFT	0
#define IND_IMASK_RID_SHIFT	24
#define IND_IMASK(imask, rid) \
	((((unsigned long)(imask)) << IND_IMASK_IMASK_SHIFT) | \
	 (((unsigned long)(rid)) << IND_IMASK_RID_SHIFT))

/*
 *  ras1_ss struct
 */
#define RAS1_SS_SS0_SHIFT	0
#define RAS1_SS_TS0_SHIFT	4
#define RAS1_SS_SS1_SHIFT	8
#define RAS1_SS_TS1_SHIFT	12
#define RAS1_SS_RID_SHIFT	24
#define RAS1_SS(ss0, ts0, ss1, ts1, rid) \
	((((unsigned long)(ss0)) << RAS1_SS_SS0_SHIFT) | \
	 (((unsigned long)(ts0)) << RAS1_SS_TS0_SHIFT) | \
	 (((unsigned long)(ss1)) << RAS1_SS_SS1_SHIFT) | \
	 (((unsigned long)(ts1)) << RAS1_SS_TS1_SHIFT) | \
	 (((unsigned long)(rid)) << RAS1_SS_RID_SHIFT))

/*
 *  ras1_iref struct
 */
#define RAS1_IREF_BI0_SHIFT	0
#define RAS1_IREF_BC0_SHIFT	3
#define RAS1_IREF_BI1_SHIFT	6
#define RAS1_IREF_BC1_SHIFT	9
#define RAS1_IREF_BI2_SHIFT	12
#define RAS1_IREF_BC2_SHIFT	15
#define RAS1_IREF_BI3_SHIFT	18
#define RAS1_IREF_BC3_SHIFT	21
#define RAS1_IREF_RID_SHIFT	24
#define RAS1_IREF(bi0, bc0, bi1, bc1, bi2, bc2, bi3, bc3, rid) \
	((((unsigned long)(bi0)) << RAS1_IREF_BI0_SHIFT) | \
	 (((unsigned long)(bc0)) << RAS1_IREF_BC0_SHIFT) | \
	 (((unsigned long)(bi1)) << RAS1_IREF_BI1_SHIFT) | \
	 (((unsigned long)(bc1)) << RAS1_IREF_BC1_SHIFT) | \
	 (((unsigned long)(bi2)) << RAS1_IREF_BI2_SHIFT) | \
	 (((unsigned long)(bc2)) << RAS1_IREF_BC2_SHIFT) | \
	 (((unsigned long)(bi3)) << RAS1_IREF_BI3_SHIFT) | \
	 (((unsigned long)(bc3)) << RAS1_IREF_BC3_SHIFT) | \
	 (((unsigned long)(rid)) << RAS1_IREF_RID_SHIFT))

/*
 *  BUMP(Indirect Texture) register addresses (BP address space)
 */

#define IND_MTXA0_ID    0x00000006
#define IND_MTXB0_ID    0x00000007
#define IND_MTXC0_ID    0x00000008
#define IND_MTXA1_ID    0x00000009
#define IND_MTXB1_ID    0x0000000a
#define IND_MTXC1_ID    0x0000000b
#define IND_MTXA2_ID    0x0000000c
#define IND_MTXB2_ID    0x0000000d
#define IND_MTXC2_ID    0x0000000e
#define IND_IMASK_ID    0x0000000f
#define IND_CMD0_ID     0x00000010
#define IND_CMD1_ID     0x00000011
#define IND_CMD2_ID     0x00000012
#define IND_CMD3_ID     0x00000013
#define IND_CMD4_ID     0x00000014
#define IND_CMD5_ID     0x00000015
#define IND_CMD6_ID     0x00000016
#define IND_CMD7_ID     0x00000017
#define IND_CMD8_ID     0x00000018
#define IND_CMD9_ID     0x00000019
#define IND_CMDA_ID     0x0000001a
#define IND_CMDB_ID     0x0000001b
#define IND_CMDC_ID     0x0000001c
#define IND_CMDD_ID     0x0000001d
#define IND_CMDE_ID     0x0000001e
#define IND_CMDF_ID     0x0000001f

#define RAS1_SS0_ID     0x00000025
#define RAS1_SS1_ID     0x00000026
#define RAS1_IREF_ID    0x00000027


/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GDINDIRECT_H__
