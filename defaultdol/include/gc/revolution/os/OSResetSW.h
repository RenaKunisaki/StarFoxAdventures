/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Reset button API
  File:     OSResetSW.h

  Copyright 2000, 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSResetSW.h,v $
  Revision 1.3  2006/07/21 12:57:16  hirose
  Added OSSetPowerCallback().

  Revision 1.2  2006/02/04 11:56:48  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2001/08/27 10:34 Shiki
    Added OSGetResetButtonState().

    1     2000/06/14 11:51p Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSRESETSW_H__
#define __OSRESETSW_H__

#include <revolution/types.h>
#include <revolution/os/OSContext.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSResetCallback)(void);
typedef void (*OSPowerCallback)(void);

BOOL            OSGetResetButtonState( void );

OSResetCallback OSSetResetCallback   ( OSResetCallback callback );
OSPowerCallback OSSetPowerCallback   ( OSPowerCallback callback );

// obsolete
BOOL            OSGetResetSwitchState( void );

#ifdef __cplusplus
}
#endif

#endif  // __OSRESETSW_H__
