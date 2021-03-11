/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDTransform.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDTransform.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2004/02/09 9:45 Hirose
    Parenthesized 'mtxId' in __*MtxToXFMem macros for safety.
    
    3     2002/08/05 19:49 Hirose
    Const type specifier support.
    
    2     2002/05/30 13:39 Hirose
    Added GDSetProjection.
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDTRANSFORM_H__
#define __GDTRANSFORM_H__

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

// Define some XF memory spaces:
#define __XFMemPosMtx 0x0000
#define __XFMemNrmMtx 0x0400
#define __XFMemDTTMtx 0x0500

// Define conversions from GX enums to XF memory spaces:
#define __PosMtxToXFMem(mtxId) ((u16)(__XFMemPosMtx + (mtxId) * 4))
#define __TexMtxToXFMem(mtxId) ((u16)(__XFMemPosMtx + (mtxId) * 4))
#define __NrmMtxToXFMem(mtxId) ((u16)(__XFMemNrmMtx + (mtxId) * 3))
#define __DTTMtxToXFMem(mtxId) ((u16)(__XFMemDTTMtx + ((mtxId)-GX_PTTEXMTX0) * 4))

/*---------------------------------------------------------------------------*/

void GDLoadPosMtxImm ( const f32 mtx[3][4], u32 id );
void GDLoadPosMtxIndx ( u16 mtx_indx, u32 id );
void GDLoadNrmMtxImm ( const f32 mtx[3][4], u32 id );
void GDLoadNrmMtxImm3x3 ( const f32 mtx[3][3], u32 id );
void GDLoadNrmMtxIndx3x3 ( u16 mtx_indx, u32 id );
void GDLoadTexMtxImm ( const f32 mtx[][4], u32 id, GXTexMtxType type );
void GDLoadTexMtxIndx ( u16 mtx_indx, u32 id, GXTexMtxType type );

void GDSetCurrentMtx ( u32 pn, u32 t0, u32 t1, u32 t2, u32 t3,
                       u32 t4, u32 t5, u32 t6, u32 t7 );

void GDSetProjection ( const f32 mtx[4][4], GXProjectionType type );


/*---------------------------------------------------------------------------*/

#define MATIDX_REG_A_POSIDX_SHIFT	0
#define MATIDX_REG_A_TEX0IDX_SHIFT	6
#define MATIDX_REG_A_TEX1IDX_SHIFT	12
#define MATIDX_REG_A_TEX2IDX_SHIFT	18
#define MATIDX_REG_A_TEX3IDX_SHIFT	24
#define MATIDX_REG_A(posIdx, tex0Idx, tex1Idx, tex2Idx, tex3Idx) \
	((((unsigned long)(posIdx))  << MATIDX_REG_A_POSIDX_SHIFT) | \
	 (((unsigned long)(tex0Idx)) << MATIDX_REG_A_TEX0IDX_SHIFT) | \
	 (((unsigned long)(tex1Idx)) << MATIDX_REG_A_TEX1IDX_SHIFT) | \
	 (((unsigned long)(tex2Idx)) << MATIDX_REG_A_TEX2IDX_SHIFT) | \
	 (((unsigned long)(tex3Idx)) << MATIDX_REG_A_TEX3IDX_SHIFT))

#define MATIDX_REG_B_TEX4IDX_SHIFT	0
#define MATIDX_REG_B_TEX5IDX_SHIFT	6
#define MATIDX_REG_B_TEX6IDX_SHIFT	12
#define MATIDX_REG_B_TEX7IDX_SHIFT	18
#define MATIDX_REG_B(tex4Idx, tex5Idx, tex6Idx, tex7Idx) \
	((((unsigned long)(tex4Idx)) << MATIDX_REG_B_TEX4IDX_SHIFT) | \
	 (((unsigned long)(tex5Idx)) << MATIDX_REG_B_TEX5IDX_SHIFT) | \
	 (((unsigned long)(tex6Idx)) << MATIDX_REG_B_TEX6IDX_SHIFT) | \
	 (((unsigned long)(tex7Idx)) << MATIDX_REG_B_TEX7IDX_SHIFT))

#define CP_MATINDEX_A	    0x30
#define CP_MATINDEX_B	    0x40

#define XF_MATINDEX_A	    0x1018
#define XF_MATINDEX_B	    0x1019

#define XF_PROJECTION_ID    0x1020 

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GDTRANSFORM_H__
