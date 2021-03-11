/*---------------------------------------------------------------------------*
  Project: AUX effects for Revolution AX
  File:    axfx.h

  Copyright (C)2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: axfx.h,v $
  Revision 1.8  06/16/2006 05:33:58  aka
  Added AXFXGetHooks().

  Revision 1.7  06/15/2006 02:25:08  aka
  Added a macro AXFX_VERSION.

  Revision 1.6  06/15/2006 01:14:29  aka
  Revised misspelling.

  Revision 1.5  06/14/2006 02:55:11  aka
  Added MIN/MAX defines of each params.

  Revision 1.4  06/09/2006 12:08:42  aka
  Added AXFX***GetMemSize***().

  Revision 1.3  06/08/2006 05:59:20  aka
  Modified for new AXFX made by Kawamura-san (EAD).

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __AXFX_H__
#define __AXFX_H__

#ifdef __cplusplus
extern "C" {
#endif


/*==========================================================================*
    lib version
 *==========================================================================*/

#define AXFX_VERSION          0x0200      // Version 2.0


/*==========================================================================*
    memory allocation
 *==========================================================================*/

typedef void*   (*AXFXAlloc) (u32);   
typedef void    (*AXFXFree)  (void*);

void AXFXSetHooks(AXFXAlloc  alloc, AXFXFree  free);
void AXFXGetHooks(AXFXAlloc *alloc, AXFXFree *free);


/*==========================================================================*
    buffer update
 *==========================================================================*/

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_BUFFERUPDATE
{
    s32       *left;
    s32       *right;
    s32       *surround;

} AXFX_BUFFERUPDATE;


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_BUFFERUPDATE_DPL2
{
    s32       *L;
    s32       *R;
    s32       *Ls;
    s32       *Rs;

} AXFX_BUFFERUPDATE_DPL2;


/*==========================================================================*
    send bus (for connecting FXs)
 *==========================================================================*/

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_BUS
{
    s32       *left;
    s32       *right;
    s32       *surround;

} AXFX_BUS;


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_BUS_DPL2
{
    s32       *L;
    s32       *R;
    s32       *Ls;
    s32       *Rs;

} AXFX_BUS_DPL2;


/*==========================================================================*
  ==========================================================================

    High Quality Reverb

  ==========================================================================
 *==========================================================================*/

/*==========================================================================*
    expanded version
 *==========================================================================*/

// early reflection type
#define AXFX_REVERBHI_EXP_EARLY_MODE_5MS           0
#define AXFX_REVERBHI_EXP_EARLY_MODE_10MS          1
#define AXFX_REVERBHI_EXP_EARLY_MODE_15MS          2
#define AXFX_REVERBHI_EXP_EARLY_MODE_20MS          3
#define AXFX_REVERBHI_EXP_EARLY_MODE_25MS          4
#define AXFX_REVERBHI_EXP_EARLY_MODE_30MS          5
#define AXFX_REVERBHI_EXP_EARLY_MODE_35MS          6
#define AXFX_REVERBHI_EXP_EARLY_MODE_40MS          7
#define AXFX_REVERBHI_EXP_EARLY_MODE_MAX           8

// fused reverb type
#define AXFX_REVERBHI_EXP_FUSED_MODE_OLD_AXFX      0
#define AXFX_REVERBHI_EXP_FUSED_MODE_METAL_TANK    1
#define AXFX_REVERBHI_EXP_FUSED_MODE_SMALL_ROOM    2
#define AXFX_REVERBHI_EXP_FUSED_MODE_LARGE_ROOM    3
#define AXFX_REVERBHI_EXP_FUSED_MODE_HALL          4
#define AXFX_REVERBHI_EXP_FUSED_MODE_CAVERNOUS     5
#define AXFX_REVERBHI_EXP_FUSED_MODE_LONGEST       6  // do not use this!
#define AXFX_REVERBHI_EXP_FUSED_MODE_MAX           7

#define AXFX_REVHI_EXP_MIN_PREDELAYTIMEMAX      0.0f
//#define AXFX_REVHI_EXP_MAX_PREDELAYTIMEMAX

#define AXFX_REVHI_EXP_MIN_PREDELAYTIME         0.0f
//#define AXFX_REVHI_EXP_MAX_PREDELAYTIME

#define AXFX_REVHI_EXP_MIN_FUSEDTIME            0.0f
//#define AXFX_REVHI_EXP_MAX_FUSEDTIME

#define AXFX_REVHI_EXP_MIN_COLORATION           0.0f
#define AXFX_REVHI_EXP_MAX_COLORATION           1.0f

#define AXFX_REVHI_EXP_MIN_DAMPING              0.0f
#define AXFX_REVHI_EXP_MAX_DAMPING              1.0f

#define AXFX_REVHI_EXP_MIN_CROSSTALK            0.0f
#define AXFX_REVHI_EXP_MAX_CROSSTALK            1.0f

#define AXFX_REVHI_EXP_MIN_EARLYGAIN            0.0f
#define AXFX_REVHI_EXP_MAX_EARLYGAIN            1.0f

#define AXFX_REVHI_EXP_MIN_FUSEDGAIN            0.0f
#define AXFX_REVHI_EXP_MAX_FUSEDGAIN            1.0f

#define AXFX_REVHI_EXP_MIN_OUTGAIN              0.0f
#define AXFX_REVHI_EXP_MAX_OUTGAIN              1.0f
    
#define AXFX_REVHI_EXP_MIN_SENDGAIN             0.0f
#define AXFX_REVHI_EXP_MAX_SENDGAIN             1.0f

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_REVERBHI_EXP
{
    // do not touch these!
    f32       *earlyLine[3];
    u32        earlyPos[3];
    u32        earlyLength;
    u32        earlyMaxLength;
    f32        earlyCoef[3];
    f32       *preDelayLine[3];
    u32        preDelayPos;
    u32        preDelayLength;
    u32        preDelayMaxLength;
    f32       *combLine[3][3];
    u32        combPos[3];
    u32        combLength[3];
    u32        combMaxLength[3];
    f32        combCoef[3];
    f32       *allpassLine[3][2];
    u32        allpassPos[2];
    u32        allpassLength[2];
    u32        allpassMaxLength[2];
    f32       *lastAllpassLine[3];
    u32        lastAllpassPos[3];
    u32        lastAllpassLength[3];
    u32        lastAllpassMaxLength[3];
    f32        allpassCoef;
    f32        lastLpfOut[3];
    f32        lpfCoef;
    u32        active;
    
    // user params
    u32        earlyMode;        // early reflection mode
    f32        preDelayTimeMax;  // max of pre delay time of fused reverb (sec)
    f32        preDelayTime;     // pre delay time of fused reverb (sec)
    u32        fusedMode;        // fused reverb mode
    f32        fusedTime;        // fused reverb time (sec)
    f32        coloration;       // coloration of all-pass filter (0.f - 1.f)
    f32        damping;          // damping of timbre  (0.f - 1.f)
    f32        crosstalk;        // crosstalk of each channels
    f32        earlyGain;        // output gain of early reflection (0.f - 1.f)
    f32        fusedGain;        // output gain of fused reverb (0.f - 1.f)
    
    AXFX_BUS  *busIn;
    AXFX_BUS  *busOut;
    f32        outGain;
    f32        sendGain;
    
} AXFX_REVERBHI_EXP;


s32  AXFXReverbHiExpGetMemSize     (AXFX_REVERBHI_EXP *reverb);
BOOL AXFXReverbHiExpInit           (AXFX_REVERBHI_EXP *reverb);
void AXFXReverbHiExpShutdown       (AXFX_REVERBHI_EXP *reverb);
BOOL AXFXReverbHiExpSettings       (AXFX_REVERBHI_EXP *reverb);
BOOL AXFXReverbHiExpSettingsUpdate (AXFX_REVERBHI_EXP *reverb);
void AXFXReverbHiExpCallback       (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_REVERBHI_EXP *reverb);


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_REVERBHI_EXP_DPL2
{
    // do not touch these!
    f32       *earlyLine[4];
    u32        earlyPos[3];
    u32        earlyLength;
    u32        earlyMaxLength;
    f32        earlyCoef[3];
    f32       *preDelayLine[4];
    u32        preDelayPos;
    u32        preDelayLength;
    u32        preDelayMaxLength;
    f32       *combLine[4][3];
    u32        combPos[3];
    u32        combLength[3];
    u32        combMaxLength[3];
    f32        combCoef[3];
    f32       *allpassLine[4][2];
    u32        allpassPos[2];
    u32        allpassLength[2];
    u32        allpassMaxLength[2];
    f32       *lastAllpassLine[4];
    u32        lastAllpassPos[4];
    u32        lastAllpassLength[4];
    u32        lastAllpassMaxLength[4];
    f32        allpassCoef;
    f32        lastLpfOut[4];
    f32        lpfCoef;
    u32        active;
    
    // user params
    u32        earlyMode;      // early reflection mode
    f32        preDelayTimeMax;// pre-delay max time of fused reverb (sec)
    f32        preDelayTime;   // pre-delay time of fused reverb (sec)
    u32        fusedMode;      // fused reverb mode
    f32        fusedTime;      // fused reverb time (sec)
    f32        coloration;     // coloration of all-pass filter (0.f-1.f)
    f32        damping;        // damping of timbre  (0.f-1.f)
    f32        crosstalk;      // crosstalk of each channels
    f32        earlyGain;      // output gain of early reflection(0.f-1.f)
    f32        fusedGain;      // output gain of fused reverb (0.f-1.f)
    
    AXFX_BUS_DPL2 *busIn;
    AXFX_BUS_DPL2 *busOut;
    f32        outGain;
    f32        sendGain;

} AXFX_REVERBHI_EXP_DPL2;


s32  AXFXReverbHiExpGetMemSizeDpl2     (AXFX_REVERBHI_EXP_DPL2 *reverb);
BOOL AXFXReverbHiExpInitDpl2           (AXFX_REVERBHI_EXP_DPL2 *reverb);
void AXFXReverbHiExpShutdownDpl2       (AXFX_REVERBHI_EXP_DPL2 *reverb);
BOOL AXFXReverbHiExpSettingsDpl2       (AXFX_REVERBHI_EXP_DPL2 *reverb);
BOOL AXFXReverbHiExpSettingsUpdateDpl2 (AXFX_REVERBHI_EXP_DPL2 *reverb);
void AXFXReverbHiExpCallbackDpl2       (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_REVERBHI_EXP_DPL2 *reverb);


/*==========================================================================*
    classic version
 *==========================================================================*/

#define AXFX_REVHI_MIN_TIME                     0.0f
//#define AXFX_REVHI_MAX_TIME

#define AXFX_REVHI_MIN_PREDELAY                 0.0f
//#define AXFX_REVHI_MAX_PREDELAY

#define AXFX_REVHI_MIN_COLORATION               0.0f
#define AXFX_REVHI_MAX_COLORATION               1.0f

#define AXFX_REVHI_MIN_DAMPING                  0.0f
#define AXFX_REVHI_MAX_DAMPING                  1.0f

#define AXFX_REVHI_MIN_MIX                      0.0f
#define AXFX_REVHI_MAX_MIX                      1.0f

#define AXFX_REVHI_MIN_CROSSTALK                0.0f
#define AXFX_REVHI_MAX_CROSSTALK                1.0f

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_REVERBHI
{
    // do not write to these
    AXFX_REVERBHI_EXP  reverbInner;
    
    // user params
    f32        coloration;
    f32        mix;
    f32        time;
    f32        damping;
    f32        preDelay;
    f32        crosstalk;

} AXFX_REVERBHI;


s32  AXFXReverbHiGetMemSize (AXFX_REVERBHI *reverb);
BOOL AXFXReverbHiInit       (AXFX_REVERBHI *reverb);
BOOL AXFXReverbHiShutdown   (AXFX_REVERBHI *reverb);
BOOL AXFXReverbHiSettings   (AXFX_REVERBHI *reverb);
void AXFXReverbHiCallback   (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_REVERBHI *reverb);


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_REVERBHI_DPL2
{
    // do not write to these
    AXFX_REVERBHI_EXP_DPL2  reverbInner;
    
    // user params
    f32        coloration;
    f32        mix;
    f32        time;
    f32        damping;
    f32        preDelay;
    f32        crosstalk;

} AXFX_REVERBHI_DPL2;


s32  AXFXReverbHiGetMemSizeDpl2 (AXFX_REVERBHI_DPL2 *reverb);
BOOL AXFXReverbHiInitDpl2       (AXFX_REVERBHI_DPL2 *reverb);
BOOL AXFXReverbHiShutdownDpl2   (AXFX_REVERBHI_DPL2 *reverb);
BOOL AXFXReverbHiSettingsDpl2   (AXFX_REVERBHI_DPL2 *reverb);
void AXFXReverbHiCallbackDpl2   (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_REVERBHI_DPL2 *reverb);


/*==========================================================================*
  ==========================================================================

    Standard Reverb

  ==========================================================================
 *==========================================================================*/

/*==========================================================================*
    expanded version
 *==========================================================================*/

// early reflection type
#define AXFX_REVERBSTD_EXP_EARLY_MODE_5MS           0
#define AXFX_REVERBSTD_EXP_EARLY_MODE_10MS          1
#define AXFX_REVERBSTD_EXP_EARLY_MODE_15MS          2
#define AXFX_REVERBSTD_EXP_EARLY_MODE_20MS          3
#define AXFX_REVERBSTD_EXP_EARLY_MODE_25MS          4
#define AXFX_REVERBSTD_EXP_EARLY_MODE_30MS          5
#define AXFX_REVERBSTD_EXP_EARLY_MODE_35MS          6
#define AXFX_REVERBSTD_EXP_EARLY_MODE_40MS          7
#define AXFX_REVERBSTD_EXP_EARLY_MODE_MAX           8

// fused reverb type
#define AXFX_REVERBSTD_EXP_FUSED_MODE_OLD_AXFX      0
#define AXFX_REVERBSTD_EXP_FUSED_MODE_METAL_TANK    1
#define AXFX_REVERBSTD_EXP_FUSED_MODE_SMALL_ROOM    2
#define AXFX_REVERBSTD_EXP_FUSED_MODE_LARGE_ROOM    3
#define AXFX_REVERBSTD_EXP_FUSED_MODE_HALL          4
#define AXFX_REVERBSTD_EXP_FUSED_MODE_CAVERNOUS     5
#define AXFX_REVERBSTD_EXP_FUSED_MODE_LONGEST       6  // do not use this!
#define AXFX_REVERBSTD_EXP_FUSED_MODE_MAX           7


#define AXFX_REVSTD_EXP_MIN_PREDELAYTIMEMAX     0.0f
//#define AXFX_REVSTD_EXP_MAX_PREDELAYTIMEMAX

#define AXFX_REVSTD_EXP_MIN_PREDELAYTIME        0.0f
//#define AXFX_REVSTD_EXP_MAX_PREDELAYTIME

#define AXFX_REVSTD_EXP_MIN_FUSEDTIME           0.0f
//#define AXFX_REVSTD_EXP_MAX_FUSEDTIME

#define AXFX_REVSTD_EXP_MIN_COLORATION          0.0f
#define AXFX_REVSTD_EXP_MAX_COLORATION          1.0f

#define AXFX_REVSTD_EXP_MIN_DAMPING             0.0f
#define AXFX_REVSTD_EXP_MAX_DAMPING             1.0f

#define AXFX_REVSTD_EXP_MIN_EARLYGAIN           0.0f
#define AXFX_REVSTD_EXP_MAX_EARLYGAIN           1.0f

#define AXFX_REVSTD_EXP_MIN_FUSEDGAIN           0.0f
#define AXFX_REVSTD_EXP_MAX_FUSEDGAIN           1.0f

#define AXFX_REVSTD_EXP_MIN_OUTGAIN             0.0f
#define AXFX_REVSTD_EXP_MAX_OUTGAIN             1.0f
    
#define AXFX_REVSTD_EXP_MIN_SENDGAIN            0.0f
#define AXFX_REVSTD_EXP_MAX_SENDGAIN            1.0f

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_REVERBSTD_EXP
{
    // do not touch these!
    f32       *earlyLine[3];
    u32        earlyPos;
    u32        earlyLength;
    u32        earlyMaxLength;
    f32        earlyCoef;
    f32       *preDelayLine[3];
    u32        preDelayPos;
    u32        preDelayLength;
    u32        preDelayMaxLength;
    f32       *combLine[3][2];
    u32        combPos[2];
    u32        combLength[2];
    u32        combMaxLength[2];
    f32        combCoef[2];
    f32       *allpassLine[3][2];
    u32        allpassPos[2];
    u32        allpassLength[2];
    u32        allpassMaxLength[2];
    f32        allpassCoef;
    f32        lastLpfOut[3];
    f32        lpfCoef;
    u32        active;
    
    // user params
    u32        earlyMode;        // early reflection mode
    f32        preDelayTimeMax;  // pre-delay max time of fused reverb (sec)
    f32        preDelayTime;     // pre-delay time of fused reverb (sec)
    u32        fusedMode;        // fused reverb mode
    f32        fusedTime;        // fused reverb time (sec)
    f32        coloration;       // coloration of all-pass filter (0.f - 1.f)
    f32        damping;          // damping of timbre  (0.f - 1.f)
    f32        earlyGain;        // output gain of early reflection (0.f - 1.f)
    f32        fusedGain;        // output gain of fused reverb (0.f - 1.f)
    
    AXFX_BUS  *busIn;
    AXFX_BUS  *busOut;
    f32        outGain;
    f32        sendGain;
    
} AXFX_REVERBSTD_EXP;


s32  AXFXReverbStdExpGetMemSize     (AXFX_REVERBSTD_EXP *reverb);
BOOL AXFXReverbStdExpInit           (AXFX_REVERBSTD_EXP *reverb);
void AXFXReverbStdExpShutdown       (AXFX_REVERBSTD_EXP *reverb);
BOOL AXFXReverbStdExpSettings       (AXFX_REVERBSTD_EXP *reverb);
BOOL AXFXReverbStdExpSettingsUpdate (AXFX_REVERBSTD_EXP *reverb);
void AXFXReverbStdExpCallback       (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_REVERBSTD_EXP *reverb);


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_REVERBSTD_EXP_DPL2
{
    // do not touch these!
    f32       *earlyLine[4];
    u32        earlyPos;
    u32        earlyLength;
    u32        earlyMaxLength;
    f32        earlyCoef;
    f32       *preDelayLine[4];
    u32        preDelayPos;
    u32        preDelayLength;
    u32        preDelayMaxLength;
    f32       *combLine[4][2];
    u32        combPos[2];
    u32        combLength[2];
    u32        combMaxLength[2];
    f32        combCoef[2];
    f32       *allpassLine[4][2];
    u32        allpassPos[2];
    u32        allpassLength[2];
    u32        allpassMaxLength[2];
    f32        allpassCoef;
    f32        lastLpfOut[4];
    f32        lpfCoef;
    u32        active;
    
    // user params
    u32        earlyMode;      // early reflection mode
    f32        preDelayTimeMax;// pre-delay max time of fused reverb(sec)
    f32        preDelayTime;   // pre-delay time of fused reverb (sec)
    u32        fusedMode;      // fused reverb mode
    f32        fusedTime;      // fused reverb time (sec)
    f32        coloration;     // coloration of all-pass filter (0.f-1.f)
    f32        damping;        // damping of timbre  (0.f-1.f)
    f32        earlyGain;      // output gain of early reflection(0.f-1.f)
    f32        fusedGain;      // output gain of fused reverb (0.f-1.f)
    
    AXFX_BUS_DPL2 *busIn;
    AXFX_BUS_DPL2 *busOut;
    f32        outGain;
    f32        sendGain;
    
} AXFX_REVERBSTD_EXP_DPL2;


s32  AXFXReverbStdExpGetMemSizeDpl2     (AXFX_REVERBSTD_EXP_DPL2 *reverb);
BOOL AXFXReverbStdExpInitDpl2           (AXFX_REVERBSTD_EXP_DPL2 *reverb);
void AXFXReverbStdExpShutdownDpl2       (AXFX_REVERBSTD_EXP_DPL2 *reverb);
BOOL AXFXReverbStdExpSettingsDpl2       (AXFX_REVERBSTD_EXP_DPL2 *reverb);
BOOL AXFXReverbStdExpSettingsUpdateDpl2 (AXFX_REVERBSTD_EXP_DPL2 *reverb);
void AXFXReverbStdExpCallbackDpl2       (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_REVERBSTD_EXP_DPL2 *reverb);


/*==========================================================================*
    classic version
 *==========================================================================*/

#define AXFX_REVSTD_MIN_TIME                    0.0f
//#define AXFX_REVSTD_MAX_TIME

#define AXFX_REVSTD_MIN_PREDELAY                0.0f
//#define AXFX_REVSTD_MAX_PREDELAY

#define AXFX_REVSTD_MIN_COLORATION              0.0f
#define AXFX_REVSTD_MAX_COLORATION              1.0f

#define AXFX_REVSTD_MIN_DAMPING                 0.0f
#define AXFX_REVSTD_MAX_DAMPING                 1.0f

#define AXFX_REVSTD_MIN_MIX                     0.0f
#define AXFX_REVSTD_MAX_MIX                     1.0f

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_REVERBSTD
{
    AXFX_REVERBSTD_EXP  reverbInner;
                
    // user params
    f32    coloration;
    f32    mix;
    f32    time;
    f32    damping;
    f32    preDelay;

} AXFX_REVERBSTD;


s32  AXFXReverbStdGetMemSize (AXFX_REVERBSTD *reverb);
BOOL AXFXReverbStdInit       (AXFX_REVERBSTD *reverb);
BOOL AXFXReverbStdShutdown   (AXFX_REVERBSTD *reverb);
BOOL AXFXReverbStdSettings   (AXFX_REVERBSTD *reverb);
void AXFXReverbStdCallback   (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_REVERBSTD *reverb);


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_REVERBSTD_DPL2
{
    AXFX_REVERBSTD_EXP_DPL2  reverbInner;
                
    // user params
    f32        coloration;
    f32        mix;
    f32        time;
    f32        damping;
    f32        preDelay;

} AXFX_REVERBSTD_DPL2;


s32  AXFXReverbStdGetMemSizeDpl2 (AXFX_REVERBSTD_DPL2 *reverb);
BOOL AXFXReverbStdInitDpl2       (AXFX_REVERBSTD_DPL2 *reverb);
BOOL AXFXReverbStdShutdownDpl2   (AXFX_REVERBSTD_DPL2 *reverb);
BOOL AXFXReverbStdSettingsDpl2   (AXFX_REVERBSTD_DPL2 *reverb);
void AXFXReverbStdCallbackDpl2   (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_REVERBSTD_DPL2 *reverb);


/*==========================================================================*
  ==========================================================================

    Delay

  ==========================================================================
 *==========================================================================*/

/*==========================================================================*
    expanded version
 *==========================================================================*/

#define AXFX_DELAY_EXP_MIN_MAXDELAY             0.04f // 0.0f < maxDelay
//#define AXFX_DELAY_EXP_MAX_MAXDELAY
    
#define AXFX_DELAY_EXP_MIN_DELAY                0.04f
//#define AXFX_DELAY_EXP_MAX_DELAY
    
#define AXFX_DELAY_EXP_MIN_FEEDBACK             0.0f
#define AXFX_DELAY_EXP_MAX_FEEDBACK             0.99f // feedback < 1.0f
    
#define AXFX_DELAY_EXP_MIN_LPF                  0.0f
#define AXFX_DELAY_EXP_MAX_LPF                  1.0f
    
#define AXFX_DELAY_EXP_MIN_OUTGAIN              0.0f
#define AXFX_DELAY_EXP_MAX_OUTGAIN              1.0f
    
#define AXFX_DELAY_EXP_MIN_SENDGAIN             0.0f
#define AXFX_DELAY_EXP_MAX_SENDGAIN             1.0f

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_DELAY_EXP
{
    // do not touch these!
    s32       *lineL;
    s32       *lineR;
    s32       *lineS;
    u32        curPos;
    u32        length;
    u32        maxLength;
    s32        feedbackGain;
    s32        lastLpfOut[3];
    s32        lpfCoef;
    s32        outGainCalc;
    s32        sendGainCalc;
    u32        active;
    
    // user params
    f32        maxDelay;  // max delay time (msec) (>0.f)
    f32        delay;     // delay time (msec) (0.f< - <=maxDelay)
    f32        feedback;  // feedback gain (0.f - <1.f)
    f32        lpf;       // LPF param (0.f - 1.f) (cutoff : low - high)
    
    AXFX_BUS  *busIn;
    AXFX_BUS  *busOut;
    f32        outGain;
    f32        sendGain;

} AXFX_DELAY_EXP;


s32  AXFXDelayExpGetMemSize     (AXFX_DELAY_EXP *delay);
BOOL AXFXDelayExpInit           (AXFX_DELAY_EXP *delay);
void AXFXDelayExpShutdown       (AXFX_DELAY_EXP *delay);
BOOL AXFXDelayExpSettings       (AXFX_DELAY_EXP *delay);
BOOL AXFXDelayExpSettingsUpdate (AXFX_DELAY_EXP *delay);
void AXFXDelayExpCallback       (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_DELAY_EXP *delay);


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_DELAY_EXP_DPL2
{
    // do not touch these!
    s32       *line[4];
    u32        curPos;
    u32        length;
    u32        maxLength;
    s32        feedbackGain;
    s32        lastLpfOut[4];
    s32        lpfCoef;
    s32        outGainCalc;
    s32        sendGainCalc;
    u32        active;
    
    // user params
    f32        maxDelay;  // max delay time (msec) (>0.f)
    f32        delay;     // delay time (msec) (0.f< - <=maxDelay)
    f32        feedback;  // feedback gain (0.f - <1.f)
    f32        lpf;       // LPF param (0.f - 1.f) (cutoff : low - high)
    
    AXFX_BUS_DPL2 *busIn;
    AXFX_BUS_DPL2 *busOut;
    f32        outGain;
    f32        sendGain;

} AXFX_DELAY_EXP_DPL2;


s32  AXFXDelayExpGetMemSizeDpl2     (AXFX_DELAY_EXP_DPL2 *delay);
BOOL AXFXDelayExpInitDpl2           (AXFX_DELAY_EXP_DPL2 *delay);
void AXFXDelayExpShutdownDpl2       (AXFX_DELAY_EXP_DPL2 *delay);
BOOL AXFXDelayExpSettingsDpl2       (AXFX_DELAY_EXP_DPL2 *delay);
BOOL AXFXDelayExpSettingsUpdateDpl2 (AXFX_DELAY_EXP_DPL2 *delay);
void AXFXDelayExpCallbackDpl2       (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_DELAY_EXP_DPL2 *delay);


/*==========================================================================*
    classic version
 *==========================================================================*/

#define AXFX_DELAY_MIN_DELAY                    1
//#define AXFX_DELAY_MAX_DELAY

#define AXFX_DELAY_MIN_FEEDBACK                 0
#define AXFX_DELAY_MAX_FEEDBACK                99

#define AXFX_DELAY_MIN_OUTPUT                   0
#define AXFX_DELAY_MAX_OUTPUT                 100

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_DELAY
{
    // do not touch these!
    s32       *line[3];
    u32        curPos[3];
    u32        length[3];
    s32        feedbackGain[3];
    s32        outGain[3];
    u32        active;
    
    // user params
    u32        delay[3];       // Delay buffer length in ms per channel
    u32        feedback[3];    // Feedback volume in % per channel
    u32        output[3];      // Output volume in % per channel

} AXFX_DELAY;


s32  AXFXDelayGetMemSize (AXFX_DELAY *delay);
BOOL AXFXDelayInit       (AXFX_DELAY *delay);
void AXFXDelayShutdown   (AXFX_DELAY *delay);
BOOL AXFXDelaySettings   (AXFX_DELAY *delay);
void AXFXDelayCallback   (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_DELAY *delay);


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_DELAY_DPL2
{
    // do not touch these!
    s32       *line[4];
    u32        curPos[4];
    u32        length[4];
    s32        feedbackGain[4];
    s32        outGain[4];
    u32        active;
    
    // user params
    u32        delay[4];       // Delay buffer length in ms per channel
    u32        feedback[4];    // Feedback volume in % per channel
    u32        output[4];      // Output volume in % per channel

} AXFX_DELAY_DPL2;


s32  AXFXDelayGetMemSizeDpl2 (AXFX_DELAY_DPL2 *delay);
BOOL AXFXDelayInitDpl2       (AXFX_DELAY_DPL2 *delay);
void AXFXDelayShutdownDpl2   (AXFX_DELAY_DPL2 *delay);
BOOL AXFXDelaySettingsDpl2   (AXFX_DELAY_DPL2 *delay);
void AXFXDelayCallbackDpl2   (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_DELAY_DPL2 *delay);


/*==========================================================================*
  ==========================================================================

    Chorus

  ==========================================================================
 *==========================================================================*/

/*==========================================================================*
    expanded version
 *==========================================================================*/

#define AXFX_CHORUS_EXP_MIN_DELAYTIME           0.1f
#define AXFX_CHORUS_EXP_MAX_DELAYTIME          50.0f

#define AXFX_CHORUS_EXP_MIN_DEAPTH              0.0f
#define AXFX_CHORUS_EXP_MAX_DEAPTH              1.0f

#define AXFX_CHORUS_EXP_MIN_RATE                0.1f
#define AXFX_CHORUS_EXP_MAX_RATE                2.0f

#define AXFX_CHORUS_EXP_MIN_FEEDBACK            0.0f
#define AXFX_CHORUS_EXP_MAX_FEEDBACK            0.99f // feedback < 1.0f

#define AXFX_CHORUS_EXP_MIN_OUTGAIN             0.0f
#define AXFX_CHORUS_EXP_MAX_OUTGAIN             1.0f
    
#define AXFX_CHORUS_EXP_MIN_SENDGAIN            0.0f
#define AXFX_CHORUS_EXP_MAX_SENDGAIN            1.0f

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_CHORUS_EXP_DELAY
{
    f32       *line[3];
    u32        inPos;
    u32        outPos;
    u32        lastPos;
    u32        sizeFP;
    u32        size;

} AXFX_CHORUS_EXP_DELAY;


typedef struct AXFX_CHORUS_EXP_LFO
{
    s32       *table;
    s32        phaseAdd;
    s32        stepSamp;
    s32        depthSamp;
    u32        phase;
    u32        sign;
    u32        lastNum;
    s32        lastValue;
    s32        grad;
    s32        gradFactor;
    
} AXFX_CHORUS_EXP_LFO;


typedef struct AXFX_CHORUS_EXP
{
    // don't touch these
    AXFX_CHORUS_EXP_DELAY  delay;
    AXFX_CHORUS_EXP_LFO    lfo;
    f32        history[3][4];
    u32        histIndex;
    u32        active;
    
    // user params
    f32        delayTime;   // 0.1 - 50.f (msec)
    f32        depth;       // 0.f - 1.f
    f32        rate;        // 0.1 - 2.f (Hz)
    f32        feedback;    // 0.f - < 1.f
    
    AXFX_BUS  *busIn;
    AXFX_BUS  *busOut;
    f32        outGain;
    f32        sendGain;
    
} AXFX_CHORUS_EXP;


s32  AXFXChorusExpGetMemSize     (AXFX_CHORUS_EXP *chorus);
BOOL AXFXChorusExpInit           (AXFX_CHORUS_EXP *chorus);
BOOL AXFXChorusExpSettings       (AXFX_CHORUS_EXP *chorus);
BOOL AXFXChorusExpSettingsUpdate (AXFX_CHORUS_EXP *chorus);
void AXFXChorusExpShutdown       (AXFX_CHORUS_EXP *chorus);
void AXFXChorusExpCallback       (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_CHORUS_EXP *chorus);


/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_CHORUS_EXP_DELAY_DPL2
{
    f32       *line[4];
    u32        inPos;
    u32        outPos;
    u32        lastPos;
    u32        sizeFP;
    u32        size;

} AXFX_CHORUS_EXP_DELAY_DPL2;


typedef struct AXFX_CHORUS_EXP_LFO_DPL2
{
    s32       *table;
    s32        phaseAdd;
    s32        stepSamp;
    s32        depthSamp;
    u32        phase;
    u32        sign;
    u32        lastNum;
    s32        lastValue;
    s32        grad;
    s32        gradFactor;
    
} AXFX_CHORUS_EXP_LFO_DPL2;


typedef struct AXFX_CHORUS_EXP_DPL2
{
    // don't touch these
    AXFX_CHORUS_EXP_DELAY_DPL2  delay;
    AXFX_CHORUS_EXP_LFO_DPL2    lfo;
    f32        history[4][4];
    u32        histIndex;
    u32        active;
    
    // user params
    f32        delayTime;   // 0.1 - 50.f (msec)
    f32        depth;       // 0.f - 1.f
    f32        rate;        // 0.1f - 2.f (Hz)
    f32        feedback;    // 0.f - < 1.f
    
    AXFX_BUS_DPL2 *busIn;
    AXFX_BUS_DPL2 *busOut;
    f32        outGain;
    f32        sendGain;

} AXFX_CHORUS_EXP_DPL2;


s32  AXFXChorusExpGetMemSizeDpl2     (AXFX_CHORUS_EXP_DPL2 *chorus);
BOOL AXFXChorusExpInitDpl2           (AXFX_CHORUS_EXP_DPL2 *chorus);
BOOL AXFXChorusExpSettingsDpl2       (AXFX_CHORUS_EXP_DPL2 *chorus);
BOOL AXFXChorusExpSettingsUpdateDpl2 (AXFX_CHORUS_EXP_DPL2 *chorus);
void AXFXChorusExpShutdownDpl2       (AXFX_CHORUS_EXP_DPL2 *chorus);
void AXFXChorusExpCallbackDpl2       (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_CHORUS_EXP_DPL2 *chorus);


/*==========================================================================*
    classic version
 *==========================================================================*/

#define AXFX_CHORUS_MIN_DELAY                   1
#define AXFX_CHORUS_MAX_DELAY                  50

#define AXFX_CHORUS_MIN_VARIATION               0
#define AXFX_CHORUS_MAX_VARIATION              50

#define AXFX_CHORUS_MIN_PERIOD                500
#define AXFX_CHORUS_MAX_PERIOD              10000

/*----------------------------------------------*
    for mono, stereo, surround
 *----------------------------------------------*/

typedef struct AXFX_CHORUS
{
    AXFX_CHORUS_EXP  chorusInner;
    
    // user params
    u32        baseDelay;      // Base delay of chorus effect in ms
    u32        variation;      // Variation of base delay in ms
    u32        period;         // Period of varying oscillation in ms
    
} AXFX_CHORUS;


s32  AXFXChorusGetMemSize (AXFX_CHORUS *chorus);
BOOL AXFXChorusInit       (AXFX_CHORUS *chorus);
BOOL AXFXChorusShutdown   (AXFX_CHORUS *chorus);
BOOL AXFXChorusSettings   (AXFX_CHORUS *chorus);
void AXFXChorusCallback   (AXFX_BUFFERUPDATE *bufferUpdate, AXFX_CHORUS *chorus);

/*----------------------------------------------*
    for Dolby prologic2
 *----------------------------------------------*/

typedef struct AXFX_CHORUS_DPL2
{
    AXFX_CHORUS_EXP_DPL2  chorusInner;
    
    // user params
    u32        baseDelay;      // Base delay of chorus effect in ms
    u32        variation;      // Variation of base delay in ms
    u32        period;         // Period of varying oscillation in ms
    
} AXFX_CHORUS_DPL2;


s32  AXFXChorusGetMemSizeDpl2 (AXFX_CHORUS_DPL2 *chorus);
BOOL AXFXChorusInitDpl2       (AXFX_CHORUS_DPL2 *chorus);
BOOL AXFXChorusShutdownDpl2   (AXFX_CHORUS_DPL2 *chorus);
BOOL AXFXChorusSettingsDpl2   (AXFX_CHORUS_DPL2 *chorus);
void AXFXChorusCallbackDpl2   (AXFX_BUFFERUPDATE_DPL2 *bufferUpdate, AXFX_CHORUS_DPL2 *chorus);


#ifdef __cplusplus
}
#endif

#endif // __AXFX_H__
