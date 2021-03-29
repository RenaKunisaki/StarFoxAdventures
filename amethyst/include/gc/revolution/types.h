/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Emulator on Microsoft Windows
  File:     types.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: types.h,v $
  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.


    8     2001/12/14 17:45 Shiki
    Removed an extra token at end of #endif directive.

    7    2001/06/11 7:52p Tian
    Integrated SN changes

    6     2001/03/05 7:41p Hashida
    Changed !(EMU) -> !(EMU) && !(WIN32)

    5     2000/11/01 4:08p Shiki
    Fixed EPPC -> !EMU.

    4     2000/11/01 3:19p Shiki
    Removed #include "ansi_prefix.PPCEABI.bare.h"

    3     2000/10/27 10:21a Tian
    Added SN

    2     2000/03/13 3:20p Shiki
    Removed M_NUMBEROF().

    17    1999/10/25 1:57p Shiki
    Enabled ATTRIBUTE_ALIGN macro for EPPC build.

    16    1999/10/13 2:10p Yasu
    Added vf32 and vf64

    15    1999/09/16 11:28a Tian
    Restored #ifndef BOOL.

    14    1999/09/15 6:13p Shiki
    Revised not to define ATTRIBUTE_ALIGN both for EPPC and MACOS.

    13    1999/09/15 10:36a Hashida
    Changed #ifndef BOOL to #ifndef X86

    12    1999/08/20 5:53p Shiki
    Added volatile integer types.

    11    1999/07/07 10:40a Tian
    Added X86 typedefs for GCC builds.

    10    1999/06/10 1:43p Tianli01
    Added ATTRIBUTE_ALIGN for static variable alignment

    9     1999/06/04 3:03p Tianli01
    Added DOLPHIN_ALIGNMENT

    8     1999/05/28 10:47a Mikepc

    7     1999/05/26 8:59a Shiki
    Fixed #include <MacTypes.h>.

    6     1999/05/25 5:29p Shiki
    Added the following lines:
    #ifndef MACOS
    #include <MacTypes.h>
    #endif

    5     1999/05/11 4:42p Shiki
    Refreshed include tree.

    3     1999/05/05 7:53p Shiki
    Changed Ptr from void* to char*.

    2     1999/05/05 7:15p Tianli01
    Ifdef-ed the EABI prefix header

    1     1999/04/30 12:49p Tianli01

    4     1999/04/20 5:58p Shiki
    a) Revised the definitions of NULL and BOOL macro.
    b) Prevent the definition of the Ptr type for MacOS.

    3     1999/03/26 2:04p Tianli01
    Added BOOL/NULL only if undefined for PPC

    2     1999/03/09 1:25p Tianli01

    1     1999/03/09 11:37a Tianli01
    Temporary floating point update.

    2     1999/03/04 2:18p Tianli01
    testing

    1     1999/03/04 2:18p Tianli01
    Initial check-in for testing

    1     1998/12/15 10:05p Shiki

  Change History:
    1998/12/10  Shiki Okasaka   Revised to reflect the coding guidelines
    1998/12/04  Shiki Okasaka   Created

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef MACOS
#include <MacTypes.h>
#endif

#ifdef  __MWERKS__          // For MetroWerks compiler
typedef signed char         s8;
typedef signed short        s16;
typedef signed long         s32;
typedef signed long long    s64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned long       u32;
typedef unsigned long long  u64;
#else       // __MWERKS__
#ifdef X86  // GCC
typedef unsigned long long  u64;
typedef   signed long long  s64;
typedef unsigned int        u32;
typedef   signed int        s32;
typedef unsigned short      u16;
typedef   signed short      s16;
typedef unsigned char       u8;
typedef   signed char       s8;
#else
#ifdef __SN__               // For SN
typedef   signed char       s8;
typedef   signed short      s16;
typedef   signed long       s32;
typedef   signed long long  s64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned long       u32;
typedef unsigned long long  u64;
#else // __SN__
// win32 style
typedef __int8              s8;
typedef __int16             s16;
typedef __int32             s32;
typedef __int64             s64;
typedef unsigned __int8     u8;
typedef unsigned __int16    u16;
typedef unsigned __int32    u32;
typedef unsigned __int64    u64;
#endif      // __SN__
#endif      // X86
#endif      // __MWERKS__

typedef volatile u8         vu8;
typedef volatile u16        vu16;
typedef volatile u32        vu32;
typedef volatile u64        vu64;
typedef volatile s8         vs8;
typedef volatile s16        vs16;
typedef volatile s32        vs32;
typedef volatile s64        vs64;

typedef float               f32;
typedef double              f64;
typedef volatile f32        vf32;
typedef volatile f64        vf64;

#ifndef MACOS
typedef char*               Ptr;
#endif

#ifndef BOOL
typedef int                 BOOL;
#endif  // BOOL

#ifndef TRUE
// Any non zero value is considered TRUE
#define TRUE                1
#endif  // TRUE

#ifndef FALSE
#define FALSE               0
#endif  // FALSE

#ifndef NULL
#ifdef  __cplusplus
#define NULL                0
#else   // __cplusplus
#define NULL                ((void *)0)
#endif  // __cplusplus
#endif  // NULL

#define DOLPHIN_ALIGNMENT 32

// Use the following pragma wherever specific alignment is required for
// static variables.
#if !defined(EMU) && !defined(WIN32)
#define ATTRIBUTE_ALIGN(num) __attribute__ ((aligned (num)))
#else
#define ATTRIBUTE_ALIGN(num)
#endif  // EMU

// SN-Phil: AT ADDRESS MACRO
// Use the following pragma wherever a fixed address is required for
// static variables.
#ifdef __SN__
// Rena: don't use this for gcc, use symbol file instead
//#define AT_ADDRESS(xyz) __attribute__((address((xyz))))
#define AT_ADDRESS(xyz)
#else
#define AT_ADDRESS(xyz) : (xyz)
#endif // __SN__


#endif  // __TYPES_H__
