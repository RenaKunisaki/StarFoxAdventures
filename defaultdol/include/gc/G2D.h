/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     G2D.h (Dolphin 2D API by Paul Donnelly, Nov. 1999)

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: G2D.h,v $
  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Ported from dolphin source tree.

    
    2     1999/12/12 10:09p Paul
    
    1     1999/12/09 12:29p Paul

 *---------------------------------------------------------------------------*/

#ifndef G2DAPI_H
#define G2DAPI_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <demo.h>

typedef enum
{
    G2D_CTG_TEXTURE      = 0x0,   // Tiles with this format use texture mapping.
                                 // The texture format is specified within the GXTexObj

    G2D_CTG_RGB_DIRECT   = 0x1,   // Tiles with this format have direct RGB8 color only
    G2D_CTG_RGBA_INDEX8  = 0x2,   // Tiles with this format have 8-bit indexed RGBA8 color only
    G2D_CTG_EMPTY        = 0x3    // Tiles with this format are not rendered at all
}
G2DMatCtg;

typedef struct
{
    s32        nReserved; // Used by the API for sorting
    G2DMatCtg  nCategory; // Material Category
    GXColor   *color;     // Material Color
    GXTexObj  *to;        // Texture object
    u8        *clut;      // Color Look-Up Table
    //s16        nWidth;    // Width of texture map (32, 64, 128, 256, 512, 1024)
    //s16        nHeight;   // Height of texture map (32, 64, 128, 256, 512, 1024)
}
G2DMatDesc;

typedef struct
{
    u8  nMaterial;  // Tile type / texture-map selection
    u8  nS;         // S texture coordinate
    u8  nT;         // T texture coordinate
    u8  nCI;        // Color Index
    u8  aUser[4];   // User data
}
G2DTileDesc;

typedef struct
{
    void         *map;            // Map of tile indices
    s8            nHS;            // Horizontal Shift  (Width of layer = 1<<nHS)
    s8            nVS;            // Vertical Shift   (Height of layer = 1<<nVS)
    s8            nBPI;           // Bytes per Index (1,2)
    s16           nTileWidth;     // Width of tiles in pixels (should be >=16, power of 2)
    s16           nTileHeight;    // Height of tiles in pixels (should be >=16, power of 2)
    s8            bWrap;          // Whether layer wraps or not
    u8            nNumMaterials;  // Number of tile types

    G2DTileDesc  *tileDesc;       // Tile Descriptor Table - specifies Type and S,T
                                  // coordinates or Color RGB or Color Index for each tile
    G2DMatDesc   *matDesc;        // Material Descriptor Table - specifies texture map or
                                  // CLUT used by tiles of this type, plus material color
}
G2DLayer;

typedef struct
{
    u16        nTlcS;     // Top Left S coordinate within texture map (in pixels)
    u16        nTlcT;     // Top Left T coordinate within texture map (in pixels)
    u16        nWidth;    // Width of sprite (in pixels)
    u16        nHeight;   // Height of sprite (in pixels)
    GXTexObj  *to;        // Source Texture Map (RGBA)

    f32        rS0, rT0;  // Normalized texture coordinates
    f32        rS1, rT1;  // No need to set these if you call G2DInitSprite
}
G2DSprite;

typedef struct
{
    f32 rPosX;      // Position vector X component (measured in tile coordinates)
    f32 rPosY;      // Position vector Y component (measured in tile coordinates)
    f32 rOriX;      // Normalized orientation vector X component
    f32 rOriY;      // Normalized orientation vector Y component
}
G2DPosOri;          // 2D Position and Orientation

extern void G2DInitSprite( G2DSprite *sprite );
extern void G2DDrawSprite( G2DSprite *sprite, G2DPosOri *posOri );
extern void G2DDrawLayer( G2DLayer *layer, s8 *aSortBuffer );
extern void G2DSetCamera( G2DPosOri *po );
extern void G2DInitWorld( u32 nWorldX, u32 nWorldY );
extern void G2DSetViewport( u16 nLeft, u16 nTop, u16 nWidth, u16 nHeight );

#ifdef __cplusplus
	}
#endif

/* Prevent re-inclusion */
#endif
