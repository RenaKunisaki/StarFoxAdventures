/*---------------------------------------------------------------------------*
  Project:  Dolphin application format loader interface
  File:     DOLLoader.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DOLLoader.h,v $
  Revision 1.2  2006/02/04 11:56:45  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     1999/07/13 7:25p Tian
    Added verbose flag to loader
    
    1     1999/07/07 9:54a Tian
    Initial check-in.  Interface for DOL static loader.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <revolution/types.h>

#ifndef __DOLLOADER_H__
#define __DOLLOADER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Parses and loads the DOL file image pointed to by buffer
// Returns the entry point to the DOL image pointed to by buffer
void*   DOLLoadImage(u8 * buffer, BOOL verbose);
void    DOLRunApp(void* entryPoint);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DOLLOADER_H__
