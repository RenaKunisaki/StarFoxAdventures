/*---------------------------------------------------------------------------*
  Project:  Revolution USB keyboard library
  File:     kbd.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: kbd.h,v $
  Revision 1.17  2007/07/13 23:49:11  carlmu
  Added KBDSetLedsRetry.
  Changed API for LED callback.

  Revision 1.16  2007/07/12 01:38:05  dante.treglia
  KBD: Removed unsupported languages from typedef enum _KBDCountryCode.

  Revision 1.15  2007/06/21 22:40:38  carlmu
  Updated for 1.4 API (changed LED handling).

  Revision 1.14  2007/05/29 21:56:06  carlmu
  Changed macro indicating NumLock state.

  Revision 1.13  2007/05/22 00:43:30  carlmu
  Changed #ifdef USBKBD to #ifndef USE_PPC_HID, etc.

  Revision 1.12  2007/05/17 21:57:58  carlmu
  Added ability to set PPC HID lib memory.

  Revision 1.11  2007/05/11 23:21:07  carlmu
  Added US_International map code.
  Added macros related to CTRL keys.

  Revision 1.10  2007/05/05 01:58:13  carlmu
  Changes for 0.8 API

  Revision 1.9  2007/05/01 21:38:13  carlmu
  Stripped dead-key-related code.
  Changed KBD_MS_BASE_KEY to KBD_MS_SHIFTED_KEY.

  Revision 1.8  2007/04/25 19:17:22  carlmu
  Added macros related to keypad digit keys.

  Revision 1.7  2007/04/18 00:01:32  carlmu
  Updated for 0.5 API.

  Revision 1.6  2007/04/10 18:43:20  carlmu
  New 0.4 API structure.

  Revision 1.5  2007/04/05 21:56:59  carlmu
  Changed kbd_usb_codes.h to kbd_hid_codes.h.

  Revision 1.4  2007/04/04 19:39:50  carlmu
  Added KBD_HID_NONE.

  Revision 1.3  2007/04/02 19:11:24  carlmu
  Get/Set LED calls now private.
  Set callback functions return old callback.
  Added scroll lock modifier state.

  Revision 1.2  2007/03/28 00:27:45  carlmu
  Updated to 0.2 API.

  Revision 1.1  2007/03/21 18:06:02  carlmu
  Initial version.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __KBD_H__
#define __KBD_H__

#include <revolution/types.h>
#include <revolution/kbd/kbd_key_defs.h>
#include <revolution/kbd/kbd_hid_codes.h>

#ifdef __cplusplus
extern "C" {
#endif

// Low-level key press API

//-----------------------------------------------------------------------------
// Types:

// Up to 4 keyboards can be attached
#define KBD_MAX_CHANNELS       (4)   // valid channel numbers are 0-3

typedef enum _KBDEc {                // function return code
  KBD_SUCCESS                 = 0,
  KBD_ERR_OTHER               = 1,   // usually from HID error
  KBD_ERR_NOT_INITIALIZED     = 2,
  KBD_ERR_ALREADY_INITIALIZED = 3,
  KBD_ERR_BAD_VALUE           = 4,   // input argument out of range
  KBD_ERR_BAD_STATUS          = 5,   // ie, keyboard disconnected
  KBD_ERR_BAD_POINTER         = 6,
  KBD_ERR_BUSY                = 7    // for KBDSetLeds
} KBDEc;

typedef enum _KBDChanStatus { // status bits may be OR-ed together
  KBD_CS_OK           = 0x00,
  KBD_CS_DISCONNECTED = 0x01,
  KBD_CS_ROLL_OVER    = 0x02,
  KBD_CS_ERROR        = 0x04
} KBDChanStatus;

typedef u8 KBDChannel;
typedef u8 KBDHIDCode;

#define KBD_HID_NONE       0 // value returned when queue is empty
#define KBD_HID_OVERFLOW 255 // value returned when queue overflows

typedef enum _KBDKeyMode {
  KBD_KM_DOWN   = 0x01,    // on=key down;             off=key up
  KBD_KM_REPEAT = 0x02,    // on=repeat key;           off=initial key press
  KBD_KM_NULL   = 0x00
} KBDKeyMode;

#define KBD_KEY_MODE_UP(_key_mode)     (!((_key_mode) & KBD_KM_DOWN))
#define KBD_KEY_MODE_DOWN(_key_mode)   ((_key_mode) & KBD_KM_DOWN)
#define KBD_KEY_MODE_REPEAT(_key_mode) ((_key_mode) & KBD_KM_REPEAT)

// xxx Some of these countries may disappear from this list.
//     Others may be added.  Names may be changed.

typedef enum _KBDCountryCode {
  KBD_CC_INTERNATIONAL,
  KBD_CC_CANADIAN_BI,
  KBD_CC_CANADIAN_FR,
  KBD_CC_DANISH,
  KBD_CC_FINNISH,
  KBD_CC_FRENCH,
  KBD_CC_GERMAN,
  KBD_CC_GREEK,
  KBD_CC_ITALIAN,
  KBD_CC_JAPANESE,
  KBD_CC_LATIN_AMERICAN,
  KBD_CC_NETHERLANDS_DUTCH,
  KBD_CC_NORWEGIAN,
  KBD_CC_PORTUGUESE,
  KBD_CC_SPANISH,
  KBD_CC_SWEDISH,
  KBD_CC_UNITED_KINGDOM,
  KBD_CC_UNITED_STATES,
  KBD_CC_RESERVED
} KBDCountryCode;

#define KBD_NUM_COUNTRY_CODES  KBD_CC_RESERVED

typedef enum _KBDModState {   // state bits may be OR-ed together
  KBD_MS_CTRL        = 0x0001,
  KBD_MS_SHIFT       = 0x0002,
  KBD_MS_ALT         = 0x0004,
  KBD_MS_GUI         = 0x0008,
  KBD_MS_EXTRA       = 0x0010,
  KBD_MS_ALTGR       = 0x0020,
  KBD_MS_LANG1       = 0x0040, // (Hiragana)
  KBD_MS_LANG2       = 0x0080, // (Katakana)
  KBD_MS_NUM_LOCK    = 0x0100,
  KBD_MS_CAPS_LOCK   = 0x0200,
  KBD_MS_SCROLL_LOCK = 0x0400,
  KBD_MS_SHIFTED_KEY = 0x0800, // when CTRL/ALT/GUI/EXTRA is on, allow shifted key to be returned
                               // (the default is that the base/unshifted key is returned)
  KBD_MS_SET_ALL     = 0x1000, // With KBDSetModState, allows setting of all bits
  KBD_MS_NULL        = 0x0000
} KBDModState;

// define alternate names for these
#define KBD_MS_HIRAGANA KBD_MS_LANG1
#define KBD_MS_KATAKANA KBD_MS_LANG2

// define convenient groups of state
#define KBD_MS_TRANSIENTS (KBD_MS_CTRL|KBD_MS_SHIFT|KBD_MS_ALT|KBD_MS_GUI|KBD_MS_EXTRA|KBD_MS_ALTGR)
#define KBD_MS_TOGGLES (KBD_MS_LANG1|KBD_MS_LANG2|KBD_MS_NUM_LOCK|KBD_MS_CAPS_LOCK|KBD_MS_SCROLL_LOCK)

typedef u16 KBDUnicode; // should it be wchar_t? xxx

// Indicate if a KBDUnicode value is private to KBD (special KBD internal value)
#define KBD_UC_IS_PRIVATE(uc) (((uc)>=0xf000 && (uc)<=0xF1FF) || ((uc)==0xFFFF))

// This macro may be used to determine if a KBDUnicode is a modifier key
#define KBD_UC_IS_MODIFIER(uc) ((uc)>=0xf000 && (uc)<=0xf01f)

// These defines may be used to determine if a KBDUnicode
// value is a regular ASCII key from the keypad, and if so, to
// convert that value to the regular ASCII value
#define KBD_UC_IS_KP_REG_KEY(uc) (((uc)>=0xf100) && ((uc)<=0xf13f))
#define KBD_KP_REG_KEY_TO_ASCII(uc) ((uc)&0x3f)

// Indicate if a key is a keypad number (digit) key with NumLock on
#define KBD_UC_IS_KP_NUM_NL_KEY(uc) (((uc)>=KBK_Keypad_0 && ((uc)<=KBK_Keypad_9)))
// Indicate if a key is a keypad number (digit) key with NumLock off
#define KBD_UC_IS_KP_NUM_UL_KEY(uc) (((uc)>=KBK_Keypad_Insert && ((uc)<=KBK_Keypad_Page_Up)))
// Convert from NumLock off digit key to NumLock on digit key
#define KBD_KP_NUM_UL_KEY_TO_KP_NUM_NL_KEY(uc) ((KBDUnicode)((uc)-0x40))

// Indicate if a ModState represents NumLock == on
#define KBD_MS_IS_NUMLOCK(ms) (((ms)&KBD_MS_NUM_LOCK) && !((ms)&KBD_MS_SHIFT))

// Indicate if a key is a private control Unicode, and convert to ASCII
#define KBD_UC_IS_CTRL_KEY  (((uc)>=0xf1c0) && ((uc)<=0xf1df))
#define KBD_CTRL_KEY_TO_ASCII(uc) ((uc)&0x1f)

typedef enum _KBDLedState {
  KBD_LED_NUM_LOCK    = 0x01,
  KBD_LED_CAPS_LOCK   = 0x02,
  KBD_LED_SCROLL_LOCK = 0x04,
  KBD_LED_NULL        = 0x00,
  KBD_LED_RESERVED    = 0x10
} KBDLedState;

//-----------------------------------------------------------------------------
// Structs:

typedef struct _KBDDevEvent {                // kbd attach/detach callbacks
  KBDChannel channel;
} KBDDevEvent;

typedef struct _KBDKeyEvent {		     // for key callback/getkey
  KBDChannel  channel;
  KBDHIDCode  hid;        // USB HID code
  KBDKeyMode  mode;       // for up/down/etc
  KBDModState modState;   // modifier state
  KBDUnicode  unicode;    // unicode, if any
} KBDKeyEvent;

typedef struct _KBDLedEvent {                // kbd LED callback
  KBDChannel  channel;
  KBDLedState leds;
  KBDEc       rc;
} KBDLedEvent;

typedef void (*KBDDevCallbackF) (KBDDevEvent *ev);
typedef void (*KBDKeyCallbackF) (KBDKeyEvent *ev);
typedef void (*KBDLedCallbackF) (KBDLedEvent *ev, void *cbArg);

//-----------------------------------------------------------------------------
// Function calls

// One of these MUST be called before KBDInit:
void  KBDInitRegionUS(void);
void  KBDInitRegionJP(void);
void  KBDInitRegionEU(void);

KBDEc KBDInit (void);  // init library
KBDEc KBDExit (void);  // terminate library

KBDDevCallbackF KBDSetAttachCallback (KBDDevCallbackF fn); // set kbd attach callback
KBDDevCallbackF KBDSetDetachCallback (KBDDevCallbackF fn); // set kbd detach callback

KBDEc KBDGetChannelStatus (KBDChannel ch, KBDChanStatus *chanStatus);
KBDEc KBDResetChannel (KBDChannel ch);                        // reset key press state

KBDEc KBDSetCountry (KBDChannel ch, KBDCountryCode country);  // set country
KBDEc KBDGetCountry (KBDChannel ch, KBDCountryCode *country); // get country

KBDKeyCallbackF KBDSetKeyCallback (KBDKeyCallbackF fn);  // set key callback
KBDEc KBDGetKey(KBDChannel ch, KBDKeyEvent *keyEvent);        // synch key API

KBDEc KBDSetLockProcessing(KBDChannel ch, BOOL  enable);
KBDEc KBDGetLockProcessing(KBDChannel ch, BOOL *enable);

KBDEc KBDSetModState(KBDChannel ch, KBDModState  modState);
KBDEc KBDGetModState(KBDChannel ch, KBDModState *modState);

KBDUnicode KBDTranslateHidCode(KBDHIDCode hid, KBDModState modState, KBDCountryCode country);

KBDEc KBDSetRepeat(KBDChannel ch, u16  delay, u16  interval);
KBDEc KBDGetRepeat(KBDChannel ch, u16 *delay, u16 *interval);

KBDEc KBDSetAccessSticky(KBDChannel, BOOL  enable);
KBDEc KBDGetAccessSticky(KBDChannel, BOOL *enable);

KBDEc KBDSetLeds(KBDChannel ch, KBDLedState  leds);
KBDEc KBDSetLedsAsync(KBDChannel ch, KBDLedState  leds, KBDLedCallbackF fn, void *cbArg);
KBDEc KBDSetLedsRetry(KBDChannel ch, KBDLedState  leds);

#ifdef USE_PPC_HID
// Temporary test function
void KBDSetHidMem(u8 *ptr, u32 size);
#endif

#ifdef __cplusplus
}
#endif

#endif // __KBD_H__
