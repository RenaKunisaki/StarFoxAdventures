/*---------------------------------------------------------------------------*
  Project:     WPAD library
  File:        wpad.h
  Programmers: TOKUNAGA, Yasuhiro
               HIRATSU Daisuke
               FUKUDA Takahiro
               TOJO Haruki

  Copyright (C) 2005-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wpad.h,v $
  Revision 1.70.2.1  2007/12/10 07:25:08  tojo
  Made the following functions privated.
   - WPADSaveConfig
   - WPADEnableMotor
   - WPADSetSpeakerVolume

  Revision 1.70  2007/07/10 08:10:28  tojo
  Removed WPAD_SYNC_EVT_BUSY.

  Revision 1.69  2007/05/03 08:22:24  tojo
  (none)

  Revision 1.68  2007/04/12 07:59:42  tojo
  (none)

  Revision 1.67  2007/04/11 00:59:29  tojo
  (none)

  Revision 1.66  2007/04/10 10:14:08  tojo
  (none)

  Revision 1.65  2007/04/02 09:54:56  tojo
  Added attaching/detaching dummy extension only for debug use.

  Revision 1.64  2007/03/19 12:48:10  tojo
  (none)

  Revision 1.63  2006/11/29 10:27:29  tojo
  Implemented WPADControlExtGimmick.

  Revision 1.62  2006/11/29 10:14:14  tojo
  (none)

  Revision 1.60  2006/10/20 01:56:44  tojo
  Added WPADGetDpdFormat().

  Revision 1.59  2006/10/18 09:55:07  tojo
  (none)

  Revision 1.58  2006/10/11 17:08:04  tojo
  Added busy state.

  Revision 1.57  2006/09/27 01:09:15  tojo
  Changed api.

  Revision 1.56  2006/09/23 06:01:23  tojo
  Changed API names.

  Revision 1.55  2006/09/22 02:49:22  tojo
  Added WPAD_ERR_CORRUPTED.

  Revision 1.54  2006/09/21 05:47:50  tojo
  Added WPAD_ERR_NOPERM and WPAD_ERR_BROKEN

  Revision 1.53  2006/09/15 13:52:03  tojo
  Added fast sync apis and check audio streaming can

  Revision 1.52  2006/09/06 03:12:34  tojo
  Added some macros.

  Revision 1.51  2006/09/06 02:40:45  tojo
  Implemented to control connectability.

  Revision 1.50  2006/09/04 01:54:20  tojo
  Added WPADSetDpdSensitivity.

  Revision 1.49  2006/09/02 02:06:50  tojo
  (none)

  Revision 1.48  2006/08/29 01:42:05  tojo
  Added WPAD_DEV_FUTURE

  Revision 1.47  2006/08/28 14:32:18  tojo
  Added WPAD_DEV_NOT_SUPPORTED

  Revision 1.46  2006/08/28 13:07:25  tojo
  Made WPADGetDpdSensitivity public.

  Revision 1.45  2006/08/16 10:55:28  mitu
  deleted WPAD_MEM_FACEDATA_LEN.

  Revision 1.44  2006/08/16 09:37:36  tojo
  (none)

  Revision 1.43  2006/08/15 01:00:08  tojo
  (none)

  Revision 1.42  2006/08/14 17:45:02  tojo
  (none)

  Revision 1.41  2006/08/14 16:58:05  tojo
  Modified WPADMEM API.

  Revision 1.40  2006/08/14 07:49:11  tojo
  (none)

  Revision 1.39  2006/08/11 10:19:23  yasumoto_yoshitaka
  surpress padding warning

  Revision 1.38  2006/08/11 07:53:55  tojo
  Added embedded memory apis.

  Revision 1.37  2006/08/10 01:18:42  tojo
  Added dpd full mode.
  Changed system config apis.

  Revision 1.36  2006/08/07 00:18:16  tojo
  (none)

  Revision 1.35  2006/08/03 13:09:34  tojo
  Moved some config api to privete apis.

  Revision 1.34  2006/08/03 11:54:45  tojo
  Changed clamp apis.

  Revision 1.33  2006/08/01 06:03:18  tojo
  (none)

  Revision 1.32  2006/07/20 01:18:12  tojo
  Added WPADIsMotorEnabled.

  Revision 1.31  2006/07/19 08:14:43  tojo
  Changed argument type u8 to u16 at WPADSetDisableChannel

  Revision 1.30  2006/07/18 10:04:49  tojo
  Added WPAD_STRM_INTERVAL
  Changed WPAD_DISABLED_WIFI_*

  Revision 1.29  2006/07/04 08:16:52  tojo
  Added speaker/config/motor functions

  Revision 1.28  2006/06/30 08:59:19  tojo
  Added WPADGetWorkMemorySize().

  Revision 1.27  2006/06/17 07:00:55  tojo
  (none)

  Revision 1.26  2006/06/17 05:19:13  tojo
  Added WPAD_ADD_LEN
  Removed WPAD_ACC_1G_VALUE, WPAD_FS_ACC_1G_VALUE

  Revision 1.25  2006/06/16 04:30:50  tojo
  Added WPAD_CLEAR_EVT_BUSY.
  Changed WPAD_CL_STICK_RESO.

  Revision 1.24  2006/06/15 07:04:03  tojo
  Changed API of WPADGetAccGravityUnit.

  Revision 1.22  2006/06/15 05:46:05  tojo
  (none)

  Revision 1.21  2006/06/15 05:20:33  tojo
  Added WPADGetAccGravityUnit.
  Removed WPADGetOrigin.
  Resumed WPADRecalibrate for backward compatibility.

  Revision 1.20  2006/06/14 11:31:14  tojo
  Added WPADGetRegisteredDevNum().

  Revision 1.19  2006/06/14 10:49:05  tojo
  Fixed typo : Extention -> Extension.
  Removed WPADIsEmulator, WPADRecalibrate.
  Added WPADIsSpeakerEnabled.

  Revision 1.18  2006/06/13 14:21:54  tojo
  Added some macros.

  Revision 1.17  2006/06/13 11:45:14  tojo
  Added delete all link keys handler.

  Revision 1.16  2006/06/13 06:29:55  tojo
  (none)

  Revision 1.15  2006/06/13 06:05:34  tojo
  Added
   WPADSetExtentionCallback
   WPADSetConnectCallback
   WPADIsEmulator

  Revision 1.14  2006/06/12 11:42:18  ekwon
  New API to query enable/disable state of DPD.

  Revision 1.13  2006/06/12 06:05:44  tojo
  (none)

  Revision 1.12  2006/06/12 00:07:39  tojo
  Changed some API
  Defined classic controller button mappings

  Revision 1.11  2006/06/05 11:47:43  tojo
  Defined WPADOrigin.
  Added WPADGetOrigin.

  Revision 1.10  2006/06/03 10:00:50  tojo
  Switched WPAD_BUTTON_C and WPAD_BUTTON_Z

  Revision 1.9  2006/06/01 08:53:36  tojo
  Changed the declaration of WPADSyncCallback

  Revision 1.8  2006/05/25 13:00:33  tojo
  wrapped wud api

  Revision 1.7  2006/05/25 12:21:04  tojo
  added WPADRegisterAllocator(),

  Revision 1.6  2006/05/23 05:48:26  tojo
  added WPAD_DPD_* and WPAD_SPEAKER_*

  Revision 1.5  2006/05/17 12:37:30  ekwon
  Added NearEmpty field to WPADInfo.

  Revision 1.4  2006/05/17 04:59:33  kawaset
  Added definitions for production version controllers. Use preprocessor macro WPADEMU for backwards compatibility.

  Revision 1.3  2006/02/07 11:44:09  yasuh-to
  Deleted structure for Classic Contoroller. CL is not release at SDK1.0.

 *---------------------------------------------------------------------------*/

#ifndef __WPAD_H__
#define __WPAD_H__

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WPAD_CHAN0                    0
#define WPAD_CHAN1                    1
#define WPAD_CHAN2                    2
#define WPAD_CHAN3                    3

#define WPAD_MAX_CONTROLLERS          4

#define WPAD_MOTOR_STOP               0
#define WPAD_MOTOR_RUMBLE             1

#ifndef WPADEMU
// Under construction. These definitions are subject to change.

#define WPAD_BUTTON_LEFT              0x0001
#define WPAD_BUTTON_RIGHT             0x0002
#define WPAD_BUTTON_DOWN              0x0004
#define WPAD_BUTTON_UP                0x0008
#define WPAD_BUTTON_PLUS              0x0010
#define WPAD_BUTTON_2                 0x0100
#define WPAD_BUTTON_1                 0x0200
#define WPAD_BUTTON_B                 0x0400
#define WPAD_BUTTON_A                 0x0800
#define WPAD_BUTTON_MINUS             0x1000
#define WPAD_BUTTON_HOME              0x8000

#define WPAD_BUTTON_Z                 0x2000
#define WPAD_BUTTON_C                 0x4000

// backwards compatibility
#define WPAD_BUTTON_START             WPAD_BUTTON_PLUS
#define WPAD_BUTTON_SELECT            WPAD_BUTTON_MINUS
#define WPAD_BUTTON_SMALL_A           WPAD_BUTTON_1
#define WPAD_BUTTON_SMALL_B           WPAD_BUTTON_2

// for classic controller extension
#define WPAD_CL_BUTTON_UP             0x0001
#define WPAD_CL_BUTTON_LEFT           0x0002
#define WPAD_CL_TRIGGER_ZR            0x0004
#define WPAD_CL_BUTTON_X              0x0008
#define WPAD_CL_BUTTON_A              0x0010
#define WPAD_CL_BUTTON_Y              0x0020
#define WPAD_CL_BUTTON_B              0x0040
#define WPAD_CL_TRIGGER_ZL            0x0080
#define WPAD_CL_RESERVED              0x0100
#define WPAD_CL_TRIGGER_R             0x0200
#define WPAD_CL_BUTTON_PLUS           0x0400
#define WPAD_CL_BUTTON_HOME           0x0800
#define WPAD_CL_BUTTON_MINUS          0x1000
#define WPAD_CL_TRIGGER_L             0x2000
#define WPAD_CL_BUTTON_DOWN           0x4000
#define WPAD_CL_BUTTON_RIGHT          0x8000


#define WPAD_DPD_OFF                  0
#define WPAD_DPD_STD                  1
#define WPAD_DPD_EXP                  3
#define WPAD_DPD_FULL                 5

#define WPAD_DPD_MAX_OBJECTS          4
#define WPAD_DPD_ANGLE               42.0f  // viewing angle
#define WPAD_DPD_IMG_RESO_WX       1024     // DPD image resolution 
#define WPAD_DPD_IMG_RESO_WY        768

#define WPAD_SPEAKER_OFF              0
#define WPAD_SPEAKER_ON               1
#define WPAD_SPEAKER_MUTE             2
#define WPAD_SPEAKER_MUTE_OFF         3
#define WPAD_SPEAKER_PLAY             4
#define WPAD_SPEAKER_RESET            5

#define WPAD_EXTGMK_OFF               0
#define WPAD_EXTGMK_ON                1

#define WPAD_ACC_RESO              1024     // accelerometer resolution
#define WPAD_ACC_MAX                  3.4f  // valid gravity = 3.4G

#define WPAD_FS_STICK_RESO          256     // stick resolution of FreeStyle unit
#define WPAD_FS_ACC_RESO           1024     // accelerometer resolution of FreeStyle unit
#define WPAD_FS_ACC_MAX               2.1f  // valid gravity = 2.1G

#define WPAD_CL_STICK_RESO         1024
#define WPAD_CL_TRIGGER_RESO        256

// device type
#define WPAD_DEV_CORE                 0
#define WPAD_DEV_FREESTYLE            1
#define WPAD_DEV_CLASSIC              2
#define WPAD_DEV_FUTURE             251
#define WPAD_DEV_NOT_SUPPORTED      252
#define WPAD_DEV_NOT_FOUND          253
#define WPAD_DEV_UNKNOWN            255

// data format of Core
#define WPAD_FMT_CORE                 0     // only buttons
#define WPAD_FMT_CORE_ACC             1     // buttons, motion sensor
#define WPAD_FMT_CORE_ACC_DPD         2     // buttons, motion sensor, pointing
// data format of Core + Nunchaku
#define WPAD_FMT_FREESTYLE            3     // buttons, nunchaku
#define WPAD_FMT_FREESTYLE_ACC        4     // buttons, motion sensor, nunchaku
#define WPAD_FMT_FREESTYLE_ACC_DPD    5     // buttons, motion sensor, pointing, nunchaku
// data format of Core + Classic Controller
#define WPAD_FMT_CLASSIC              6     // buttons, classic
#define WPAD_FMT_CLASSIC_ACC          7     // buttons, motion sensor, classic
#define WPAD_FMT_CLASSIC_ACC_DPD      8     // buttons, motion sensor, pointing, classic
// data format of Core + DPD full mode
#define WPAD_FMT_CORE_ACC_DPD_FULL    9     // buttons, motion sensor, pointing

#define WPAD_STATE_DISABLED           0
#define WPAD_STATE_ENABLING           1
#define WPAD_STATE_ENABLED            2
#define WPAD_STATE_SETUP              3
#define WPAD_STATE_DISABLING          4

// disable channel to avoid WiFi interference.
#define WPAD_DISABLE_CLEAR            0x0000    // enable all range
#define WPAD_DISABLE_WIFI_CHAN_1      0x0001
#define WPAD_DISABLE_WIFI_CHAN_2      0x0002
#define WPAD_DISABLE_WIFI_CHAN_3      0x0004
#define WPAD_DISABLE_WIFI_CHAN_4      0x0008
#define WPAD_DISABLE_WIFI_CHAN_5      0x0010
#define WPAD_DISABLE_WIFI_CHAN_6      0x0020
#define WPAD_DISABLE_WIFI_CHAN_7      0x0040
#define WPAD_DISABLE_WIFI_CHAN_8      0x0080
#define WPAD_DISABLE_WIFI_CHAN_9      0x0100
#define WPAD_DISABLE_WIFI_CHAN_10     0x0200
#define WPAD_DISABLE_WIFI_CHAN_11     0x0400
#define WPAD_DISABLE_WIFI_CHAN_12     0x0800
#define WPAD_DISABLE_WIFI_CHAN_13     0x1000
#define WPAD_DISABLE_WIFI_CHAN_14     0x2000

// battery level
#define WPAD_BATTERY_LEVEL_CRITICAL   0
#define WPAD_BATTERY_LEVEL_LOW        1
#define WPAD_BATTERY_LEVEL_MEDIUM     2
#define WPAD_BATTERY_LEVEL_HIGH       3
#define WPAD_BATTERY_LEVEL_MAX        4

#define WPAD_LED_CHAN_1             0x1
#define WPAD_LED_CHAN_2             0x2
#define WPAD_LED_CHAN_3             0x4
#define WPAD_LED_CHAN_4             0x8

#define WPAD_SYNC_EVT_START           0
#define WPAD_SYNC_EVT_DONE            1

#define WPAD_CLEAR_EVT_BUSY          -1
#define WPAD_CLEAR_EVT_START          0
#define WPAD_CLEAR_EVT_DONE           1

#define WPAD_ADDR_LEN                 6

#define WPAD_STRM_INTERVAL            OSNanosecondsToTicks(6666667)

// clamp algorithm for stick
#define WPAD_STICK_CLAMP_OCTA_WITH_PLAY         0
#define WPAD_STICK_CLAMP_OCTA_WITHOUT_PLAY      1
#define WPAD_STICK_CLAMP_CIRCLE_WITH_PLAY       2
#define WPAD_STICK_CLAMP_CIRCLE_WITHOUT_PLAY    3
// clamp algorithm for trigger
#define WPAD_TRIGGER_FIXED_BASE       0
#define WPAD_TRIGGER_INDIVIDUAL_BASE  1
// clamp algorithm for accelerometer
#define WPAD_ACC_CLAMP_CUBE           0
#define WPAD_ACC_CLAMP_SPHERE         1

#define WPAD_SENSOR_BAR_POS_BOTTOM    0
#define WPAD_SENSOR_BAR_POS_TOP       1

#define WPAD_MEM_GAMEDATA_LEN         3888

#define WPAD_SYNC_TYPE_STD            0
#define WPAD_SYNC_TYPE_SMP            1

#else
// SI-based WPAD

#define WPAD_BUTTON_HOME              0x0001
#define WPAD_BUTTON_SELECT            0x0002
#define WPAD_BUTTON_A                 0x0004
#define WPAD_BUTTON_B                 0x0008
#define WPAD_BUTTON_SMALL_A           0x0010
#define WPAD_BUTTON_SMALL_B           0x0020
#define WPAD_BUTTON_START             0x0100
#define WPAD_BUTTON_UP                0x0200
#define WPAD_BUTTON_DOWN              0x0400
#define WPAD_BUTTON_RIGHT             0x0800
#define WPAD_BUTTON_LEFT              0x1000
#define WPAD_BUTTON_Z1                0x2000
#define WPAD_BUTTON_Z2                0x4000
#define WPAD_BUTTON_X                 WPAD_BUTTON_SMALL_A
#define WPAD_BUTTON_Y                 WPAD_BUTTON_SMALL_B
#define WPAD_TRIGGER_Z                WPAD_BUTTON_Z1
#define WPAD_TRIGGER_R                WPAD_BUTTON_Z2
#define WPAD_TRIGGER_L                0x8000

#define WPAD_DPD_MAX_OBJECTS          4
#define WPAD_DPD_ANGLE               42.0f  // viewing angle
#define WPAD_DPD_IMG_RESO_WX       1024     // DPD image resolution 
#define WPAD_DPD_IMG_RESO_WY        768

#define WPAD_ACC_RESO              1024     // accelerometer resolution
#define WPAD_ACC_1G_VALUE           205     // gravity value

#define WPAD_FS_STICK_RESO          256     // stick resolution of FreeStyle unit
#define WPAD_FS_ACC_RESO           1024     // accelerometer resolution of FreeStyle unit
#define WPAD_FS_ACC_1G_VALUE        205     // gravity value of FreeStyle unit

#define WPAD_DEV_CORE                 0
#define WPAD_DEV_FREESTYLE            1
#define WPAD_DEV_DOLPHIN            254
#define WPAD_DEV_UNKNOWN            255
#define WPAD_DEV_NOT_FOUND          253     // For backwards compatibility. Don't use.

#define WPAD_FMT_CORE                 0     // data format of Core controller 
#define WPAD_FMT_FREESTYLE            1     // data format of FreeStyle controller 
#define WPAD_FMT_DOLPHIN            255     // data format of Dolphin controller(for debug) 

#endif

#define WPAD_ERR_NONE                 0
#define WPAD_ERR_NO_CONTROLLER       -1
#define WPAD_ERR_BUSY                -2
#define WPAD_ERR_TRANSFER            -3
#define WPAD_ERR_INVALID             -4
#define WPAD_ERR_NOPERM              -5
#define WPAD_ERR_BROKEN              -6
#define WPAD_ERR_CORRUPTED           -7

#ifdef  __MWERKS__
#pragma warn_padding off
#endif
typedef struct DPDObject
{
    s16       x;                    // 0 <=  x   < WPAD_DPD_IMG_RESO_WX
    s16       y;                    // 0 <=  y   < WPAD_DPD_IMG_RESO_WY
    u16       size;
    u8        traceId;
    // 1 byte padding
} DPDObject;
#ifdef  __MWERKS__
#pragma warn_padding reset
#endif

#ifdef  __MWERKS__
#pragma warn_padding off
#endif
typedef struct DPDObjEx
{
    s16       range_x1;
    s16       range_y1;
    s16       range_x2;
    s16       range_y2;
    u16       pixel;
    s8        radius;
    // 1 byte padding
} DPDObjEx;
#ifdef  __MWERKS__
#pragma warn_padding reset
#endif

typedef struct WPADStatus
{
    u16       button;               // Or-ed WPAD_BUTTON_* bits
    s16       accX;                 // -WPAD_ACC_RESO/2 <= accX < WPAD_ACC_RESO/2
    s16       accY;                 // -WPAD_ACC_RESO/2 <= accY < WPAD_ACC_RESO/2
    s16       accZ;                 // -WPAD_ACC_RESO/2 <= accZ < WPAD_ACC_RESO/2
    DPDObject obj[WPAD_DPD_MAX_OBJECTS];

    u8        dev;                  // one of WPAD_DEV_* number
    s8        err;                  // one of WPAD_ERR_* number
} WPADStatus;

typedef struct WPADFSStatus
{
    u16       button;               // Or-ed WPAD_BUTTON_* bits
    s16       accX;                 // -WPAD_ACC_RESO/2 <= accX < WPAD_ACC_RESO/2
    s16       accY;                 // -WPAD_ACC_RESO/2 <= accY < WPAD_ACC_RESO/2
    s16       accZ;                 // -WPAD_ACC_RESO/2 <= accZ < WPAD_ACC_RESO/2
    DPDObject obj[WPAD_DPD_MAX_OBJECTS];

    u8        dev;                  // one of WPAD_DEV_* number
    s8        err;                  // one of WPAD_ERR_* number

    s16       fsAccX;               // -WPAD_FS_ACC_RESO/2 <= fsAccX < WPAD_ACC_RESO/2
    s16       fsAccY;               // -WPAD_FS_ACC_RESO/2 <= fsAccY < WPAD_ACC_RESO/2
    s16       fsAccZ;               // -WPAD_FS_ACC_RESO/2 <= fsAccZ < WPAD_ACC_RESO/2
    s8        fsStickX;             // -WPAD_FS_STICK_RESO/2 <= fsStickX < WPAD_FS_STICK_RESO/2
    s8        fsStickY;             // -WPAD_FS_STICK_RESO/2 <= fsStickY < WPAD_FS_STICK_RESO/2
} WPADFSStatus;

typedef struct WPADCLStatus
{
    u16       button;               // Or-ed WPAD_BUTTON_* bits
    s16       accX;                 // -WPAD_ACC_RESO/2 <= accX < WPAD_ACC_RESO/2
    s16       accY;                 // -WPAD_ACC_RESO/2 <= accY < WPAD_ACC_RESO/2
    s16       accZ;                 // -WPAD_ACC_RESO/2 <= accZ < WPAD_ACC_RESO/2
    DPDObject obj[WPAD_DPD_MAX_OBJECTS];

    u8        dev;                  // one of WPAD_DEV_* number
    s8        err;                  // one of WPAD_ERR_* number

    u16       clButton;             // Or-ed WPAD_CL_BUTTON * bits
    s16       clLStickX;            // -WPAD_CL_STICK_RESO/2 <= clLStickX < WPAD_CL_STICK_RESO/2
    s16       clLStickY;            // -WPAD_CL_STICK_RESO/2 <= clLStickY < WPAD_CL_STICK_RESO/2
    s16       clRStickX;            // -WPAD_CL_STICK_RESO/2 <= clRStickX < WPAD_CL_STICK_RESO/2
    s16       clRStickY;            // -WPAD_CL_STICK_RESO/2 <= clRStickY < WPAD_CL_STICK_RESO/2
    u8        clTriggerL;           // 0 <= clTriggerL < WPAD_CL_TRIGGER_RESO
    u8        clTriggerR;           // 0 <= clTriggerR < WPAD_CL_TRIGGER_RESO
} WPADCLStatus;

typedef struct WPADStatusEx
{
    u16       button;               // Or-ed WPAD_BUTTON_* bits
    s16       accX;                 // -WPAD_ACC_RESO/2 <= accX < WPAD_ACC_RESO/2
    s16       accY;                 // -WPAD_ACC_RESO/2 <= accY < WPAD_ACC_RESO/2
    s16       accZ;                 // -WPAD_ACC_RESO/2 <= accZ < WPAD_ACC_RESO/2
    DPDObject obj[WPAD_DPD_MAX_OBJECTS];

    u8        dev;                  // one of WPAD_DEV_* number
    s8        err;                  // one of WPAD_ERR_* number

    DPDObjEx  exp[WPAD_DPD_MAX_OBJECTS];
    
} WPADStatusEx;

typedef struct WPADInfo
{
    BOOL dpd;                       // the active status of pointer unit
    BOOL speaker;                   // the active status of speaker unit
    BOOL attach;                    // whether an attachment is installed (=1) or not (=0)
    BOOL lowBat;                    // whether the current battery level is lower than a threshold (0x20)
    BOOL nearempty;                 // sound buffer is nearly empty
    u8 battery;                     // the current battery level (WPAD_BATTERY_LEVEL_*)
    u8 led;                         // the current pattern of 4 LEDs
    u8 protocol;                    // not used
    u8 firmware;                    // not used
} WPADInfo;

typedef struct WPADAcc
{
    s16 x;
    s16 y;
    s16 z;
} WPADAcc;


typedef void   ( *WPADExtensionCallback         )( s32  chan, s32 result );
typedef void   ( *WPADSamplingCallback          )( s32  chan );
typedef void   ( *WPADConnectCallback           )( s32  chan, s32 reason );
typedef void   ( *WPADCallback                  )( s32  chan, s32 result );
typedef void * ( *WPADAlloc                     )( u32  size );
typedef u8     ( *WPADFree                      )( void *ptr );
typedef void   ( *WPADSyncDeviceCallback        )( s32  result, s32 num );
typedef void   ( *WPADClearDeviceCallback       )( s32  result );
typedef void   ( *WPADFlushCallback             )( u32  result );

#define WPADButtonDown( buttonLast, button )   \
    ( (u16) (((buttonLast) ^ (button)) & (button)) )

#define WPADButtonUp( buttonLast, button )   \
    ( (u16) (((buttonLast) ^ (button)) & (buttonLast)) )

#define WPADStartMotor( chan )  WPADControlMotor( ( chan ), WPAD_MOTOR_RUMBLE )
#define WPADStopMotor( chan )   WPADControlMotor( ( chan ), WPAD_MOTOR_STOP   )


#ifdef WPADEMU
// only for WPADEmu
BOOL WPADAttach                 ( s32 chan );
BOOL WPADRecalibrate            ( s32 chan );

#endif


void WPADInit                   ( void );
void WPADShutdown               ( void );
void WPADReconnect              ( void );
void WPADRegisterAllocator      ( WPADAlloc alloc, WPADFree free );
u32  WPADGetWorkMemorySize      ( void );
s32  WPADProbe                  ( s32 chan, u32 *type );
s32  WPADGetStatus              ( void );
u32  WPADGetDataFormat          ( s32 chan );
s32  WPADSetDataFormat          ( s32 chan, u32 fmt );
void WPADRead                   ( s32 chan, void *status );
void WPADSetAutoSamplingBuf     ( s32 chan, void *buf, u32 length);
u32  WPADGetLatestIndexInBuf    ( s32 chan );
void WPADGetAccGravityUnit      ( s32 chan, u32 type, WPADAcc *acc );
void WPADGetCLTriggerThreshold  ( s32 chan, u8 *left, u8 *right );

#ifdef WPADEMU
void WPADClampStick             ( u32 fmt, void *status );
#else
void WPADClampStick             ( s32 chan, void *status, u32 type );
void WPADClampTrigger           ( s32 chan, void *status, u32 type );
void WPADClampAcc               ( s32 chan, void *status, u32 type );
#endif

BOOL WPADIsDpdEnabled           ( s32 chan );
u8   WPADGetDpdFormat           ( s32 chan );
s32  WPADControlDpd             ( s32 chan, u32 command, WPADCallback callback );
u8   WPADGetDpdSensitivity      ( void );
void WPADSetDpdSensitivity      ( u8 level );

s32  WPADControlExtGimmick      ( s32 chan, u32 command, WPADCallback callback );

void WPADDisconnect             ( s32 chan );
s32  WPADGetInfo                ( s32 chan, WPADInfo *info );
s32  WPADGetInfoAsync           ( s32 chan, WPADInfo *info, WPADCallback callback );
void WPADGetAddress             ( s32 chan, u8 *addr );
u8   WPADGetRegisteredDevNum    ( void );
u8   WPADGetTemporaryDevNum     ( void );
u8   WPADGetRadioSensitivity    ( s32 chan );

BOOL WPADIsSpeakerEnabled       ( s32 chan );
s32  WPADControlSpeaker         ( s32 chan, u32 command, WPADCallback callback );
u8   WPADGetSpeakerVolume       ( void );
s32  WPADSendStreamData         ( s32 chan, void *buf, u16 len );
BOOL WPADCanSendStreamData      ( s32 chan );

void WPADControlMotor           ( s32 chan, u32 command );
BOOL WPADIsMotorEnabled         ( void );

void WPADSetAutoSleepTime       ( u8 minute );
u8   WPADGetSensorBarPosition   ( void );
void WPADClearPortMapTable      ( void );

s32  WPADReadGameData           ( s32 chan, void *p_buf, u16 len, u16 offset, WPADCallback callback );
s32  WPADWriteGameData          ( s32 chan, void *p_buf, u16 len, u16 offset, WPADCallback callback );
void WPADSetGameTitleUtf16      ( const u16  *title );
s32  WPADGetGameTitleUtf16      ( s32 chan, const u16 **title );
s32  WPADGetGameDataTimeStamp   ( s32 chan, OSTime *time );

BOOL WPADSetAcceptConnection    ( BOOL accept );
BOOL WPADGetAcceptConnection    ( void );

BOOL WPADSetDisableChannel      ( u16 disable );
BOOL WPADStartSyncDevice        ( void );
BOOL WPADStartClearDevice       ( void );
BOOL WPADStartSimpleSync        ( void );
BOOL WPADStopSimpleSync         ( void );
BOOL WPADStartFastSyncDevice    ( void );
BOOL WPADStartFastSimpleSync    ( void );

s32  WPADGetSyncType            ( s32 chan, u8 *type );

WPADSamplingCallback            WPADSetSamplingCallback     ( s32 chan, WPADSamplingCallback  callback );
WPADExtensionCallback           WPADSetExtensionCallback    ( s32 chan, WPADExtensionCallback callback );
WPADConnectCallback             WPADSetConnectCallback      ( s32 chan, WPADConnectCallback   callback );
WPADSyncDeviceCallback          WPADSetSyncDeviceCallback   ( WPADSyncDeviceCallback  callback );
WPADSyncDeviceCallback          WPADSetSimpleSyncCallback   ( WPADSyncDeviceCallback  callback );
WPADClearDeviceCallback         WPADSetClearDeviceCallback  ( WPADClearDeviceCallback callback );

// These can be used in only debug build so that application can debug extension handling
BOOL WPADAttachDummyExtension( s32 chan, u32 type );
BOOL WPADDetachDummyExtension( s32 chan );

#ifdef __cplusplus
}
#endif


#endif // __WPAD_H__
