/*---------------------------------------------------------------------------*
  Project:     MEM library
  File:        expHeap.h
  Programmers: Takano Makoto

  Copyright 2005 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/

#ifndef MEM_EXPHEAP_H__
#define MEM_EXPHEAP_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <revolution/mem/heapCommon.h>

/* =======================================================================
    Constant Definitions
   ======================================================================== */

// Memory allocation direction
enum
{
    MEM_EXPHEAP_ALLOC_DIR_FRONT,    // Allocate from front
    MEM_EXPHEAP_ALLOC_DIR_REAR      // Allocate from rear
};

// Memory allocation mode
enum
{
    /*
        If this attribute value is set, the function allocates a memory block from the first unused region found that is larger than the memory block size.
        
        
    */
    MEM_EXPHEAP_ALLOC_MODE_FIRST,
    
    /*
        If this attribute value is set, the function searches for the unused region closest in size to the memory block to be allocated, and allocates the memory block from that unused region.
        
        
    */
    MEM_EXPHEAP_ALLOC_MODE_NEAR
};


/* =======================================================================
    Type Definitions
   ======================================================================== */

typedef struct MEMiExpHeapMBlockHead MEMiExpHeapMBlockHead;

// Header information for memory block
struct MEMiExpHeapMBlockHead
{
    u16                      signature;      // Signature
    union                                // Attribute
    {
        u16                      val;
        struct
        {
            u16                  allocDir  : 1;   // Memory allocation direction
            u16                  alignment : 7;   // Alignment
            u16                  groupID   : 8;   // Group ID
        }
        fields;
    }
    attribute;
    
    u32                      blockSize;      // Block size (data area only)
    
    MEMiExpHeapMBlockHead*   pMBHeadPrev;    // Previous block
    MEMiExpHeapMBlockHead*   pMBHeadNext;    // Next block
};

typedef struct MEMiExpMBlockList MEMiExpMBlockList;

// Memory block list
struct MEMiExpMBlockList
{
    MEMiExpHeapMBlockHead*   head;   // Pointer for memory block linked to header
    MEMiExpHeapMBlockHead*   tail;   // Pointer to the memory block linked to the tail of the expanded heap
};

typedef struct MEMiExpHeapHead MEMiExpHeapHead;

// Header information for expanded heap
struct MEMiExpHeapHead
{
    MEMiExpMBlockList  mbFreeList;     // Free list
    MEMiExpMBlockList  mbUsedList;     // Used list
    
    u16                groupID;        // Current group ID (lower 8 bits only)
    
    union                                // Attribute
    {
        u16                val;
        struct
        {
            u16            _reserved : 15;
            u16            allocMode :  1;    // Memory allocation mode
        }
        fields;
    }
    feature;
};



// Callback function type called for every memory block
typedef void        (*MEMHeapVisitor)( void* memBlock, MEMHeapHandle heap, u32 userParam );


/* =======================================================================
    Macro Functions
   ======================================================================== */


/* =======================================================================
    Function Prototype
   ======================================================================== */

#if defined(_DEBUG)

void        MEMiDumpExpHeap( MEMHeapHandle heap );


// #if defined(_DEBUG)
#endif


MEMHeapHandle  MEMCreateExpHeapEx(
                        void*       startAddress,
                        u32         size,
                        u16         optFlag );

void*       MEMDestroyExpHeap( MEMHeapHandle heap );

void*       MEMAllocFromExpHeapEx(
                        MEMHeapHandle heap,
                        u32           size,
                        int           alignment );

u32         MEMResizeForMBlockExpHeap(
                        MEMHeapHandle     heap,
                        void*             memBlock,
                        u32               size );

void        MEMFreeToExpHeap( MEMHeapHandle heap, void* memBlock );

u32         MEMGetTotalFreeSizeForExpHeap( MEMHeapHandle heap );

u32         MEMGetAllocatableSizeForExpHeapEx( MEMHeapHandle heap, int alignment );

BOOL        MEMiIsEmptyExpHeap( MEMHeapHandle heap );

u16         MEMSetAllocModeForExpHeap( MEMHeapHandle heap, u16 mode );

u16         MEMGetAllocModeForExpHeap( MEMHeapHandle heap );

u16         MEMSetGroupIDForExpHeap( MEMHeapHandle heap, u16 groupID );

u16         MEMGetGroupIDForExpHeap( MEMHeapHandle heap );

void        MEMVisitAllocatedForExpHeap(
                        MEMHeapHandle    heap,
                        MEMHeapVisitor   visitor,
                        u32              userParam );

u32         MEMGetSizeForMBlockExpHeap( const void* memBlock );

u16         MEMGetGroupIDForMBlockExpHeap( const void* memBlock );

u16         MEMGetAllocDirForMBlockExpHeap( const void* memBlock );

u32         MEMAdjustExpHeap( MEMHeapHandle heap );


/* =======================================================================
    Inline function
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         MEMCreateExpHeap

  Description:  Creates an expanded heap.

  Arguments:    startAddress: 	Start address of heap area
                size: 	Size of heap area

  Returns:      If the function succeeds, a handle for the created expanded heap is returned.
                If the function fails, HEAP_INVALID_HANDLE is returned.
 *---------------------------------------------------------------------------*/
static inline MEMHeapHandle
MEMCreateExpHeap(
    void*   startAddress,
    u32     size
)
{
    return MEMCreateExpHeapEx( startAddress, size, 0 );
}


/*---------------------------------------------------------------------------*
  Name:         MEMAllocFromExpHeap

  Description:  Allocates a memory block from the expanded heap.
                Alignment of the memory block is 4-byte fixed.

  Arguments:    heap:   	Handle for expanded heap
                size: 	Size of the memory block to be allocated (in bytes)

  Returns:      Returns the pointer to the allocated memory block if the memory block was successfully allocated.
                
                If the operation fails, NULL is returned.
 *---------------------------------------------------------------------------*/
static inline void*
MEMAllocFromExpHeap(
    MEMHeapHandle    heap,
    u32              size
)
{
    return MEMAllocFromExpHeapEx( heap, size, MEM_HEAP_DEFAULT_ALIGNMENT );
}

/*---------------------------------------------------------------------------*
  Name:         MEMGetAllocatableSizeForExpHeap

  Description:  Gets a memory block of the maximum allocatable size from the expanded heap.
                Alignment of the memory block is 4-byte fixed.

  Arguments:    heap:     	Handle for expanded heap

  Returns:      Returns the maximum allocatable size from the expanded heap (in bytes).
 *---------------------------------------------------------------------------*/
static inline u32
MEMGetAllocatableSizeForExpHeap( MEMHeapHandle heap )
{
    return MEMGetAllocatableSizeForExpHeapEx( heap, MEM_HEAP_DEFAULT_ALIGNMENT );
}



#if ! defined(_DEBUG)

#define     MEMCheckExpHeap( heap, optFlag )                      (TRUE)
#define     MEMCheckForMBlockExpHeap( memBlock, heap, optFlag )   (TRUE)

// #if ! defined(_DEBUG)
#else

BOOL        MEMCheckExpHeap( MEMHeapHandle heap, u32 optFlag );

BOOL        MEMCheckForMBlockExpHeap(
                    const void*       memBlock,
                    MEMHeapHandle     heap,
                    u32               optFlag );

// #if defined(_DEBUG)
#endif



#ifdef __cplusplus
} /* extern "C" */
#endif

/* MEM_EXPHEAP_H__ */
#endif
