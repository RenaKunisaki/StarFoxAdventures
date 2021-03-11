/*---------------------------------------------------------------------------*
  Project:  Dolphin VI library
  File:     vifuncs.h

  Copyright 1998- 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: vifuncs.h,v $
  Revision 1.18  2007/07/12 02:19:05  urata
  Added VIResetDimmingCount.

  Revision 1.17  2006/11/28 23:56:21  urata
  Removed VIResetDimmingCount

  Revision 1.16  2006/11/28 06:01:10  urata
  Added VIResetDimmingCount.

  Revision 1.15  2006/10/27 00:29:58  urata
  Removed VI[Set/Get]ScreenSaverMode and added VIEnableDimming.

  Revision 1.14  2006/10/26 01:53:22  urata
  Added VI_SCREEN_SAVER_MODE_OFF/VI_SCREEN_SAVER_MODE_ON.

  Revision 1.13  2006/10/26 01:11:01  urata
  Added VI[Set/Get]ScreenSaverMode.

  Revision 1.12  2006/10/24 16:19:05  urata
  Added VISetTimeToDimming and removed VIEnableDimming.

  Revision 1.11  2006/09/14 04:31:27  urata
  Removed VIGetDVDStopMotorCount.

  Revision 1.10  2006/09/06 12:17:23  urata
  Removed VIEnableDVDStopMotor.

  Revision 1.9  2006/08/28 12:24:19  urata
  Added some functions for VIDimming and DVDStopMotor.

  Revision 1.8  2006/08/17 11:53:59  urata
  Revised the argument type of "VISetDimming".

  Revision 1.7  2006/08/17 08:42:23  urata
  Added VISetDimming.

  Revision 1.6  2006/06/06 14:05:28  urata
  Renamed VI[Set/Get]ProgressiveTest to VI[Set/Get]VSyncTimingTest.

  Revision 1.5  2006/06/06 13:04:27  urata
    Changed the return's type of VISetProgressiveTest from "VIRetraceCallback"
    to "void".

  Revision 1.4  2006/06/05 00:21:51  urata
  Changed the return's type of VISetProgressiveTest to "VIRetraceCallback"
  from "void".

  Revision 1.3  2006/05/29 13:26:14  urata
  Added VISetProgressiveTest and VIGetProgressiveTest.

  Revision 1.2  2006/02/04 11:56:48  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.


    10    2003/11/19 11:57 Hashida
    Added VIGetScanMode.

    9     2002/08/05 15:53 Shiki
    Added const keyword to the VIConfigure() function prototype.

    8     2002/03/14 18:50 Hashida
    Added VIGetNextFrameBuffer and VIGetCurrentFrameBuffer.

    7     2001/07/11 5:10p Hashida
    Updated header.

    6     2001/06/25 3:29p Hashida
    Added VIGetDTVStatus.

    5     2000/04/28 4:26p Carl
    Changed set callback routines to return old callback.

    4     2000/02/25 11:30a Hashida
    Added VISetPreRetraceCallback and VISetPostRetraceCallback

    3     2000/02/17 4:17p Hashida
    Added VIGetTvFormat

    2     2000/01/28 11:01p Hashida
    Added VIFlush().

    1     2000/01/26 3:51p Hashida
    Initial revision

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __VIFUNCS_H
#define __VIFUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/vi/vitypes.h>
#include <revolution/gx/GXStruct.h>     // For GXRenderModeObj structure


#define VIPadFrameBufferWidth(width)     ((u16)(((u16)(width) + 15) & ~15))


typedef enum _VITimeToDIM
{
    VI_DM_DEFAULT=0,
    VI_DM_10M,
    VI_DM_15M
} VITimeToDIM;



void VIInit                  ( void );
void VIFlush                 ( void );
void VIWaitForRetrace        ( void );

void VIConfigure             ( const GXRenderModeObj* rm );
void VIConfigurePan          ( u16 PanPosX, u16 PanPosY,
                               u16 PanSizeX, u16 PanSizeY );
void VISetNextFrameBuffer    ( void *fb );

void *VIGetNextFrameBuffer   ( void );
void *VIGetCurrentFrameBuffer( void );

VIRetraceCallback VISetPreRetraceCallback  (VIRetraceCallback callback);
VIRetraceCallback VISetPostRetraceCallback (VIRetraceCallback callback);

void VISetBlack              ( BOOL black );
u32  VIGetRetraceCount       ( void );
u32  VIGetNextField          ( void );
u32  VIGetCurrentLine        ( void );
u32  VIGetTvFormat           ( void );
u32  VIGetScanMode           ( void );
u32  VIGetDTVStatus          ( void );

BOOL VIEnableDimming           ( BOOL enable );
u32  VIGetDimmingCount         ( void );
VITimeToDIM VISetTimeToDimming ( VITimeToDIM time );
extern BOOL VIResetDimmingCount( void );


// For test of switch to Progressive from Interlace
void VISetVSyncTimingTest    ( void );
u32  VIGetVSyncTimingTest    ( void );
/********************************/
#ifdef __cplusplus
}
#endif

#endif
