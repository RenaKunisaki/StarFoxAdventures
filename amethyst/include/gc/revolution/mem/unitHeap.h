/*---------------------------------------------------------------------------*
  Project:     MEM library
  File:        unitHeap.h
  Programmers: Takano Makoto

  Copyright 2005 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef MEM_UNITHEAP_H__
#define MEM_UNITHEAP_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <revolution/types.h>
#include <revolution/mem/heapCommon.h>

/* ========================================================================
    Type Definitions
   ======================================================================== */

typedef struct MEMiUntHeapMBlockHead MEMiUntHeapMBlockHead;

// Header information for memory block
struct MEMiUntHeapMBlockHead
{
    MEMiUntHeapMBlockHead*  pMBlkHdNext;    // Next block
};


typedef struct MEMiUntMBlockList MEMiUntMBlockList;

// Memory block list
struct MEMiUntMBlockList
{
    MEMiUntHeapMBlockHead*  head;           // Pointer for memory block linked to header
};

typedef struct MEMiUntHeapHead MEMiUntHeapHead;

// Header information for unit heap
struct MEMiUntHeapHead
{
    MEMiUntMBlockList    mbFreeList;     // Free list
    u32                  mBlkSize;       // Memory block size
};



/* ========================================================================
    Macro Functions
   ======================================================================== */


/* ========================================================================
    Function Prototype
   ======================================================================== */

#if defined(_DEBUG)

void        MEMiDumpUnitHeap( MEMHeapHandle heap );

// #if defined(_DEBUG)
#endif

MEMHeapHandle  MEMCreateUnitHeapEx(
                        void*           startAddress,
                        u32             heapSize,
                        u32             memBlockSize,
                        int             alignment,
                        u16             optFlag );

void*       MEMDestroyUnitHeap  ( MEMHeapHandle heap );

void*       MEMAllocFromUnitHeap( MEMHeapHandle heap );

void        MEMFreeToUnitHeap(
                        MEMHeapHandle  heap,
                        void*          memBlock );

u32         MEMCountFreeBlockForUnitHeap( MEMHeapHandle heap );

u32         MEMCalcHeapSizeForUnitHeap(
                        u32             memBlockSize,
                        u32             memBlockNum,
                        int             alignment );

/* =======================================================================
    Inline function
   ======================================================================== */
/*---------------------------------------------------------------------------*
  Name:         MEMCreateUnitHeap

  Description:  Creates unit heap

  Arguments:    startAddress:  	Start address of heap area
                heapSize: 	Size of heap area
                memBlockSize: 	Memory block size

  Returns:      If the function succeeds, a handle for the created unit heap is returned.
                If function fails, NNS_FND_INVALID_HEAP_HANDLE is returned.
 *---------------------------------------------------------------------------*/
static inline MEMHeapHandle
MEMCreateUnitHeap(
    void*   startAddress,
    u32     heapSize,
    u32     memBlockSize
)
{
    return MEMCreateUnitHeapEx( startAddress, heapSize, memBlockSize, MEM_HEAP_DEFAULT_ALIGNMENT, 0 );
}


/*---------------------------------------------------------------------------*
  Name:         MEMGetMemBlockSizeForUnitHeap

  Description:  Gets the memory block size for unit heap.

  Arguments:    heap: 	unit heap handle

  Returns:      Returns memory block size for unit heap.
 *---------------------------------------------------------------------------*/
static inline u32
MEMGetMemBlockSizeForUnitHeap( MEMHeapHandle heap )
{
    return ( ( (const MEMiUntHeapHead*)( (const u8*)heap + sizeof(MEMiHeapHead) ) )->mBlkSize );
}


#ifdef __cplusplus
} /* extern "C" */
#endif

/* MEM_UNITHEAP_H__ */
#endif

