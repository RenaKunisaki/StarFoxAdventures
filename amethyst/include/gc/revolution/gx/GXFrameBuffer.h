/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXFrameBuffer.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXFrameBuffer.h,v $
  Revision 1.4  2006/08/10 13:38:01  urata
  Removed all rendermodes of progressive 50Hz.

  Revision 1.3  2006/05/18 09:40:10  urata
  Added the rendermode of progressive for PAL and MPAL.

  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    19    2002/02/12 14:29 Hirose
    Added GXNtsc480ProgSoft mode.
    
    18    2002/07/24 9:06 Hirose
    Resolved version conflicts.
    
    18    2002/04/09 15:15 Hirose
    const type specifier support. (worked by hiratsu@IRD)
    
    17    2001/11/15 9:24p Hirose
    Added GXGetYScaleFactor and GXGetNumXfbLines.
    
    16    2001/11/02 5:33p Hashida
    Renamed eu60 -> eurgb60
    
    15    2001/11/01 3:37p Hashida
    Added EURGB60 mode support.

    14    2001/10/18 11:54a Hashida
    No change.

    13    2001/10/14 8:05p Hashida
    Added RenderModeObj for EU60 mode.
    
    12    2001/09/21 3:44p Hashida
    Fixed wrong settings for Y origin in the double strike modes.
    
    11    2001/07/26 6:04p Carl
    Fixed extern for GXPal524IntAa.
    
    10    2001/04/12 3:49p Carl
    Added stuff for NTSC progressive modes.
    
    9     2001/01/11 4:31p Hashida
    Added a comment to show developers what the actual settings for each
    render mode is.
    
    8     2000/08/10 2:39p Carl
    Fixed GX_MAX_Z16 to equal GX_MAX_Z24.
    Added a comment to deprecate its usage.
    
    7     2000/06/06 1:29p Carl
    Removed obsolete API.
    
    6     2000/04/26 4:09p Alligator
    mod GXSetDispCopyFrame2Field to take GXCopyMode as param
    
    5     2000/04/08 2:47p Hirose
    Updated GXSetCopyFilter.
    
    4     2000/03/23 4:23p Carl
    Added GXAdjustForOverscan prototype.
    
    3     2000/03/13 6:18p Danm
    Fixed AA sampling locations to use x,y pairs.
    
    2     2000/02/12 5:16p Alligator
    Integrate ArtX source tree changes
    
    10    1999/10/13 5:19p Alligator
    changed CopySrc functions to use xorig, yorig, wd, ht
    
    9     1999/09/30 10:40p Yasu
    Renamed some GX functions and enums
    
    8     1999/09/24 4:23p Yasu
    Add GX_MAX_Z24/16 macro

    7     1999/09/22 6:14p Yasu
    Changed the parameter of GXSetDispCopySrc and GXSetTexCopySrc().

    6     1999/09/21 2:33p Alligator
    add aa flag, if aa set 16b pix format

    5     1999/09/16 3:42p Alligator
    update render mode api

    4     1999/09/09 3:04p Alligator
    move GXSetRenderMode to GX lib from emu

    3     1999/09/02 3:18p Ryan
    Made Frame Buffer Api changes

    2     1999/07/28 4:07p Alligator
    update header files and emulator for hw changes

    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXFRAMEBUFFER_H__
#define __GXFRAMEBUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

/*---------------------------------------------------------------------------*/
/*  Render Modes                                                             */
/*    (see the bottom of this file for actual settings used for each         */
/*     render mode)                                                          */
/*---------------------------------------------------------------------------*/
extern GXRenderModeObj GXNtsc240Ds;
extern GXRenderModeObj GXNtsc240DsAa;
extern GXRenderModeObj GXNtsc240Int;
extern GXRenderModeObj GXNtsc240IntAa;
extern GXRenderModeObj GXNtsc480IntDf;
extern GXRenderModeObj GXNtsc480Int;
extern GXRenderModeObj GXNtsc480IntAa;
extern GXRenderModeObj GXNtsc480Prog;
extern GXRenderModeObj GXNtsc480ProgSoft;
extern GXRenderModeObj GXNtsc480ProgAa;
extern GXRenderModeObj GXMpal240Ds;
extern GXRenderModeObj GXMpal240DsAa;
extern GXRenderModeObj GXMpal240Int;
extern GXRenderModeObj GXMpal240IntAa;
extern GXRenderModeObj GXMpal480IntDf;
extern GXRenderModeObj GXMpal480Int;
extern GXRenderModeObj GXMpal480IntAa;
extern GXRenderModeObj GXMpal480Prog;
extern GXRenderModeObj GXMpal480ProgSoft;
extern GXRenderModeObj GXMpal480ProgAa;
extern GXRenderModeObj GXPal264Ds;
extern GXRenderModeObj GXPal264DsAa;
extern GXRenderModeObj GXPal264Int;
extern GXRenderModeObj GXPal264IntAa;
extern GXRenderModeObj GXPal528IntDf;
extern GXRenderModeObj GXPal528Int;
extern GXRenderModeObj GXPal524IntAa; // Reduced due to overlap requirement!
extern GXRenderModeObj GXEurgb60Hz240Ds;
extern GXRenderModeObj GXEurgb60Hz240DsAa;
extern GXRenderModeObj GXEurgb60Hz240Int;
extern GXRenderModeObj GXEurgb60Hz240IntAa;
extern GXRenderModeObj GXEurgb60Hz480IntDf;
extern GXRenderModeObj GXEurgb60Hz480Int;
extern GXRenderModeObj GXEurgb60Hz480IntAa;
extern GXRenderModeObj GXEurgb60Hz480Prog;
extern GXRenderModeObj GXEurgb60Hz480ProgSoft;
extern GXRenderModeObj GXEurgb60Hz480ProgAa;

/*---------------------------------------------------------------------------*/
void GXAdjustForOverscan ( const GXRenderModeObj *rmin, GXRenderModeObj *rmout,
                           u16 hor, u16 ver );
void GXSetDispCopySrc ( u16 left, u16 top, u16 wd, u16 ht );
void GXSetTexCopySrc ( u16 left, u16 top, u16 wd, u16 ht );
void GXSetDispCopyDst ( u16 wd, u16 ht );
void GXSetTexCopyDst ( u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap );

void GXSetDispCopyFrame2Field( GXCopyMode mode );
void GXSetCopyClamp( GXFBClamp clamp );
u32  GXSetDispCopyYScale( f32 vscale );
void GXSetCopyClear( GXColor clear_clr, u32 clear_z);
void GXSetCopyFilter( GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7] );
void GXSetDispCopyGamma( GXGamma gamma );

void GXCopyDisp( void *dest, GXBool clear );
void GXCopyTex ( void *dest, GXBool clear );

f32  GXGetYScaleFactor( u16 efbHeight, u16 xfbHeight );
u16  GXGetNumXfbLines( u16 efbHeight, f32 yScale );

void GXClearBoundingBox( void );
void GXReadBoundingBox ( u16 *left, u16 *top, u16 *right, u16 *bottom );

/*---------------------------------------------------------------------------*/

// The clear Z value is always in 24-bit format, regardless of pixel fmt.
// Thus the GX_MAX_Z16 is not really necessary.  You should not use it.
// It is included here only for backwards compatibility.

#define GX_MAX_Z24  0x00ffffff

#define GX_MAX_Z16  0x00ffffff

/*---------------------------------------------------------------------------*/

// Actual settings used for each GXRenderModeObj for developers' reference
//
// The following is intentionally ifdef'ed out to show each GXRenderModeObj
// settings to developers.
//

#if 0

GXRenderModeObj GXNtsc240Ds = 
{
    VI_TVMODE_NTSC_DS,      // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC/2 - 480/2)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

     // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

     // vertical filter[7], 1/64 units, 6 bits each
      0,         // line n-1
      0,         // line n-1
     21,         // line n
     22,         // line n
     21,         // line n
      0,         // line n+1
      0          // line n+1
};
GXRenderModeObj GXNtsc240DsAa =
{
    VI_TVMODE_NTSC_DS,      // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC/2 - 480/2)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
     21,        // line n
     22,        // line n
     21,        // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXNtsc240Int = 
{
    VI_TVMODE_NTSC_INT,     // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXNtsc240IntAa = 
{
    VI_TVMODE_NTSC_INT,     // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
     21,        // line n
     22,        // line n
     21,        // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXNtsc480IntDf = 
{
    VI_TVMODE_NTSC_INT,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     8,         // line n-1
     8,         // line n-1
    10,         // line n
    12,         // line n
    10,         // line n
     8,         // line n+1
     8          // line n+1
};
GXRenderModeObj GXNtsc480Int = 
{
    VI_TVMODE_NTSC_INT,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXNtsc480IntAa = 
{
    VI_TVMODE_NTSC_INT,     // viDisplayMode
    640,             // fbWidth
    242,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     4,         // line n-1
     8,         // line n-1
    12,         // line n
    16,         // line n
    12,         // line n
     8,         // line n+1
     4          // line n+1
};
GXRenderModeObj GXNtsc480Prog = 
{
    VI_TVMODE_NTSC_PROG,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXNtsc480ProgSoft = 
{
    VI_TVMODE_NTSC_PROG,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     8,         // line n-1
     8,         // line n-1
    10,         // line n
    12,         // line n
    10,         // line n
     8,         // line n+1
     8          // line n+1
};
GXRenderModeObj GXNtsc480ProgAa = 
{
    VI_TVMODE_NTSC_PROG,     // viDisplayMode
    640,             // fbWidth
    242,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_NTSC - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_NTSC - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     4,         // line n-1
     8,         // line n-1
    12,         // line n
    16,         // line n
    12,         // line n
     8,         // line n+1
     4          // line n+1
};

/*---------------------------------------------------------------------------*/

GXRenderModeObj GXMpal240Ds = 
{
    VI_TVMODE_MPAL_DS,      // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL/2 - 480/2)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXMpal240DsAa = 
{
    VI_TVMODE_MPAL_DS,      // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL/2 - 480/2)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXMpal240Int = 
{
    VI_TVMODE_MPAL_INT,     // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXMpal240IntAa = 
{
    VI_TVMODE_MPAL_INT,     // viDisplayMode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXMpal480IntDf = 
{
    VI_TVMODE_MPAL_INT,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     8,         // line n-1
     8,         // line n-1
    10,         // line n
    12,         // line n
    10,         // line n
     8,         // line n+1
     8          // line n+1
};
GXRenderModeObj GXMpal480Int = 
{
    VI_TVMODE_MPAL_INT,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXMpal480IntAa = 
{
    VI_TVMODE_MPAL_INT,     // viDisplayMode
    640,             // fbWidth
    242,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     4,         // line n-1
     8,         // line n-1
    12,         // line n
    16,         // line n
    12,         // line n
     8,         // line n+1
     4          // line n+1
};
GXRenderModeObj GXMpal480Prog = 
{
    VI_TVMODE_MPAL_PROG,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXMpal480ProgSoft = 
{
    VI_TVMODE_MPAL_PROG,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     8,         // line n-1
     8,         // line n-1
    10,         // line n
    12,         // line n
    10,         // line n
     8,         // line n+1
     8          // line n+1
};
GXRenderModeObj GXMpal480ProgAa = 
{
    VI_TVMODE_MPAL_PROG,     // viDisplayMode
    640,             // fbWidth
    242,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_MPAL - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_MPAL - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     4,         // line n-1
     8,         // line n-1
    12,         // line n
    16,         // line n
    12,         // line n
     8,         // line n+1
     4          // line n+1
};
/*---------------------------------------------------------------------------*/

GXRenderModeObj GXPal264Ds = 
{
    VI_TVMODE_PAL_DS,       // viDisplayMode
    640,             // fbWidth
    264,             // efbHeight
    264,             // xfbHeight
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    (VI_MAX_HEIGHT_PAL/2 - 528/2)/2,        // viYOrigin
    640,             // viWidth
    528,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXPal264DsAa = 
{
    VI_TVMODE_PAL_DS,       // viDisplayMode
    640,             // fbWidth
    264,             // efbHeight
    264,             // xfbHeight
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    (VI_MAX_HEIGHT_PAL/2 - 528/2)/2,        // viYOrigin
    640,             // viWidth
    528,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXPal264Int = 
{
    VI_TVMODE_PAL_INT,      // viDisplayMode
    640,             // fbWidth
    264,             // efbHeight
    264,             // xfbHeight
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    (VI_MAX_HEIGHT_PAL - 528)/2,        // viYOrigin
    640,             // viWidth
    528,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXPal264IntAa = 
{
    VI_TVMODE_PAL_INT,      // viDisplayMode
    640,             // fbWidth
    264,             // efbHeight
    264,             // xfbHeight
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    (VI_MAX_HEIGHT_PAL - 528)/2,        // viYOrigin
    640,             // viWidth
    528,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXPal528IntDf = 
{
    VI_TVMODE_PAL_INT,      // viDisplayMode
    640,             // fbWidth
    528,             // efbHeight
    528,             // xfbHeight
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    (VI_MAX_HEIGHT_PAL - 528)/2,        // viYOrigin
    640,             // viWidth
    528,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     8,         // line n-1
     8,         // line n-1
    10,         // line n
    12,         // line n
    10,         // line n
     8,         // line n+1
     8          // line n+1
};
GXRenderModeObj GXPal528Int = 
{
    VI_TVMODE_PAL_INT,       // viDisplayMode
    640,             // fbWidth
    528,             // efbHeight
    528,             // xfbHeight
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    (VI_MAX_HEIGHT_PAL - 528)/2,        // viYOrigin
    640,             // viWidth
    528,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXPal524IntAa = 
{
    VI_TVMODE_PAL_INT,      // viDisplayMode
    640,             // fbWidth
    264,             // efbHeight (maximum is 528/2)
    524,             // xfbHeight (allow for required overlap of 4 lines)
    (VI_MAX_WIDTH_PAL - 640)/2,         // viXOrigin
    (VI_MAX_HEIGHT_PAL - 528)/2,        // viYOrigin
    640,             // viWidth
    524,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     4,         // line n-1
     8,         // line n-1
    12,         // line n
    16,         // line n
    12,         // line n
     8,         // line n+1
     4          // line n+1
};

GXRenderModeObj GXEurgb60Hz240Ds = 
{
    VI_TVMODE_EURGB60_DS,      // viTVmode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60/2 - 480/2)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

     // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

     // vertical filter[7], 1/64 units, 6 bits each
      0,         // line n-1
      0,         // line n-1
     21,         // line n
     22,         // line n
     21,         // line n
      0,         // line n+1
      0          // line n+1
};
GXRenderModeObj GXEurgb60Hz240DsAa =
{
    VI_TVMODE_EURGB60_DS,      // viTVmode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60/2 - 480/2)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
     21,        // line n
     22,        // line n
     21,        // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXEurgb60Hz240Int = 
{
    VI_TVMODE_EURGB60_INT,     // viTVmode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXEurgb60Hz240IntAa = 
{
    VI_TVMODE_EURGB60_INT,     // viTVmode
    640,             // fbWidth
    240,             // efbHeight
    240,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_TRUE,         // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
     21,        // line n
     22,        // line n
     21,        // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXEurgb60Hz480IntDf = 
{
    VI_TVMODE_EURGB60_INT,     // viTVmode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     8,         // line n-1
     8,         // line n-1
    10,         // line n
    12,         // line n
    10,         // line n
     8,         // line n+1
     8          // line n+1
};
GXRenderModeObj GXEurgb60Hz480Int = 
{
    VI_TVMODE_EURGB60_INT,     // viTVmode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXEurgb60Hz480IntAa = 
{
    VI_TVMODE_EURGB60_INT,     // viTVmode
    640,             // fbWidth
    242,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_DF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     4,         // line n-1
     8,         // line n-1
    12,         // line n
    16,         // line n
    12,         // line n
     8,         // line n+1
     4          // line n+1
};
GXRenderModeObj GXEurgb60Hz480Prog = 
{
    VI_TVMODE_EURGB60_PROG,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     0,         // line n-1
     0,         // line n-1
    21,         // line n
    22,         // line n
    21,         // line n
     0,         // line n+1
     0          // line n+1
};
GXRenderModeObj GXEurgb60Hz480ProgSoft = 
{
    VI_TVMODE_EURGB60_PROG,     // viDisplayMode
    640,             // fbWidth
    480,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_FALSE,        // aa

    // sample points arranged in increasing Y order
     6,  6,  6,  6,  6,  6,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     6,  6,  6,  6,  6,  6,  // pix 1
     6,  6,  6,  6,  6,  6,  // pix 2
     6,  6,  6,  6,  6,  6,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     8,         // line n-1
     8,         // line n-1
    10,         // line n
    12,         // line n
    10,         // line n
     8,         // line n+1
     8          // line n+1
};
GXRenderModeObj GXEurgb60Hz480ProgAa = 
{
    VI_TVMODE_EURGB60_PROG,     // viDisplayMode
    640,             // fbWidth
    242,             // efbHeight
    480,             // xfbHeight
    (VI_MAX_WIDTH_EURGB60 - 640)/2,        // viXOrigin
    (VI_MAX_HEIGHT_EURGB60 - 480)/2,       // viYOrigin
    640,             // viWidth
    480,             // viHeight
    VI_XFBMODE_SF,   // xFBmode
    GX_FALSE,        // field_rendering
    GX_TRUE,         // aa

    // sample points arranged in increasing Y order
     3,  2,  9,  6,  3, 10,  // pix 0, 3 sample points, 1/12 units, 4 bits each
     3,  2,  9,  6,  3, 10,  // pix 1
     9,  2,  3,  6,  9, 10,  // pix 2
     9,  2,  3,  6,  9, 10,  // pix 3

    // vertical filter[7], 1/64 units, 6 bits each
     4,         // line n-1
     8,         // line n-1
    12,         // line n
    16,         // line n
    12,         // line n
     8,         // line n+1
     4          // line n+1
};


#endif // #if 0
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // __GXFRAMEBUFFER_H__
