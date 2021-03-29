/*---------------------------------------------------------------------------*
  Project:  Dolphin VI library
  File:     vi.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: vi.h,v $
  Revision 1.3  2006/02/25 09:59:26  urata
  Added "vi3in1.h" and "vi3in1types.h".

  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     2000/01/26 3:51p Hashida
    Divided into two header files, one if for define's and the other for
    function prototypes.
    
    15    1999/11/17 5:04p Hashida
    Added a debug mode.
    
    14    1999/11/17 3:30p Hashida
    Added a debug mode.
    
    13    1999/11/12 5:21p Hashida
    Added VIGetCurrentLine().
    
    12    1999/10/06 11:02a Hashida
    Removed the old enum and prototypes.
    
    11    1999/09/29 6:17p Hashida
    Changed the definition of VI_TVMODE_* so that it can be used easily.
    
    10    1999/09/16 11:09a Hashida
    Removed VISetRetraceCount stub.
    
    9     1999/09/15 9:06p Hashida
    Changed a function name (VIGetCurrentFieldCount -> VIGetRetraceCount).
    Added VIGetNextField.
    
    8     1999/09/13 3:21p Hashida
    Added VISetBlack and VIGetCurrentFieldCount
    
    7     1999/09/10 11:20a Hashida
    Added VIPadFrameBufferWidth, VIConfigurePan, VISetNextFrameBuffer and
    VISetRetraceCount
    
    6     1999/09/10 10:26a Hashida
    Changed VIDispMode to VITVMode
    
    5     1999/09/09 11:04p Hashida
    Added VIConfigureTVScreen and VIConfigureXFrameBuffer.
    
    4     1999/07/20 2:14p Rod
    Added an enum value for the VI_COUNT to make the app run a max frame
    rate ( no clamping )
    
    3     1999/07/20 2:11p Rod
    Added a function ( VISetCount) to clamp the refresh rate to a set
    maximum
    
    2     1999/06/07 4:34p Alligator
    Changed fn name to VISetFrameBuffer
    
    1     1999/06/01 2:04p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __VI_H
#define __VI_H

#include <revolution/vi/vitypes.h>
#include <revolution/vi/vifuncs.h>
#include <revolution/vi/vi3in1.h>
#include <revolution/vi/vi3in1types.h>

#endif
