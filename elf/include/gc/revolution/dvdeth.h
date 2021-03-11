/*---------------------------------------------------------------------------*
  Project:  Dolphin OS
  File:     dvdeth.h

  Copyright 2002 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dvdeth.h,v $
  Revision 1.2  2006/02/04 11:56:44  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:27  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:06  yasuh-to
  Ported from dolphin source tree.

    
    7     2003/09/02 6:30p Ueno_kyu
    
   
    5     2003/02/28 5:31p Ooshima
    2nd release version
    
    4     2003/01/09 1:17p Ooshima
    Deleted all DVD function
    
    1     2002/12/11 8:59a Ooshima
    Initial version

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifdef  DVDETH
#ifndef __DVDETH_H__
#define __DVDETH_H__

#include <revolution/types.h>
#include <revolution/dvd.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL	DVDEthInit(const u8* addr, const u8* netmask, const u8* gateway);
void	DVDEthShutdown(void);

BOOL	DVDLowInit(const u8* pServerAddr, u16 ServerPort);

s32     DVDWritePrio	( DVDFileInfo* fileInfo, void* addr, s32 length,
                          s32 offset, s32 prio );

BOOL    DVDWriteAsyncPrio( DVDFileInfo* fileInfo, void* addr, s32 length,
                          s32 offset,
                          DVDCallback callback, s32 prio );

#define DVDWrite(fileInfo, addr, length, offset)                 \
						DVDWritePrio((fileInfo), (addr), (length), (offset), 2)

#define DVDWriteAsync(fileInfo, addr, length, offset, callback)  \
                        DVDWriteAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)

BOOL    DVDRemove      ( const char* fileName, DVDFileInfo* fileInfo );

BOOL    DVDCreate      ( const char* fileName, DVDFileInfo* fileInfo );

BOOL    DVDFstInit(void* fstAddr, u32 fstLen);

BOOL    DVDFstRefresh(void);


#ifdef __cplusplus
}
#endif

#endif  // __DVDETH_H__
#endif  //  DVDETH
