/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Expansion Interface API
  File:     OSSerial.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSExpansion.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2002/10/25 14:27 Shiki
    Added support for Memory Card 1019 and 2043.

    3     2002/08/08 12:40 Shiki
    Added EXI_ETHER.

    2     2002/03/26 8:59 Shiki
    Added EXI_IS_VIEWER.

    1     2001/10/18 13:49 Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OS_EXPANSION_H__
#define __OS_EXPANSION_H__

#include <revolution/types.h>
#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXI_MEMORY_CARD_59      0x00000004
#define EXI_MEMORY_CARD_123     0x00000008
#define EXI_MEMORY_CARD_251     0x00000010
#define EXI_MEMORY_CARD_507     0x00000020

#ifndef EXI_MEMORY_CARD_1019

#define EXI_MEMORY_CARD_1019    0x00000040
#define EXI_MEMORY_CARD_2043    0x00000080

// Since SDK 9/5/02 Patch 1, EXI_MEMORY_CARD_*[A-G] are not
// returned with EXIGetType(). EXIGetType() returns EXI_MEMORY_CARD_1019
// for any type of Memory Card 1019 and EXI_MEMORY_CARD_2043 for any type
// of Memory Card 2043.
#define EXI_MEMORY_CARD_1019A   0x00000140
#define EXI_MEMORY_CARD_1019B   0x00000240
#define EXI_MEMORY_CARD_1019C   0x00000340
#define EXI_MEMORY_CARD_1019D   0x00000440
#define EXI_MEMORY_CARD_1019E   0x00000540
#define EXI_MEMORY_CARD_1019F   0x00000640
#define EXI_MEMORY_CARD_1019G   0x00000740

#define EXI_MEMORY_CARD_2043A   0x00000180
#define EXI_MEMORY_CARD_2043B   0x00000280
#define EXI_MEMORY_CARD_2043C   0x00000380
#define EXI_MEMORY_CARD_2043D   0x00000480
#define EXI_MEMORY_CARD_2043E   0x00000580
#define EXI_MEMORY_CARD_2043F   0x00000680
#define EXI_MEMORY_CARD_2043G   0x00000780

#endif  // EXI_MEMORY_CARD_1019

#define EXI_USB_ADAPTER         0x01010000
#define EXI_NPDP_GDEV           0x01020000

#define EXI_MODEM               0x02020000
#define EXI_ETHER               0x04020200

#define EXI_MARLIN              0x03010000

#define EXI_IS_VIEWER           0x05070000

BOOL  EXIProbe        ( s32 chan );
s32   EXIProbeEx      ( s32 chan );

s32   EXIGetType      ( s32 chan, u32 dev, u32* type );
char* EXIGetTypeString( u32 type );

#ifdef __cplusplus
}
#endif

#endif  // __OS_EXPANSION_H__
