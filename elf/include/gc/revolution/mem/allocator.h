/*---------------------------------------------------------------------------*
  Project:     MEM library
  File:        allocator.h
  Programmers: Takano Makoto

  Copyright 2005 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef MEM_ALLOCATOR_H__
#define MEM_ALLOCATOR_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <revolution/mem/heapCommon.h>

/* ========================================================================
    Type Definitions
   ======================================================================== */

typedef struct MEMAllocator MEMAllocator;

typedef void* (*MEMFuncAllocatorAlloc)( MEMAllocator* pAllocator, u32 size );

typedef void  (*MEMFuncAllocatorFree) ( MEMAllocator* pAllocator, void* memBlock );

typedef struct MEMAllocatorFunc MEMAllocatorFunc;

struct MEMAllocatorFunc
{
    MEMFuncAllocatorAlloc    pfAlloc;
    MEMFuncAllocatorFree     pfFree;
};

struct MEMAllocator
{
    MEMAllocatorFunc const *  pFunc;
    void*                     pHeap;
    u32                       heapParam1;
    u32                       heapParam2;
};


/* ========================================================================
    Function Prototype
   ======================================================================== */

void*       MEMAllocFromAllocator( MEMAllocator* pAllocator, u32 size );

void        MEMFreeToAllocator   ( MEMAllocator* pAllocator, void* memBlock );

void        MEMInitAllocatorForExpHeap(
                    MEMAllocator*    pAllocator,
                    MEMHeapHandle    heap,
                    int              alignment );

void        MEMInitAllocatorForFrmHeap(
                    MEMAllocator*    pAllocator,
                    MEMHeapHandle    heap,
                    int              alignment );

void        MEMInitAllocatorForUnitHeap( MEMAllocator* pAllocator, MEMHeapHandle heap );

void        MEMInitAllocatorForOSHeap ( MEMAllocator* pAllocator, OSHeapHandle heap );


#ifdef __cplusplus
} /* extern "C" */
#endif

/* MEM_ALLOCATOR_H__ */
#endif
