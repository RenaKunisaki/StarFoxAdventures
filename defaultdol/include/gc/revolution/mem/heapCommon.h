/*---------------------------------------------------------------------------*
  Project:     MEM library
  File:        heapCommon.h
  Programmers: Takano Makoto

  Copyright 2005 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
 *---------------------------------------------------------------------------*/
#ifndef MEM_HEAPCOMMON_H__
#define MEM_HEAPCOMMON_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <revolution/types.h>
#include <revolution/os.h>
#include <revolution/mem/list.h>


/* ========================================================================
    Macro Constants
   ======================================================================== */

// Invalid heap handle
#define MEM_HEAP_INVALID_HANDLE      NULL

// Default alignment size when memory is allocated from heap
#define MEM_HEAP_DEFAULT_ALIGNMENT   4


/* ------------------------------------------------------------------------
    Fill-related
   ------------------------------------------------------------------------ */

// Zero out memory when memory is allocated.
#define MEM_HEAP_OPT_0_CLEAR         (1 << 0)

// Memory fill when heap is created, memory is allocated, or memory is freed.
#define MEM_HEAP_OPT_DEBUG_FILL      (1 << 1)

// Presence or absence of exclusive control
#define MEM_HEAP_OPT_THREAD_SAFE     (1 << 2)


/* ------------------------------------------------------------------------
    Heap check related
   ------------------------------------------------------------------------ */
//  If this bit stands, output error
#define MEM_HEAP_ERROR_PRINT         (1 << 0)



// Signature of expanded heap
#define MEMi_EXPHEAP_SIGNATURE       ('EXPH')
// Signature of frame heap
#define MEMi_FRMHEAP_SIGNATURE       ('FRMH')
// Signature of unit heap
#define MEMi_UNTHEAP_SIGNATURE       ('UNTH')

/* ========================================================================
    enum constant
   ======================================================================== */

typedef enum
{
    MEM_HEAP_TYPE_EXP,      // Expanded heap
    MEM_HEAP_TYPE_FRM,      // Frame heap
    MEM_HEAP_TYPE_UNIT,     // Unit heap
    MEM_HEAP_TYPE_UNKNOWN   // Unknown heap handle
}
MEMHeapType;

enum 
{
    MEM_HEAP_FILL_NOUSE,    // When debug fill is not used
    MEM_HEAP_FILL_ALLOC,    // When debug fill is allocated
    MEM_HEAP_FILL_FREE,     // When debug fill is freed
    
    MEM_HEAP_FILL_MAX
};



/* =======================================================================
    Type Definitions
   ======================================================================== */

typedef struct MEMiHeapHead MEMiHeapHead;

// Header common among heaps
struct MEMiHeapHead
{
    u32       signature;
    
    MEMLink   link;
    MEMList   childList;
    
    void*     heapStart;      // Heap start address
    void*     heapEnd;        // Heap end (+1) address
    OSMutex   mutex;          // For exclusive use between threads
    
    union                     // Attribute
    {
        u32       val;
        struct
        {
            u32   _reserved : 24;
            u32   optFlag   :  8; // Option flag.
        }
        fields;
    }
    attribute;
};


typedef MEMiHeapHead* MEMHeapHandle;   // Type to represent heap handle


/* =======================================================================
    Function Prototype
   ======================================================================== */

MEMHeapHandle    MEMFindContainHeap( const void* memBlock );

MEMHeapHandle    MEMFindParentHeap( MEMHeapHandle heap );


/* ========================================================================
    Inline function
   ======================================================================== */

/*---------------------------------------------------------------------------*
  Name:         MEMGetHeapStartAddress

  Description:  Get start address of memory area used by heap

  Arguments:    heap: 	Heap handle

  Returns:      Return start address of memory area used by heap
 *---------------------------------------------------------------------------*/
static inline void*
MEMGetHeapStartAddress( MEMHeapHandle heap )
{
    return (void*)heap;
}

/*---------------------------------------------------------------------------*
  Name:         MEMGetHeapEndAddress

  Description:  Get end address +1 of memory area used by heap

  Arguments:    heap: 	Heap handle

  Returns:      Return end address +1 of memory area used by heap
 *---------------------------------------------------------------------------*/
static inline void*
MEMGetHeapEndAddress( MEMHeapHandle heap )
{
    return heap->heapEnd;
}

/*---------------------------------------------------------------------------*
  Name:         MEMGetHeapTotalUsableSize

  Description:  Gets the memory size allocated to the heap.
                The header is not included.

  Arguments:    heap: 	Heap handle

  Returns:      Returns the memory size allocated to the heap. The header is not included.
 *---------------------------------------------------------------------------*/
static inline s32
MEMGetHeapTotalUsableSize( MEMHeapHandle heap )
{
    return ((s32)(heap->heapEnd) - (s32)(heap->heapStart));
}

/*---------------------------------------------------------------------------*
  Name:         MemGetHeapTotalSize

  Description:  Gets the memory size allocated to the heap (including that of the header).
                

  Arguments:    heap:    	heap handle

  Returns:      Returns the memory size allocated to the heap (including that of the header).
                
 *---------------------------------------------------------------------------*/
static inline s32
MEMGetHeapTotalSize( MEMHeapHandle heap )
{
    return ((s32)(heap->heapEnd) - (s32)(heap));
}


/*---------------------------------------------------------------------------*
  Name:         MEMGetHeapType

  Description:  Gets the type of heap specific to the heap handle.

  Arguments:    heap: 	Heap handle

  Returns:      Gets the heap type from the heap handle.
 *---------------------------------------------------------------------------*/
static inline MEMHeapType
MEMGetHeapType( MEMHeapHandle heap )
{
    switch ( heap->signature )
    {
    case MEMi_EXPHEAP_SIGNATURE:    return MEM_HEAP_TYPE_EXP;
    case MEMi_FRMHEAP_SIGNATURE:    return MEM_HEAP_TYPE_FRM;
    case MEMi_UNTHEAP_SIGNATURE:    return MEM_HEAP_TYPE_UNIT;
    default:                        return MEM_HEAP_TYPE_UNKNOWN;
    }
}


/*---------------------------------------------------------------------------*
  Name:         MEMIsExpHeap

  Description:  Determines whether the heap handle is for an expanded heap.

  Arguments:    heap:    	heap handle

  Returns:      Returns TRUE if an expanded heap.
                Else returns FALSE.
 *---------------------------------------------------------------------------*/
static inline BOOL
MEMIsExpHeap( MEMHeapHandle heap )
{
    return ( heap->signature == MEMi_EXPHEAP_SIGNATURE );
}


/*---------------------------------------------------------------------------*
  Name:         MEMIsFrmHeap

  Description:  Determines whether the heap handle is for a frame heap.

  Arguments:    heap:    	heap handle

  Returns:      Returns TRUE if a frame heap.
                Else returns FALSE.
 *---------------------------------------------------------------------------*/
static inline BOOL
MEMIsFrmHeap( MEMHeapHandle heap )
{
    return ( heap->signature == MEMi_FRMHEAP_SIGNATURE );
}


/*---------------------------------------------------------------------------*
  Name:         MEMIsUnitHeap

  Description:  Determines whether the heap handle is for a unit heap.

  Arguments:    heap:    	heap handle

  Returns:      Returns TRUE if a unit heap.
                Else returns FALSE.
 *---------------------------------------------------------------------------*/
static inline BOOL
MEMIsUnitHeap( MEMHeapHandle heap )
{
    return ( heap->signature == MEMi_UNTHEAP_SIGNATURE );
}



/* =======================================================================
    Debug Functions
   ======================================================================== */
#if ! defined(_DEBUG)

#define   MEMDumpHeap( heap )                  ((void)0)
#define   MEMSetFillValForHeap( type, val )    (0)
#define   MEMGetFillValForHeap( type )         (0)

#else

void      MEMDumpHeap( MEMHeapHandle heap );

u32       MEMSetFillValForHeap( int type, u32 val );

u32       MEMGetFillValForHeap( int type );

#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

/* MEM_HEAPCOMMON_H__ */
#endif

