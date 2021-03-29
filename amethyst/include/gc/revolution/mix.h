/*---------------------------------------------------------------------------*
  Project:  Mixer application for AX
  File:     mix.h

  Copyright (C)1998-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mix.h,v $
  Revision 1.11  2006/10/26 02:01:22  aka
  Changed from OS_SOUND_MODE _ to SC_SOUND_MODE_.

  Revision 1.10  2006/10/23 01:51:13  aka
  Rolled-back MIXInit().
  Added MIXInitSpecifyMem().

  Revision 1.9  2006/10/10 04:53:47  aka
  Removed MIXSetMemory().
  Revised MIXInit().

  Revision 1.8  2006/10/06 08:04:14  aka
  Added MIXGetMemorySize().
  Added MIXSetMemory().

  Revision 1.7  2006/07/24 10:34:56  aka
  Removed unused definitions.

  Revision 1.6  2006/07/24 10:14:45  aka
  Removed test functions.

  Revision 1.5  2006/07/24 09:14:17  aka
  Changed in ration to modification of AX lib.

  Revision 1.4  2006/07/19 07:50:33  aka
  Modified to support controller speakers.

  Revision 1.3  2006/02/09 04:47:54  aka
  Changed copyright.

  Revision 1.2  2005/11/07 06:47:18  aka
  Changed for Revolution's audio spec.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Imported from Dolphin tree.
    
    3     1/08/02 6:42p Billyjack
    - Added DPL2 support
    
    2     5/30/01 3:09p Billyjack
    added definitions for MIXSetSoundMode modes (stereo / mono)
    
    1     5/09/01 1:28p Billyjack
    created
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __MIX_H__
#define __MIX_H__

#include <revolution/sc.h>

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
    mixer sound mode definitions
 *---------------------------------------------------------------------------*/
#define MIX_SOUND_MODE_MONO         SC_SOUND_MODE_MONO
#define MIX_SOUND_MODE_STEREO       SC_SOUND_MODE_STEREO
#define MIX_SOUND_MODE_SURROUND     SC_SOUND_MODE_SURROUND
#define MIX_SOUND_MODE_DPL2         3

/*---------------------------------------------------------------------------*
    mixer channel mode definitions
 *---------------------------------------------------------------------------*/
#define MIX_MODE_AUXA_PREFADER      0x00000001
#define MIX_MODE_AUXB_PREFADER      0x00000002
#define MIX_MODE_AUXC_PREFADER      0x00000004
#define MIX_MODE_MUTE               0x00000008

#define MIX_MODE_UPDATE_INPUT       0x10000000    
#define MIX_MODE_UPDATE_INPUT1      0x20000000    
#define MIX_MODE_UPDATE_MIX         0x40000000    
#define MIX_MODE_UPDATE_MIX1        0x80000000    

/*---------------------------------------------------------------------------*
    mixer channel mode definitions for remote speakers
 *---------------------------------------------------------------------------*/
#define MIX_MODE_AUX0_PREFADER      0x00000001
#define MIX_MODE_AUX1_PREFADER      0x00000002
#define MIX_MODE_AUX2_PREFADER      0x00000004
#define MIX_MODE_AUX3_PREFADER      0x00000008

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
typedef struct MIXChannel
{
    AXVPB   *axvpb;
    
    u32     mode;           

    int     input;
    int     auxA;               // AUX send A
    int     auxB;               // AUX send B
    int     auxC;               // AUX send C
    int     pan;                // 0 - 127 Left - Right
    int     span;               // 0 - 127 Front - Back
    int     fader;              // fader

    int     l;                  // left
    int     r;                  // right
    int     f;                  // front
    int     b;                  // back
    int     l1;                 // DPL2 left rear
    int     r1;                 // DPL2 right rear

    u16     v;
    u16     v1;
    u16     vL;            
    u16     vL1;            
    u16     vR;
    u16     vR1;
    u16     vS;
    u16     vS1;
    u16     vAL;
    u16     vAL1;
    u16     vAR;
    u16     vAR1;
    u16     vAS;
    u16     vAS1;
    u16     vBL;
    u16     vBL1;
    u16     vBR;
    u16     vBR1;
    u16     vBS;
    u16     vBS1;
    u16     vCL;
    u16     vCL1;
    u16     vCR;
    u16     vCR1;
    u16     vCS;
    u16     vCS1;

} MIXChannel;

typedef struct MIXRmtChannel
{
    u32     mode;
    
    int     fader0;
    int     fader1;
    int     fader2;
    int     fader3;
    
    int     aux0;
    int     aux1;
    int     aux2;
    int     aux3;
    
    u16     vMain0;            
    u16     vMain0_1;            
    u16     vAux0;
    u16     vAux0_1;

    u16     vMain1;            
    u16     vMain1_1;            
    u16     vAux1;
    u16     vAux1_1;

    u16     vMain2;            
    u16     vMain2_1;            
    u16     vAux2;
    u16     vAux2_1;

    u16     vMain3;            
    u16     vMain3_1;            
    u16     vAux3;
    u16     vAux3_1;

} MIXRmtChannel;


/*---------------------------------------------------------------------------*
    Exposed function prototypes
 *---------------------------------------------------------------------------*/

void    MIXInit             (void);
void    MIXInitSpecifyMem   (void* mem);
void    MIXQuit             (void);
void    MIXSetSoundMode     (u32 mode);
u32     MIXGetSoundMode     (void);

#define MIXGetMemorySize(num)  ((sizeof(MIXChannel) + sizeof(MIXRmtChannel)) * num)

void    MIXInitChannel      (
                             AXVPB *p,      // pointer to voice
                             u32 mode,      // initial aux A, B, mute modes
                             int input,     // initial input atten / gain
                             int auxA,      // initial aux A atten / gain
                             int auxB,      // initial aux B atten / gain
                             int auxC,      // initial aux C atten / gain
                             int pan,       // initial pan 
                             int span,      // initial span
                             int fader      // initial fader atten / gain
                             );    

void    MIXReleaseChannel   (AXVPB *p);

void    MIXResetControls    (AXVPB *p);

void    MIXSetInput         (AXVPB *p, int dB);
void    MIXAdjustInput      (AXVPB *p, int dB);
int     MIXGetInput         (AXVPB *p);

void    MIXAuxAPostFader    (AXVPB *p);
void    MIXAuxAPreFader     (AXVPB *p);
BOOL    MIXAuxAIsPostFader  (AXVPB *p);
void    MIXSetAuxA          (AXVPB *p, int dB);
void    MIXAdjustAuxA       (AXVPB *p, int dB);
int     MIXGetAuxA          (AXVPB *p);

void    MIXAuxBPostFader    (AXVPB *p);
void    MIXAuxBPreFader     (AXVPB *p);
BOOL    MIXAuxBIsPostFader  (AXVPB *p);
void    MIXSetAuxB          (AXVPB *p, int dB);
void    MIXAdjustAuxB       (AXVPB *p, int dB);
int     MIXGetAuxB          (AXVPB *p);

void    MIXAuxCPostFader    (AXVPB *p);
void    MIXAuxCPreFader     (AXVPB *p);
BOOL    MIXAuxCIsPostFader  (AXVPB *p);
void    MIXSetAuxC          (AXVPB *p, int dB);
void    MIXAdjustAuxC       (AXVPB *p, int dB);
int     MIXGetAuxC          (AXVPB *p);

void    MIXSetPan           (AXVPB *p, int pan);
void    MIXAdjustPan        (AXVPB *p, int pan);
int     MIXGetPan           (AXVPB *p);

void    MIXSetSPan          (AXVPB *p, int span);
void    MIXAdjustSPan       (AXVPB *p, int span);
int     MIXGetSPan          (AXVPB *p);

void    MIXMute             (AXVPB *p);
void    MIXUnMute           (AXVPB *p);
BOOL    MIXIsMute           (AXVPB *p);

void    MIXSetFader         (AXVPB *p, int dB);
void    MIXAdjustFader      (AXVPB *p, int dB);
int     MIXGetFader         (AXVPB *p);

void    MIXUpdateSettings   (void);

void    MIXRmtSetVolumes    (
                             AXVPB *axvpb,   // pointer to voice
                             u32    mode,    // aux0,1,2,3 modes
                             int    fader0,  // initial fader0 atten / gain
                             int    fader1,  // initial fader1 atten / gain
                             int    fader2,  // initial fader2 atten / gain
                             int    fader3,  // initial fader3 atten / gain
                             int    aux0,    // initial aux0   atten / gain
                             int    aux1,    // initial aux1   atten / gain
                             int    aux2,    // initial aux2   atten / gain
                             int    aux3     // initial aux3   atten / gain
                            );
    
void    MIXRmtAuxPostFader  (AXVPB *p, int num);
void    MIXRmtAuxPreFader   (AXVPB *p, int num);
BOOL    MIXRmtAuxIsPostFader(AXVPB *p, int num);
    
void    MIXRmtSetAux        (AXVPB *p, int num, int dB);
void    MIXRmtAdjustAux     (AXVPB *p, int num, int dB);
int     MIXRmtGetAux        (AXVPB *p, int num);
    
void    MIXRmtSetFader      (AXVPB *p, int num, int dB);
void    MIXRmtAdjustFader   (AXVPB *p, int num, int dB);
int     MIXRmtGetFader      (AXVPB *p, int num);

#ifdef __cplusplus
}
#endif

#endif // __MIX_H__ 
