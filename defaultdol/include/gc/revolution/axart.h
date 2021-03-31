/*---------------------------------------------------------------------------*
  Project:  Revolution AXART library
  File:     axart.h

  Copyright (C)1998-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: axart.h,v $
  Revision 1.6.14.1  2007/09/13 02:54:15  aka
  For SDK3.1 patch1.

  Revision 1.7  2007/09/07 05:55:35  aka
  Added AXARTRemoveArticulator().

  Revision 1.6  2006/10/16 11:21:41  aka
  Added 3 articulators for remote speakers.
  Added AXARTInitArtRmt().
  Added AXARTInitArtRmtFader().
  Added AXARTInitArtRmtAuxVolume().

  Revision 1.5  2006/09/29 05:55:44  aka
  Added AXART_FADER.
  Added AXARTInitArtFader().

  Revision 1.4  2006/02/09 04:44:57  aka
  Changed copyright.

  Revision 1.3  2006/02/04 13:05:46  hashida
  (none)

  Revision 1.2  2005/11/07 06:47:11  aka
  Changed for Revolution's audio spec.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Imported from Dolphin tree.
    
    7     2002/08/21 6:38p Akagi
    - Set #pragma warn_padding off around the AXART_3D{} definition.
    - Set #pragma warn_padding off around the AXART_PANNING{} definition.
    - Set #pragma warn_padding off around the AXART_SRCTYPE{} definition.
    
    6     2002/08/15 11:03a Billyjack
    Added low pass filter articulator.
    
    5     2002/04/11 1:58p Billyjack
    - Added IIR articulator, code commented out.
    
    4     2002/01/08 6:40p Billyjack
    - Added AXARTInitSound().
    
    3     2001/09/26 3:44p Billyjack
    extern'd lfo samples for ANSI C compatibility
    
    2     2001/08/20 6:15p Billyjack
    Added comments.
    
    1     2001/08/15 1:11p Billyjack
    Created
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <revolution.h>

#ifndef __AXART_H__
#define __AXART_H__

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
    AXART_ART generic articulator header
 *---------------------------------------------------------------------------*/
typedef struct
{

    void        *next;
    u32         type;

} AXART_ART;

// Articulator types
#define AXART_TYPE_3D               1
#define AXART_TYPE_PANNING          2
#define AXART_TYPE_ITD              3
#define AXART_TYPE_SRCTYPE          4
#define AXART_TYPE_PITCH            5
#define AXART_TYPE_PITCH_ENV        6
#define AXART_TYPE_PITCH_MOD        7
#define AXART_TYPE_VOLUME           8
#define AXART_TYPE_AUXA_VOLUME      9
#define AXART_TYPE_AUXB_VOLUME      10
#define AXART_TYPE_AUXC_VOLUME      11
#define AXART_TYPE_VOLUME_ENV       12
#define AXART_TYPE_AUXA_VOLUME_ENV  13
#define AXART_TYPE_AUXB_VOLUME_ENV  14
#define AXART_TYPE_AUXC_VOLUME_ENV  15
#define AXART_TYPE_VOLUME_MOD       16
#define AXART_TYPE_AUXA_VOLUME_MOD  17
#define AXART_TYPE_AUXB_VOLUME_MOD  18
#define AXART_TYPE_AUXC_VOLUME_MOD  19
#define AXART_TYPE_LPF              20
#define AXART_TYPE_FADER            21
#define AXART_TYPE_RMT              22
#define AXART_TYPE_RMT_FADER        24
#define AXART_TYPE_RMT_AUX_VOLUME   25


/*---------------------------------------------------------------------------*
    LFO used with modulation articulators
 *---------------------------------------------------------------------------*/
typedef struct
{
    
    f32         *lfo;           // lfo samples in RAM
    u32         length;         // lfo samples for 1 period
    f32         delta;          // lfo delta per 5ms 
    
    u32         sampleIndex;    // lfo sample index      
    f32         counter;        // lfo counter
    f32         sample1;        // lfo last sample for interpolation
    f32         sample;         // lfo sample
    f32         output;         // lfo output

} AXART_LFO;

#define AXART_SINE_SAMPLES          64
#define AXART_SQUARE_SAMPLES        64
#define AXART_SAW_SAMPLES           64
#define AXART_REVERSESAW_SAMPLES    64
#define AXART_TRIANGLE_SAMPLES      64
#define AXART_NOISE_SAMPLES         64

extern f32 AXARTSine[AXART_SINE_SAMPLES];
extern f32 AXARTSquare[AXART_SQUARE_SAMPLES];
extern f32 AXARTSaw[AXART_SAW_SAMPLES];
extern f32 AXARTReverseSaw[AXART_REVERSESAW_SAMPLES];
extern f32 AXARTTriangle[AXART_TRIANGLE_SAMPLES];
extern f32 AXARTNoise[AXART_NOISE_SAMPLES];


/*---------------------------------------------------------------------------*
    articulators
 *---------------------------------------------------------------------------*/

#ifdef  __MWERKS__
#pragma warn_padding off
#endif

typedef struct
{
    AXART_ART   art;

    // runtime user params

    f32         hAngle;         // horizontal angle
    f32         vAngle;         // vertical angle
    f32         dist;           // distance from listener
    f32         closingSpeed;   // for doppler
    u32         update;         // set to true after changing params

    // do not write to these params

    u8          pan;
    u8          span;
    u8          src;
    u16         itdL;
    u16         itdR;
    f32         pitch;
    s32         attenuation;

} AXART_3D;

typedef struct
{
    AXART_ART   art;

    // runtime user params

    u8          pan;            // left - right 0 - 127, 64 is center
    u8          span;           // rear - front 0 - 127, 127 is front

} AXART_PANNING;

typedef struct
{

    AXART_ART   art;
    
    // runtime user params

    u8          src;            // use one of the following
                                // AX_SRC_TYPE_NONE
                                // AX_SRC_TYPE_LINEAR
                                // AX_SRC_TYPE_4TAP_8K
                                // AX_SRC_TYPE_4TAP_12K
                                // AX_SRC_TYPE_4TAP_16K

} AXART_SRCTYPE;

#ifdef  __MWERKS__
#pragma warn_padding reset
#endif

typedef struct
{
    AXART_ART   art;

    // runtime user params

    u16         itdL;           // sample shift left, 0 - 31
    u16         itdR;           // sample shift right, 0 - 31

} AXART_ITD;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         cents;

} AXART_PITCH;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         cents;

} AXART_PITCH_ENV;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;

    // runtime user params
    
    s32         cents;

} AXART_PITCH_MOD;

typedef struct
{
    AXART_ART   art;
    
    // runtime user params

    s32         attenuation;

} AXART_VOLUME;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXA_VOLUME;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXB_VOLUME;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXC_VOLUME;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         attenuation;

} AXART_VOLUME_ENV;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         attenuation;

} AXART_AUXA_VOLUME_ENV;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         attenuation;

} AXART_AUXB_VOLUME_ENV;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         attenuation;

} AXART_AUXC_VOLUME_ENV;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;

    // runtime user params
    
    s32         attenuation;

} AXART_VOLUME_MOD;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;
    
    // runtime user params

    s32         attenuation;

} AXART_AUXA_VOLUME_MOD;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXB_VOLUME_MOD;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXC_VOLUME_MOD;

typedef struct
{
    AXART_ART   art;
    u32         initLPF;

    // runtime user params
    u32         frequency;  // indexed roll off frequency, see below
    u32         update;     // set to TRUE after changing type or frequency

} AXART_LPF;

#define AXART_LPF_FREQ_16000_HZ     0
#define AXART_LPF_FREQ_12800_HZ     1
#define AXART_LPF_FREQ_10240_HZ     2
#define AXART_LPF_FREQ_8000_HZ      3
#define AXART_LPF_FREQ_6400_HZ      4
#define AXART_LPF_FREQ_5120_HZ      5
#define AXART_LPF_FREQ_4000_HZ      6
#define AXART_LPF_FREQ_3200_HZ      7
#define AXART_LPF_FREQ_2560_HZ      8
#define AXART_LPF_FREQ_2000_HZ      9
#define AXART_LPF_FREQ_1600_HZ      10
#define AXART_LPF_FREQ_1280_HZ      11
#define AXART_LPF_FREQ_1000_HZ      12
#define AXART_LPF_FREQ_800_HZ       13
#define AXART_LPF_FREQ_640_HZ       14
#define AXART_LPF_FREQ_500_HZ       15
#define AXART_LPF_FREQ_400_HZ       16
#define AXART_LPF_FREQ_320_HZ       17
#define AXART_LPF_FREQ_256_HZ       18
#define AXART_LPF_FREQ_200_HZ       19
#define AXART_LPF_FREQ_160_HZ       20
#define AXART_LPF_FREQ_128_HZ       21
#define AXART_LPF_FREQ_100_HZ       22
#define AXART_LPF_FREQ_80_HZ        23

typedef struct
{
    AXART_ART   art;
    
    // runtime user params

    s32         attenuation;

} AXART_FADER;


/*---------------------------------------------------------------------------*
    articulators for remote speakers
 *---------------------------------------------------------------------------*/

// enable/disable flag of remote outputs
typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    u16         on;

} AXART_RMT;

// fader value
typedef struct
{
    AXART_ART   art;
    
    // runtime user params

    s32         attenuation0;
    s32         attenuation1;
    s32         attenuation2;
    s32         attenuation3;

} AXART_RMT_FADER;

// aux send level
typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         attenuation0;
    s32         attenuation1;
    s32         attenuation2;
    s32         attenuation3;

} AXART_RMT_AUX_VOLUME;


/*---------------------------------------------------------------------------*
    AXART_SOUND struct used per sound in sound list to articulate
 *---------------------------------------------------------------------------*/
typedef struct
{

    void        *next;          // next sound in list
    void        *prev;          // prev sound in list

    AXVPB       *axvpb;         // user acquired voice
    f32         sampleRate;     // normal sample rate

    AXART_ART   *articulators;  // list of articulators

} AXART_SOUND;


/*---------------------------------------------------------------------------*
    Function prototypes
 *---------------------------------------------------------------------------*/

void AXARTInit                  (void);
void AXARTQuit                  (void);
void AXARTServiceSounds         (void);
void AXARTInitSound             (AXART_SOUND *sound, AXVPB *voice, u32 sampleRate);
void AXARTAddSound              (AXART_SOUND *sound);
void AXARTRemoveSound           (AXART_SOUND *sound);
void AXARTSet3DDistanceScale    (f32 scale);
void AXARTSet3DDopplerScale     (f32 scale);
void AXARTAddArticulator        (AXART_SOUND *sound, AXART_ART *articulator);
BOOL AXARTRemoveArticulator     (AXART_SOUND *sound, AXART_ART *articulator);

void AXARTInitLfo               (AXART_LFO *lfo, f32 *samples, u32 length, f32 delta);

void AXARTInitArt3D             (AXART_3D              *articulator);
void AXARTInitArtPanning        (AXART_PANNING         *articulator);
void AXARTInitArtItd            (AXART_ITD             *articulator);
void AXARTInitArtSrctype        (AXART_SRCTYPE         *articulator);

void AXARTInitArtPitch          (AXART_PITCH           *articulator);
void AXARTInitArtPitchEnv       (AXART_PITCH_ENV       *articulator);
void AXARTInitArtPitchMod       (AXART_PITCH_MOD       *articulator);

void AXARTInitArtVolume         (AXART_VOLUME          *articulator);
void AXARTInitArtAuxAVolume     (AXART_AUXA_VOLUME     *articulator);
void AXARTInitArtAuxBVolume     (AXART_AUXB_VOLUME     *articulator);
void AXARTInitArtAuxCVolume     (AXART_AUXC_VOLUME     *articulator);

void AXARTInitArtVolumeEnv      (AXART_VOLUME_ENV      *articulator);
void AXARTInitArtAuxAVolumeEnv  (AXART_AUXA_VOLUME_ENV *articulator);
void AXARTInitArtAuxBVolumeEnv  (AXART_AUXB_VOLUME_ENV *articulator);
void AXARTInitArtAuxCVolumeEnv  (AXART_AUXC_VOLUME_ENV *articulator);

void AXARTInitArtVolumeMod      (AXART_VOLUME_MOD      *articulator);
void AXARTInitArtAuxAVolumeMod  (AXART_AUXA_VOLUME_MOD *articulator);
void AXARTInitArtAuxBVolumeMod  (AXART_AUXB_VOLUME_MOD *articulator);
void AXARTInitArtAuxCVolumeMod  (AXART_AUXC_VOLUME_MOD *articulator);

void AXARTInitArtLpf            (AXART_LPF             *articulator);
void AXARTInitArtFader          (AXART_FADER           *articulator);

void AXARTInitArtRmt            (AXART_RMT             *articulator);
void AXARTInitArtRmtFader       (AXART_RMT_FADER       *articulator);
void AXARTInitArtRmtAuxVolume   (AXART_RMT_AUX_VOLUME  *articulator);


#ifdef __cplusplus
}
#endif

#endif // __AXART_H__ 
