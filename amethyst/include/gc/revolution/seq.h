/*---------------------------------------------------------------------------*
  Project:  MIDI Sequencer application for AX synthesizer
  File:     seq.h

  Copyright (C)1998-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: seq.h,v $
  Revision 1.2  2006/01/31 06:34:11  aka
  Changed arguments of SEQAddSequence().

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Imported from dolphin tree.
    
    4     02/08/21 6:33p Akagi
    Set #pragma warn_padding off around the SEQTRACK{} definition.
    
    3     8/16/01 12:27p Billyjack
    added zeroBuffer offset to API
    
    2     5/11/01 4:17p Billyjack
    fixed problem with songs ending and set state to SEQ_STATE_RUN
    
    1     5/09/01 1:28p Billyjack
    created
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef __SEQ_H__
#define __SEQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/syn.h>

#define SEQ_MAX_TRACKS 64

/*---------------------------------------------------------------------------*
    callback interface
 *---------------------------------------------------------------------------*/
typedef void    (*SEQCALLBACK)(void *track, u8 controller);

#ifdef  __MWERKS__
#pragma warn_padding off
#endif

typedef struct _SEQTRACK
{
    void        *sequence;              // pointer to parent 
    u8          *start;                 // pointer to start of track
    u8          *end;                   // pointer to end of track
    u8          *current;               // pointer to current read position
    u8          status;                 // running status
    f32         beatsPerSec;            // beats per sec
    u32         defaultTicksPerFrame;   // ticks per audio frame
    u32         ticksPerFrame;          // ticks per audio frame
    u32         delay;                  // ticks to delay for next event
    u32         state;                  // state of track

} SEQTRACK;

#ifdef  __MWERKS__
#pragma warn_padding reset
#endif

typedef struct _SEQSEQUENCE
{
    void        *next;                  // next sequence
    u32         state;                  // stop, run, run looped, pause
    u16         nTracks;                // number of sequencer tracks
    s16         timeFormat;             // MIDI file time format
    u32         tracksRunning;          // number of tracks still running
    u32         end;                    // flag end of sequence
    SYNSYNTH    synth;                  // synth used for this sequence      
    SEQCALLBACK callback[128];          // controller event callbacks
    SEQTRACK    track[SEQ_MAX_TRACKS];  // sequencer tracks

} SEQSEQUENCE;

#define SEQ_STATE_STOP      0
#define SEQ_STATE_RUN       1
#define SEQ_STATE_RUNLOOPED 2
#define SEQ_STATE_PAUSE     3

#define SEQ_ALL_TRACKS      0xFFFFFFFF

/*---------------------------------------------------------------------------*
    function prototypes
 *---------------------------------------------------------------------------*/
void    SEQInit             (void);
void    SEQQuit             (void);
void    SEQRunAudioFrame    (void);

void    SEQAddSequence(
            SEQSEQUENCE     *sequence,          // user allocated SEQSEQUENCE
            u8              *midiStream,        // pointer to MIDI stream
            u8              *wavetable,         // pointer to wave table
            u8              *samples,           // pointer to samples
            u8              *zerobuffer,        // pointer to zero buffer
            u32             priorityVoiceAlloc, // priority for allocating notes
            u32             priorityNoteOn,     // priority for notes that are on
            u32             priorityNoteRelease // priority for notes in release stage
            );

void    SEQRemoveSequence   (SEQSEQUENCE *sequence);

void    SEQRegisterControllerCallback(
            SEQSEQUENCE     *sequence,          // user initialized SEQSEQUENCE
            u8              controller,         // MIDI controller
            SEQCALLBACK     callback            // callback function
            );

void    SEQSetState         (SEQSEQUENCE *sequence, u32 state);
u32     SEQGetState         (SEQSEQUENCE *sequence);
void    SEQSetTempo         (SEQSEQUENCE *sequence, u32 track, f32 tempo);
f32     SEQGetTempo         (SEQSEQUENCE *sequence, u32 track);
void    SEQSetVolume        (SEQSEQUENCE *sequence, s32 dB);
s32     SEQGetVolume        (SEQSEQUENCE *sequence);


#ifdef __cplusplus
}
#endif

#endif // __SEQ_H__ 
