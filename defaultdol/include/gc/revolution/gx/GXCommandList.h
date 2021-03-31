/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXCommandList.h

  Copyright 1998-2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXCommandList.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    6     2002/07/24 9:06 Hirose
    Resolved version conflicts.
    
    5     2001/07/16 4:06p Carl
    Added refs to filter conversion arrays.
    
    4     2001/05/22 7:54p Carl
    Added full set of CP commands.
    
    3     2001/02/15 4:25p Carl
    Added refs to texture ID arrays.
    
    2     2001/02/05 8:11p Carl
    Added GX_LOAD_BP_REG.
    
    3     1999/07/14 4:21p Alligator
    Split gx.h into separate header file for each major section of the API
    
    2     1999/06/08 11:54a Ryan
    Added GX prefix to defines
    
    1     1999/06/01 2:04p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXCOMMANDLIST_H__
#define __GXCOMMANDLIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

/****************************************************************************/

// Display list opcodes:

#define GX_NOP                      0x00
#define GX_DRAW_QUADS               0x80
#define GX_DRAW_TRIANGLES           0x90
#define GX_DRAW_TRIANGLE_STRIP      0x98
#define GX_DRAW_TRIANGLE_FAN        0xA0
#define GX_DRAW_LINES               0xA8
#define GX_DRAW_LINE_STRIP          0xB0
#define GX_DRAW_POINTS              0xB8

#define GX_LOAD_BP_REG              0x61
#define GX_LOAD_CP_REG              0x08
#define GX_LOAD_XF_REG              0x10
#define GX_LOAD_INDX_A              0x20
#define GX_LOAD_INDX_B              0x28
#define GX_LOAD_INDX_C              0x30
#define GX_LOAD_INDX_D              0x38

#define GX_CMD_CALL_DL              0x40
#define GX_CMD_INVL_VC              0x48

#define GX_OPCODE_MASK              0xF8
#define GX_VAT_MASK                 0x07

/****************************************************************************/

// Texture register IDs:

extern u8 GXTexMode0Ids[8];
extern u8 GXTexMode1Ids[8];
extern u8 GXTexImage0Ids[8];
extern u8 GXTexImage1Ids[8];
extern u8 GXTexImage2Ids[8];
extern u8 GXTexImage3Ids[8];
extern u8 GXTexTlutIds[8];

// Filter conversion arrays:

extern u8 GX2HWFiltConv[6];
extern u8 HW2GXFiltConv[8];

/****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // __GXCOMMANDLIST_H__
