/*---------------------------------------------------------------------------*
  Project:  Synth application for AX
  File:     syn.h

  Copyright (C)1998-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: syn.h,v $
  Revision 1.10  2006/11/21 04:57:08  aka
  Removed zeroBaseXXX from SYNSYNTH structure.

  Revision 1.9  2006/10/23 01:39:50  aka
  Rolled-back SYNInit().
  Added SYNInitSpecifyMem().

  Revision 1.8  2006/10/19 02:51:31  aka
  Added a definition of SYNVOICE.
  Revised SYNGetMemorySize().

  Revision 1.7  2006/10/10 04:53:09  aka
  Removed SYNSetMemory().
  Revised SYNInit().

  Revision 1.6  2006/10/10 02:40:06  aka
  Added SYNGetMemorySize().
  Added SYNSetMemory().

  Revision 1.5  2006/07/25 08:07:55  aka
  Modified to support controller speakers.

  Revision 1.4  2006/01/31 06:02:06  aka
  Changed arguments of SYNInitSynth().

  Revision 1.3  2006/01/30 11:50:10  aka
  Changed copyright.

  Revision 1.2  2006/01/30 11:31:35  aka
  Renamed from ARAM to MRAM.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Imported from dolphin tree.
    
    2     8/16/01 12:27p Billyjack
    added zeroBuffer offset to API
    
    1     5/09/01 1:28p Billyjack
    created
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __SYN_H__
#define __SYN_H__

#include <revolution/wt.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    synthesizer
 *---------------------------------------------------------------------------*/
    
#define SYN_INPUT_BUFFER_SIZE      256

typedef struct SYNSYNTH SYNSYNTH;
    
typedef void (*SYNCallback)(AXVPB *axvpb, SYNSYNTH *synth, u8 midiChannel);

struct SYNSYNTH
{
    void        *next;                  // synth list

    WTINST      *percussiveInst;        // pointer to instrument[0] in wavetable
    WTINST      *melodicInst;           // pointer to instrument[0] in wavetable
    WTREGION    *region;                // pointer to region[0] in wavetable
    WTART       *art;                   // pointer to articulation[0] in wavetable
    WTSAMPLE    *sample;                // pointer to sample[0] in wavetable
    WTADPCM     *adpcm;                 // pointer to adpcm[0] in wavetable
    u32         samplesBaseWord;        // base address (16bit) of samples in RAM
    u32         samplesBaseByte;        // base address (8bit)of samples in RAM
    u32         samplesBaseNibble;      // base address (nibble) of samples in RAM
    u32         priorityVoiceAlloc;     // priority for allocating new note
    u32         priorityNoteOn;         // priority for notes that are on
    u32         priorityNoteRelease;    // peiority for nots being released

    WTINST      *inst       [16];       // pointer to instrument per channel

    s32         masterVolume;           // master volume for synth             

    u8          controller  [16][128];  // MIDI controller registers

    u8          rpn         [16];       // weather to enter rpn or nrpn data
    s16         dataEntry   [16];       // data entry value

    s32         pwMaxCents  [16];       // pitch wheel cents at + max
    s32         pwCents     [16];       // current pitch wheel cents
    
    s32         volAttn     [16];       // MIDI channel volume
    s32         expAttn     [16];       // expression volume
    s32         auxAAttn    [16];       // aux A (reverb)
    s32         auxBAttn    [16];       // aux B (chorus)

    u8          input[SYN_INPUT_BUFFER_SIZE][3];
    u8          *inputPosition;
    u32         inputCounter;
    
    u32         notes;                  // notes running

    void        *keyGroup[16][16];      // storage for key group notes
    void        *voice[16][128];        // storage for voices index references

    SYNCallback initCallback;
    SYNCallback updateCallback;

};

/*---------------------------------------------------------------------------*
    a synthesizer voice
 *---------------------------------------------------------------------------*/
    
typedef struct SYNVOICE
{
    // references to other objects
    void        *next;              // used by synth
    AXVPB       *axvpb;             // AX voice belonging to this voice
    SYNSYNTH    *synth;             // this voice belongs to this synth
    u8          midiChannel;        // MIDI channel for this voice
    u8          keyNum;             // key number for this voice
    u8          keyVel;             // key velocity
    u8          pan;                // absolute panning for drum sounds
    u8          keyGroup;           // key group 1 - 15
    
    // pointer to objects in wavetable
    WTREGION    *region;            // instrument region         
    WTART       *art;               // articulation (VE, PE, LFO)
    WTSAMPLE    *sample;            // sample data   
    WTADPCM     *adpcm;             // adpcm data

    // on hold from hold pedal
    u32         hold;               // 1 if we are to hold after key off

    // vars for voice processing
    u32         type;               // one shot or looped
    f32         srcRatio;           // base src ratio
    s32         cents;              // base relitive pitch
    s32         attn;               // base attenuation (region + vel)
   
    s32         lfoState;           // LFO table index
    s32         lfoAttn;            // LFO attenaution
    s32         lfoCents;           // LFO pitch
    s32         lfoFreq;            // LFO table index / audio frame
    s32         lfoDelay;           // LFO delay in audio frames
    s32         lfoAttn_;           // LFO volume at 1.0
    s32         lfoCents_;          // LFO pitch at 1.0
    s32         lfoModAttn;         // LFO MOD wheel to attenuation
    s32         lfoModCents;        // LFO MOD wheel to cents

    u32         veState;
    s32         veAttn;             // 0x00010000 = 0,1dB
    s32         veAttack;           // 0 - 99 table lookup for linear volume
    s32         veAttackDelta;      // add this to attack per frame
    s32         veDecay;            // 0x00010000 = 0,1dB per frame
    s32         veSustain;          // 0x00010000 = 0,1dB
    s32         veRelease;          // 0x00010000 = 0,1dB per frame

    u32         peState;            // state of envelope
    s32         peCents;            // current pitch
    s32         peAttack;           // pitch delta per frame
    s32         peDecay;            // pitch delta per frame
    s32         peSustain;          // pitch at sustain
    s32         peRelease;          // pitch delta per frame
    s32         pePitch;            // pitch cents at 100%    

} SYNVOICE;


#define SYN_VOICE_TYPE_ONESHOT  0
#define SYN_VOICE_TYPE_LOOPED   1

#define SYN_VOICE_STATE_ATTACK  0
#define SYN_VOICE_STATE_DECAY   1
#define SYN_VOICE_STATE_SUSTAIN 2
#define SYN_VOICE_STATE_RELEASE 3
#define SYN_VOICE_STATE_DONE    4

/*---------------------------------------------------------------------------*
    Exposed function prototypes
 *---------------------------------------------------------------------------*/

void	SYNInit             (void);
void	SYNInitSpecifyMem   (void* mem);
void	SYNQuit             (void);

#define SYNGetMemorySize(num)  (sizeof(SYNVOICE) * num)
        
void    SYNRunAudioFrame    (void);

void    SYNInitSynth        (
                             SYNSYNTH *synth,                // user allocated synth
                             u8       *wavetable,            // pointer to wave table
                             u8       *samples,              // pointer to samples
                             u8       *zerobuffer,           // pointer to zero buffer
                             u32      priorityVoiceAlloc,    // priority for voice allocation
                             u32      priorityNoteOn,        // priority for note on
                             u32      priorityNoteRelease    // priority for note release
                             );

void    SYNQuitSynth        (SYNSYNTH *synth);
void    SYNMidiInput        (SYNSYNTH *synth, u8 *input);
u8      SYNGetMidiController(SYNSYNTH *synth, u8 midiChannel, u8 function);
void    SYNSetMasterVolume  (SYNSYNTH *synth, s32 dB);
s32     SYNGetMasterVolume  (SYNSYNTH *synth);
u32     SYNGetActiveNotes   (SYNSYNTH *synth);

SYNCallback SYNSetInitCallback  (SYNSYNTH *synth, SYNCallback callback);
SYNCallback SYNSetUpdateCallback(SYNSYNTH *synth, SYNCallback callback);
    

#ifdef __cplusplus
}
#endif

#endif // __SYN_H__ 
