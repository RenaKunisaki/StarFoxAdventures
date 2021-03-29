/*---------------------------------------------------------------------------*
  Project:    Revolution SDK
  File:       revodefs.h
  Programmer: TOKUNAGA, Yasuhiro

  Copyright (C) 2005-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef __REVODEFS_H__
#define __REVODEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define REVOLUTION_DEF_STR(X)      REVOLUTION_DEF_VAL(X)
#define REVOLUTION_DEF_VAL(X)      #X
#define REVOLUTION_DEF_CAT(X,Y)    X ## Y
#define REVOLUTION_LIB_CC          REVOLUTION_DEF_STR(__CWCC__) "_" REVOLUTION_DEF_STR(__CWBUILD__)

#ifdef _DEBUG
#define REVOLUTION_LIB_VERSION(lib)    \
    const char* __ ## lib ## Version = "<< RVL_SDK - " #lib " \tdebug build: " __DATE__ " " __TIME__ " (" REVOLUTION_LIB_CC ") >>"
#else
#define REVOLUTION_LIB_VERSION(lib)    \
    const char* __ ## lib ## Version = "<< RVL_SDK - " #lib " \trelease build: " __DATE__ " " __TIME__ " (" REVOLUTION_LIB_CC ") >>"
#endif

#ifdef __cplusplus
}
#endif

#endif  // __REVODEFS_H__
