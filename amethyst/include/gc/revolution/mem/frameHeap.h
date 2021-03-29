/*---------------------------------------------------------------------------*
  Project:     MEM library
  File:        frameHeap.h
  Programmers: Takano Makoto

  Copyright 2005 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef MEM_FRAMEHEAP_H__
#define MEM_FRAMEHEAP_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <revolution/types.h>
#include <revolution/mem/heapCommon.h>

/* =======================================================================
    Constant Definitions
   ======================================================================== */

#define MEM_FRMHEAP_FREE_HEAD (1 <<0)
#define MEM_FRMHEAP_FREE_TAIL (1 <<1)
#define MEM_FRMHEAP_FREE_ALL  (MEM_FRMHEAP_FREE_HEAD | MEM_FRMHEAP_FREE_TAIL)


/* =======================================================================
    Type Definitions
   ======================================================================== */

typedef struct MEMiFrmHeapState MEMiFrmHeapState;

// Structure for state storage
struct MEMiFrmHeapState
{
    u32                 tagName;        // Tag name
    void*               headAllocator;  // Head location of frame heap
    void*               tailAllocator;  // Tail location of frame heap
    MEMiFrmHeapState*   pPrevState;     // Pointer for last state storage
};

typedef struct MEMiFrmHeapHead MEMiFrmHeapHead;

// Header information for frame heap
struct MEMiFrmHeapHead
{
    void*               headAllocator;  // Pointer for head memory allocation
    void*               tailAllocator;  // Pointer for tail memory allocation

    MEMiFrmHeapState*   pState;         // State storage parameter
};


/* =======================================================================
    Macro Functions
   ======================================================================== */


/* =======================================================================
    Function Prototype
   ======================================================================== */

void*       MEMiGetFreeStartForFrmHeap( MEMHeapHandle heap );

void*       MEMiGetFreeEndForFrmHeap  ( MEMHeapHandle heap );

#if defined(_DEBUG)

void        MEMiDumpFrmHeap( MEMHeapHandle heap );

// #if defined(_DEBUG)
#endif

MEMHeapHandle  MEMCreateFrmHeapEx(
                                void*   startAddress,
                                u32     size,
                                u16     optFlag );

void*            MEMDestroyFrmHeap( MEMHeapHandle heap );

void*            MEMAllocFromFrmHeapEx(
                                MEMHeapHandle   heap,
                                u32             size,
                                int             alignment );

void             MEMFreeToFrmHeap( MEMHeapHandle heap, int mode );

u32              MEMGetAllocatableSizeForFrmHeapEx( MEMHeapHandle heap, int alignment );

BOOL             MEMRecordStateForFrmHeap( MEMHeapHandle heap, u32 tagName );

BOOL             MEMFreeByStateToFrmHeap( MEMHeapHandle heap, u32 tagName );

u32              MEMAdjustFrmHeap( MEMHeapHandle heap );

u32              MEMResizeForMBlockFrmHeap(
                                MEMHeapHandle    heap,
                                void*            memBlock,
                                u32              newSize );


/* =======================================================================
    Inline function
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         MEMCreateFrmHeap

  Description:  Creates a frame heap.

  Arguments:    startAddress: 	Start address of heap area
                size: 	Size of heap area

  Returns:      Returns the handle for the created frame heap if the function succeeds.
                MEM_INVALID_HEAP_HANDLE is returned if the function fails.
 *---------------------------------------------------------------------------*/
static inline MEMHeapHandle
MEMCreateFrmHeap(
    void*   startAddress,
    u32     size
)
{
    return MEMCreateFrmHeapEx( startAddress, size, 0 );
}


/*---------------------------------------------------------------------------*
  Name:         MEMAllocFromFrmHeap

  Description:  Allocates a memory block from the frame heap.
                Alignment of the memory block is 4-byte fixed.

  Arguments:    heap: 	Handle for the frame heap
                size: 	Size of the memory block to be allocated (in bytes)

  Returns:      Returns the pointer to the allocated memory block if the memory block was successfully allocated.
                
                If the operation fails, NULL is returned.
 *---------------------------------------------------------------------------*/
static inline void*
MEMAllocFromFrmHeap(
    MEMHeapHandle   heap,
    u32             size
)
{
    return MEMAllocFromFrmHeapEx( heap, size, MEM_HEAP_DEFAULT_ALIGNMENT );
}


/*---------------------------------------------------------------------------*
  Name:         MEMGetAllocatableSizeForFrmHeap

  Description:  Gets the maximum allocatable size in the frame heap.
                Alignment of the memory block is 4-byte fixed.

  Arguments:    heap: 	Handle for the frame heap

  Returns:      Returns the maximum allocatable size in the frame heap (in bytes).
 *---------------------------------------------------------------------------*/
static inline u32
MEMGetAllocatableSizeForFrmHeap( MEMHeapHandle heap )
{
    return MEMGetAllocatableSizeForFrmHeapEx( heap, MEM_HEAP_DEFAULT_ALIGNMENT );
}


#ifdef __cplusplus
} /* extern "C" */
#endif

/* MEM_FRAMEHEAP_H__ */
#endif
