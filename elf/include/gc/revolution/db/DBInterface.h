/*---------------------------------------------------------------------------*
  Project:  Dolphin Debugger stubs
  File:     DBInterface.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: DBInterface.h,v $
  Revision 1.2  2006/02/04 11:56:45  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    2     1999/09/01 1:00p Tian
    Added exception return field.  It is filled in but not necessarily used
    by the debugger.
    
    1     1999/07/08 8:06p Tian
    Moved to include/dolphin/db/
    
    2     1999/05/11 10:14a Shiki
    Refreshed include tree.

    1     1999/04/30 12:47p Tianli01

    2     1999/03/31 4:59p Tianli01
    Renamed OSException to __OSException, moved structure offset #define's
    into OSLoMem.h

    1     1999/03/26 2:19p Tianli01
    Initial checkin.  Debugger stubs and exception handlers.  Exception
    handling path functional but does not conform to specification yet.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __DBINTERFACE_H__
#define __DBINTERFACE_H__

#include <revolution/os.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DBInterface
{
    u32     bPresent;       // non zero if debug kernel is loaded
    u32     exceptionMask;  // bitmask of exceptions

    // ptr to the destination so it does not have to be in low
    // memory, nor do we have to perform 2 loads
    void    (*ExceptionDestination) ( void );

    // Return address from DBExceptionDestination that debugger should
    // jump to.
    void    *exceptionReturn;
} DBInterface;

// Set dynamically to OSLoMem->OS_DBINTERFACE_ADDR by DBInit,
// which is used ONLY by testing code
extern DBInterface* __DBInterface;

BOOL                __DBIsExceptionMarked       ( __OSException exception );

// The well-known destination the debugger
void                __DBExceptionDestination    ( void );
    
#ifdef _DEBUG
// XXX for use in testing the protocol      XXX 
// XXX must be eliminated before release    XXX
void                __DBInit                    ( void );
void                __DBSetPresent              ( u32 value );
void                __DBMarkException           ( __OSException exception, 
                                                  BOOL value );
#endif


#ifdef __cplusplus
}
#endif
#endif // __DBINTERFACE_H__
