/*---------------------------------------------------------------------------*
  Project:  Gamecube controller(PAD) library
  File:     pad.h

  Copyright (C) 1998-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: pad.h,v $
  Revision 1.6  09/06/2006 14:53:27  yasuh-to
  (none)

  Revision 1.5  09/06/2006 14:34:08  yasuh-to
  Modified Clamp2 APIs.
  Clamp2 uses new paramaters these are more widly than GC's.

  Revision 1.4  08/25/2006 11:28:58  yasuh-to
  Added new clamp API.

  Revision 1.3  02/06/2006 01:06:15  yasuh-to
  Restored the APIs

    
    33    1/09/04 10:52 Shiki
    Added PADIsBarrel().

    32    7/22/03 13:22 Shiki
    Declared __PADDisableRumble().

    30    8/22/02 11:58 Shiki
    Clean up.

    29    8/20/02 10:28 Shiki
    Added PADClampCircle().

    28    8/19/02 21:42 Shiki
    Set #pragma warn_padding off around the PADStatus{} definition.

    27    8/12/02 15:27 Shiki
    Added __padding field to PADStatus{}.

    26    8/05/02 18:11 Shiki
    Added const keyword to the PADControlAllMotors() function prototype.

    25    10/29/01 11:57 Shiki
    Modified __PADDisableRecalibration() to make it return the previous
    state.

    24    10/12/01 16:29 Shiki
    Modified PADSetSamplingCallback() to make it return the previous
    callback pointer.

    23    9/05/01 16:25 Shiki
    Added __PADDisableRecalibration() and __PADDisableXPatch().

    22    9/03/01 14:46 Shiki
    Added PADSetSamplingCallback().

    21    7/30/01 11:01 Shiki
    Modified PADRead() to return OR-ed bit mask of controllers that support
    rumble motors.

    20    01/03/21 18:55 Shiki
    Fixed not to define PAD_USESPEC for EMU builds.

    19    01/03/21 18:32 Shiki
    Added PADGetSpec().

    18    01/03/21 17:22 Shiki
    Added PADSetAnalogMode().

    17    01/01/30 14:17 Shiki
    Removed __PADWirelessIDCheck().

    16    11/20/00 6:03p Shiki
    Added more PAD_SPEC_*.

    15    10/17/00 10:14a Hashida
    From IRD: Added PADRecalibrate and PADSync

    14    10/02/00 10:18a Hashida
    From Shiki: Added PAD_WIRELESS_FIX_ID define name.

    13    9/05/00 9:12p Shiki
    Added PAD_TYPE_* define names.

    12    9/05/00 9:04p Shiki
    Added PADGetType().

    11    8/29/00 10:18p Shiki
    Added __PADWirelessIDCheck().

    10    8/24/00 7:32p Shiki
    Revised PAD_BUTTON/TRIGGER_* define names for the final controller.

    9     3/23/00 4:34p Shiki
    Clean up.

    8     3/23/00 4:31p Shiki
    Added PADSetSpec().

    7     3/22/00 8:11p Shiki
    Revised PAD_TRIGGER_* defines for the final controller IC.

    6     3/10/00 2:56p Shiki
    Revised motor control functions.

    5     3/09/00 8:28p Shiki
    Added PADControlMotor().

    4     2/24/00 7:48p Shiki
    Added PAD_TRIGGER_*.

    3     2/24/00 5:49p Shiki
    Updated to the final spec.

    2     12/15/99 8:25p Shiki
    Added substickX/Y into PADStatus.

    10    9/23/99 4:55p Shiki
    Renamed 'errno' of PADStatus to 'err'.

    9     9/13/99 6:26p Shiki
    Inserted (u16) casts in PADButtonUp/Down macros.

    7     5/11/99 4:42p Shiki
    Refreshed include tree.

    5     5/06/99 8:18p Shiki
    Renamed PADNormalize() to PADClamp()

    4     5/06/99 6:12p Shiki
    Added PADNormalize().

    3     5/05/99 5:42p Shiki
    Added PADButtonDown and PADButtonUp macros.

    2     5/04/99 8:46p Shiki
    Fixed comment.

    1     5/04/99 8:32p Shiki

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __PAD_H__
#define __PAD_H__

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

// CAUTION: Define PAD_USESPEC as long as bread boards or prototype
//          controllers (including DS4) are in use.
#define PAD_USESPEC

#ifdef  __MWERKS__
#pragma warn_padding off
#endif

typedef struct PADStatus
{
    u16 button;                 // Or-ed PAD_BUTTON_* bits
    s8  stickX;                 // -128 <= stickX       <= 127
    s8  stickY;                 // -128 <= stickY       <= 127
    s8  substickX;              // -128 <= substickX    <= 127
    s8  substickY;              // -128 <= substickY    <= 127
    u8  triggerLeft;            //    0 <= triggerLeft  <= 255
    u8  triggerRight;           //    0 <= triggerRight <= 255
    u8  analogA;                //    0 <= analogA      <= 255
    u8  analogB;                //    0 <= analogB      <= 255
    s8  err;                    // one of PAD_ERR_* number
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

// clamp algorithm for stick
#define PAD_STICK_CLAMP_OCTA_WITH_MARGIN         0
#define PAD_STICK_CLAMP_OCTA_WITHOUT_MARGIN      1
#define PAD_STICK_CLAMP_CIRCLE_WITH_MARGIN       2
#define PAD_STICK_CLAMP_CIRCLE_WITHOUT_MARGIN    3
// clamp algorithm for trigger
#define PAD_TRIGGER_FIXED_BASE       0
#define PAD_TRIGGER_INDIVIDUAL_BASE  1


BOOL PADInit             ( void );
BOOL PADReset            ( u32 mask );
u32  PADRead             ( PADStatus* status );
void PADSetSamplingRate  ( u32 msec );
void PADClamp            ( PADStatus* status );
void PADClampCircle      ( PADStatus* status );
void PADClamp2           ( PADStatus* status, u32 type );
void PADClampCircle2     ( PADStatus* status, u32 type );
void PADClampTrigger     ( PADStatus* status, u32 type );
void PADControlAllMotors ( const u32* command );
void PADControlMotor     ( int chan, u32 command );
BOOL PADRecalibrate      ( u32 mask );
BOOL PADSync             ( void );
void PADSetAnalogMode    ( u32 mode );
BOOL PADIsBarrel         ( s32 chan );

typedef void (* PADSamplingCallback )(void);

PADSamplingCallback PADSetSamplingCallback( PADSamplingCallback callback );

// obsolete. Use SIProbe() instead.
BOOL PADGetType          ( s32 chan, u32* type );

#ifdef  PAD_USESPEC
void PADSetSpec          ( u32 model );
u32  PADGetSpec          ( void );
#endif  // PAD_USESPEC

#ifdef  _DEBUG
void __PADTestSamplingRate( u32 tvmode );
#endif  // _DEBUG

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
