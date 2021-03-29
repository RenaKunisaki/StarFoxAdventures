/*---------------------------------------------------------------------------*
  Project:  Revolution USB keyboard library
  File:     kbd_key_defs.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: kbd_key_defs.h,v $
  Revision 1.6  2007/05/30 18:33:48  carlmu
  Added KBK_Keypad_Equals.

  Revision 1.5  2007/05/11 23:19:47  carlmu
  Changed mappings for CTRL keys.
  Removed obsolete comments.

  Revision 1.4  2007/04/25 19:16:40  carlmu
  Control keys moved to private range.

  Revision 1.3  2007/04/18 00:02:07  carlmu
  Updated for 0.5 API.

  Revision 1.2  2007/03/28 00:28:14  carlmu
  Updated to 0.2 API.

  Revision 1.1  2007/03/21 18:06:16  carlmu
  Initial version.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __KBD_KEY_DEFS_H__
#define __KBD_KEY_DEFS_H__

// Defines used for Key names (Unicode values for keys).

// This contains the Unicodes for keyboard keys.
// Right now it just contains the custom Unicodes for keys that
// represent non-printing functions (and some that do).

// All keyboard key names have the prefix KBK_.
// All those which represent printing characters should use the Unicode
// value for that key.  Some exceptions are made for keypad keys, which
// use the Unicode value plus a given constant.

// Official 16-bit Unicodes use the range 0x0000-0xDFFF and 0xF900-0xFFEF.
// The area from 0xE000-0xF8FF is available for custom usage.
// The code 0xFFFF is also guaranteed not to be an official Unicode.

// Our custom map is as follows (at a high level):
//
// 0xf000 - 0xf05f : modifier keys + special keys
// 0xf060 - 0xf07f : function keys (F-keys)
// 0xf080 - 0xf0ff : reserved for future HID keys
// 0xf100 - 0xf13f : keypad versions of main ascii keys (numlock on)
// 0xf140 - 0xf17f : keypad versions of editing keys (numlock off)
// 0xf180 - 0xf1bf : editing keys (which may be duplicated on keypad)
// 0xf1c0 - 0xf1ff : control keys; reserved range

//-----------------------------------------------------------------------------

// Special values

// KBK_Void is used to indicate an undefined key press.
// It is used in the maps to indicate a key with no translation available.
// We are using the value of 0, and although that's a regular ASCII/Unicode value,
// this is probably okay since 0 is rarely ever used as itself (Ctrl-@).

#define KBK_Void     0x0000

// KBK_Err is a special return value for the Translate function that
// indicates bad arguments were passed in.

#define KBK_Error    0xffff

//-----------------------------------------------------------------------------

// Modifier keys / special keys: 0xf000-0xf05f

// These codes are modifiers that affect what key is looked up from the tables

#define KBK_Mod_Shift          0xf001
#define KBK_Mod_AltGr          0xf005
#define KBK_Lang_Toggle        0xf006
#define KBK_Num_Lock           0xf007
#define KBK_Caps_Lock          0xf008

// Extra modifiers; don't affect lookup, but are passed on as ModState

#define KBK_Mod_Control        0xf000
#define KBK_Mod_Alt            0xf002
#define KBK_Mod_GUI            0xf003
#define KBK_Mod_Extra          0xf004

// Other odd keys: 0xf020-0xf05f

#define KBK_Print_Screen       0xf020
#define KBK_Scroll_Lock        0xf021
#define KBK_Pause              0xf022
#define KBK_Application        0xf02f

// Katakana_Hiragana (key 2 right of space bar)
// Henkan (key right of space bar)
// Muhenkan (key left of space bar)
// Kanji (key in upper left corner)
#define KBK_Katakana_Hiragana  0xf040
#define KBK_Henkan             0xf041
#define KBK_Muhenkan           0xf042
#define KBK_Kanji              0xf043

#define KBK_Katakana           0xf044
#define KBK_Hiragana           0xf045
#define KBK_Zenkaku_Hankaku    0xf046

#define KBK_Hangul_English     0xf050
#define KBK_Hanja              0xf051

// all F-keys: 0xf060-0xf07f

#define KBK_F1                 0xf061
#define KBK_F2                 0xf062
#define KBK_F3                 0xf063
#define KBK_F4                 0xf064
#define KBK_F5                 0xf065
#define KBK_F6                 0xf066
#define KBK_F7                 0xf067
#define KBK_F8                 0xf068
#define KBK_F9                 0xf069
#define KBK_F10                0xf06a
#define KBK_F11                0xf06b
#define KBK_F12                0xf06c

// Space from 0xf080-0xf0ff is reserved for
// possible future HID keys (0x66 - 0xDF).

// Keypad codes (for NumLock = on): 0xf100-0xf13f
// These keys are the Ascii code of the corresponding
// character on the (NumLocked) key, offset by 0xf100.

// Note that KBK_Keypad_Comma is defined here and used in maps
// where comma is shown on the keypad, but the NumLocked version
// of this key still uses the Period (0x2e) as the base code.

#define KBK_Keypad_Comma       0xf12c
#define KBK_Keypad_Period      0xf12e
#define KBK_Keypad_0           0xf130
#define KBK_Keypad_1           0xf131
#define KBK_Keypad_2           0xf132
#define KBK_Keypad_3           0xf133
#define KBK_Keypad_4           0xf134
#define KBK_Keypad_5           0xf135
#define KBK_Keypad_6           0xf136
#define KBK_Keypad_7           0xf137
#define KBK_Keypad_8           0xf138
#define KBK_Keypad_9           0xf139

// Keypad codes (for NumLock = irrelevant): 0xf100-0xf13f
// These keys are the Ascii code of the corresponding
// character on the key, offset by 0xf100.

#define KBK_Keypad_Slash       0xf12f
#define KBK_Keypad_Asterisk    0xf12a
#define KBK_Keypad_Minus       0xf12d
#define KBK_Keypad_Plus        0xf12b
#define KBK_Keypad_Equals      0xf13d
#define KBK_Keypad_Enter       0xf10d

// Keypad codes (for NumLock = off): 0xf140-0xf17f
// These keys are the Ascii code of the corresponding
// character on the (NumLocked) key, offset by 0xf140.

#define KBK_Keypad_Delete      0xf16e // KP_Period
#define KBK_Keypad_Insert      0xf170 // KP_0
#define KBK_Keypad_End         0xf171 // KP_1
#define KBK_Keypad_Down_Arrow  0xf172 // KP_2
#define KBK_Keypad_Page_Down   0xf173 // KP_3
#define KBK_Keypad_Left_Arrow  0xf174 // KP_4
#define KBK_Keypad_Space       0xf175 // KP_5
#define KBK_Keypad_Right_Arrow 0xf176 // KP_6
#define KBK_Keypad_Home        0xf177 // KP_7
#define KBK_Keypad_Up_Arrow    0xf178 // KP_8
#define KBK_Keypad_Page_Up     0xf179 // KP_9

// Editing keys: 0xf180-0xf1bf
// These codes are based upon the keypad codes (see below).
// These keys are the Ascii code of the corresponding
// character on the (NumLocked) key, offset by 0xf180.

#define KBK_Delete             0xf1ae // KP_Period
#define KBK_Insert             0xf1b0 // KP_0
#define KBK_End                0xf1b1 // KP_1
#define KBK_Down_Arrow         0xf1b2 // KP_2
#define KBK_Page_Down          0xf1b3 // KP_3
#define KBK_Left_Arrow         0xf1b4 // KP_4
                                      // KP_5
#define KBK_Right_Arrow        0xf1b6 // KP_6
#define KBK_Home               0xf1b7 // KP_7
#define KBK_Up_Arrow           0xf1b8 // KP_8
#define KBK_Page_Up            0xf1b9 // KP_9

// Normal ASCII Control keys: 0xf1c0-0xf1df
// These keys are the Ascii code of the corresponding
// character on the key, offset by 0xf1c0.

#define KBK_Backspace          0xf1c8
#define KBK_Tab                0xf1c9
#define KBK_Enter              0xf1cd
#define KBK_Escape             0xf1db

// Range from 0xf1e0-0xf1ff is reserved for future use.

// The spacebar
// This is just the regular ASCII code for space.
// It is included here for convenience.
#define KBK_Space              0x0020

#endif //  __KBD_KEY_DEFS_H__
