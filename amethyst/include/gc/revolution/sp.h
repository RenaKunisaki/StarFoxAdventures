/*---------------------------------------------------------------------------*
  Project:  Sound Pipeline (SP) for AX
  File:     sp.h

  Copyright (C)1998-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
  
  $Log: sp.h,v $
  Revision 1.2  2006/01/31 07:11:12  aka
  Changed arguments of SPInitSoundTable().

    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __SP_H__
#define __SP_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    AXPBADPCM       adpcm;
    AXPBADPCMLOOP   adpcmloop;

} SPAdpcmEntry;


typedef struct
{
    u32             type;
    u32             sampleRate;
    u32             loopAddr;
    u32             loopEndAddr;
    u32             endAddr;
    u32             currentAddr;
    SPAdpcmEntry    *adpcm;

} SPSoundEntry;


#define SP_TYPE_ADPCM_ONESHOT   0
#define SP_TYPE_ADPCM_LOOPED    1
#define SP_TYPE_PCM16_ONESHOT   2
#define SP_TYPE_PCM16_LOOPED    3
#define SP_TYPE_PCM8_ONESHOT    4
#define SP_TYPE_PCM8_LOOPED     5


typedef struct
{

    u32             entries;
    SPSoundEntry    sound[1];

} SPSoundTable;



void SPInitSoundTable(SPSoundTable *table, u8 *samples, u8 *zerobuffer);
SPSoundEntry * SPGetSoundEntry(SPSoundTable *table, u32 index);
void SPPrepareSound(SPSoundEntry *sound, AXVPB *axvpb, u32 sampleRate);
void SPPrepareEnd(SPSoundEntry *sound, AXVPB *axvpb);


#ifdef __cplusplus
}
#endif

#endif // __SP_H__ 
