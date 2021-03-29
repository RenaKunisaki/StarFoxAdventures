/*---------------------------------------------------------------------------*
  Project:     KPAD library version 2
  File:        kpad.h
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

#ifdef __KPADOLD_H__
#error KPAD version 2 (kpad.h) cannot be used with KPADOld (kpadOld.h)
#endif

#ifndef __KPAD_H__
#define __KPAD_H__

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

#define KPAD_BUTTON_PLUS    WPAD_BUTTON_PLUS
#define KPAD_BUTTON_MINUS   WPAD_BUTTON_MINUS
#define KPAD_BUTTON_1       WPAD_BUTTON_1
#define KPAD_BUTTON_2       WPAD_BUTTON_2
#define KPAD_BUTTON_Z       WPAD_BUTTON_Z
#define KPAD_BUTTON_C       WPAD_BUTTON_C

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

#define KPAD_BUTTON_MASK    0x0000ffff
#define KPAD_BUTTON_RPT     0x80000000

#define KPAD_RING_BUFS      16
#define KPAD_MAX_READ_BUFS  KPAD_RING_BUFS

typedef enum KPADPlayMode {
    KPAD_PLAY_MODE_LOOSE = 0,
    KPAD_PLAY_MODE_TIGHT
} KPADPlayMode ;

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
} KPADEXStatus ;

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

    u8   dev_type ;
    s8   wpad_err ;
    s8   dpd_valid_fg ;
    u8   data_format ;

    KPADEXStatus    ex_status ;
} KPADStatus ;

void KPADInit ( void ) ;
void KPADReset( void ) ;
s32  KPADRead ( s32 chan, KPADStatus samplingBufs[], u32 length ) ;

void KPADSetPosParam ( s32 chan, f32 play_radius, f32 sensitivity ) ;
void KPADSetHoriParam( s32 chan, f32 play_radius, f32 sensitivity ) ;
void KPADSetDistParam( s32 chan, f32 play_radius, f32 sensitivity ) ;
void KPADSetAccParam ( s32 chan, f32 play_radius, f32 sensitivity ) ;

void KPADGetPosParam ( s32 chan, f32 *play_radius, f32 *sensitivity ) ;
void KPADGetHoriParam( s32 chan, f32 *play_radius, f32 *sensitivity ) ;
void KPADGetDistParam( s32 chan, f32 *play_radius, f32 *sensitivity ) ;
void KPADGetAccParam ( s32 chan, f32 *play_radius, f32 *sensitivity ) ;

void KPADSetBtnRepeat( s32 chan, f32 delay_sec, f32 pulse_sec ) ;

void KPADSetObjInterval  ( f32 interval               ) ;
s32  KPADCalibrateDPD    ( s32 chan                   ) ;
void KPADSetSensorHeight ( s32 chan, f32 level        ) ;

// These APIs are not recommended.
// Please use KPADGetUnifiedWpadStatus() instead.
// These are provided for compatibility with KPADOld. But not 100% compatible.

WPADStatus   *KPADGetWPADRingBuffer  ( s32 chan ) ;
WPADFSStatus *KPADGetWPADFSRingBuffer( s32 chan ) ;
WPADCLStatus *KPADGetWPADCLRingBuffer( s32 chan ) ;

void KPADSetFSStickClamp   ( s8 min, s8 max ) ;

void KPADDisableDPD( s32 chan ) ;
void KPADEnableDPD ( s32 chan ) ;

#define KPAD_STATE_CTRL_DPD_START       0
#define KPAD_STATE_CTRL_DPD_FINISHED    1

typedef void ( *KPADControlDpdCallback )( s32 chan, s32 reason ) ;

void KPADSetControlDpdCallback( s32 chan, KPADControlDpdCallback callback ) ;


void KPADEnableAimingMode ( s32 chan ) ;
void KPADDisableAimingMode( s32 chan ) ;

void KPADGetProjectionPos( Vec2 *dst, const Vec2 *src, const Rect *projRect, f32 viRatio ) ;


typedef struct KPADUnifiedWpadStatus {
    union {
        WPADStatus      core ;
        WPADFSStatus    fs ;
        WPADCLStatus    cl ;
    } u ;
    u8     fmt ;
    u8     padding ;
} KPADUnifiedWpadStatus ;

void KPADGetUnifiedWpadStatus( s32 chan, KPADUnifiedWpadStatus *dst, u32 count ) ;

void KPADEnableStickCrossClamp ( void ) ;
void KPADDisableStickCrossClamp( void ) ;

void KPADSetSamplingCallback( s32 chan, WPADSamplingCallback callback ) ;

void KPADSetReviseMode ( s32 chan, BOOL sw ) ;
f32  KPADReviseAcc     ( Vec *acc ) ;
f32  KPADGetReviseAngle( void ) ;

void KPADSetPosPlayMode ( s32 chan, KPADPlayMode mode ) ;
void KPADSetHoriPlayMode( s32 chan, KPADPlayMode mode ) ;
void KPADSetDistPlayMode( s32 chan, KPADPlayMode mode ) ;
void KPADSetAccPlayMode ( s32 chan, KPADPlayMode mode ) ;

KPADPlayMode KPADGetPosPlayMode ( s32 chan ) ;
KPADPlayMode KPADGetHoriPlayMode( s32 chan ) ;
KPADPlayMode KPADGetDistPlayMode( s32 chan ) ;
KPADPlayMode KPADGetAccPlayMode ( s32 chan ) ;

#ifdef __cplusplus
}
#endif

#endif // __KPAD_H__
