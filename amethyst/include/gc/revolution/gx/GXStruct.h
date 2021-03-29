/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXStruct.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXStruct.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    11    2002/08/12 17:01 Hirose
    Turned off padding warnings in this header file.
    
    10    2002/07/24 9:07 Hirose
    Resolved version conflicts.
    
    9     2001/05/10 9:43p Hirose
    added tag for each structure definition
    
    8     2001/01/24 5:09p Carl
    Fixed texobj, texregion for emulator.
    
    7     2001/01/19 7:54p Carl
    Optimized TexObjs and TexRegions.
    
    6     2000/10/30 6:26p Carl
    Had to enlarge GXTlutRegion to accommodate change in tlut data.
    
    5    2000/04/04 11:17a Alligator
    Moved constants from GXStruct.h to GXEnum.h since they are dependent on
    enumerated values.
    
    4     2000/03/13 6:18p Danm
    Fixed AA sampling locations to use x,y pairs.
    
    3     2000/01/26 4:06p Hashida
    Include vitypes.h instead of vi.h
    
    2     2000/01/13 5:55p Alligator
    Integrated with ArtX GX library code
    
    18    1999/11/17 10:23p Hirose
    Changed size of some structures
    
    17    1999/11/16 6:17p Alligator
    
    16    1999/11/15 4:42p Hirose
    Changed size of GXTexObj and GXTexRegion
    
    15    1999/10/26 8:04p Alligator
    Added s10-bit color
    
    14    1999/10/24 7:42a Yasu
    Added GX_MAX_TEXMAP
    
    13    1999/10/22 4:07p Yasu
    Added definitions of HW information
    
    12    1999/10/11 10:20a Hirose
    Changed size of GXTexObj
    
    11    1999/10/04 2:42p Yasu
    Added GXInitFogAdjTable
    
    10    1999/09/21 2:33p Alligator
    Added aa flag, if aa set 16b pix format
    
    9     1999/09/21 11:49a Hirose
    Changed GXTlutRegion size
    
    8     1999/09/17 3:38p Hirose
    Changed GXTlutObj size
    
    7     1999/09/16 3:49p Alligator
    
    6     1999/09/16 3:42p Alligator
    update render mode api
    
    5     1999/09/09 3:04p Alligator
    Moved GXSetRenderMode to GX lib from emu
    
    4     1999/09/02 3:18p Ryan
    Made Frame Buffer Api changes
    
    3     1999/09/01 2:27p Ryan
    Added temp GXTesRegion and GXTlutRegion structs
    
    2     1999/07/20 6:10p Alligator
    Added GXGetVtxDescv, GXGetVtxAttrFmtv
    
    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXSTRUCT_H__
#define __GXSTRUCT_H__

/********************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************************/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/vi/vitypes.h>

/*---------------------------------------------------------------------------*/

#ifdef __MWERKS__
#pragma warn_padding    off
#endif // __MWERKS__

/*---------------------------------------------------------------------------*/

/********************************/
typedef struct _GXColor
{
    u8  r,
        g, 
        b, 
        a;

} GXColor;

typedef struct _GXColorS10
{
    s16    r, g, b, a; // s10-bit components for Tev constant color
} GXColorS10;

/********************************/
typedef struct _GXTexObj
{
#ifdef EMU
    u32 dummy[16]; // Emulator version
#else
    u32 dummy[8];  // Real hardware version
#endif    
} GXTexObj;

/********************************/
typedef struct _GXTlutObj
{
    u32 dummy[3];
    
} GXTlutObj;

/********************************/
typedef struct _GXLightObj
{
    u32 dummy[16];
    
} GXLightObj;

/********************************/
typedef struct _GXVtxAttrFmtList
{
    GXAttr        attr;
    GXCompCnt     cnt;
    GXCompType    type;
    u8            frac;
    
} GXVtxAttrFmtList;

/********************************/
typedef struct _GXTexRegion
{
#ifdef EMU
    u32 dummy[8]; // Emulator version
#else
    u32 dummy[4]; // Real hardware version
#endif
} GXTexRegion;

/********************************/
typedef struct _GXTlutRegion
{
    u32 dummy[4];
    
} GXTlutRegion;

/********************************/
typedef struct _GXVtxDescList
{
    GXAttr        attr;
    GXAttrType    type;
    
} GXVtxDescList;

/********************************/
typedef struct _GXRenderModeObj
{
    VITVMode          viTVmode;
    u16               fbWidth;   // No xscale from efb to xfb
    u16               efbHeight; // embedded frame buffer
    u16               xfbHeight; // external frame buffer, may yscale efb
    u16               viXOrigin;
    u16               viYOrigin;
    u16               viWidth;
    u16               viHeight;
    VIXFBMode         xFBmode;   // Whether single-field or double-field in 
                                 // XFB.
    u8                field_rendering;    // Rendering fields or frames?
    u8                aa;                 // Anti-aliasing on?
    u8                sample_pattern[12][2]; // aa sample pattern
    u8                vfilter[7];         // Vertical filter coefficients
} GXRenderModeObj;

/********************************/
typedef struct _GXFogAdjTable
{
    u16			r[10];
} GXFogAdjTable;

/********************************/

/*---------------------------------------------------------------------------*/

#ifdef __MWERKS__
#pragma warn_padding    reset
#endif // __MWERKS__

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
