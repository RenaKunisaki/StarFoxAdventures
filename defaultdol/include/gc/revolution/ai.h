/*---------------------------------------------------------------------------*
  Project:  Revolution OS - Audio Interface (AI) Device driver and API
  File:     ai.h

  Copyright (C)1999-2006 Nintendo  All Rights Reserved.
 
  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ai.h,v $
  Revision 1.4  02/09/2006 04:41:11  aka
  Changed copyright.

  Revision 1.3  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.2  01/11/2006 06:07:37  aka
  Removed HW audio streaming functions.

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Imported from Dolphin tree.
    
    10     5/01/01 6:44p Eugene
    Added XXXReset() functions to clear respective init flags. 
    All XXXInit() functions modified to return safely if respective init
    flags are asserted.
    
    9     4/30/01 6:38p Eugene
    removed AISetStreamSampleRate() from public view. Added AICheckInit().
    
    8     3/25/01 9:38p Eugene
    Added AIGetDMAStartAddr() and AIGetDMALength() to retrieve most recent
    parameters programmed into AI-FIFO DMA engine.
    
    7     12/08/00 10:51p Eugene
    Callback registration functions now return old callback (if any).
    
    6     11/23/00 8:05p Eugene
    Added support for RevB features (hardware SRC for DSP). 
    Specifically, AIGetDSPSampleRate() and AISetDSPSampleRate(). 
    These functions exist for RevA/HW1 builds, but don't do anything.
    
    5     5/27/00 3:55a Eugene
    
    4     5/01/00 6:23p Eugene
    Added prototype for AIInitDMA(). 
    
    3     4/11/00 5:33p Eugene
    
    2     3/08/00 2:54p Tian
    Added stack argument to AIInit
    
    1     3/07/00 7:08p Eugene
    Public header file for AI driver/API.
    
  $NoKeywords: $
 
 *---------------------------------------------------------------------------*
 
  Description
  -----------
  This is the public API definition file for the Audio Interface device 
  driver. Applications or OS components which need access to the AI 
  hardware must include this file.
 
 *---------------------------------------------------------------------------*/
  
#ifndef __AI_H__
#define __AI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
 * Includes
 *---------------------------------------------------------------------------*/
#include <revolution/types.h>

/*---------------------------------------------------------------------------*
 * Definitions
 *---------------------------------------------------------------------------*/
#define AI_SAMPLERATE_48KHZ  0x00000001   // SRC sample rates for DSP
#define AI_SAMPLERATE_32KHZ  0x00000000

/*---------------------------------------------------------------------------*
 * Types/Declarations
 *---------------------------------------------------------------------------*/
typedef void (*AIDCallback)(void);
 
/*---------------------------------------------------------------------------*
 * Globals
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * Function Prototypes
 *---------------------------------------------------------------------------*/

// AI-FIFO DMA ---------------------------------------------------------------
AIDCallback AIRegisterDMACallback       (AIDCallback callback);

void        AIInitDMA                   (u32 start_addr, u32 length);
BOOL        AIGetDMAEnableFlag          (void);
void        AIStartDMA                  (void);
void        AIStopDMA                   (void);
u32         AIGetDMABytesLeft           (void);

u32         AIGetDMAStartAddr           (void);
u32         AIGetDMALength              (void);

u32         AIGetDSPSampleRate          (void);
void        AISetDSPSampleRate          (u32 rate);

// General/initialization ------------------------------------------------------
void        AIInit                      (u8*  stack);
BOOL        AICheckInit                 (void);
void        AIReset                     (void);

#ifdef __cplusplus
}
#endif

#endif // __AI_H__
