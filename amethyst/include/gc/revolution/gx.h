/*---------------------------------------------------------------------------*
  Project:  Dolphin/Revolution GX library
  File:     gx.h

  Copyright 1998-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: gx.h,v $
  Revision 1.3  05/31/2006 09:29:30  hirose
  Added a new bug flag.

  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.2  2005/12/27 07:05:43  hirose
  Updated flag definitions.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GX_H__
#define __GX_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
// Defines version related flags if not exists.

#ifndef GX_REV
#define GX_REV  2
#endif // GX_REV

#ifndef BUG_CLR_LOAD_DLY
#define BUG_CLR_LOAD_DLY
#endif // BUG_CLR_LOAD_DLY

#ifndef BUG_XF_STALL
#define BUG_XF_STALL
#endif // BUG_XF_STALL

#ifndef BUG_CP_LOAD_DLY
#define BUG_CP_LOAD_DLY
#endif // BUG_CP_LOAD_DLY


/*---------------------------------------------------------------------------*
    header file for each major section of the API
 *---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXCull.h>
#include <revolution/gx/GXLighting.h>
#include <revolution/gx/GXTexture.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXBump.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXFrameBuffer.h>
#include <revolution/gx/GXCpu2Efb.h>
#include <revolution/gx/GXPerf.h>
#include <revolution/gx/GXFifo.h>
#include <revolution/gx/GXManage.h>
#include <revolution/gx/GXGet.h>
#include <revolution/gx/GXDraw.h>
#include <revolution/gx/GXVert.h>
#include <revolution/gx/GXCommandList.h>
#include <revolution/gx/GXDispList.h>
#include <revolution/gx/GXVerify.h>
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // __GX_H__
