/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Reset API
  File:     OSReset.h

  Copyright 2001-2003 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSReset.h,v $
  Revision 1.15.2.1  2007/12/05 11:30:41  wada_jumpei
  Merged with main.

  Revision 1.16  2007/12/05 07:07:56  wada_jumpei
  Added OSLaunchManualViewer.

  Revision 1.15  2007/06/29 00:25:57  carlmu
  Added OS_SHUTDOWN_PRIO_KBD.

  Revision 1.14  2007/04/27 09:21:21  ooizumi
  Added OSReturnToSetting.

  Revision 1.13  2007/04/12 08:46:24  ooizumi
  Added reset code for channel installer.

  Revision 1.12  2007/02/28 04:10:54  ooizumi
  Added OSReturnToDataManager.

  Revision 1.11  2007/02/23 07:24:39  ooizumi
  Added definitions for launching disc app from channel app.

  Revision 1.10  2006/09/21 09:11:09  hirose
  Added definition of OS_SD_LAUNCH.

  Revision 1.9  2006/09/20 13:42:32  hiratsu
  Added NAND priority.

  Revision 1.8  2006/09/14 13:35:41  tojo
  Added OS_SHUTDOWN_PRIO_WPAD.

  Revision 1.7  2006/08/29 05:20:08  ooizumi
  Enabled OSExec APIs.

  Revision 1.6  2006/08/10 13:48:37  urata
  Added OS_SHUTDOWN_PRIO_VI.

  Revision 1.5  2006/07/24 06:14:34  hirose
  Cleanup.

  Revision 1.4  2006/07/24 00:23:34  hirose
  Changes due to new reset/shutdown architecture.

  Revision 1.3  2006/06/16 12:45:42  ooizumi
  Commented out definition of OSExec.

  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Moved from the dolphin source tree


    20    2004/01/30 16:42 Hashida
    Added OSExec.


    19    2003/11/28 19:04 Shiki
    Removed OSGetRestartCode().

    18    2003/11/13 16:57 Shiki
    Fixed OSIsRestart() macro for new OSGetResetCode().

    17    2003/11/13 14:20 Shiki
    Added support for OSGetRestartCode().

    16    2003/10/24 9:33 Shiki
    Defined OS_RESETCODE_NETCONFIG.

    15    2003/07/10 14:38 Shiki
    Defined OS_RESET_PRIO_ALARM.

    14    2003/04/16 22:32 Hashida
    Added OSSetBootDol.

    13    2003/03/12 11:28 Shiki
    Defined OS_RESET_PRIO_SO and OS_RESET_PRIO_IP.

    12    2003/02/13 14:24 Shiki
    Defined OS_RESET_TIMEOUT.

    11    2002/11/19 14:55 Hirose
    Added OS_RESET_PRIO_GX.

    10    2002/10/28 20:28 Hashida
    Added OSIsRestart().

    9     2002/04/11 17:59 Hashida
    Added OSGetSavedRegion

    8     2001/09/15 3:31a Hashida
    Added OS_RESET_SHUTDOWN

    7     2001/09/07 5:30a Hashida
    Added OSSet/GetSaveRegion.

    6     2001/07/18 10:55p Hashida
    Changed definitions.

    5     2001/07/11 10:19p Hashida
    Added code definition for restarting.

    4     2001/05/17 8:20p Shiki
    Modified priority from s32 to u32.

    3     2001/05/17 7:58p Shiki
    Revised.

    2     2001/04/23 16:31 Shiki
    Added menu param to OSResetSystem().

    1     2001/04/09 13:52 Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSRESET_H__
#define __OSRESET_H__

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_RESETCODE_RESTART    0x80000000      // not by cold/hot reset
#define OS_RESETCODE_SYSTEM     0x40000000      // reserved by system
#define OS_RESETCODE_SWITCH     0x20000000      // reserved by system

// NETCONFIG ... 0x00010000
// INSTALLER ... 0x00020000
// XXXXXXXXX ... 0x00030000
// :

#define OS_RESETCODE_EXEC       0xC0000000
#define OS_RESETCODE_NETCONFIG  0xC0010000

#define OS_RESETCODE_LAUNCH     0xA0000000
#define OS_RESETCODE_INSTALLER  0xA0020000

#define OS_RESET_TIMEOUT        OSMillisecondsToTicks(1000)

#define OS_RESET_RESTART        0
#define OS_RESET_HOTRESET       1
#define OS_RESET_SHUTDOWN       2

#define OS_SHUTDOWN_PRIO_SO         110     // SO API
#define OS_SHUTDOWN_PRIO_IP         111     // IP API
#define OS_SHUTDOWN_PRIO_MIDI       126     // MIDI API
#define OS_SHUTDOWN_PRIO_CARD       127     // CARD API
#define OS_SHUTDOWN_PRIO_PAD        127     // PAD API
#define OS_SHUTDOWN_PRIO_WPAD       127     // WPAD API
#define OS_SHUTDOWN_PRIO_STEERING   127     // SI Steering
#define OS_SHUTDOWN_PRIO_GX         127     // GX API
#define OS_SHUTDOWN_PRIO_MEMPROT    127     // OS Memory Protection
#define OS_SHUTDOWN_PRIO_ALARM      4294967295  // OSAlarm
#define OS_SHUTDOWN_PRIO_VI         127     // VI API
#define OS_SHUTDOWN_PRIO_NAND       255     // NAND API
#define OS_SHUTDOWN_PRIO_KBD        127     // KBD (keyboard)

// Event types for __OSShutdownDevices()
#define OS_SD_FATALERR          0
#define OS_SD_REBOOT            1
#define OS_SD_SHUTDOWN          2
#define OS_SD_IDLE              3
#define OS_SD_RESTART           4
#define OS_SD_RETURNTOMENU      5
#define OS_SD_EXEC              6
#define OS_SD_LAUNCH            7


typedef BOOL (* OSShutdownFunction )( BOOL final, u32 event );
typedef struct OSShutdownFunctionInfo OSShutdownFunctionInfo;

struct OSShutdownFunctionInfo
{
    // public
    OSShutdownFunction      func;
    u32                     priority;

    // private
    OSShutdownFunctionInfo* next;
    OSShutdownFunctionInfo* prev;
};

void OSRegisterShutdownFunction     ( OSShutdownFunctionInfo* info );
void OSUnregisterShutdownFunction   ( OSShutdownFunctionInfo* info );


void OSRebootSystem         ( void );
void OSShutdownSystem       ( void );
void OSRestart              ( u32 resetCode );
void OSReturnToMenu         ( void );
void OSReturnToDataManager  ( void );

#define OS_SETTING_CALENDAR        1u
#define OS_SETTING_DISPLAY         2u
#define OS_SETTING_SOUND           3u
#define OS_SETTING_PARENTAL        4u
#define OS_SETTING_INTERNET        5u
#define OS_SETTING_WC24            6u
#define OS_SETTING_UPDATE          7u

void OSReturnToSetting      ( u8 setting );

u32  OSGetResetCode         ( void );
void OSGetSaveRegion        ( void** start, void** end );
void OSGetSavedRegion       ( void** start, void** end );
void OSSetSaveRegion        ( void* start, void* end );


// Obsoleted (implemented as stubs)
void OSResetSystem          ( int reset, u32 resetCode, BOOL forceMenu );
u32  OSSetBootDol           ( u32 dolOffset );

// Obsoleted
#if 0
typedef BOOL (* OSResetFunction )(BOOL final);
typedef struct OSResetFunctionInfo OSResetFunctionInfo;

struct OSResetFunctionInfo
{
    // public
    OSResetFunction      func;
    u32                  priority;

    // private
    OSResetFunctionInfo* next;
    OSResetFunctionInfo* prev;
};

void OSRegisterResetFunction  ( OSResetFunctionInfo* info );
void OSUnregisterResetFunction( OSResetFunctionInfo* info );
#endif

void OSExecv                  ( const char* dolfile, char* const argv[] );
void OSExecl                  ( const char* dolfile,
                                const char* arg0, .../*, (char*)NULL */ );

void OSLaunchManualViewer     ( const char* arg );

/*---------------------------------------------------------------------------*
  Name:         OSIsRestart

  Description:  Check to see if it's restarted

  Arguments:    None

  Returns:      True if restarted, False if cold-started
 *---------------------------------------------------------------------------*/
#define OSIsRestart()   \
            ((OSGetResetCode() & OS_RESETCODE_RESTART) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif

#endif  // __OSRESET_H__
