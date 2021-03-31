/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXVerify.h

  Copyright 1998- 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXVerify.h,v $
  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    6     2002/07/24 9:07 Hirose
    Resolved version conflicts.
    
    6     2002/04/09 15:15 Hirose
    const type specifier support. (worked by hiratsu@IRD)
    
    5     2001/08/07 3:08p Carl
    Fixed typo.
    
    4    2000/ 11/14 11:33p Hirose
    added stubs for GXVerify stuff of NDEBUG build
    
    3     2000/10/06 6:20p Carl
    Updated warning system to print messages through callback (part 2)
    
    2     2000/08/27 5:45p Alligator
    print warning messages through a callback function
    
    1    2000/01/28 3:55p Alligator
    integrate library from ArtX
    
    1     1999/08/30 11:06a Vimal
    
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXVERIFY_H
#define __GXVERIFY_H

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>

typedef enum {
    GX_WARN_NONE,       // No warnings reported
    GX_WARN_SEVERE,     // Reports only severest warnings
    GX_WARN_MEDIUM,     // Reports severe and medium warnings
    GX_WARN_ALL         // Reports any and all warning info
} GXWarningLevel;

#ifdef _DEBUG
extern void GXSetVerifyLevel( GXWarningLevel level );
#else
#define GXSetVerifyLevel(level)     ((void)0)
#endif

// Note: Warning IDs are subject to change.
//       Warning messages are subject to change as well.

typedef void (*GXVerifyCallback)(GXWarningLevel level, 
                                 u32            id, 
                                 const char*          msg);

#ifdef _DEBUG
GXVerifyCallback GXSetVerifyCallback( GXVerifyCallback cb );
#else
#define GXSetVerifyCallback(cb)     ((GXVerifyCallback)0)
#endif

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif // __GXVERIFY_H
