/*---------------------------------------------------------------------------*
  Project:     font library
  File:        fnt.h

  Copyright (C) 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: fnt.h,v $
  Revision 1.1  2006/10/18 02:49:26  nishida_yasunari
  Initial check in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __FNT_H__
#define __FNT_H__

#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FNT_RESOURCE_HEADER_SIZE    (16 * 1024)

typedef enum
{
    FNT_CONSTRUCT_MORE_DATA,
    FNT_CONSTRUCT_FINISH,
    FNT_CONSTRUCT_ERROR,
    FNT_CONSTRUCT_CONTINUE,     // internal use
    FNT_MAX_CONSTRUCT_RESULT
} FNTConstructResult;

typedef enum
{
    FNT_ENCODING_UTF8,          // UTF-8
    FNT_ENCODING_UTF16,         // UTF-16
    FNT_ENCODING_SJIS,          // ShiftJIS
    FNT_ENCODING_CP1252,        // CP1252
    FNT_MAX_ENCODING
} FNTEncoding;

typedef struct
{
    void*   buffer;             // pointer to font buffer
    void*   info;               // pointer to font information (in buffer)
    u16*    adjust;             // glyph index adjust array (in buffer)
    u32     bufferSize;         // font buffer size
    u8      encoding;           // encoding for string
    u8      padding_[3];
    //20B
    u32     construct[108/4];   // construct context
    //128B
    u8      bufferTop[];        // font buffer top
} FNTHeader;

typedef struct
{
    void*       image;      // pointer to texture iamge sheet
    s8          left;       // left space width
    u8          glyphWidth; // glyph width
    s8          charWidth;  // character width = left + glyphWidth + right space wdith
    u8          charHeight; // character height
    GXTexFmt    texFormat;  // texture image sheet format
    u16         texWidth;   // texture image sheet width  (texels)
    u16         texHeight;  // texture image sheet height (texels)
    u16         cellX;      // horizontal position in texture image
    u16         cellY;      // vertical position in texture image
} FNTTexture;

typedef struct
{
    u8  width;          // width (for TAB & scale standard)
    u8  height;         // height
    u8  maxCharWidth;   // max character width
    u8  ascent;         // ascent
    u8  descent;        // descent
    s8  baselinePos;    // base line position
    s8  leading;        // leading (line feed)
    u8  cellWidth;      // cell width (max glyph wdith)
    u8  cellHeight;     // cell height
    u16 texWidth;       // texture image sheet width  (texels)
    u16 texHeight;      // texture image sheet height (texels)
} FNTMetrics;

// init functions
u32 FNTGetDataSize( void* resource );
BOOL FNTConstruct( FNTHeader* font, void* resource );
void FNTInitStreamingConstruct( FNTHeader* font, u32 dataSize );
FNTConstructResult FNTStreamingConstruct(
    FNTHeader* font,
    void* stream,
    u32 streamSize);

// texture functions
char* FNTGetTexture(
    const FNTHeader* font,
    const char* string,
    FNTTexture* tex );
GXTexFmt FNTGetTextureFormat( const FNTHeader* font );

// metrics functions
char* FNTGetWidth( const FNTHeader* font, const char* string, s32* width );
void FNTGetMetrics( const FNTHeader* font, FNTMetrics* metrics );

// encoding functions
FNTEncoding FNTGetEncoding( const FNTHeader* font );
FNTEncoding FNTSetEncoding( FNTHeader* font, FNTEncoding encoding );

#ifdef __cplusplus
}
#endif

#endif // __FNT_H__
