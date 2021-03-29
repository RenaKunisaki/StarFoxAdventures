/*---------------------------------------------------------------------------*
  Project:     MIDI to SI library 
  File:        midi.h
  Programmer:  HIRATSU Daisuke

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#include <revolution/types.h>
#include <revolution/os.h>

#ifndef MIDI_H__
#define MIDI_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef void (*MIDICallback)(s32 chan, s32 ret);

#define MIDI_CHAN0                   0
#define MIDI_CHAN1                   1
#define MIDI_CHAN2                   2
#define MIDI_CHAN3                   3
#define MIDI_MAX_CHAN                4

#define MIDI_JSTAT_MASK           0x3a
#define MIDI_JSTAT_FLAGS_SHIFT       4
#define MIDI_JSTAT_FLAGS_MASK     0x30
#define MIDI_JSTAT_SEND           0x08
#define MIDI_JSTAT_RECV           0x02

#define MIDI_READY                   0
#define MIDI_NOT_READY               1
#define MIDI_BUSY                    2

void MIDIInit(void);
s32  MIDIGetStatus(s32 chan, u8* status);
s32  MIDIGetStatusAsync(s32 chan, u8* status, MIDICallback callback);
s32  MIDIRead(s32 chan, u8* dst, u8* status);
s32  MIDIReadAsync(s32 chan, u8* dst, u8* status, MIDICallback callback);


#ifdef __cplusplus
}
#endif


#endif  // MIDI_H__
