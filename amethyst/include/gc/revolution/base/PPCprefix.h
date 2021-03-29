/*---------------------------------------------------------------------------*
  Project:  Dolphin OS PPC prefix header, Release version 
  File:     PPCprefix.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: PPCprefix.h,v $
  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     1999/05/03 5:25p Tianli01
    PPC->EPPC
    
    1     1999/04/30 12:49p Tianli01
    
    1     1999/03/09 11:36a Tianli01
    Initial check-in.  PPC/X86 prefix header files

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#define NDEBUG
#include "ansi_prefix.PPCEABI.bare.h"
#define EPPC
