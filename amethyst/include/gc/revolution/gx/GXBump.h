/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:	    GXBump.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXBump.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2003/03/19 13:34 Hirose
    Added __GXSetIndirectMask().
    
    3     2002/07/24 9:06 Hirose
    Resolved version conflicts.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXBUMP_H__
#define __GXBUMP_H__

#include <revolution/types.h>


//******************************************************************************
//
//  Graphics Library Indirect Texture (Bump) functions.
//  
//******************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Bump (indirect texture functions).
//------------------------------------------------------------------------------
extern void GXSetNumIndStages (u8 nIndStages);
extern void GXSetIndTexOrder (GXIndTexStageID ind_stage, GXTexCoordID tex_coord,
			      GXTexMapID tex_map);
extern void GXSetIndTexCoordScale (GXIndTexStageID ind_state, 
				   GXIndTexScale scale_s, GXIndTexScale scale_t);
extern void GXSetIndTexMtx (GXIndTexMtxID mtx_sel, const f32 offset[2][3], s8 scale_exp);
extern void GXSetTevIndirect (GXTevStageID tev_stage, GXIndTexStageID ind_stage,
		      GXIndTexFormat format, GXIndTexBiasSel bias_sel, 
		      GXIndTexMtxID matrix_sel,
		      GXIndTexWrap wrap_s, GXIndTexWrap wrap_t,
		      GXBool add_prev, GXBool ind_lod, GXIndTexAlphaSel alpha_sel);

//------------------------------------------------------------------------------
// Convenience functions
//------------------------------------------------------------------------------
extern void GXSetTevDirect (GXTevStageID tev_stage);
extern void GXSetTevIndWarp (GXTevStageID tev_stage, GXIndTexStageID ind_stage,
			     GXBool signed_offset, GXBool replace_mode,
			     GXIndTexMtxID matrix_sel);
extern void GXSetTevIndTile (GXTevStageID tev_stage, GXIndTexStageID ind_stage,
			     u16 tilesize_s, u16 tilesize_t, 
			     u16 tilespacing_s, u16 tilespacing_t, 
			     GXIndTexFormat format, GXIndTexMtxID matrix_sel,
			     GXIndTexBiasSel bias_sel, GXIndTexAlphaSel alpha_sel);
extern void GXSetTevIndBumpST (GXTevStageID tev_stage, GXIndTexStageID ind_stage, 
			       GXIndTexMtxID matrix_sel);
extern void GXSetTevIndBumpXYZ (GXTevStageID tev_stage, GXIndTexStageID ind_stage, 
			       GXIndTexMtxID matrix_sel);
extern void GXSetTevIndRepeat (GXTevStageID tev_stage);

//------------------------------------------------------------------------------
extern void __GXSetIndirectMask ( u32 mask );

//------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif // __GXBUMP_H__
