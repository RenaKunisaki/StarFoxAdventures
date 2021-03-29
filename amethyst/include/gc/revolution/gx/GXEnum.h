/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXEnum.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXEnum.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    40    2002/12/06 9:28 Hirose
    Added GX_TEV_KCSEL_n_8/GX_TEV_KASEL_n_8 (n=even).
    
    39    2002/11/18 18:10 Hirose
    Workaround for GXAbortFrame() problem with PE copyout.
    
    38    2002/10/17 15:11 Hirose
    Added new enums for orthographic fog support.
    
    37    2002/07/24 9:06 Hirose
    Resolved version conflicts.
    
    36    2001/12/11 4:31p Carl
    Fixed warning with GX_NO_LEGACY_HW1.
    
    35    2001/11/27 2:39p Carl
    Added GX_NO_LEGACY_HW1 conditional.
    
    34    2001/7/12 6:07p Carl
    Removed extra #ifdef's.
    
    33    2001/05/10 9:43p Hirose
    Added tag for each enum definition
    
    32    2001/04/27 1:19p Carl
    Changes for making DL context saving optional.
    
    31    2001/04/09 3:43p Carl
    Changed GXXFFlushVal keywords.
    
    30    2001/04/04 3:28p Carl
    Second round of XF flush fixes.
    
    29    2001/03/16 7:22p Carl
    Changes for XF flush.
    
    28    2001/01/16 7:59p Carl
    Added new CTF.
    
    27    2000/11/08 6:48p Hirose
    Fixed the enum value of GX_CC_KONST
    
    26     2000/11/06 4:06p Carl
    Added GXClipMode.
    
    25    2000/11/02 4:37p Carl
    Made Z16 tex fmt same value for both revs.
    
    24    2000/11/02 3:22p Carl
    Removed CCV bit from tex fmts.
    
    23    2000/10/18 4:05p Carl
    Latest round of HW2 modifications and minor updates.
    
    22    2000/10/11 6:25p Carl
    Made CCV, CTF, ZTF into visible symbols.
    
    21     2000/10/03 7:55p Hirose
    Update for HW2(Rev.B) GX API extensions
    
    20    2000/08/29 3:21p Carl
    allow perf counter api to work with draw sync callback [Rob]
        
    
    19    2000/06/14 4:50p Carl
    Removed inverted Z formats.
    
    18    2000/06/08 1:58p Carl
    Added GXAlphaReadMode.
    
    17     2000/06/06 6:57p Alligator
    added vcache mem and stall counters
    
    16     2000/06/06 12:02p Alligator
    made changes to perf counter api
    
    15    2000/04/26 4:08p Alligator
    Added GXCopyMode enumeration
    
    14    2000/04/28 4:55p Carl
    Fixed GX_MAX_VTXATTRFMTLIST_SZ.
    
    13    2000/04/17 4:24p Alligator
    integrate ArtX update.  Perf monitor api and indirect texture api
    added. Misc bug fixes.
    
    12    2000/04/07 12:00p Alligator
    made vtx attr list size smaller (remove index attributes from list)
    
    11    2000/04/04 11:17a Alligator
    moved constants from GXStruct.h to GXEnum.h since they are dependent on
    enumerated values.
    
    10    2000/04/26 5:39p Alligator
    Latest integration with ArtX
    
    9     2000/03/03 4:21p Alligator
    integrate with ArtX source
    
    8     2000/02/12 5:16p Alligator
    Integrate ArtX source tree changes
    
    7     2000/02/04 5:09p Alligator
    added GX_TF_A8 type to texture formats.  For tex copy from alpha
    channel.
    
    6     2000/02/01 7:22p Alligator
    Second GX update from ArtX
    
    5     2000/01/13 5:55p Alligator
    integrate with ArtX GX library code
    
    4     1999/12/08 4:55p Alligator
    remove source alpha saturate blend source
    
    3     1999/12/07 5:34p Hirose
    added GXTexCacheSize
    
    2     1999/12/07 2:16p Alligator
    unlock
    
    35    1999/11/15 5:56p Yasu
    Fixed GXPrimitive enum
    
    34    1999/11/11 6:17p Yasu
    Delete GXTevSwap, not used anymore.
    
    33    1999/11/05 6:49p Hirose
    validated GX_AF_SPEC
    
    32    1999/10/26 1:34p Alligator
    change GXSetDefaultMatrix to GXSetDefaultMtx
    
    31    1999/10/24 4:34p Yasu
    Add GX_LIGHT_NULL as 0x00
    
    30    1999/10/20 2:39p Yasu
    Add TexMatrixIndex
    
    29    1999/10/19 2:49p Alligator
    
    28    1999/10/19 2:35p Alligator
    added GX_VA_NBT for 9-element normal to vcd
    
    27    1999/10/13 5:17p Alligator
    added _NULL types to some enums
    
    26    1999/10/06 10:38a Alligator
    changed enums for compressed Z format
    updated Init functions to make GX_ZC_LINEAR default
    
    25    1999/10/04 6:47p Yasu
    Rename -Sz to -Size
    
    24    1999/10/04 12:44p Yasu
    
    21    1999/09/30 10:59p Yasu
    Add patch for renaming GXExtras.h temporary
    
    20    1999/09/30 10:40p Yasu
    Renamed some GX functions and enums
    
    19    1999/09/30 4:50p Yasu
    Return GX_TG_CLR* back to GX_TG_COLOR*
    
    18    1999/09/30 4:26p Yasu
    Change GX_TG_COLOR* to GX_TG_CLR*

    17    1999/9/17 3:29p Hirose
    corrected GX_TLUT_16k to GX_TLUT_16K

    16     1999/09/09 1:20p Hirose
    added GXSpotFn and GXDistAttnFn enums

    15    1999/09/08 7:12p Yasu
    Delete alias of enum GX_MAT_?x4

    14    1999/09/03 2:44p Ryan

    13     1999/09/02 3:18p Ryan
    Made Frame Buffer Api changes

    12    1999/09/01 3:17p Ryan
    Added GXZTexOp enum and added three new TexFormats for Z textures

    10    1999/08/28 12:10a Yasu
    Change enum name GX_MAT_2x4/3x4 -> GX_MTX2x4/3x4 and
    Swap values GX_MTX2x4 <--> GX_MTX3x4

    9     1999/08/27 2:24p Yasu
    Change enum GXBlendMode  GX_CM_XXX -> GX_BM_XXX

    8     1999/08/17 2:39p Alligator
    changed the values of some enums to match hardware

    7     1999/08/17 1:33p Alligator
    added GXSetTexGenEnables function, modified GXTexCoordID enumeration for
    bitwise OR of enables.

    6     1999/08/17 12:02p Alligator
    updated to reflect arbitrary tlut size

    5     1999/08/12 2:29p Hirose

    4     1999/08/05 11:17a Ryan
    removed all Mac-only enums

    3     1999/07/28 4:07p Alligator
    update header files and emulator for hw changes

    2     1999/07/23 12:30p Alligator
    Fixed pnmtx index per vertex emulation

    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXENUM_H__
#define __GXENUM_H__

/********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************************/
#include <revolution/types.h>

/********************************/
typedef u8 GXBool;

/********************************/
#define GX_TRUE        ((GXBool)1)
#define GX_FALSE       ((GXBool)0)
#define GX_ENABLE      ((GXBool)1)
#define GX_DISABLE     ((GXBool)0)

/********************************/
typedef enum _GXAttr
{
    GX_VA_PNMTXIDX = 0,    // position/normal matrix index
    GX_VA_TEX0MTXIDX,      // texture 0 matrix index
    GX_VA_TEX1MTXIDX,      // texture 1 matrix index
    GX_VA_TEX2MTXIDX,      // texture 2 matrix index
    GX_VA_TEX3MTXIDX,      // texture 3 matrix index
    GX_VA_TEX4MTXIDX,      // texture 4 matrix index
    GX_VA_TEX5MTXIDX,      // texture 5 matrix index
    GX_VA_TEX6MTXIDX,      // texture 6 matrix index
    GX_VA_TEX7MTXIDX,      // texture 7 matrix index
    GX_VA_POS      = 9,    // position
    GX_VA_NRM,             // normal
    GX_VA_CLR0,            // color 0
    GX_VA_CLR1,            // color 1
    GX_VA_TEX0,            // input texture coordinate 0
    GX_VA_TEX1,            // input texture coordinate 1
    GX_VA_TEX2,            // input texture coordinate 2
    GX_VA_TEX3,            // input texture coordinate 3
    GX_VA_TEX4,            // input texture coordinate 4
    GX_VA_TEX5,            // input texture coordinate 5
    GX_VA_TEX6,            // input texture coordinate 6
    GX_VA_TEX7,            // input texture coordinate 7

    GX_POS_MTX_ARRAY,      // position matrix array pointer
    GX_NRM_MTX_ARRAY,      // normal matrix array pointer
    GX_TEX_MTX_ARRAY,      // texture matrix array pointer
    GX_LIGHT_ARRAY,        // light parameter array pointer
    GX_VA_NBT,             // normal, bi-normal, tangent 
    GX_VA_MAX_ATTR,        // maximum number of vertex attributes

    GX_VA_NULL     = 0xff  // NULL attribute (to mark end of lists)

} GXAttr;

#define GX_MAX_VTXDESC              GX_VA_MAX_ATTR
#define GX_MAX_VTXDESCLIST_SZ       (GX_MAX_VTXDESC+1)    // +1 for GX_VA_NULL

#define GX_MAX_VTXATTRFMT           GX_VA_MAX_ATTR
#define GX_MAX_VTXATTRFMTLIST_SZ    (GX_MAX_VTXATTRFMT+1) // +1 for GX_VA_NULL

#define GX_MAX_VTXARRAY             16

/********************************/
typedef enum _GXVtxFmt
{
    GX_VTXFMT0 = 0,
    GX_VTXFMT1,
    GX_VTXFMT2,
    GX_VTXFMT3,
    GX_VTXFMT4,
    GX_VTXFMT5,
    GX_VTXFMT6,
    GX_VTXFMT7,
    GX_MAX_VTXFMT

} GXVtxFmt;

#define GX_MAX_VTXFMT               GX_MAX_VTXFMT

/********************************/
typedef enum _GXAttrType
{
    GX_NONE    = 0,
    GX_DIRECT,
    GX_INDEX8,
    GX_INDEX16

} GXAttrType;

/********************************/
typedef enum _GXTexCoordID
{
    GX_TEXCOORD0 = 0x0,    	// generated texture coordinate 0
    GX_TEXCOORD1, 		// generated texture coordinate 1
    GX_TEXCOORD2, 		// generated texture coordinate 2
    GX_TEXCOORD3, 		// generated texture coordinate 3
    GX_TEXCOORD4, 		// generated texture coordinate 4
    GX_TEXCOORD5, 		// generated texture coordinate 5
    GX_TEXCOORD6, 		// generated texture coordinate 6
    GX_TEXCOORD7, 		// generated texture coordinate 7
    GX_MAX_TEXCOORD = 8,
    GX_TEXCOORD_NULL = 0xff

} GXTexCoordID;

#define GX_MAX_TEXCOORDGEN          GX_MAX_TEXCOORD

/********************************/
typedef enum _GXTexGenType
{
    GX_TG_MTX3x4 = 0,
    GX_TG_MTX2x4,
    GX_TG_BUMP0,
    GX_TG_BUMP1,
    GX_TG_BUMP2,
    GX_TG_BUMP3,
    GX_TG_BUMP4,
    GX_TG_BUMP5,
    GX_TG_BUMP6,
    GX_TG_BUMP7,
    GX_TG_SRTG

} GXTexGenType;

/********************************/
typedef enum _GXTexGenSrc
{
    GX_TG_POS = 0,
    GX_TG_NRM,
	GX_TG_BINRM,
	GX_TG_TANGENT,
    GX_TG_TEX0,
    GX_TG_TEX1,
    GX_TG_TEX2,
    GX_TG_TEX3,
    GX_TG_TEX4,
    GX_TG_TEX5,
    GX_TG_TEX6,
    GX_TG_TEX7,
    GX_TG_TEXCOORD0,
    GX_TG_TEXCOORD1,
    GX_TG_TEXCOORD2,
    GX_TG_TEXCOORD3,
    GX_TG_TEXCOORD4,
    GX_TG_TEXCOORD5,
    GX_TG_TEXCOORD6,
    GX_TG_COLOR0,
    GX_TG_COLOR1

} GXTexGenSrc;

/********************************/
typedef enum _GXCompCnt
{
#if ( GX_REV == 1 )
    GX_POS_XY    = 0,
    GX_POS_XYZ   = 1,
    GX_NRM_XYZ   = 0,
    GX_NRM_NBT   = 1, // not really used, just to make api consistent
    GX_CLR_RGB   = 0,
    GX_CLR_RGBA  = 1,
    GX_TEX_S     = 0,
    GX_TEX_ST    = 1

#else // ( GX_REV >= 2 )
    GX_POS_XY    = 0,
    GX_POS_XYZ   = 1,
    GX_NRM_XYZ   = 0,
    GX_NRM_NBT   = 1, // one index per NBT
    GX_NRM_NBT3  = 2, // one index per each of N/B/T
    GX_CLR_RGB   = 0,
    GX_CLR_RGBA  = 1,
    GX_TEX_S     = 0,
    GX_TEX_ST    = 1

#endif
} GXCompCnt;

/********************************/
typedef enum _GXCompType
{
    GX_U8  = 0,
    GX_S8  = 1,
    GX_U16 = 2,
    GX_S16 = 3,
    GX_F32 = 4,

    GX_RGB565 = 0,
    GX_RGB8   = 1,
    GX_RGBX8  = 2,
    GX_RGBA4  = 3,
    GX_RGBA6  = 4,
    GX_RGBA8  = 5

} GXCompType;

/********************************/
typedef enum _GXChannelID
{
    GX_COLOR0,
    GX_COLOR1,
    GX_ALPHA0,
    GX_ALPHA1,
    GX_COLOR0A0,          // Color 0 + Alpha 0
    GX_COLOR1A1,          // Color 1 + Alpha 1
	GX_COLOR_ZERO,        // RGBA = 0
	GX_ALPHA_BUMP,        // bump alpha 0-248, RGB=0
	GX_ALPHA_BUMPN,       // normalized bump alpha, 0-255, RGB=0
    GX_COLOR_NULL = 0xff

} GXChannelID;

/********************************/
typedef enum _GXColorSrc
{
    GX_SRC_REG = 0,
    GX_SRC_VTX

} GXColorSrc;

/********************************/
typedef enum _GXLightID
{
    GX_LIGHT0     = 0x001,
    GX_LIGHT1     = 0x002,
    GX_LIGHT2     = 0x004,
    GX_LIGHT3     = 0x008,
    GX_LIGHT4     = 0x010,
    GX_LIGHT5     = 0x020,
    GX_LIGHT6     = 0x040,
    GX_LIGHT7     = 0x080,
    GX_MAX_LIGHT  = 0x100,
    GX_LIGHT_NULL = 0x000
} GXLightID;

/********************************/
typedef enum _GXDiffuseFn
{
    GX_DF_NONE = 0,
    GX_DF_SIGN,
    GX_DF_CLAMP

} GXDiffuseFn;

/********************************/
typedef enum _GXAttnFn
{
    GX_AF_SPEC = 0,    // use specular attenuation
    GX_AF_SPOT = 1,    // use distance/spotlight attenuation
    GX_AF_NONE         // attenuation is off

} GXAttnFn;

/********************************/
typedef enum _GXSpotFn
{
    GX_SP_OFF = 0,
    GX_SP_FLAT,
    GX_SP_COS,
    GX_SP_COS2,
	GX_SP_SHARP,
	GX_SP_RING1,
	GX_SP_RING2
} GXSpotFn;

/********************************/
typedef enum _GXDistAttnFn
{
    GX_DA_OFF = 0,
    GX_DA_GENTLE,
    GX_DA_MEDIUM,
    GX_DA_STEEP
} GXDistAttnFn;

/********************************/
typedef enum _GXPosNrmMtx
{
    GX_PNMTX0 =  0,
    GX_PNMTX1 =  3,
    GX_PNMTX2 =  6,
    GX_PNMTX3 =  9,
    GX_PNMTX4 = 12,
    GX_PNMTX5 = 15,
    GX_PNMTX6 = 18,
    GX_PNMTX7 = 21,
    GX_PNMTX8 = 24,
    GX_PNMTX9 = 27

} GXPosNrmMtx;

/********************************/
typedef enum _GXTexMtx
{
    GX_TEXMTX0  = 30,
    GX_TEXMTX1  = 33,
    GX_TEXMTX2  = 36,
    GX_TEXMTX3  = 39,
    GX_TEXMTX4  = 42,
    GX_TEXMTX5  = 45,
    GX_TEXMTX6  = 48,
    GX_TEXMTX7  = 51,
    GX_TEXMTX8  = 54,
    GX_TEXMTX9  = 57,
    GX_IDENTITY = 60

} GXTexMtx;

/********************************/
#if ( GX_REV != 1 ) // Revision 2 or greater only
typedef enum _GXPTTexMtx
{
    GX_PTTEXMTX0  = 64,
    GX_PTTEXMTX1  = 67,
    GX_PTTEXMTX2  = 70,
    GX_PTTEXMTX3  = 73,
    GX_PTTEXMTX4  = 76,
    GX_PTTEXMTX5  = 79,
    GX_PTTEXMTX6  = 82,
    GX_PTTEXMTX7  = 85,
    GX_PTTEXMTX8  = 88,
    GX_PTTEXMTX9  = 91,
    GX_PTTEXMTX10 = 94,
    GX_PTTEXMTX11 = 97,
    GX_PTTEXMTX12 = 100,
    GX_PTTEXMTX13 = 103,
    GX_PTTEXMTX14 = 106,
    GX_PTTEXMTX15 = 109,
    GX_PTTEXMTX16 = 112,
    GX_PTTEXMTX17 = 115,
    GX_PTTEXMTX18 = 118,
    GX_PTTEXMTX19 = 121,
    GX_PTIDENTITY = 125

} GXPTTexMtx;
#endif // ( GX_REV != 1 )

/********************************/
typedef enum _GXTexMtxType
{
    GX_MTX3x4 = 0,
    GX_MTX2x4

} GXTexMtxType;

/********************************/
typedef enum _GXPrimitive
{
    GX_POINTS        = 0xb8,
    GX_LINES         = 0xa8,
    GX_LINESTRIP     = 0xb0,
    GX_TRIANGLES     = 0x90,
    GX_TRIANGLESTRIP = 0x98,
    GX_TRIANGLEFAN   = 0xa0,
    GX_QUADS         = 0x80

} GXPrimitive;

/********************************/
typedef enum _GXTexOffset
{
    GX_TO_ZERO,
    GX_TO_SIXTEENTH,
    GX_TO_EIGHTH,
    GX_TO_FOURTH,
    GX_TO_HALF,
    GX_TO_ONE,
    GX_MAX_TEXOFFSET

} GXTexOffset;

/********************************/
typedef enum _GXCullMode
{
    GX_CULL_NONE,
    GX_CULL_FRONT,
    GX_CULL_BACK,
    GX_CULL_ALL

} GXCullMode;

/********************************/
typedef enum _GXClipMode
{
    // Note: these are (by design) backwards of typical enable/disables!
    GX_CLIP_ENABLE = 0,
    GX_CLIP_DISABLE = 1

} GXClipMode;

/********************************/
typedef enum _GXTexWrapMode
{
    GX_CLAMP,
    GX_REPEAT,
    GX_MIRROR,
    GX_MAX_TEXWRAPMODE

} GXTexWrapMode;

/********************************/
typedef enum _GXTexFilter
{
    GX_NEAR,
    GX_LINEAR,
    GX_NEAR_MIP_NEAR,
    GX_LIN_MIP_NEAR,
    GX_NEAR_MIP_LIN,
    GX_LIN_MIP_LIN

} GXTexFilter;

/********************************/
typedef enum _GXCITexFmt
{
    GX_TF_C4    = 0x8,
    GX_TF_C8    = 0x9,
    GX_TF_C14X2 = 0xa

} GXCITexFmt;

/********************************/
typedef enum _GXTexFmt
{
#if ( GX_REV == 1 )
    GX_TF_I4     = 0x0,
    GX_TF_I8     = 0x1,
    GX_TF_IA4    = 0x2,
    GX_TF_IA8    = 0x3,
    GX_TF_RGB565 = 0x4,
    GX_TF_RGB5A3 = 0x5,
    GX_TF_RGBA8  = 0x6,
    GX_TF_CMPR   = 0xE,
    GX_TF_A8     = (0x20 | GX_TF_I8), 
    GX_TF_Z8     = (0x10 | GX_TF_I8),
    GX_TF_Z16    = (0x10 | GX_TF_IA8),
    GX_TF_Z24X8  = (0x10 | GX_TF_RGBA8)

#else // ( GX_REV >= 2 )

#define _GX_TF_CTF     0x20 /* copy-texture-format only */
#define _GX_TF_ZTF     0x10 /* Z-texture-format */

    GX_TF_I4     = 0x0,
    GX_TF_I8     = 0x1,
    GX_TF_IA4    = 0x2,
    GX_TF_IA8    = 0x3,
    GX_TF_RGB565 = 0x4,
    GX_TF_RGB5A3 = 0x5,
    GX_TF_RGBA8  = 0x6,
    GX_TF_CMPR   = 0xE,

    GX_CTF_R4    = 0x0 | _GX_TF_CTF,
    GX_CTF_RA4   = 0x2 | _GX_TF_CTF,
    GX_CTF_RA8   = 0x3 | _GX_TF_CTF,
    GX_CTF_YUVA8 = 0x6 | _GX_TF_CTF,
    GX_CTF_A8    = 0x7 | _GX_TF_CTF,
    GX_CTF_R8    = 0x8 | _GX_TF_CTF,
    GX_CTF_G8    = 0x9 | _GX_TF_CTF,
    GX_CTF_B8    = 0xA | _GX_TF_CTF,
    GX_CTF_RG8   = 0xB | _GX_TF_CTF,
    GX_CTF_GB8   = 0xC | _GX_TF_CTF,

    GX_TF_Z8     = 0x1 | _GX_TF_ZTF,
    GX_TF_Z16    = 0x3 | _GX_TF_ZTF,
    GX_TF_Z24X8  = 0x6 | _GX_TF_ZTF,

    GX_CTF_Z4    = 0x0 | _GX_TF_ZTF | _GX_TF_CTF,
    GX_CTF_Z8M   = 0x9 | _GX_TF_ZTF | _GX_TF_CTF,
    GX_CTF_Z8L   = 0xA | _GX_TF_ZTF | _GX_TF_CTF,
    GX_CTF_Z16L  = 0xC | _GX_TF_ZTF | _GX_TF_CTF,

    GX_TF_A8     = GX_CTF_A8 // to keep compatibility
#endif
} GXTexFmt;

/********************************/
typedef enum _GXTlutFmt
{
    GX_TL_IA8    = 0x0,
    GX_TL_RGB565 = 0x1,
    GX_TL_RGB5A3 = 0x2,
    GX_MAX_TLUTFMT

} GXTlutFmt;

/********************************/
typedef enum _GXTlutSize
{
    GX_TLUT_16 = 1,	// number of 16 entry blocks.
    GX_TLUT_32 = 2,
    GX_TLUT_64 = 4,
    GX_TLUT_128 = 8,
    GX_TLUT_256 = 16,
    GX_TLUT_512 = 32,
    GX_TLUT_1K = 64,
    GX_TLUT_2K = 128,
    GX_TLUT_4K = 256,
    GX_TLUT_8K = 512,
    GX_TLUT_16K = 1024

} GXTlutSize;

/********************************/
typedef enum _GXTlut
{
    // default 256-entry TLUTs
    GX_TLUT0 = 0,
    GX_TLUT1,
    GX_TLUT2,
    GX_TLUT3,
    GX_TLUT4,
    GX_TLUT5,
    GX_TLUT6,
    GX_TLUT7,
    GX_TLUT8,
    GX_TLUT9,
    GX_TLUT10,
    GX_TLUT11,
    GX_TLUT12,
    GX_TLUT13,
    GX_TLUT14,
    GX_TLUT15,
    GX_BIGTLUT0,
    GX_BIGTLUT1,
    GX_BIGTLUT2,
    GX_BIGTLUT3

} GXTlut;

/********************************/
typedef enum _GXTexMapID
{
    GX_TEXMAP0,
    GX_TEXMAP1,
    GX_TEXMAP2,
    GX_TEXMAP3,
    GX_TEXMAP4,
    GX_TEXMAP5,
    GX_TEXMAP6,
    GX_TEXMAP7,
    GX_MAX_TEXMAP,

    GX_TEXMAP_NULL = 0xff,
    GX_TEX_DISABLE = 0x100	// mask: disables texture look up

} GXTexMapID;

#define GX_MAX_TEXMAPS      GX_MAX_TEXMAP

/********************************/
typedef enum _GXTexCacheSize
{
    GX_TEXCACHE_32K,
    GX_TEXCACHE_128K,
    GX_TEXCACHE_512K,
    GX_TEXCACHE_NONE

} GXTexCacheSize;

/********************************/
typedef enum _GXIndTexFormat
{
    GX_ITF_8,		// 8 bit texture offsets.
    GX_ITF_5,		// 5 bit texture offsets.
    GX_ITF_4,		// 4 bit texture offsets.
    GX_ITF_3,		// 3 bit texture offsets.
    GX_MAX_ITFORMAT

} GXIndTexFormat;

/********************************/
typedef enum _GXIndTexBiasSel
{
    GX_ITB_NONE,	
    GX_ITB_S,
    GX_ITB_T,
    GX_ITB_ST,
    GX_ITB_U,
    GX_ITB_SU,
    GX_ITB_TU,
    GX_ITB_STU,
    GX_MAX_ITBIAS

} GXIndTexBiasSel;

/********************************/
typedef enum _GXIndTexAlphaSel
{
    GX_ITBA_OFF,
    GX_ITBA_S,
    GX_ITBA_T,
    GX_ITBA_U,
    GX_MAX_ITBALPHA

} GXIndTexAlphaSel;

/********************************/
typedef enum _GXIndTexMtxID
{
    GX_ITM_OFF,
    GX_ITM_0,
    GX_ITM_1,
    GX_ITM_2,
    GX_ITM_S0 = 5,
    GX_ITM_S1,
    GX_ITM_S2,
    GX_ITM_T0 = 9,
    GX_ITM_T1,
    GX_ITM_T2

} GXIndTexMtxID;

/********************************/
typedef enum _GXIndTexWrap
{
    GX_ITW_OFF,		// no wrapping
    GX_ITW_256,		// wrap 256
    GX_ITW_128,		// wrap 128
    GX_ITW_64, 		// wrap 64
    GX_ITW_32, 		// wrap 32
    GX_ITW_16, 		// wrap 16
    GX_ITW_0, 		// wrap 0
    GX_MAX_ITWRAP

} GXIndTexWrap;

/********************************/
typedef enum _GXIndTexScale
{
    GX_ITS_1,		// Scale by 1.
    GX_ITS_2,		// Scale by 1/2.
    GX_ITS_4,		// Scale by 1/4.
    GX_ITS_8,		// Scale by 1/8.
    GX_ITS_16,		// Scale by 1/16.
    GX_ITS_32,		// Scale by 1/32.
    GX_ITS_64,		// Scale by 1/64.
    GX_ITS_128,		// Scale by 1/128.
    GX_ITS_256,		// Scale by 1/256.
    GX_MAX_ITSCALE

} GXIndTexScale;

/********************************/
typedef enum _GXIndTexStageID
{
    GX_INDTEXSTAGE0,	// Indirect Texture Stage Names.
    GX_INDTEXSTAGE1,
    GX_INDTEXSTAGE2,
    GX_INDTEXSTAGE3,
    GX_MAX_INDTEXSTAGE

} GXIndTexStageID;

/********************************/
typedef enum _GXTevStageID
{
    GX_TEVSTAGE0,
    GX_TEVSTAGE1,
    GX_TEVSTAGE2,
    GX_TEVSTAGE3,
    GX_TEVSTAGE4,
    GX_TEVSTAGE5,
    GX_TEVSTAGE6,
    GX_TEVSTAGE7,
    GX_TEVSTAGE8,
    GX_TEVSTAGE9,
    GX_TEVSTAGE10,
    GX_TEVSTAGE11,
    GX_TEVSTAGE12,
    GX_TEVSTAGE13,
    GX_TEVSTAGE14,
    GX_TEVSTAGE15,
    GX_MAX_TEVSTAGE

} GXTevStageID;

#define GX_MAX_TEVSTAGES            GX_MAX_TEVSTAGE

/********************************/
typedef enum _GXTevRegID
{
    GX_TEVPREV = 0,
    GX_TEVREG0,
    GX_TEVREG1,
    GX_TEVREG2,
    GX_MAX_TEVREG

} GXTevRegID;

/********************************/
typedef enum _GXTevOp
{
#if ( GX_REV == 1 )
    GX_TEV_ADD,
    GX_TEV_SUB

#else // ( GX_REV >= 2 )
    GX_TEV_ADD = 0,
    GX_TEV_SUB = 1,
    
    GX_TEV_COMP_R8_GT    = 8,
    GX_TEV_COMP_R8_EQ    = 9,
    GX_TEV_COMP_GR16_GT  = 10,
    GX_TEV_COMP_GR16_EQ  = 11,
    GX_TEV_COMP_BGR24_GT = 12,
    GX_TEV_COMP_BGR24_EQ = 13,
    GX_TEV_COMP_RGB8_GT  = 14,
    GX_TEV_COMP_RGB8_EQ  = 15,
    
    GX_TEV_COMP_A8_GT = GX_TEV_COMP_RGB8_GT, // for alpha channel
    GX_TEV_COMP_A8_EQ = GX_TEV_COMP_RGB8_EQ  // for alpha channel

#endif
} GXTevOp;

/********************************/
typedef enum _GXTevColorArg
{
#if ( GX_REV == 1 )
    GX_CC_CPREV,
    GX_CC_APREV,
    GX_CC_C0,
    GX_CC_A0,
    GX_CC_C1,
    GX_CC_A1,
    GX_CC_C2,
    GX_CC_A2,
    GX_CC_TEXC,
    GX_CC_TEXA,
    GX_CC_RASC,
    GX_CC_RASA,
    GX_CC_ONE,
    GX_CC_HALF,
    GX_CC_QUARTER,
    GX_CC_ZERO,

    GX_CC_TEXRRR,
    GX_CC_TEXGGG,
    GX_CC_TEXBBB

#else // ( GX_REV >= 2 )
    GX_CC_CPREV,
    GX_CC_APREV,
    GX_CC_C0,
    GX_CC_A0,
    GX_CC_C1,
    GX_CC_A1,
    GX_CC_C2,
    GX_CC_A2,
    GX_CC_TEXC,
    GX_CC_TEXA,
    GX_CC_RASC,
    GX_CC_RASA,
    GX_CC_ONE,
    GX_CC_HALF,
    GX_CC_KONST,
    GX_CC_ZERO

#ifndef GX_NO_LEGACY_HW1
    ,             // separator needed only in this case

    GX_CC_TEXRRR, // obsolete
    GX_CC_TEXGGG, // obsolete
    GX_CC_TEXBBB, // obsolete

    GX_CC_QUARTER = GX_CC_KONST // obsolete, to keep compatibility
#endif

#endif
} GXTevColorArg;

/********************************/
typedef enum _GXTevAlphaArg
{
#if ( GX_REV == 1 )
    GX_CA_APREV,
    GX_CA_A0,
    GX_CA_A1,
    GX_CA_A2,
    GX_CA_TEXA,
    GX_CA_RASA,
    GX_CA_ONE,
    GX_CA_ZERO

#else // ( GX_REV >= 2 )
    GX_CA_APREV,
    GX_CA_A0,
    GX_CA_A1,
    GX_CA_A2,
    GX_CA_TEXA,
    GX_CA_RASA,
    GX_CA_KONST,
    GX_CA_ZERO

#ifndef GX_NO_LEGACY_HW1
    ,                       // separator needed only in this case

    GX_CA_ONE = GX_CA_KONST // obsolete, to keep compatibility
#endif

#endif
} GXTevAlphaArg;

/********************************/
typedef enum _GXTevBias
{
    GX_TB_ZERO,
    GX_TB_ADDHALF,
    GX_TB_SUBHALF,
    GX_MAX_TEVBIAS

} GXTevBias;

/********************************/
typedef enum _GXTevClampMode
{
    GX_TC_LINEAR,
    GX_TC_GE,
    GX_TC_EQ,
    GX_TC_LE,
    GX_MAX_TEVCLAMPMODE

} GXTevClampMode;

/********************************/
#if ( GX_REV != 1 ) // GX Revision 2 or later only
typedef enum _GXTevKColorID
{
    GX_KCOLOR0 = 0,
    GX_KCOLOR1,
    GX_KCOLOR2,
    GX_KCOLOR3,
    GX_MAX_KCOLOR

} GXTevKColorID;
#endif // ( GX_REV != 1 )

/********************************/
#if ( GX_REV != 1 ) // GX Revision 2 or later only
typedef enum _GXTevKColorSel
{
    GX_TEV_KCSEL_8_8  = 0x00,
    GX_TEV_KCSEL_7_8  = 0x01,
    GX_TEV_KCSEL_6_8  = 0x02,
    GX_TEV_KCSEL_5_8  = 0x03,
    GX_TEV_KCSEL_4_8  = 0x04,
    GX_TEV_KCSEL_3_8  = 0x05,
    GX_TEV_KCSEL_2_8  = 0x06,
    GX_TEV_KCSEL_1_8  = 0x07,

    GX_TEV_KCSEL_1    = GX_TEV_KCSEL_8_8,
    GX_TEV_KCSEL_3_4  = GX_TEV_KCSEL_6_8,
    GX_TEV_KCSEL_1_2  = GX_TEV_KCSEL_4_8,
    GX_TEV_KCSEL_1_4  = GX_TEV_KCSEL_2_8,

    GX_TEV_KCSEL_K0   = 0x0C,
    GX_TEV_KCSEL_K1   = 0x0D,
    GX_TEV_KCSEL_K2   = 0x0E,
    GX_TEV_KCSEL_K3   = 0x0F,
    GX_TEV_KCSEL_K0_R = 0x10,
    GX_TEV_KCSEL_K1_R = 0x11,
    GX_TEV_KCSEL_K2_R = 0x12,
    GX_TEV_KCSEL_K3_R = 0x13,
    GX_TEV_KCSEL_K0_G = 0x14,
    GX_TEV_KCSEL_K1_G = 0x15,
    GX_TEV_KCSEL_K2_G = 0x16,
    GX_TEV_KCSEL_K3_G = 0x17,
    GX_TEV_KCSEL_K0_B = 0x18,
    GX_TEV_KCSEL_K1_B = 0x19,
    GX_TEV_KCSEL_K2_B = 0x1A,
    GX_TEV_KCSEL_K3_B = 0x1B,
    GX_TEV_KCSEL_K0_A = 0x1C,
    GX_TEV_KCSEL_K1_A = 0x1D,
    GX_TEV_KCSEL_K2_A = 0x1E,
    GX_TEV_KCSEL_K3_A = 0x1F

} GXTevKColorSel;
#endif // ( GX_REV != 1 )

/********************************/
#if ( GX_REV != 1 ) // GX Revision 2 or later only
typedef enum _GXTevKAlphaSel
{
    GX_TEV_KASEL_8_8  = 0x00,
    GX_TEV_KASEL_7_8  = 0x01,
    GX_TEV_KASEL_6_8  = 0x02,
    GX_TEV_KASEL_5_8  = 0x03,
    GX_TEV_KASEL_4_8  = 0x04,
    GX_TEV_KASEL_3_8  = 0x05,
    GX_TEV_KASEL_2_8  = 0x06,
    GX_TEV_KASEL_1_8  = 0x07,

    GX_TEV_KASEL_1    = GX_TEV_KASEL_8_8,
    GX_TEV_KASEL_3_4  = GX_TEV_KASEL_6_8,
    GX_TEV_KASEL_1_2  = GX_TEV_KASEL_4_8,
    GX_TEV_KASEL_1_4  = GX_TEV_KASEL_2_8,

    GX_TEV_KASEL_K0_R = 0x10,
    GX_TEV_KASEL_K1_R = 0x11,
    GX_TEV_KASEL_K2_R = 0x12,
    GX_TEV_KASEL_K3_R = 0x13,
    GX_TEV_KASEL_K0_G = 0x14,
    GX_TEV_KASEL_K1_G = 0x15,
    GX_TEV_KASEL_K2_G = 0x16,
    GX_TEV_KASEL_K3_G = 0x17,
    GX_TEV_KASEL_K0_B = 0x18,
    GX_TEV_KASEL_K1_B = 0x19,
    GX_TEV_KASEL_K2_B = 0x1A,
    GX_TEV_KASEL_K3_B = 0x1B,
    GX_TEV_KASEL_K0_A = 0x1C,
    GX_TEV_KASEL_K1_A = 0x1D,
    GX_TEV_KASEL_K2_A = 0x1E,
    GX_TEV_KASEL_K3_A = 0x1F

} GXTevKAlphaSel;
#endif // ( GX_REV != 1 )

/********************************/
#if ( GX_REV != 1 ) // GX Revision 2 or later only
typedef enum _GXTevSwapSel
{
    GX_TEV_SWAP0 = 0,
    GX_TEV_SWAP1,
    GX_TEV_SWAP2,
    GX_TEV_SWAP3,
    GX_MAX_TEVSWAP

} GXTevSwapSel;
#endif // ( GX_REV != 1 )

/********************************/
#if ( GX_REV != 1 ) // GX Revision 2 or later only
typedef enum _GXTevColorChan
{
    GX_CH_RED = 0,
    GX_CH_GREEN,
    GX_CH_BLUE,
    GX_CH_ALPHA

} GXTevColorChan;
#endif // ( GX_REV != 1 )

/********************************/
typedef enum _GXAlphaOp
{
    GX_AOP_AND,
    GX_AOP_OR,
    GX_AOP_XOR,
    GX_AOP_XNOR,
    GX_MAX_ALPHAOP

} GXAlphaOp;

/********************************/
typedef enum _GXTevScale
{
    GX_CS_SCALE_1,
    GX_CS_SCALE_2,
    GX_CS_SCALE_4,
    GX_CS_DIVIDE_2,
    GX_MAX_TEVSCALE

} GXTevScale;

/********************************/
typedef enum _GXFogType
{
    GX_FOG_NONE		     = 0x00,

    GX_FOG_PERSP_LIN     = 0x02,
    GX_FOG_PERSP_EXP     = 0x04,
    GX_FOG_PERSP_EXP2    = 0x05,
    GX_FOG_PERSP_REVEXP  = 0x06,
    GX_FOG_PERSP_REVEXP2 = 0x07,

    GX_FOG_ORTHO_LIN     = 0x0A,
    GX_FOG_ORTHO_EXP     = 0x0C,
    GX_FOG_ORTHO_EXP2    = 0x0D,
    GX_FOG_ORTHO_REVEXP  = 0x0E,
    GX_FOG_ORTHO_REVEXP2 = 0x0F,

    // For compatibility with former versions
    GX_FOG_LIN 		= GX_FOG_PERSP_LIN,
    GX_FOG_EXP		= GX_FOG_PERSP_EXP,
    GX_FOG_EXP2		= GX_FOG_PERSP_EXP2,
    GX_FOG_REVEXP  	= GX_FOG_PERSP_REVEXP,
    GX_FOG_REVEXP2 	= GX_FOG_PERSP_REVEXP2

} GXFogType;

/********************************/
typedef enum _GXBlendMode
{
#if ( GX_REV == 1 )
    GX_BM_NONE,
    GX_BM_BLEND,
    GX_BM_LOGIC,
    GX_MAX_BLENDMODE

#else // ( GX_REV >= 2 )
    GX_BM_NONE,
    GX_BM_BLEND,
    GX_BM_LOGIC,
    GX_BM_SUBTRACT,
    GX_MAX_BLENDMODE

#endif
} GXBlendMode;

/********************************/
typedef enum _GXBlendFactor
{
    GX_BL_ZERO,
    GX_BL_ONE,
    GX_BL_SRCCLR,
    GX_BL_INVSRCCLR,
    GX_BL_SRCALPHA,
    GX_BL_INVSRCALPHA,
    GX_BL_DSTALPHA,
    GX_BL_INVDSTALPHA,

    GX_BL_DSTCLR = GX_BL_SRCCLR,
    GX_BL_INVDSTCLR = GX_BL_INVSRCCLR

} GXBlendFactor;

/********************************/
typedef enum _GXCompare
{
    GX_NEVER,
    GX_LESS,
    GX_EQUAL,
    GX_LEQUAL,
    GX_GREATER,
    GX_NEQUAL,
    GX_GEQUAL,
    GX_ALWAYS

} GXCompare;

/********************************/
typedef enum _GXLogicOp
{
    GX_LO_CLEAR,
    GX_LO_AND,
    GX_LO_REVAND,
    GX_LO_COPY,
    GX_LO_INVAND,
    GX_LO_NOOP,
    GX_LO_XOR,
    GX_LO_OR,
    GX_LO_NOR,
    GX_LO_EQUIV,
    GX_LO_INV,
    GX_LO_REVOR,
    GX_LO_INVCOPY,
    GX_LO_INVOR,
    GX_LO_NAND,
    GX_LO_SET

} GXLogicOp;

/********************************/
typedef enum _GXPixelFmt
{
    GX_PF_RGB8_Z24,
    GX_PF_RGBA6_Z24,
    GX_PF_RGB565_Z16,
    GX_PF_Z24,
    GX_PF_Y8,
    GX_PF_U8,
    GX_PF_V8,
    GX_PF_YUV420

} GXPixelFmt;

/********************************/
typedef enum _GXZFmt16
{
    GX_ZC_LINEAR,
    GX_ZC_NEAR,
    GX_ZC_MID,
    GX_ZC_FAR

} GXZFmt16;

/********************************/
typedef enum _GXTevMode
{
    GX_MODULATE,
    GX_DECAL,
    GX_BLEND,
    GX_REPLACE,
    GX_PASSCLR

} GXTevMode;

/********************************/
typedef enum _GXGamma
{
    GX_GM_1_0,
    GX_GM_1_7,
    GX_GM_2_2

} GXGamma;

/********************************/
typedef enum _GXProjectionType
{
    GX_PERSPECTIVE,
    GX_ORTHOGRAPHIC

} GXProjectionType;


/********************************/
typedef enum _GXEvent
{
    GX_VCACHE_MISS_ALL,
    GX_VCACHE_MISS_POS,
    GX_VCACHE_MISS_NRM

} GXEvent;

/********************************/
typedef enum _GXFBClamp
{
    GX_CLAMP_NONE,
    GX_CLAMP_TOP,
    GX_CLAMP_BOTTOM

} GXFBClamp;

/********************************/
typedef enum _GXAnisotropy
{
    GX_ANISO_1,
    GX_ANISO_2,
    GX_ANISO_4,
    GX_MAX_ANISOTROPY

} GXAnisotropy;

/********************************/
typedef enum _GXZTexOp
{
    GX_ZT_DISABLE,
    GX_ZT_ADD,
    GX_ZT_REPLACE,
    GX_MAX_ZTEXOP

} GXZTexOp;

/********************************/
typedef enum _GXAlphaReadMode
{
    GX_READ_00,
    GX_READ_FF,
    GX_READ_NONE

} GXAlphaReadMode;

/********************************/
typedef enum _GXPerf0
{
    GX_PERF0_VERTICES,
    GX_PERF0_CLIP_VTX,
    GX_PERF0_CLIP_CLKS,
    GX_PERF0_XF_WAIT_IN,
    GX_PERF0_XF_WAIT_OUT,
    GX_PERF0_XF_XFRM_CLKS,
    GX_PERF0_XF_LIT_CLKS,
    GX_PERF0_XF_BOT_CLKS,
    GX_PERF0_XF_REGLD_CLKS,
    GX_PERF0_XF_REGRD_CLKS,
    GX_PERF0_CLIP_RATIO,

    GX_PERF0_TRIANGLES,
    GX_PERF0_TRIANGLES_CULLED,
    GX_PERF0_TRIANGLES_PASSED,
    GX_PERF0_TRIANGLES_SCISSORED,
    GX_PERF0_TRIANGLES_0TEX,
    GX_PERF0_TRIANGLES_1TEX,
    GX_PERF0_TRIANGLES_2TEX,
    GX_PERF0_TRIANGLES_3TEX,
    GX_PERF0_TRIANGLES_4TEX,
    GX_PERF0_TRIANGLES_5TEX,
    GX_PERF0_TRIANGLES_6TEX,
    GX_PERF0_TRIANGLES_7TEX,
    GX_PERF0_TRIANGLES_8TEX,
    GX_PERF0_TRIANGLES_0CLR,
    GX_PERF0_TRIANGLES_1CLR,
    GX_PERF0_TRIANGLES_2CLR,

    GX_PERF0_QUAD_0CVG,
    GX_PERF0_QUAD_NON0CVG,
    GX_PERF0_QUAD_1CVG,
    GX_PERF0_QUAD_2CVG,
    GX_PERF0_QUAD_3CVG,
    GX_PERF0_QUAD_4CVG,
    GX_PERF0_AVG_QUAD_CNT,

    GX_PERF0_CLOCKS,
    GX_PERF0_NONE

} GXPerf0;

/********************************/
typedef enum _GXPerf1
{
    GX_PERF1_TEXELS,
    GX_PERF1_TX_IDLE,
    GX_PERF1_TX_REGS,
    GX_PERF1_TX_MEMSTALL,
    GX_PERF1_TC_CHECK1_2,
    GX_PERF1_TC_CHECK3_4,
    GX_PERF1_TC_CHECK5_6,
    GX_PERF1_TC_CHECK7_8,
    GX_PERF1_TC_MISS,

    GX_PERF1_VC_ELEMQ_FULL,
    GX_PERF1_VC_MISSQ_FULL,
    GX_PERF1_VC_MEMREQ_FULL,
    GX_PERF1_VC_STATUS7,
    GX_PERF1_VC_MISSREP_FULL,
    GX_PERF1_VC_STREAMBUF_LOW,
    GX_PERF1_VC_ALL_STALLS,
    GX_PERF1_VERTICES,

    GX_PERF1_FIFO_REQ,
    GX_PERF1_CALL_REQ,
    GX_PERF1_VC_MISS_REQ,
    GX_PERF1_CP_ALL_REQ,

    GX_PERF1_CLOCKS,
    GX_PERF1_NONE

} GXPerf1;

/********************************/
typedef enum _GXVCachePerf
{
    GX_VC_POS,
    GX_VC_NRM,
    GX_VC_CLR0,
    GX_VC_CLR1,
    GX_VC_TEX0,
    GX_VC_TEX1,
    GX_VC_TEX2,
    GX_VC_TEX3,
    GX_VC_TEX4,
    GX_VC_TEX5,
    GX_VC_TEX6,
    GX_VC_TEX7,
    GX_VC_ALL = 0xf

} GXVCachePerf;

/********************************/
typedef enum _GXCopyMode
{
    GX_COPY_PROGRESSIVE = 0,
    GX_COPY_INTLC_EVEN  = 2,
    GX_COPY_INTLC_ODD   = 3

} GXCopyMode;

/********************************/
typedef enum _GXMiscToken
{
    GX_MT_XF_FLUSH           = 1,
    GX_MT_DL_SAVE_CONTEXT    = 2,
    GX_MT_ABORT_WAIT_COPYOUT = 3, 
    GX_MT_NULL               = 0

} GXMiscToken;

/********************************/
typedef enum _GXXFFlushVal
{
    GX_XF_FLUSH_NONE  = 0,
    GX_XF_FLUSH_SAFE  = 8

} GXXFFlushVal;

/********************************/
// Compile-time checks to ensure that enumerations are compiled
// as 32-bit integers.  If your build breaks at this line,
// please ensure that you enable the "enumerations are integers"
// option for the compiler.

#define GXCTASSERT(cond)  int GXCTASSERT( int assertion_failed[ (cond) != 0 ] )

GXCTASSERT(sizeof(GXAttrType) == sizeof(u32));

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXENUM_H__
