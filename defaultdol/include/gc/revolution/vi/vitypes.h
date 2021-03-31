/*---------------------------------------------------------------------------*
  Project:  Dolphin VI library
  File:     vitypes.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: vitypes.h,v $
  Revision 1.5  2006/08/10 13:34:54  urata
  Removed "VI_TVMODE_PAL_PROG" (50Hz Progressive).

  Revision 1.4  07/12/2006 16:00:50  urata
  Clean up.

  Revision 1.3  03/02/2006 07:52:51  urata
  Added PAL, MPAL and EURGB60 progressive mode.

  Revision 1.2  2006/02/04 11:56:48  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    11    2003/11/19 11:08 Hashida
    Added a comment.
    
    10    2001/11/02 5:33p Hashida
    Renamed eu60 -> eurgb60
    
    9     2001/11/01 3:37p Hashida
    Added EURGB60 mode support.
    
    8     2001/10/18 12:06p Hashida
    No change.

    7     2001/10/12 2:49a Hashida
    Added EU60 mode.
    
    6     2001/09/19 12:26a Hashida
    Added DEBUG_PAL (NTSC 50Hz).
    
    5     2000/08/11 6:26p Hashida
    Added 3D and progressive mode support
    
    4     2000/5/15 1:52p Hashida
    Added definitions VI_MAX_WIDTH_NTSC, etc.
    
    3     2000/02/25 11:29a Hashida
    Added VIRetraceCallback definition.
    
    2     2000/01/26 6:32p Hashida
    Added VI_FIELD_ABOVE/BELOW definitions.
    
    1     2000/1/26 3:51p Hashida
    Initial revision
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __VITYPES_H
#define __VITYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

#define VI_DISPLAY_PIX_SZ           2

#define VI_INTERLACE                0
#define VI_NON_INTERLACE            1
#define VI_PROGRESSIVE              2
// 3 is reserved

#define VI_NTSC                     0
#define VI_PAL                      1
#define VI_MPAL                     2
#define VI_DEBUG                    3
#define VI_DEBUG_PAL                4
#define VI_EURGB60                  5
// 6,7,8,9 are reserved

#define VI_TVMODE(FMT, INT)   ( ((FMT) << 2) + (INT) )

typedef enum
{
    VI_TVMODE_NTSC_INT      = VI_TVMODE(VI_NTSC,        VI_INTERLACE),
    VI_TVMODE_NTSC_DS       = VI_TVMODE(VI_NTSC,        VI_NON_INTERLACE),
    VI_TVMODE_NTSC_PROG     = VI_TVMODE(VI_NTSC,        VI_PROGRESSIVE),

    VI_TVMODE_PAL_INT       = VI_TVMODE(VI_PAL,         VI_INTERLACE),
    VI_TVMODE_PAL_DS        = VI_TVMODE(VI_PAL,         VI_NON_INTERLACE),

    
    VI_TVMODE_EURGB60_INT   = VI_TVMODE(VI_EURGB60,     VI_INTERLACE),
    VI_TVMODE_EURGB60_DS    = VI_TVMODE(VI_EURGB60,     VI_NON_INTERLACE),
    VI_TVMODE_EURGB60_PROG  = VI_TVMODE(VI_EURGB60,     VI_PROGRESSIVE),
    
    VI_TVMODE_MPAL_INT      = VI_TVMODE(VI_MPAL,        VI_INTERLACE),
    VI_TVMODE_MPAL_DS       = VI_TVMODE(VI_MPAL,        VI_NON_INTERLACE),
    VI_TVMODE_MPAL_PROG     = VI_TVMODE(VI_MPAL,        VI_PROGRESSIVE),
    
    VI_TVMODE_DEBUG_INT     = VI_TVMODE(VI_DEBUG,       VI_INTERLACE),

    VI_TVMODE_DEBUG_PAL_INT = VI_TVMODE(VI_DEBUG_PAL,   VI_INTERLACE),
    VI_TVMODE_DEBUG_PAL_DS  = VI_TVMODE(VI_DEBUG_PAL,   VI_NON_INTERLACE)
} VITVMode;
    
typedef enum
{
    VI_XFBMODE_SF = 0,
    VI_XFBMODE_DF
} VIXFBMode;


#define VI_FIELD_ABOVE              1
#define VI_FIELD_BELOW              0

// Maximum screen space
#define VI_MAX_WIDTH_NTSC           720
#define VI_MAX_HEIGHT_NTSC          480

#define VI_MAX_WIDTH_PAL            720
#define VI_MAX_HEIGHT_PAL           574

#define VI_MAX_WIDTH_MPAL           720
#define VI_MAX_HEIGHT_MPAL          480

#define VI_MAX_WIDTH_EURGB60        VI_MAX_WIDTH_NTSC
#define VI_MAX_HEIGHT_EURGB60       VI_MAX_HEIGHT_NTSC


typedef void (*VIRetraceCallback)(u32 retraceCount);

#ifdef __cplusplus
}
#endif

#endif
