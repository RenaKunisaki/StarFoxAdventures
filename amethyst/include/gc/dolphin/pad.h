/*---------------------------------------------------------------------------*
  Project:  Dolphin OS
  File:     pad.h

  Copyright 1998-2004 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: pad.h,v $
  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    33    2004/01/09 10:52 Shiki
    Added PADIsBarrel().

    32    2003/07/22 13:22 Shiki
    Declared __PADDisableRumble().

    30    2002/08/22 11:58 Shiki
    Clean up.

    29    2002/08/20 10:28 Shiki
    Added PADClampCircle().

    28    2002/08/19 21:42 Shiki
    Set #pragma warn_padding off around the PADStatus{} definition.

    27    2002/08/12 15:27 Shiki
    Added __padding field to PADStatus{}.

    26    2002/08/05 18:11 Shiki
    Added const keyword to the PADControlAllMotors() function prototype.

    25    2001/10/29 11:57 Shiki
    Modified __PADDisableRecalibration() to make it return the previous
    state.

    24    2001/10/12 16:29 Shiki
    Modified PADSetSamplingCallback() to make it return the previous
    callback pointer.

    23    2001/09/05 16:25 Shiki
    Added __PADDisableRecalibration() and __PADDisableXPatch().

    22    2001/09/03 14:46 Shiki
    Added PADSetSamplingCallback().

    21    2001/07/30 11:01 Shiki
    Modified PADRead() to return OR-ed bit mask of controllers that support
    rumble motors.

    20    2001/03/21 18:55 Shiki
    Fixed not to define PAD_USESPEC for EMU builds.

    19    2001/03/21 18:32 Shiki
    Added PADGetSpec().

    18    2001/03/21 17:22 Shiki
    Added PADSetAnalogMode().

    17    2001/01/30 14:17 Shiki
    Removed __PADWirelessIDCheck().

    16    2000/11/20 6:03p Shiki
    Added more PAD_SPEC_*.

    15    2000/10/17 10:14a Hashida
    From IRD: Added PADRecalibrate and PADSync

    14    2000/10/02 10:18a Hashida
    From Shiki: Added PAD_WIRELESS_FIX_ID define name.

    13    2000/09/05 9:12p Shiki
    Added PAD_TYPE_* define names.

    12    2000/09/05 9:04p Shiki
    Added PADGetType().

    11    2000/08/29 10:18p Shiki
    Added __PADWirelessIDCheck().

    10    2000/08/24 7:32p Shiki
    Revised PAD_BUTTON/TRIGGER_* define names for the final controller.

    9     2000/03/23 4:34p Shiki
    Clean up.

    8     2000/03/23 4:31p Shiki
    Added PADSetSpec().

    7     2000/03/22 8:11p Shiki
    Revised PAD_TRIGGER_* defines for the final controller IC.

    6     2000/03/10 2:56p Shiki
    Revised motor control functions.

    5     2000/03/09 8:28p Shiki
    Added PADControlMotor().

    4     2000/02/24 7:48p Shiki
    Added PAD_TRIGGER_*.

    3     2000/02/24 5:49p Shiki
    Updated to the final spec.

    2     1999/12/15 8:25p Shiki
    Added substickX/Y into PADStatus.

    10    1999/09/23 4:55p Shiki
    Renamed 'errno' of PADStatus to 'err'.

    9     1999/09/13 6:26p Shiki
    Inserted (u16) casts in PADButtonUp/Down macros.

    7     1999/05/11 4:42p Shiki
    Refreshed include tree.

    5     1999/05/06 8:18p Shiki
    Renamed PADNormalize() to PADClamp()

    4     1999/05/06 6:12p Shiki
    Added PADNormalize().

    3     1999/05/05 5:42p Shiki
    Added PADButtonDown and PADButtonUp macros.

    2     1999/05/04 8:46p Shiki
    Fixed comment.

    1     1999/05/04 8:32p Shiki

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __PAD_H__
#define __PAD_H__

#include <dolphin/types.h>
#include <dolphin/os.h>

#ifdef __cplusplus
extern "C" {
#endif

// CAUTION: Define PAD_USESPEC as long as bread boards or prototype
//          controllers (including DS4) are in use.
#ifndef EMU
#define PAD_USESPEC
#endif  // EMU

#ifdef  __MWERKS__
#pragma warn_padding off
#endif

typedef struct PADStatus
{
    u16 button;                 // Or-ed PAD_BUTTON_* bits
    s8  stickX;                 // -128 <= stickX    <= 127
    s8  stickY;                 // -128 <= stickY    <= 127
    s8  substickX;              // -128 <= substickX    <= 127
    s8  substickY;              // -128 <= substickY    <= 127
    u8  triggerLeft;            //    0 <= triggerLeft  <= 255
    u8  triggerRight;           //    0 <= triggerRight <= 255
    u8  analogA;                //    0 <= analogA      <= 255
    u8  analogB;                //    0 <= analogB      <= 255
    s8  err;                    // One of PAD_ERR_* number
} PADStatus;

#ifdef  __MWERKS__
#pragma warn_padding reset
#endif

#define PAD_MAX_CONTROLLERS     4

#define PAD_BUTTON_LEFT         0x0001
#define PAD_BUTTON_RIGHT        0x0002
#define PAD_BUTTON_DOWN         0x0004
#define PAD_BUTTON_UP           0x0008
#define PAD_TRIGGER_Z           0x0010
#define PAD_TRIGGER_R           0x0020
#define PAD_TRIGGER_L           0x0040
#define PAD_BUTTON_A            0x0100
#define PAD_BUTTON_B            0x0200
#define PAD_BUTTON_X            0x0400
#define PAD_BUTTON_Y            0x0800
#define PAD_BUTTON_MENU         0x1000
#define PAD_BUTTON_START        0x1000

#define PAD_CHAN0               0
#define PAD_CHAN1               1
#define PAD_CHAN2               2
#define PAD_CHAN3               3

#define PAD_CHAN0_BIT           0x80000000
#define PAD_CHAN1_BIT           0x40000000
#define PAD_CHAN2_BIT           0x20000000
#define PAD_CHAN3_BIT           0x10000000

#define PAD_ERR_NONE            0
#define PAD_ERR_NO_CONTROLLER   -1
#define PAD_ERR_NOT_READY       -2
#define PAD_ERR_TRANSFER        -3

#define PAD_MOTOR_STOP          0
#define PAD_MOTOR_RUMBLE        1
#define PAD_MOTOR_STOP_HARD     2

#ifdef  PAD_USESPEC
#define PAD_SPEC_0              0       // bread board    (ver 0.x)
#define PAD_SPEC_1              1       // 1st prototypes (ver 1.0)
#define PAD_SPEC_2              2       // 2nd prototypes (ver 3.0 FPGA)
#define PAD_SPEC_3              3       // DS3
#define PAD_SPEC_4              4       // DS4
#define PAD_SPEC_5              5       // DS5 (production version)
#endif  // PAD_USESPEC

// PADSetAnalogMode() param
#define PAD_MODE_0              0
#define PAD_MODE_1              1
#define PAD_MODE_2              2
#define PAD_MODE_3              3
#define PAD_MODE_4              4
#define PAD_MODE_5              5
#define PAD_MODE_6              6
#define PAD_MODE_7              7

#define PADButtonDown(buttonLast, button)   \
    ((u16) (((buttonLast) ^ (button)) & (button)))

#define PADButtonUp(buttonLast, button)     \
    ((u16) (((buttonLast) ^ (button)) & (buttonLast)))

#define PADStartMotor(chan)     PADControlMotor((chan), PAD_MOTOR_RUMBLE)
#define PADStopMotorHard(chan)  PADControlMotor((chan), PAD_MOTOR_STOP_HARD)
#define PADStopMotor(chan)      PADControlMotor((chan), PAD_MOTOR_STOP)

BOOL PADInit             ( void );
BOOL PADReset            ( u32 mask );
u32  PADRead             ( PADStatus* status );
void PADSetSamplingRate  ( u32 msec );
void PADClamp            ( PADStatus* status );
void PADClampCircle      ( PADStatus* status );
void PADControlAllMotors ( const u32* command );
void PADControlMotor     ( int chan, u32 command );
BOOL PADRecalibrate      ( u32 mask );
BOOL PADSync             ( void );
void PADSetAnalogMode    ( u32 mode );
BOOL PADIsBarrel         ( s32 chan );

typedef void (* PADSamplingCallback )(void);

PADSamplingCallback PADSetSamplingCallback( PADSamplingCallback callback );

// Obsolete. Use SIProbe() instead.
BOOL PADGetType          ( s32 chan, u32* type );

#ifdef  PAD_USESPEC
void PADSetSpec          ( u32 model );
u32  PADGetSpec          ( void );
#endif  // PAD_USESPEC

#ifdef  _DEBUG
void __PADTestSamplingRate( u32 tvmode );
#endif  // _DEBUG

#ifdef  MACOS
void PADConfigure( void );
#endif  // MACOS

//
// System internal use only (do not use/touch)
//

BOOL __PADDisableRecalibration( BOOL disable );
BOOL __PADDisableRumble       ( BOOL disable );
void __PADDisableXPatch       ( void );

extern u32 __PADFixBits;

//
//
//

#ifdef __cplusplus
}
#endif

#endif  // __PAD_H__
