/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXGeometry.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXGeometry.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    14    2002/08/20 9:58 Hirose
    Included os.h instead of defining prototype of OSPanic locally.
    
    13    2002/07/24 9:07 Hirose
    Resolved version conflicts.
    
    13    2002/04/09 15:15 Hirose
    const type specifier support. (worked by hiratsu@IRD)
    
    12     2001/04/17 5:29p Tian
    Changed all inlines to static inline
    
    11    2001/04/04 1:10p Carl
    Latest XF bug fix.
    
    10    2001/03/16 7:22p Carl
    Changes for XF stall bug.
    
    9     2000/10/18 5:44p Hirose
    Modified definition rule of GXSetTexCoordGen
    
    8     2000/10/18 4:05p Carl
    Latest round of HW2 modifications and minor updates.
    
    7     2000/10/03 7:55p Hirose
    Update for HW2(Rev.B) GX API extensions
    
    6     2000/09/29 9:51p Hirose
    Replaced "MAC" flag by better alternatives
    
    5     2000/07/07 6:00p Dante
    PC Compatibility
    
    4     2000/03/27 4:59p Danm
    Made GXEnd inline in non-debug builds
    
    3     2000/03/14 1:36p Alligator
    Changed GXSetTexCoordGen matrix type to u32 so developers can override
    matrix memory map without spurious warnings.
    
    2     2000/03/03 4:21p Alligator
    Integrated with ArtX source
    
    7     1999/10/29 3:35p Hirose
    Added GXSetNumTexGens(u8)
    
    6     1999/10/13 4:32p Alligator
    Changed GXSetViewport, GXSetScissor to use xorig, yorig, wd, ht
    
    5     1999/09/30 10:40p Yasu
    Renamed some GX functions and enums
    
    4     1998/01/05 2:39p Ryan
    Routine update to match header file to man pages
    
    3     1999/08/17 1:33p Alligator
    Added GXSetTexGenEnables function, modified GXTexCoordID enumeration for
    bitwise OR of enables.
    
    2     1999/07/28 4:07p Alligator
    Updated header files and emulator for hw changes
    
    1     1999/07/14 4:20p Alligator
    Split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXGEOMETRY_H__
#define __GXGEOMETRY_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/os.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

void GXSetVtxDesc           ( GXAttr attr, GXAttrType type );
void GXSetVtxDescv          ( const GXVtxDescList *attrPtr );
void GXClearVtxDesc         ( void );

void GXSetVtxAttrFmt( 
    GXVtxFmt       vtxfmt, 
    GXAttr         attr, 
    GXCompCnt      cnt, 
    GXCompType     type, 
    u8             frac );

void GXSetVtxAttrFmtv       ( GXVtxFmt vtxfmt, const GXVtxAttrFmtList *list );
void GXSetArray             ( GXAttr attr, const void *base_ptr, u8 stride );
void GXBegin                ( GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts );

#if defined(EMU) || defined(WIN32)
void GXEnd                  ( void );
#else // !EMU
static inline void GXEnd           ( void )
{
#ifdef _DEBUG
    extern GXBool __GXinBegin;
    if (!__GXinBegin)
        OSPanic(__FILE__, __LINE__, "GXEnd: called without a GXBegin");
    __GXinBegin = GX_FALSE;
#endif
}
#endif // EMU

#if ( GX_REV != 1 ) // GX revision 2 or later only
void GXSetTexCoordGen2(
    GXTexCoordID     dst_coord,
    GXTexGenType     func,
    GXTexGenSrc      src_param,
    u32              mtx,
    GXBool           normalize,
    u32              postmtx );
#endif // ( GX_REV != 1 )

#if ( GX_REV == 1 || defined(EMU) ) // GX revision 1 or emulator
void GXSetTexCoordGen( 
    GXTexCoordID     dst_coord,
    GXTexGenType     func,
    GXTexGenSrc      src_param,
    u32              mtx );
#else // GX revision 2 or later and real hardware
static inline void GXSetTexCoordGen ( 
    GXTexCoordID    dst_coord,
    GXTexGenType    func,
    GXTexGenSrc     src_param,
    u32             mtx )
{
    GXSetTexCoordGen2(dst_coord, func, src_param, mtx, 
                      GX_FALSE, GX_PTIDENTITY);
}
#endif // ( GX_REV == 1 || defined(EMU) )


void GXSetNumTexGens        ( u8 nTexGens );

void GXInvalidateVtxCache   ( void );
void GXSetLineWidth         ( u8 width, GXTexOffset texOffsets );
void GXSetPointSize         ( u8 pointSize, GXTexOffset texOffsets );
void GXEnableTexOffsets	    ( GXTexCoordID coord, GXBool line_enable, 
			      GXBool point_enable );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXGEOMETRY_H__
