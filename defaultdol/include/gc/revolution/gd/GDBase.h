/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     GDBase.h

  Copyright 2001 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GDBase.h,v $
  Revision 1.3  2006/03/20 11:22:57  hiratsu
  Fixed unsafe macro.

  Revision 1.2  2006/02/04 11:56:46  hashida
  (none)

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2002/08/05 19:49 Hirose
    Const type specifier support.
    
    3     2001/09/19 3:28p Carl
    Changed overflow checking in functions that alter the offset.
    
    2     2001/09/12 3:56p Carl
    Renamed some functions to make it clear which act on "current".
    Added some new defines for command lengths.
    
    1     2001/09/12 1:55p Carl
    Initial revision of GD: Graphics Display List Library.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GDBASE_H__
#define __GDBASE_H__

/*---------------------------------------------------------------------------*/
#include <revolution/types.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXCommandList.h>
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

// Graphics Display-list object
//
// Note that no "end of data" marker is kept.  If you modify the ptr
// and intend to later reset it to the end of data, you must keep track
// of that value prior to changing the ptr.
//
typedef struct _GDLObj {
    u8  *start;
    u32  length;
    u8  *ptr;
    u8  *top;
} GDLObj;

typedef void (*GDOverflowCallback)(void);

extern GDLObj *__GDCurrentDL;

/****************************************************************************/
/* Function Declarations */
/****************************************************************************/

// Initialize/get GDL object parameters.
void                GDInitGDLObj(GDLObj *dl, void *start, u32 length);
static inline void *GDGetGDLObjStart(const GDLObj *dl);
static inline u32   GDGetGDLObjLength(const GDLObj *dl);
static inline void *GDGetGDLObjPointer(const GDLObj *dl);
static inline u32   GDGetGDLObjOffset(const GDLObj *dl);  // For convenience
    
// Manipulate "current" GDL object.
static inline void    GDSetCurrent(GDLObj *dl);
static inline GDLObj *GDGetCurrent();
static inline GXBool  GDIsCurrent(const GDLObj *dl);

// This next series of functions all apply to the "current" GDL object.

// Makes sure current GDL is flushed out of cache (needed for HW-side only).
void GDFlushCurrToMem();

// Get the current offset of the write ptr within DL.
static inline u32  GDGetCurrOffset();

// Reposition the write ptr within current GDL.
static inline void GDSetCurrOffset(u32 offset);

// Move offset forward by delta for current GDL.
static inline void GDAdvCurrOffset(s32 delta);

// For cache-flushing purposes, return actual pointer (start + offset).
static inline void *GDGetCurrPointer();

// Convenience functions for getting current GDLObj parameters.
static inline void *GDGetCurrStart();
static inline u32   GDGetCurrLength();

// Pads current GDL out to 32B.
void        GDPadCurr32();

// Overflow-related functions.
void        GDSetOverflowCallback(GDOverflowCallback callback);

GDOverflowCallback GDGetOverflowCallback( void );

//----------------------------------------------------------------------------
// "Internal" Functions
//----------------------------------------------------------------------------

void               GDOverflowed();
static inline void GDOverflowCheck(u32 size);

// Raw write-byte function: No overflow checking
static inline void __GDWrite(u8 data);

// Low-level functions for writing every kind of data
static inline void GDWrite_u8(u8 data);
static inline void GDWrite_s8(s8 data);
static inline void GDWrite_u16(u16 data);
static inline void GDWrite_s16(s16 data);
static inline void GDWrite_u32(u32 data);
static inline void GDWrite_s32(s32 data);
static inline void GDWrite_f32(f32 data);
static inline void GDWrite_u24(u32 data);

// Mid-level write functions for writing specific command structures
static inline void GDWriteBPCmd(u32 regval);
static inline void GDWriteCPCmd(u8 addr, u32 val);
static inline void GDWriteXFCmd(u16 addr, u32 val);
static inline void GDWriteXFCmdHdr(u16 addr, u8 len);
static inline void GDWriteXFIndxACmd(u16 addr, u8 len, u16 index);
static inline void GDWriteXFIndxBCmd(u16 addr, u8 len, u16 index);
static inline void GDWriteXFIndxCCmd(u16 addr, u8 len, u16 index);
static inline void GDWriteXFIndxDCmd(u16 addr, u8 len, u16 index);

/****************************************************************************/
/* Inline Function Definitions */
/****************************************************************************/

// "Get" (from object) functions
static inline void *GDGetGDLObjStart(const GDLObj *dl)
{
    return (void *) dl->start;
}
        
static inline u32 GDGetGDLObjLength(const GDLObj *dl)
{
    return dl->length;
}

static inline void *GDGetGDLObjPointer(const GDLObj *dl)
{
    return dl->ptr;
}

static inline u32 GDGetGDLObjOffset(const GDLObj *dl)
{
    return (u32) (dl->ptr - dl->start);
}

// Manipulate "current" DL buffer
static inline void GDSetCurrent(GDLObj *dl)
{
    __GDCurrentDL = dl;
}

static inline GDLObj *GDGetCurrent()
{
    return __GDCurrentDL;
}

static inline GXBool GDIsCurrent(const GDLObj *dl)
{
    return (dl == __GDCurrentDL);
}

// Get the current offset of the write ptr within DL
static inline u32 GDGetCurrOffset()
{
    return (u32) (__GDCurrentDL->ptr - __GDCurrentDL->start);
}

// Reposition the write ptr within DL
static inline void GDSetCurrOffset(u32 offset)
{
    // Note: we do not check the validity of offset here.
    // That is done only when you try to write data into the DL.
    // You can change this if you find it necessary.
    __GDCurrentDL->ptr = __GDCurrentDL->start + offset;
}

// Move offset forward by delta
static inline void GDAdvCurrOffset(s32 delta)
{
    // Note: see the note in GDSetCurrOffset.
    __GDCurrentDL->ptr += delta;
}

// For cache-flushing purposes, return actual pointer (start + offset)
static inline void *GDGetCurrPointer()
{
    return (void *) __GDCurrentDL->ptr;
}

// Convenience functions
static inline void *GDGetCurrStart()
{
    return (void *) __GDCurrentDL->start;
}

static inline u32 GDGetCurrLength()
{
    return __GDCurrentDL->length;
}

// Overflow check
static inline void GDOverflowCheck(u32 size)
{
    if (__GDCurrentDL->ptr + size > __GDCurrentDL->top)
    {
        GDOverflowed();
    }
}

// Raw write-byte function: No overflow checking
static inline void __GDWrite(u8 data)
{
    *__GDCurrentDL->ptr++ = data;
}

// Low-level functions for writing every kind of data
//----------------------------------------------------------------------------

static inline void GDWrite_u8(u8 data)
{
    GDOverflowCheck(1);
    __GDWrite(data);
}

static inline void GDWrite_s8(s8 data)
{
    GDWrite_u8((u8) data);
}

static inline void GDWrite_u16(u16 data)
{
    GDOverflowCheck(2);
    __GDWrite((u8) (data >> 8));
    __GDWrite((u8) (data & 0xff));
}

static inline void GDWrite_s16(s16 data)
{
    GDWrite_u16((u16) data);
}

static inline void GDWrite_u32(u32 data)
{
    GDOverflowCheck(4);
    __GDWrite((u8) ((data >> 24) & 0xff));
    __GDWrite((u8) ((data >> 16) & 0xff));
    __GDWrite((u8) ((data >>  8) & 0xff));
    __GDWrite((u8) ((data >>  0) & 0xff));
}

static inline void GDWrite_s32(s32 data)
{
    GDWrite_u32((u32) data);
}

static inline void GDWrite_f32(f32 data)
{
    typedef union {
        f32 f;
        u32 u;
    } _FloatInt;

    _FloatInt fid;

    fid.f = data;

    GDWrite_u32(fid.u);
}

static inline void GDWrite_u24(u32 data)
{
    GDOverflowCheck(3);
    __GDWrite((u8) ((data >> 16) & 0xff));
    __GDWrite((u8) ((data >>  8) & 0xff));
    __GDWrite((u8) ((data >>  0) & 0xff));
}

// Mid-level write functions for writing specific command structures
//----------------------------------------------------------------------------

// This is for sending BP (raster state) commands.
// All BP commands have the form:
//
// | 8 bits     | 8 bits     | 24 bits    |
// | cmd. token | reg. addr. | reg. value |
//
// We combine the register address and value together into a single u32 value.

static inline void GDWriteBPCmd(u32 regval)
{
    GDWrite_u8(GX_LOAD_BP_REG);
    GDWrite_u32(regval);
}

// This is for sending CP (command processor) commands.
// All CP commands have the form:
//
// | 8 bits     | 8 bits     | 32 bits    |
// | cmd. token | reg. addr. | reg. value |
//
// CP has a different register address space than BP or XF.

static inline void GDWriteCPCmd(u8 addr, u32 val)
{
    GDWrite_u8(GX_LOAD_CP_REG);
    GDWrite_u8(addr);
    GDWrite_u32(val);
}

// This is for sending immediate XF (transform unit) commands.
// All immediate XF commands have the form:
//
// | 8 bits     | 16 bits    | 16 bits    | 32 bits * length |
// | cmd. token | length - 1 | 1st addr.  | reg. value(s)    |
//
// Length (the number of values being sent) can be up to 16 only.
//
// XF has a different register address space than BP or CP.

static inline void GDWriteXFCmd(u16 addr, u32 val)
{
    GDWrite_u8(GX_LOAD_XF_REG);
    GDWrite_u16(0); // 0 means one value follows
    GDWrite_u16(addr);
    GDWrite_u32(val);
}

// When using this function, follow it with (len) 32-bit writes.
static inline void GDWriteXFCmdHdr(u16 addr, u8 len)
{
    GDWrite_u8(GX_LOAD_XF_REG);
    GDWrite_u16((u16) ((len) - 1));
    GDWrite_u16(addr);
}

// This is used internally below for XF indexed load commands:
#define __XFAddrLen(addr, len) ((u16) (((len) << 12) | (addr)))

// This is for sending indexed XF (transform unit) commands.
// There are 4 different XF index units, which are typically used as follows:
// A: pos. mtx's  B: nrm. mtx's  C: tex. mtx's  D: light obj's.
// All indexed XF commands have the form:
//
// | 8 bits     | 16 bits     | 4 bits     | 12 bits    |
// | cmd. token | index value | length - 1 | 1st addr.  |
//
// Length (the number of values being sent) can be up to 16 only.
//
// XF has a different register address space than BP or CP.

static inline void GDWriteXFIndxACmd(u16 addr, u8 len, u16 index)
{
    GDWrite_u8(GX_LOAD_INDX_A);
    GDWrite_u16(index);
    GDWrite_u16(__XFAddrLen((addr), ((len)-1)) );
}

static inline void GDWriteXFIndxBCmd(u16 addr, u8 len, u16 index)
{
    GDWrite_u8(GX_LOAD_INDX_B);
    GDWrite_u16(index);
    GDWrite_u16(__XFAddrLen((addr), ((len)-1)) );
}

static inline void GDWriteXFIndxCCmd(u16 addr, u8 len, u16 index)
{
    GDWrite_u8(GX_LOAD_INDX_C);
    GDWrite_u16(index);
    GDWrite_u16(__XFAddrLen((addr), ((len)-1)) );
}

static inline void GDWriteXFIndxDCmd(u16 addr, u8 len, u16 index)
{
    GDWrite_u8(GX_LOAD_INDX_D);
    GDWrite_u16(index);
    GDWrite_u16(__XFAddrLen((addr), ((len)-1)) );
}

/*---------------------------------------------------------------------------*/

// This is the BP mask register that may be used to limit which bits
// of a BP register get written to.  It only affects the very next BP
// command that follows it (the mask automatically resets).  It is sent
// down as a BP command itself: GDWriteBPCmd(SS_MASK(mask));

#define SS_MASK(mask) ((u32)((0xfe << 24) | (mask)))

// These defines are intended to help make patching more clear.
// The CMD_LENGTH describes the total length of each basic command type.
// (For XF commands, this only applies to commands where length = 1.)
// The DATA_OFFSET describes the number of bytes to skip from the start
// of the command until you reach the actual register data value.
// The DATA_LENGTH describes how many bytes long the register data value is.
// In the case of XF commands, this is the minimum multiple (length = 1).

#define BP_CMD_LENGTH  5
#define BP_DATA_OFFSET 2
#define BP_DATA_LENGTH 3

#define CP_CMD_LENGTH  6
#define CP_DATA_OFFSET 2
#define CP_DATA_LENGTH 4

#define XF_CMD_LENGTH  9
#define XF_DATA_OFFSET 5
#define XF_DATA_LENGTH 4

// For XF indexed commands, we describe the index field offset & length.

#define XF_INDX_CMD_LENGTH 5
#define XF_INDX_OFFSET 1
#define XF_INDX_LENGTH 2

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------------*/

#endif // __GDBASE_H__
