/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDVert.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDVert.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2001/10/13 2:12a Hirose
    Added GDCmd() and GDParam() definitions.
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDVERT_H__
#define __GDVERT_H__

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

/*---------------------------------------------*
 * GD Vertex functions                         *
 * Declarations & definitions!                 *
 *---------------------------------------------*/

//-------------------------------
// Define gfx function templates
//-------------------------------

// Templates syntax:
//  __GXCDEF (function_prefix, num_of_components, component_type)
//  __GXCDEFX(function_name, num_of_comps, src_comp_type, dest_comp_type)

#define __GXCDEF(prfx,n,t)  __GXCDEF##n(prfx##n##t,t,t)
#define __GXCDEFX(func,n,t) __GXCDEF##n(func,t,t)

// 1 component
#define __GXCDEF1(func,ts,td) \
    static inline void func(const ts x)  \
    {                             \
        GDWrite_##td(x); \
    }

// 2 components
#define __GXCDEF2(func,ts,td) \
    static inline void func(const ts x, const ts y) \
    {                             \
        GDWrite_##td(x); \
        GDWrite_##td(y); \
    }

// 3 components
#define __GXCDEF3(func,ts,td) \
    static inline void func(const ts x, const ts y, const ts z) \
    {                             \
        GDWrite_##td(x); \
        GDWrite_##td(y); \
        GDWrite_##td(z); \
    }

// 4 components
#define __GXCDEF4(func,ts,td) \
    static inline void func(const ts x, const ts y, const ts z, const ts w) \
    {                             \
        GDWrite_##td(x); \
        GDWrite_##td(y); \
        GDWrite_##td(z); \
        GDWrite_##td(w); \
    }


//---------------------------
// Generate inline functions
//---------------------------

//---- GDCmd[n][t] ----
__GXCDEF( GDCmd, 1, u8  )
__GXCDEF( GDCmd, 1, u16 )
__GXCDEF( GDCmd, 1, u32 )

//---- GDParam[n][t] ----
__GXCDEF( GDParam, 1, u8  )
__GXCDEF( GDParam, 1, u16 )
__GXCDEF( GDParam, 1, u32 )
__GXCDEF( GDParam, 1, s8  )
__GXCDEF( GDParam, 1, s16 )
__GXCDEF( GDParam, 1, s32 )
__GXCDEF( GDParam, 1, f32 )
__GXCDEF( GDParam, 3, f32 )
__GXCDEF( GDParam, 4, f32 )

//---- GDPosition[n][t] ----
  // For GD_POS_XYZ
__GXCDEF( GDPosition, 3, f32 )
__GXCDEF( GDPosition, 3, u8  )
__GXCDEF( GDPosition, 3, s8  )
__GXCDEF( GDPosition, 3, u16 )
__GXCDEF( GDPosition, 3, s16 )
  // For GD_POS_XY
__GXCDEF( GDPosition, 2, f32 )
__GXCDEF( GDPosition, 2, u8  )
__GXCDEF( GDPosition, 2, s8  )
__GXCDEF( GDPosition, 2, u16 )
__GXCDEF( GDPosition, 2, s16 )
  // For Index
__GXCDEFX( GDPosition1x16, 1, u16 )
__GXCDEFX( GDPosition1x8,  1, u8  )

//---- GDNormal[n][t] ----
  // For GD_NRM or GD_NBT
__GXCDEF( GDNormal, 3, f32 )
__GXCDEF( GDNormal, 3, s16 )
__GXCDEF( GDNormal, 3, s8  )
  // For Index
__GXCDEFX( GDNormal1x16, 1, u16 )
__GXCDEFX( GDNormal1x8,  1, u8  )

//---- GDColor[n][t] ----
  // For GD_CLR_RGBA8 or RGBX8
__GXCDEF( GDColor, 4, u8  )
__GXCDEF( GDColor, 1, u32 )
  // For GD_CLR_RGBA6 or RGB8
__GXCDEF( GDColor, 3, u8  )
  // For GD_CLR_RGBA4 or RGB565
__GXCDEF( GDColor, 1, u16 )
  // For Index
__GXCDEFX( GDColor1x16, 1, u16 )
__GXCDEFX( GDColor1x8,  1, u8  )

//---- GDTexCoord[n][t] ----
  // For GD_TEX_ST
__GXCDEF( GDTexCoord, 2, f32 )
__GXCDEF( GDTexCoord, 2, s16 )
__GXCDEF( GDTexCoord, 2, u16 )
__GXCDEF( GDTexCoord, 2, s8  )
__GXCDEF( GDTexCoord, 2, u8  )
  // For GD_TEX_S
__GXCDEF( GDTexCoord, 1, f32 )
__GXCDEF( GDTexCoord, 1, s16 )
__GXCDEF( GDTexCoord, 1, u16 )
__GXCDEF( GDTexCoord, 1, s8  )
__GXCDEF( GDTexCoord, 1, u8  )
  // For Index
__GXCDEFX( GDTexCoord1x16, 1, u16 )
__GXCDEFX( GDTexCoord1x8,  1, u8  )

//---- GDMatrixIndex* ----
// GDMatrixIndex1u8
__GXCDEF( GDMatrixIndex, 1, u8 )
#define GDMatrixIndex1x8    GDMatrixIndex1u8

//------------------------
// Undefine all templates
//------------------------
#undef  __GXCDEF
#undef  __GXCDEFX
#undef  __GXCDEF1
#undef  __GXCDEF2
#undef  __GXCDEF3
#undef  __GXCDEF4

/*---------------------------------------------------------------------------*/
// Packing macro for a several color format
#define	GDPackedRGB565(r,g,b)   \
	((u16)((((r)&0xf8)<<8)|(((g)&0xfc)<<3)|(((b)&0xf8)>>3)))
#define	GDPackedRGBA4(r,g,b,a)  \
	((u16)((((r)&0xf0)<<8)|(((g)&0xf0)<<4)|(((b)&0xf0)   )|(((a)&0xf0)>>4)))
#define	GDPackedRGB5A3(r,g,b,a) \
	((u16)((a)>=224 ? \
	((((r)&0xf8)<<7)|(((g)&0xf8)<<2)|(((b)&0xf8)>>3)|(1<<15)): \
	((((r)&0xf0)<<4)|(((g)&0xf0)   )|(((b)&0xf0)>>4)|(((a)&0xe0)<<7))))

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  // __GDVERT_H__
