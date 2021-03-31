/*---------------------------------------------------------------------------*
  Project: AUX effects for Revolution AX
  File:    axfx_presets.h

  Copyright (C)2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: axfx_presets.h,v $
  Revision 1.1  06/08/2006 05:58:46  aka
  Imported new AXFX made by Kawamura-san (EAD).

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __AXFX_PRESETS_H__
#define __AXFX_PRESETS_H__

#ifdef __cplusplus
extern "C" {
#endif


/*==========================================================================*
    presets for AXFX effects
 *==========================================================================*/

/*--------------------------------------------------------------------------*
  for delay (mono, stereo, surround, DPL2)
 *--------------------------------------------------------------------------*/

#define AXFX_PRESET_DELAY_EXP_TYPE1(fx)             \
    {                                               \
        (fx)->delay       =  80.f;                  \
        (fx)->maxDelay    = 100.f;                  \
        (fx)->feedback    =  0.6f;                  \
        (fx)->lpf         =  1.0f;                  \
                                                    \
        (fx)->busIn       = NULL;                   \
        (fx)->busOut      = NULL;                   \
        (fx)->outGain     = 1.0f;                   \
        (fx)->sendGain    = 0.0f;                   \
    }

#define AXFX_PRESET_DELAY_EXP_TYPE2(fx)             \
    {                                               \
        (fx)->delay       =  40.f;                  \
        (fx)->maxDelay    = 100.f;                  \
        (fx)->feedback    =  0.5f;                  \
        (fx)->lpf         =  0.4f;                  \
                                                    \
        (fx)->busIn       = NULL;                   \
        (fx)->busOut      = NULL;                   \
        (fx)->outGain     = 1.0f;                   \
        (fx)->sendGain    = 0.0f;                   \
    }

#define AXFX_PRESET_DELAY_EXP_FLANGE1(fx)           \
    {                                               \
        (fx)->delay       =  0.1f;                  \
        (fx)->maxDelay    = 100.f;                  \
        (fx)->feedback    =  0.6f;                  \
        (fx)->lpf         =  1.0f;                  \
                                                    \
        (fx)->busIn       = NULL;                   \
        (fx)->busOut      = NULL;                   \
        (fx)->outGain     = 1.0f;                   \
        (fx)->sendGain    = 0.0f;                   \
    }

#define AXFX_PRESET_DELAY_EXP_FLANGE2(fx)           \
    {                                               \
        (fx)->delay       =  0.5f;                  \
        (fx)->maxDelay    = 100.f;                  \
        (fx)->feedback    =  0.6f;                  \
        (fx)->lpf         =  0.7f;                  \
                                                    \
        (fx)->busIn       = NULL;                   \
        (fx)->busOut      = NULL;                   \
        (fx)->outGain     = 1.0f;                   \
        (fx)->sendGain    = 0.0f;                   \
    }


/*--------------------------------------------------------------------------*
  for standard reverb (mono, stereo, surround, DPL2)
 *--------------------------------------------------------------------------*/

#define AXFX_PRESET_REVERBSTD_EXP_OLD_TYPE1(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_30MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.03f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_OLD_AXFX;   \
        (fx)->fusedTime       = 1.5f;                                     \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.5f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.0f;                                     \
        (fx)->fusedGain       = 0.5f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_OLD_TYPE2(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_40MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.05f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_OLD_AXFX;   \
        (fx)->fusedTime       = 3.f;                                      \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.7f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.4f;                                     \
        (fx)->fusedGain       = 0.5f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_METAL_PIPE(fx)                          \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_5MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.00f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_METAL_TANK; \
        (fx)->fusedTime       = 0.5f;                                     \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.9f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.2f;                                     \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_METAL_TANK(fx)                          \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_35MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.03f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_METAL_TANK; \
        (fx)->fusedTime       = 3.f;                                      \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.8f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.25f;                                    \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_SMALL_ROOM(fx)                          \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_25MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.01f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_SMALL_ROOM; \
        (fx)->fusedTime       = 0.5f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_MEDIUM_ROOM(fx)                         \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_30MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.02f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_SMALL_ROOM; \
        (fx)->fusedTime       = 1.0f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_LARGE_ROOM(fx)                          \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_40MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.03f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_LARGE_ROOM; \
        (fx)->fusedTime       = 1.5f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.25f;                                    \
        (fx)->fusedGain       = 0.35f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_LONG_ROOM(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_20MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.02f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_LARGE_ROOM; \
        (fx)->fusedTime       = 2.0f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.45f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_SMALL_HALL(fx)                          \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_25MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.02f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_HALL;       \
        (fx)->fusedTime       = 2.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.35f;                                    \
        (fx)->fusedGain       = 0.45f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_LARGE_HALL(fx)                          \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_35MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.03f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_HALL;       \
        (fx)->fusedTime       = 3.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.3f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.45f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_CAVERNOUS(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_40MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.05f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_CAVERNOUS;  \
        (fx)->fusedTime       = 4.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.7f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.5f;                                     \
        (fx)->fusedGain       = 0.5f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBSTD_EXP_CATHEDRAL(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBSTD_EXP_EARLY_MODE_40MS;       \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.05f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBSTD_EXP_FUSED_MODE_CAVERNOUS;  \
        (fx)->fusedTime       = 5.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.3f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.35f;                                    \
        (fx)->fusedGain       = 0.50f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }


/*--------------------------------------------------------------------------*
  for high quality reverb (mono, stereo, surround, DPL2)
 *--------------------------------------------------------------------------*/

#define AXFX_PRESET_REVERBHI_EXP_OLD_TYPE1(fx)                            \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_30MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.03f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_OLD_AXFX;    \
        (fx)->fusedTime       = 1.5f;                                     \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.5f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.0f;                                     \
        (fx)->fusedGain       = 0.5f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_OLD_TYPE2(fx)                            \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_40MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.05f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_OLD_AXFX;    \
        (fx)->fusedTime       = 3.f;                                      \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.7f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.4f;                                     \
        (fx)->fusedGain       = 0.5f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_METAL_PIPE(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_5MS;         \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.00f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_METAL_TANK;  \
        (fx)->fusedTime       = 0.5f;                                     \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.9f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.2f;                                     \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_METAL_TANK(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_35MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.03f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_METAL_TANK;  \
        (fx)->fusedTime       = 3.f;                                      \
        (fx)->coloration      = 0.5f;                                     \
        (fx)->damping         = 0.8f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.25f;                                    \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_SMALL_ROOM(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_25MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.01f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_SMALL_ROOM;  \
        (fx)->fusedTime       = 0.5f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_MEDIUM_ROOM(fx)                          \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_30MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.02f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_SMALL_ROOM;  \
        (fx)->fusedTime       = 1.0f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.3f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_LARGE_ROOM(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_40MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.03f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_LARGE_ROOM;  \
        (fx)->fusedTime       = 1.5f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.25f;                                    \
        (fx)->fusedGain       = 0.35f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_LONG_ROOM(fx)                            \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_20MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.02f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_LARGE_ROOM;  \
        (fx)->fusedTime       = 2.0f;                                     \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.45f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_SMALL_HALL(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_25MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.01f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_HALL;        \
        (fx)->fusedTime       = 2.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.4f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.35f;                                    \
        (fx)->fusedGain       = 0.45f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_LARGE_HALL(fx)                           \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_35MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.02f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_HALL;        \
        (fx)->fusedTime       = 3.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.3f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.3f;                                     \
        (fx)->fusedGain       = 0.45f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_CAVERNOUS(fx)                            \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_40MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.05f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_CAVERNOUS;   \
        (fx)->fusedTime       = 4.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.7f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.5f;                                     \
        (fx)->fusedGain       = 0.5f;                                     \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }

#define AXFX_PRESET_REVERBHI_EXP_CATHEDRAL(fx)                            \
    {                                                                     \
        (fx)->earlyMode       = AXFX_REVERBHI_EXP_EARLY_MODE_40MS;        \
                                                                          \
        (fx)->preDelayTimeMax = 0.05f;                                    \
        (fx)->preDelayTime    = 0.05f;                                    \
                                                                          \
        (fx)->fusedMode       = AXFX_REVERBHI_EXP_FUSED_MODE_CAVERNOUS;   \
        (fx)->fusedTime       = 5.f;                                      \
        (fx)->coloration      = 0.6f;                                     \
        (fx)->damping         = 0.3f;                                     \
                                                                          \
        (fx)->crosstalk       = 0.1f;                                     \
                                                                          \
        (fx)->earlyGain       = 0.35f;                                    \
        (fx)->fusedGain       = 0.50f;                                    \
                                                                          \
        (fx)->busIn           = NULL;                                     \
        (fx)->busOut          = NULL;                                     \
        (fx)->outGain         = 1.0f;                                     \
        (fx)->sendGain        = 0.0f;                                     \
    }


/*--------------------------------------------------------------------------*
  for chorus (mono, stereo, surround, DPL2)
 *--------------------------------------------------------------------------*/

#define AXFX_PRESET_CHORUS_EXP_TYPE1(fx)            \
    {                                               \
        (fx)->delayTime      = 20.f;                \
        (fx)->depth          = 0.6f;                \
        (fx)->rate           = 0.1f;                \
        (fx)->feedback       = 0.0f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_TYPE2(fx)            \
    {                                               \
        (fx)->delayTime      = 30.f;                \
        (fx)->depth          = 0.3f;                \
        (fx)->rate           = 0.5f;                \
        (fx)->feedback       = 0.1f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_TYPE3(fx)            \
    {                                               \
        (fx)->delayTime      = 40.f;                \
        (fx)->depth          = 0.1f;                \
        (fx)->rate           = 1.5f;                \
        (fx)->feedback       = 0.0f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_FLANGER1(fx)         \
    {                                               \
        (fx)->delayTime      = 0.8f;                \
        (fx)->depth          = 0.9f;                \
        (fx)->rate           = 0.5f;                \
        (fx)->feedback       = 0.9f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_FLANGER2(fx)         \
    {                                               \
        (fx)->delayTime      = 0.3f;                \
        (fx)->depth          = 0.9f;                \
        (fx)->rate           = 2.0f;                \
        (fx)->feedback       = 0.8f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_ECHO1(fx)            \
    {                                               \
        (fx)->delayTime      = 30.f;                \
        (fx)->depth          = 0.1f;                \
        (fx)->rate           = 0.5f;                \
        (fx)->feedback       = 0.7f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_ECHO2(fx)            \
    {                                               \
        (fx)->delayTime      = 40.f;                \
        (fx)->depth          = 0.8f;                \
        (fx)->rate           = 1.0f;                \
        (fx)->feedback       = 0.8f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_WOBBLE1(fx)          \
    {                                               \
        (fx)->delayTime      = 40.f;                \
        (fx)->depth          = 0.8f;                \
        (fx)->rate           = 1.5f;                \
        (fx)->feedback       = 0.0f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }

#define AXFX_PRESET_CHORUS_EXP_WOBBLE2(fx)          \
    {                                               \
        (fx)->delayTime      = 50.f;                \
        (fx)->depth          = 1.f;                 \
        (fx)->rate           = 2.f;                 \
        (fx)->feedback       = 0.3f;                \
                                                    \
        (fx)->busIn          = NULL;                \
        (fx)->busOut         = NULL;                \
        (fx)->outGain        = 1.0f;                \
        (fx)->sendGain       = 0.0f;                \
    }


#ifdef __cplusplus
}
#endif

#endif  // __AXFX_PRESETS_H__
