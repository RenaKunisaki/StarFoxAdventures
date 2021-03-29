/*---------------------------------------------------------------------------*
  Project:  TPL data access library
  File:     TPL.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  This TPL library provides a simple interface for TPL files, which used
  to be the texture data format for the character pipeline in the Nintendo GameCube
  software development kit.
  This file also contains wrapper definitions for the conventional TEX API in
  the new library.
 *---------------------------------------------------------------------------*/

#ifndef TPL_H
#define TPL_H

/*---------------------------------------------------------------------------*/
#include <revolution/gx/GXTexture.h>
#include <revolution/os.h>
#include <revolution/dvd.h>

// Macros for conventional character pipeline functions
#ifdef  COMPATIBLE_OLD_TEXPALETTE

#define TEXPalettePtr       TPLPalettePtr
#define TEXDescriptorPtr    TPLDescriptorPtr
#define TEXGetPalette       TPLGetPalette
#define TEXReleasePalette   TPLReleasePalette
#define TEXGet              TPLGet

#define TEXGetGXTexObjFromPalette   TPLGetGXTexObjFromPalette
#define TEXGetGXTexObjFromPaletteCI TPLGetGXTexObjFromPaletteCI

#endif

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Structure definitions
 *---------------------------------------------------------------------------*/
typedef struct
{
    u16             numEntries;
    u8              unpacked;
    u8              pad8;

    GXTlutFmt       format;
    Ptr             data;

} TPLClutHeader, *TPLClutHeaderPtr;

/*---------------------------------------------------------------------------*/
typedef struct
{
    u16             height;
    u16             width;

    u32             format;
    Ptr             data;

    GXTexWrapMode   wrapS;
    GXTexWrapMode   wrapT;

    GXTexFilter     minFilter;
    GXTexFilter     magFilter;

    float           LODBias;

    u8              edgeLODEnable;
    u8              minLOD;
    u8              maxLOD;
    u8              unpacked;

} TPLHeader, *TPLHeaderPtr;

/*---------------------------------------------------------------------------*/
typedef struct
{
    TPLHeaderPtr        textureHeader;
    TPLClutHeaderPtr    CLUTHeader;

} TPLDescriptor, *TPLDescriptorPtr;

/*---------------------------------------------------------------------------*/
typedef struct
{
    u32                 versionNumber;

    u32                 numDescriptors;
    TPLDescriptorPtr    descriptorArray;

} TPLPalette, *TPLPalettePtr;

/*---------------------------------------------------------------------------*
    Function prototypes
 *---------------------------------------------------------------------------*/
void                TPLBind         ( TPLPalettePtr pal );
TPLDescriptorPtr    TPLGet          ( TPLPalettePtr pal, u32 id );

void TPLGetGXTexObjFromPalette      ( TPLPalettePtr pal, GXTexObj *to, u32 id );
void TPLGetGXTexObjFromPaletteCI    ( TPLPalettePtr pal, GXTexObj *to,
                                      GXTlutObj *tlo, GXTlut tluts, u32 id );



/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
