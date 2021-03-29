/*---------------------------------------------------------------------------*
  Project:  RSO
  File:     RSOLink.h

  Copyright 2006 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  @version $Id: RSOLink.h,v 1.11 2006/12/19 04:07:35 srd_nisiwaki Exp $

 *---------------------------------------------------------------------------*/

//RSOLink.c header

#ifndef __RSO_LINK_H__
#define __RSO_LINK_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
   RSOObject
 *---------------------------------------------------------------------------*/
RSOImportTable*         RSOGetImport                    ( const RSOSymbolHeader* imp );
const RSOExportTable*   RSOGetExport                    ( const RSOSymbolHeader* exp );
BOOL                    RSOLocateObject                 ( void* newModule, void* bss ); 
BOOL                    RSOLocateObjectFixed            (void* newModule,void* bss);
BOOL                    RSOStaticLocateObject           ( void* newModule );
BOOL                    RSOUnLocateObject               ( void* oldModule );
int                     RSOLink                         ( RSOObjectHeader* rsoImp, const RSOObjectHeader* rsoExp );
void                    RSOUnLink                      ( RSOObjectHeader* rsoImp, const RSOObjectHeader* rsoExp );
RSOHash                 RSOGetHash                      ( const char * symbolname );
/*---------------------------------------------------------------------------*
   RSOImport
 *---------------------------------------------------------------------------*/
BOOL                RSOIsImportSymbolResolved       ( const RSOObjectHeader* rso, int index );
BOOL                RSOIsImportSymbolResolvedAll    ( const RSOObjectHeader* rso );
int                 RSOGetNumImportSymbols          ( const RSOSymbolHeader* imp );
const char*         RSOGetImportSymbolName          ( const RSOSymbolHeader* imp, int index );

int                 RSOGetNumImportSymbolsUnresolved( const RSOObjectHeader* rso );

/*---------------------------------------------------------------------------*
   RSOExport
 *---------------------------------------------------------------------------*/
int                 RSOGetNumExportSymbols          ( const RSOSymbolHeader* exp );
const void*         RSOFindExportSymbolAddr         ( const RSOObjectHeader* rso, const char* name );
RSOExportTable*     RSOFindExportSymbol             ( const RSOObjectHeader* rso, const char* name );
const char*         RSOGetExportSymbolName          ( const RSOSymbolHeader* exp, int index );
const void*         RSOGetExportSymbolAddr          ( const RSOObjectHeader* rso, int index );

/*---------------------------------------------------------------------------*
   RSOLinkList
 *---------------------------------------------------------------------------*/
BOOL                RSOListInit                    (void* i_staticRso);
BOOL                RSOLinkList                    (void* i_newRso, void* i_bss);
BOOL                RSOLinkListFixed               (void* i_newRso, void* i_bss,int i_fixed);
BOOL                RSOUnLinkList                  (void* i_oldRso);

u32                 RSOGetFixedSize                 (void *i_rso,int i_fixed_level);
/*---------------------------------------------------------------------------*
   RSOLinkFar
 *---------------------------------------------------------------------------*/
int RSOGetFarCodeSize(const RSOObjectHeader* rsoImp, const RSOObjectHeader* rsoExp);
int RSOLinkFar(RSOObjectHeader* rsoImp, const RSOObjectHeader* rsoExp,void *buff);
/*---------------------------------------------------------------------------*
   RSOLinkJump
 *---------------------------------------------------------------------------*/
int RSOGetJumpCodeSize(const RSOObjectHeader *i_rsoRso);
void RSOMakeJumpCode(const RSOObjectHeader *i_rsoExp,void *i_buff);
int RSOLinkJump(RSOObjectHeader* i_rsoImp,const RSOObjectHeader* i_rsoExp,void *i_buff);

#ifdef __cplusplus
}
#endif
    
#endif // __RSO_LINK_H__
