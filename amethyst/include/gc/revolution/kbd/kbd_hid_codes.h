/*---------------------------------------------------------------------------*
  Project:  Revolution USB keyboard library
  File:     kbd_hid_codes.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: kbd_hid_codes.h,v $
  Revision 1.2  2007/07/13 10:22:26  hiratsu
  Fixed missing typedef.

  Revision 1.1  2007/04/05 21:57:48  carlmu
  Changed kbd_usb_codes.h to kbd_hid_codes.h.

  Revision 1.2  2007/03/28 00:28:14  carlmu
  Updated to 0.2 API.

  Revision 1.1  2007/03/21 18:06:16  carlmu
  Initial version.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __KBD_HID_CODES_H__
#define __KBD_HID_CODES_H__

#ifdef __cplusplus
extern "C" {
#endif

// Table of USB HID key codes

// We don't know the size of the enum, so we don't use this
// enum as a type for HID codes.  Instead we use KBDHIDCode (u8).

typedef enum _KBDHIDEnum {
    KBD_HID_No_Event = 0x00,
    KBD_HID_Roll_Over = 0x01,
    KBD_HID_Post_Fail= 0x02,
    KBD_HID_Undefined_Error = 0x03,
    KBD_HID_A = 0x04, 
    KBD_HID_B = 0x05,
    KBD_HID_C = 0x06,
    KBD_HID_D = 0x07,
    KBD_HID_E = 0x08,
    KBD_HID_F = 0x09,
    KBD_HID_G = 0x0A,
    KBD_HID_H = 0x0B,
    KBD_HID_I = 0x0C,
    KBD_HID_J = 0x0D,
    KBD_HID_K = 0x0E,
    KBD_HID_L = 0x0F,
    KBD_HID_M = 0x10,
    KBD_HID_N = 0x11,
    KBD_HID_O = 0x12,
    KBD_HID_P = 0x13,
    KBD_HID_Q = 0x14,
    KBD_HID_R = 0x15,
    KBD_HID_S = 0x16,
    KBD_HID_T = 0x17,
    KBD_HID_U = 0x18,
    KBD_HID_V = 0x19,
    KBD_HID_W = 0x1A,
    KBD_HID_X = 0x1B,
    KBD_HID_Y = 0x1C,
    KBD_HID_Z = 0x1D,
    KBD_HID_1 = 0x1E,
    KBD_HID_2 = 0x1F,
    KBD_HID_3 = 0x20,
    KBD_HID_4 = 0x21,
    KBD_HID_5 = 0x22,
    KBD_HID_6 = 0x23,
    KBD_HID_7 = 0x24,
    KBD_HID_8 = 0x25,
    KBD_HID_9 = 0x26,
    KBD_HID_0 = 0x27,
    KBD_HID_Enter = 0x28,
    KBD_HID_Escape = 0x29,
    KBD_HID_Backspace = 0x2A,
    KBD_HID_Tab = 0x2B,
    KBD_HID_Spacebar = 0x2C,
    KBD_HID_Hypen_Underscore = 0x2D,
    KBD_HID_Equals_Plus = 0x2e,
    KBD_HID_Bracketleft_Braceleft = 0x2f,
    KBD_HID_Bracketright_Braceright = 0x30,
    KBD_HID_Backslash_Bar = 0x31,
    KBD_HID_NonUS_Pound_Tilde = 0x32,
    KBD_HID_Semicolon_Colon = 0x33,
    KBD_HID_Apostrophe_Doublequote = 0x34,
    KBD_HID_Grave_Tilde = 0x35,
    KBD_HID_Comma_Less = 0x36,
    KBD_HID_Period_Greater = 0x37,
    KBD_HID_Slash_Question = 0x38,
    KBD_HID_Caps_Lock = 0x39,
    KBD_HID_F1 = 0x3A,
    KBD_HID_F2 = 0x3B,
    KBD_HID_F3 = 0x3C,
    KBD_HID_F4 = 0x3D,
    KBD_HID_F5 = 0x3E,
    KBD_HID_F6 = 0x3F,
    KBD_HID_F7 = 0x40,
    KBD_HID_F8 = 0x41,
    KBD_HID_F9 = 0x42,
    KBD_HID_F10 = 0x43,
    KBD_HID_F11 = 0x44,
    KBD_HID_F12 = 0x45,
    KBD_HID_Print_Screen = 0x46,
    KBD_HID_Scroll_Lock = 0x47,
    KBD_HID_Pause = 0x48,
    KBD_HID_Insert = 0x49,
    KBD_HID_Home = 0x4A,
    KBD_HID_Page_Up = 0x4B,
    KBD_HID_Delete_Forward = 0x4C,
    KBD_HID_End = 0x4D,
    KBD_HID_Page_Down = 0x4E,
    KBD_HID_Right_Arrow = 0x4F,
    KBD_HID_Left_Arrow = 0x50,
    KBD_HID_Down_Arrow = 0x51,
    KBD_HID_Up_Arrow = 0x52,
    KBD_HID_Keypad_Num_Lock = 0x53,
    KBD_HID_Keypad_Slash = 0x54,
    KBD_HID_Keypad_Asterisk = 0x55,
    KBD_HID_Keypad_Minus = 0x56,
    KBD_HID_Keypad_Plus = 0x57,
    KBD_HID_Keypad_Enter = 0x58,
    KBD_HID_Keypad_1_End = 0x59,
    KBD_HID_Keypad_2_Down_Arrow = 0x5A,
    KBD_HID_Keypad_3_Page_Down = 0x5B,
    KBD_HID_Keypad_4_Left_Arrow = 0x5C,
    KBD_HID_Keypad_5_Space = 0x5D,
    KBD_HID_Keypad_6_Right_Arrow = 0x5E,
    KBD_HID_Keypad_7_Home = 0x5F,
    KBD_HID_Keypad_8_Up_Arrow = 0x60,
    KBD_HID_Keypad_9_Page_Up = 0x61,
    KBD_HID_Keypad_0_Insert = 0x62,
    KBD_HID_Keypad_Period_Delete = 0x63,
    KBD_HID_NonUS_backslash_bar = 0x64,
    KBD_HID_Application = 0x65,
    KBD_HID_Power = 0x66,
    KBD_HID_Keypad_Equals = 0x67,
    KBD_HID_F13 = 0x68,
    KBD_HID_F14 = 0x69,
    KBD_HID_F15 = 0x6A,
    KBD_HID_F16 = 0x6B,
    KBD_HID_F17 = 0x6C,
    KBD_HID_F18 = 0x6D,
    KBD_HID_F19 = 0x6E,
    KBD_HID_F20 = 0x6F,
    KBD_HID_F21 = 0x70,
    KBD_HID_F22 = 0x71,
    KBD_HID_F23 = 0x72,
    KBD_HID_F24 = 0x73,
    KBD_HID_Execute = 0x74,
    KBD_HID_Help = 0x75,
    KBD_HID_Menu = 0x76,
    KBD_HID_Select = 0x77,
    KBD_HID_Stop = 0x78,
    KBD_HID_Again = 0x79,
    KBD_HID_Undo = 0x7A,
    KBD_HID_Cut = 0x7B,
    KBD_HID_Copy = 0x7C,
    KBD_HID_Paste = 0x7D,
    KBD_HID_Find = 0x7E,
    KBD_HID_Mute = 0x7F,
    KBD_HID_Volume_Up = 0x80,
    KBD_HID_Volume_Down = 0x81,
    KBD_HID_Locking_Caps_Lock = 0x82,
    KBD_HID_Locking_Num_Lock = 0x83,
    KBD_HID_Locking_Scroll_Lock = 0x84,
    KBD_HID_Keypad_Comma = 0x85,
    KBD_HID_Keypad_Equals_AS400 = 0x86,
    KBD_HID_I1_Ro = 0x87,
    KBD_HID_I2_Katakana_Hiragana = 0x88,
    KBD_HID_I3_Yen = 0x89,
    KBD_HID_I4_Henkan = 0x8A,
    KBD_HID_I5_Muhenkan = 0x8B,
    KBD_HID_I6 = 0x8C,
    KBD_HID_I7 = 0x8D,
    KBD_HID_I8 = 0x8E,
    KBD_HID_I9 = 0x8F,
    KBD_HID_L1_Hangul_English = 0x90,
    KBD_HID_L2_Hanja = 0x91,
    KBD_HID_L3_Katakana = 0x92,
    KBD_HID_L4_Hiragana = 0x93,
    KBD_HID_L5_Zenkaku_Hankaku = 0x94,
    KBD_HID_L6 = 0x95,
    KBD_HID_L7 = 0x96,
    KBD_HID_L8 = 0x97,
    KBD_HID_L9 = 0x98,
    KBD_HID_Alternate_Erase = 0x99,
    KBD_HID_SysReq_Attention = 0x9A,
    KBD_HID_Cancel = 0x9B,
    KBD_HID_Clear = 0x9C,
    KBD_HID_Prior = 0x9D,
    KBD_HID_Return = 0x9E,
    KBD_HID_Separator = 0x9F,
    KBD_HID_Out = 0xA0,
    KBD_HID_Oper = 0xA1,
    KBD_HID_Clear_Again = 0xA2,
    KBD_HID_CrSel_Props = 0xA3,
    KBD_HID_ExSel = 0xA4,
    // various extra reserved & keypad keys from 0xA5 to 0xDF omitted
    KBD_HID_Left_Control = 0xE0,
    KBD_HID_Left_Shift = 0xE1,
    KBD_HID_Left_Alt = 0xE2,
    KBD_HID_Left_GUI = 0xE3,
    KBD_HID_Right_Control = 0xE4,
    KBD_HID_Right_Shift = 0xE5,
    KBD_HID_Right_AltGr = 0xE6,
    KBD_HID_Right_GUI = 0xE7
    // codes 0xE8 and up are reserved
} KBDHIDEnum;

#ifdef __cplusplus
}
#endif

#endif //  __KBD_HID_CODES_H__
