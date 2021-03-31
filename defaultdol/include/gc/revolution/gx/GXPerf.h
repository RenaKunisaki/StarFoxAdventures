/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXPerf.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXPerf.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    9     2004/03/31 15:32 Hirose
    Added __GXSetBWDials().
    
    8     2002/07/24 9:07 Hirose
    Resolved version conflicts.
    
    7     2001/03/02 1:49p Carl
    Added GXReadClksPerVtx
    
    6     2000/08/23 2:53p Alligator
    allow perf counter api to work with draw sync callback
    
    5     2000/07/05 2:05p Tian
    Added GXResetCPMetrics and GXInitXfRasMetric.  Changed
    GXReadXfRasMetric to not write to pipe.
    
    4     2000/06/30 8:40p Alligator
    added GXReadXfRasMetric
    
    3     2000/06/06 6:57p Alligator
    added vcache mem and stall counters
    
    2     2000/06/06 12:02p Alligator
    made changes to perf counter api
    
    1     2000/04/17 12:49p Alligator
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXPERF_H__
#define __GXPERF_H__

#ifdef __cplusplus
extern "C" {
#endif
    
/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>

/*---------------------------------------------------------------------------*/
void GXSetGPMetric     (  GXPerf0       perf0,
                          GXPerf1       perf1 );

void GXClearGPMetric   (  void );

void GXReadGPMetric    (  u32*          cnt0,
                          u32*          cnt1 );


#define GXSetGP0Metric(perf0)    GXSetGPMetric((perf0), GX_PERF1_NONE)

#define GXClearGP0Metric()       GXClearGPMetric()

u32  GXReadGP0Metric   ( void );


#define GXSetGP1Metric(perf1)    GXSetGPMetric(GX_PERF0_NONE, (perf1))

#define GXClearGP1Metric()       GXClearGPMetric()

u32  GXReadGP1Metric   ( void );


void GXReadMemMetric   ( u32*           cp_req,
                         u32*           tc_req,
                         u32*           cpu_rd_req,
                         u32*           cpu_wr_req,
                         u32*           dsp_req,
                         u32*           io_req,
                         u32*           vi_req,
                         u32*           pe_req,
                         u32*           rf_req,
                         u32*           fi_req );

void GXClearMemMetric  ( void );


void GXReadPixMetric   (  u32*          top_pixels_in, 
                          u32*          top_pixels_out, 
                          u32*          bot_pixels_in, 
                          u32*          bot_pixels_out,
                          u32*          clr_pixels_in,
                          u32*          copy_clks );

void GXClearPixMetric  (  void  );


void GXSetVCacheMetric  ( GXVCachePerf  attr );

void GXReadVCacheMetric ( u32*          check, 
                          u32*          miss, 
                          u32*          stall );

void GXClearVCacheMetric ( void );


// DO NOT use with GXReadGP*Metric
void GXReadXfRasMetric  ( u32*          xf_wait_in,
                          u32*          xf_wait_out,
                          u32*          ras_busy,
                          u32*          clocks );

// DO NOT use with GXReadGP*Metric
void GXInitXfRasMetric();

    
u32 GXReadClksPerVtx( void );

/*---------------------------------------------------------------------------*/

void __GXSetBWDials     ( u16           cpDial,
                          u16           tcDial,
                          u16           peDial,
                          u16           cpuRdDial,
                          u16           cpuWrDial );

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXPERF_H__
