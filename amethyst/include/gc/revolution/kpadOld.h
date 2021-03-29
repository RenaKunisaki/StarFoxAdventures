/*---------------------------------------------------------------------------*
  Project:     KPAD library
  File:        kpadold.h
  Programmers: Keizo Ohta
               HIRATSU Daisuke
               Tojo Haruki
               Tetsuya Sasaki

  Copyright 2005-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifdef __KPAD_H__
#error KPADOld (kpadOld.h) cannot be used with KPAD version 2 (kpad.h)
#endif

#ifndef __KPADOLD_H__
#define __KPADOLD_H__

#include <revolution/wpad.h>
#include <revolution/types.h>
#include <revolution/mtx/GeoTypes.h>

#ifdef __cplusplus
extern "C" {
#endif


#define KPAD_BUTTON_LEFT    WPAD_BUTTON_LEFT
#define KPAD_BUTTON_RIGHT   WPAD_BUTTON_RIGHT
#define KPAD_BUTTON_DOWN    WPAD_BUTTON_DOWN
#define KPAD_BUTTON_UP      WPAD_BUTTON_UP
#define KPAD_BUTTON_A       WPAD_BUTTON_A
#define KPAD_BUTTON_B       WPAD_BUTTON_B
#define KPAD_BUTTON_HOME    WPAD_BUTTON_HOME

#ifndef WPADEMU
#define KPAD_BUTTON_PLUS    WPAD_BUTTON_PLUS
#define KPAD_BUTTON_MINUS   WPAD_BUTTON_MINUS
#define KPAD_BUTTON_1       WPAD_BUTTON_1
#define KPAD_BUTTON_2       WPAD_BUTTON_2
#define KPAD_BUTTON_Z       WPAD_BUTTON_Z
#define KPAD_BUTTON_C       WPAD_BUTTON_C
#else
#define KPAD_BUTTON_START   WPAD_BUTTON_START
#define KPAD_BUTTON_SELECT  WPAD_BUTTON_SELECT
#define KPAD_BUTTON_SMALL_A WPAD_BUTTON_SMALL_A
#define KPAD_BUTTON_SMALL_B WPAD_BUTTON_SMALL_B
#define KPAD_BUTTON_Z1      WPAD_BUTTON_Z1
#define KPAD_BUTTON_Z2      WPAD_BUTTON_Z2
#endif  // end of WPADEMU

#define KPAD_CL_BUTTON_UP       WPAD_CL_BUTTON_UP   
#define KPAD_CL_BUTTON_LEFT     WPAD_CL_BUTTON_LEFT 
#define KPAD_CL_TRIGGER_ZR      WPAD_CL_TRIGGER_ZR  
#define KPAD_CL_BUTTON_X        WPAD_CL_BUTTON_X    
#define KPAD_CL_BUTTON_A        WPAD_CL_BUTTON_A    
#define KPAD_CL_BUTTON_Y        WPAD_CL_BUTTON_Y    
#define KPAD_CL_BUTTON_B        WPAD_CL_BUTTON_B    
#define KPAD_CL_TRIGGER_ZL      WPAD_CL_TRIGGER_ZL  
#define KPAD_CL_RESERVED        WPAD_CL_RESERVED    
#define KPAD_CL_TRIGGER_R       WPAD_CL_TRIGGER_R   
#define KPAD_CL_BUTTON_PLUS     WPAD_CL_BUTTON_PLUS 
#define KPAD_CL_BUTTON_HOME     WPAD_CL_BUTTON_HOME 
#define KPAD_CL_BUTTON_MINUS    WPAD_CL_BUTTON_MINUS
#define KPAD_CL_TRIGGER_L       WPAD_CL_TRIGGER_L   
#define KPAD_CL_BUTTON_DOWN     WPAD_CL_BUTTON_DOWN 
#define KPAD_CL_BUTTON_RIGHT    WPAD_CL_BUTTON_RIGHT


#ifdef WPADEMU
#define KPAD_GC_BUTTON_LEFT     PAD_BUTTON_LEFT
#define KPAD_GC_BUTTON_RIGHT    PAD_BUTTON_RIGHT
#define KPAD_GC_BUTTON_DOWN     PAD_BUTTON_DOWN
#define KPAD_GC_BUTTON_UP       PAD_BUTTON_UP
#define KPAD_GC_TRIGGER_Z       PAD_TRIGGER_Z
#define KPAD_GC_TRIGGER_R       PAD_TRIGGER_R
#define KPAD_GC_TRIGGER_L       PAD_TRIGGER_L
#define KPAD_GC_BUTTON_Z        PAD_TRIGGER_Z
#define KPAD_GC_BUTTON_R        PAD_TRIGGER_R
#define KPAD_GC_BUTTON_L        PAD_TRIGGER_L
#define KPAD_GC_BUTTON_A        PAD_BUTTON_A
#define KPAD_GC_BUTTON_B        PAD_BUTTON_B
#define KPAD_GC_BUTTON_X        PAD_BUTTON_X
#define KPAD_GC_BUTTON_Y        PAD_BUTTON_Y
#define KPAD_GC_BUTTON_START    PAD_BUTTON_START
#endif  // end of WPADEMU

#define KPAD_BUTTON_MASK    0x0000ffff
#define KPAD_BUTTON_RPT     0x80000000

#define KPAD_RING_BUFS      12
#define KPAD_MAX_READ_BUFS  10


typedef struct Vec2{
    f32 x;
    f32 y;
} Vec2;

typedef struct Rect {
    f32 left;
    f32 top;
    f32 right;
    f32 bottom;
} Rect;

typedef union KPADEXStatus{
    struct {
        Vec2    stick ;

        Vec     acc ;
        f32     acc_value ;
        f32     acc_speed ;
    } fs ;

    struct {
        u32     hold;
        u32     trig;
        u32     release;
    
        Vec2    lstick;
        Vec2    rstick;
        
        f32     ltrigger;
        f32     rtrigger;
    } cl ;

    struct {
        Vec2    stick ;
        Vec2    substick ;

        f32     ltrigger ;
        f32     rtrigger ;
    } gc ;
} KPADEXStatus ;

#ifdef  __MWERKS__
#pragma warn_padding off
#endif
typedef struct KPADStatus{
    u32  hold ;
    u32  trig ;
    u32  release ;

    Vec  acc ;
    f32  acc_value ;
    f32  acc_speed ;

    Vec2 pos ;
    Vec2 vec ;
    f32  speed ;

    Vec2 horizon ;
    Vec2 hori_vec ;
    f32  hori_speed ;

    f32  dist ;
    f32  dist_vec ;
    f32  dist_speed ;

    Vec2 acc_vertical ;

    u32  dev_type ;

    KPADEXStatus    ex_status ;

    s8   dpd_valid_fg ;
    s8   wpad_err ;
    // 2 bytes padding
} KPADStatus ;
#ifdef  __MWERKS__
#pragma warn_padding reset
#endif


void KPADInit ( void ) ;
void KPADReset( void ) ;
s32  KPADRead ( s32 chan, KPADStatus samplingBufs[], u32 length ) ;

void KPADSetPosParam ( s32 chan, f32 play_radius, f32 sensitivity ) ;
void KPADSetHoriParam( s32 chan, f32 play_radius, f32 sensitivity ) ;
void KPADSetDistParam( s32 chan, f32 play_radius, f32 sensitivity ) ;
void KPADSetAccParam ( s32 chan, f32 play_radius, f32 sensitivity ) ;

void KPADSetBtnRepeat( s32 chan, f32 delay_sec, f32 pulse_sec ) ;

void KPADSetObjInterval  ( f32 interval               ) ;
s32  KPADCalibrateDPD    ( s32 chan                   ) ;
void KPADSetSensorHeight ( s32 chan, f32 level        ) ;

WPADStatus   *KPADGetWPADRingBuffer  ( s32 chan ) ;
WPADFSStatus *KPADGetWPADFSRingBuffer( s32 chan ) ;
WPADCLStatus *KPADGetWPADCLRingBuffer( s32 chan ) ;

void KPADSetFSStickClamp   ( s8 min, s8 max ) ;

void KPADDisableDPD( s32 chan );
void KPADEnableDPD ( s32 chan );

#define KPAD_STATE_CTRL_DPD_START       0
#define KPAD_STATE_CTRL_DPD_FINISHED    1

typedef void ( *KPADControlDpdCallback )( s32 chan, s32 reason );

void KPADSetControlDpdCallback( s32 chan, KPADControlDpdCallback callback );


void KPADEnableAimingMode ( s32 chan );
void KPADDisableAimingMode( s32 chan );

void KPADGetProjectionPos( Vec2 *dst, const Vec2 *src, const Rect *projRect, f32 viRatio );

#ifdef WPADEMU
void KPADSetGCStickClamp   ( s8 min, s8 max ) ;
void KPADSetGCSubstickClamp( s8 min, s8 max ) ;
void KPADSetGCTriggerClamp ( u8 min, u8 max ) ;
#endif  // end of WPADEMU




#ifdef __cplusplus
}
#endif

#endif // __KPADOLD_H__
