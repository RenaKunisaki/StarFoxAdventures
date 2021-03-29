/*---------------------------------------------------------------------------*
  Project:  Dolphin application format library
  File:     dolformat.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dolformat.h,v $
  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Ported from dolphin source tree.

    
    2     2000/04/06 10:00p Shiki
    Added padding to DolImage.

    4     1999/08/06 11:02a Tian
    Corrected comments

    3     1999/07/07 6:52p Tian
    Cleanup

    2     1999/07/07 10:49a Tian
    Fixed build error

    1     1999/07/07 9:52a Tian
    Initial check-in.  DOL format structure.  Based on code by Jojo Wesener.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <revolution/types.h>

#ifndef __DOLFORMAT_H__
#define __DOLFORMAT_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// We impose limits on the number of segments allowed
#define  DOL_MAX_TEXT           7
#define  DOL_MAX_DATA           11

// Other constraints
// 1. All section addresses must be 32 byte aligned (for DVD alignment)
// 2. All section lengths must be 4 byte aligned (DVD constraint).
//    Constraint #1 let's us round up lengths safely with no fear of
//    overlap.
// 3. DolImage itself should be a multiple of 32 byte.

typedef struct DolImage
{
    u8*     textData[DOL_MAX_TEXT]; // Actual text segments
    u8*     dataData[DOL_MAX_DATA]; // Actual data segments

    u32     text[DOL_MAX_TEXT];     // Virtual address destination
    u32     data[DOL_MAX_DATA];     // Virtual address destination

    u32     textLen[DOL_MAX_TEXT];  // Length of each text segment
    u32     dataLen[DOL_MAX_DATA];  // Length of each data segment

    u32     bss;                    // All bss sections lumped together
    u32     bssLen;

    u32     entry;                  // Entry point

#if ((3*(DOL_MAX_TEXT+DOL_MAX_DATA)+3)*4)%32
    u8       padding[32-((3*(DOL_MAX_TEXT+DOL_MAX_DATA)+3)*4)%32];
#endif
} DolImage;

#ifdef EPPC
#include <revolution/dolformat/DOLLoader.h>
#endif // EPPC

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __DOLFORMAT_H__
