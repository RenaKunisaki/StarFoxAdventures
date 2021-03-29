/*---------------------------------------------------------------------------*
  Project: OS - Fast F32 cast using gekko
  File:    OSFastCast.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: OSFastCast.h,v $
  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.


    11    2002/09/02 21:41 Shiki
    Clean up.

    10    2002/08/21 22:24 Hashida
    Made SN compliant by SN-Phil.

    9     2002/08/09 10:52 Hirose
    Added OSSetGQR* functions and related definitions.

    8     2002/07/03 10:29 Hirose
    Workaround for problems with CW1.3.2 strong optimization.

    7    2001/06/11 7:52p Tian
    Integrated SN changes

    6     2001/04/17 5:29p Tian
    Changed all inlines to static inline

    5     2000/07/25 7:43p Tian
    Updated to avoid GQR1, so we do not collide with Metrowerks' proposed
    Gekko ABI.

    4     2000/3/28 12:05p Tian
    Fixed typo that was accidentally checked in.  It was Paul's fault.

    3     2000/03/27 4:28p Tian
    Ifdefed out for win32

    2     2000/3/27 2:25p Tian
    Fixed bug in non-gekko code

    1     2000/03/27 2:00p Tian
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSFASTCAST_H__
#define __OSFASTCAST_H__

#ifndef _WIN32


#ifdef __cplusplus
extern "C" {
#endif
#ifdef GEKKO

// GQR formats we use
#define OS_GQR_F32 0x0000
#define OS_GQR_U8  0x0004
#define OS_GQR_U16 0x0005
#define OS_GQR_S8  0x0006
#define OS_GQR_S16 0x0007

// GQR scale factors
#define OS_GQR_SCALE_NONE   0

#define OS_GQR_SCALE_2      1
#define OS_GQR_SCALE_4      2
#define OS_GQR_SCALE_8      3
#define OS_GQR_SCALE_16     4
#define OS_GQR_SCALE_32     5
#define OS_GQR_SCALE_64     6
#define OS_GQR_SCALE_128    7
#define OS_GQR_SCALE_256    8
#define OS_GQR_SCALE_512    9
#define OS_GQR_SCALE_1024   10
#define OS_GQR_SCALE_2048   11
#define OS_GQR_SCALE_4096   12
#define OS_GQR_SCALE_8192   13
#define OS_GQR_SCALE_16384  14
#define OS_GQR_SCALE_32768  15
#define OS_GQR_SCALE_65536  16
#define OS_GQR_SCALE_MAX    31

#define OS_GQR_DIVIDE_2     63
#define OS_GQR_DIVIDE_4     62
#define OS_GQR_DIVIDE_8     61
#define OS_GQR_DIVIDE_16    60
#define OS_GQR_DIVIDE_32    59
#define OS_GQR_DIVIDE_64    58
#define OS_GQR_DIVIDE_128   57
#define OS_GQR_DIVIDE_256   56
#define OS_GQR_DIVIDE_512   55
#define OS_GQR_DIVIDE_1024  54
#define OS_GQR_DIVIDE_2048  53
#define OS_GQR_DIVIDE_4096  52
#define OS_GQR_DIVIDE_8192  51
#define OS_GQR_DIVIDE_16384 50
#define OS_GQR_DIVIDE_32768 49
#define OS_GQR_DIVIDE_65536 48
#define OS_GQR_DIVIDE_MAX   32



// The GQRs that we use for FastCast.  Note that in the future, the compiler
// will reserve GQRs 0 and 1, so we avoid using GQR1.
#define OS_FASTCAST_U8  2
#define OS_FASTCAST_U16 3
#define OS_FASTCAST_S8  4
#define OS_FASTCAST_S16 5


// Function to set up GQRs.

#ifdef __SN__
#define __OSMtGQR(gqrId, val)       \
    asm volatile ("mtspr   GQR%0,%1 # __OSMtGQR" : : "i" (gqrId), "b" (val) )
#else
#define __OSMtGQR(gqrId, val)       \
    asm                             \
    {                               \
        mtspr   GQR ## gqrId, val   \
    }
#endif

static inline void OSSetGQR2( u32 type, u32 scale )
{
    register u32 val = (((scale<<8)|type)<<16)|((scale<<8)|type);
    __OSMtGQR(2, val);
}

static inline void OSSetGQR3( u32 type, u32 scale )
{
    register u32 val = (((scale<<8)|type)<<16)|((scale<<8)|type);
    __OSMtGQR(3, val);
}

static inline void OSSetGQR4( u32 type, u32 scale )
{
    register u32 val = (((scale<<8)|type)<<16)|((scale<<8)|type);
    __OSMtGQR(4, val);
}

static inline void OSSetGQR5( u32 type, u32 scale )
{
    register u32 val = (((scale<<8)|type)<<16)|((scale<<8)|type);
    __OSMtGQR(5, val);
}

static inline void OSSetGQR6( u32 type, u32 scale )
{
    register u32 val = (((scale<<8)|type)<<16)|((scale<<8)|type);
    __OSMtGQR(6, val);
}

static inline void OSSetGQR7( u32 type, u32 scale )
{
    register u32 val = (((scale<<8)|type)<<16)|((scale<<8)|type);
    __OSMtGQR(7, val);
}


// This initializes the fast casting facility.
// GQRs 1-4 are set to load/store u8, u16, s8, s16 respectively
static inline void OSInitFastCast ( void )
{
#ifdef __SN__
    //Rena: changed syntax to stop gcc complaining
    //about missing terminating quote
          asm volatile (
        "li      3, 0x0004    \n"
        "oris    3, 3, 0x0004 \n"
        "mtspr   GQR2, 3      \n"

        "li      3, 0x0005    \n"
        "oris    3, 3, 0x0005 \n"
        "mtspr   GQR3, 3      \n"

        "li      3, 0x0006    \n"
        "oris    3, 3, 0x0006 \n"
        "mtspr   GQR4, 3      \n"

        "li      3, 0x0007    \n"
        "oris    3, 3, 0x0007 \n"
        "mtspr   GQR5, 3      \n"
    : : : "r3" );
#else
    asm
    {
        li      r3, OS_GQR_U8
        oris    r3, r3, OS_GQR_U8
        mtspr   GQR2, r3

        li      r3, OS_GQR_U16
        oris    r3, r3, OS_GQR_U16
        mtspr   GQR3, r3

        li      r3, OS_GQR_S8
        oris    r3, r3, OS_GQR_S8
        mtspr   GQR4, r3

        li      r3, OS_GQR_S16
        oris    r3, r3, OS_GQR_S16
        mtspr   GQR5, r3
    }
#endif
}


/*---------------------------------------------------------------------------*
  int to float
 *---------------------------------------------------------------------------*/

#ifdef __SN__

#define OSu8tof32(in,out)   asm volatile ("psq_l   %0, 0(%1), 1, 2  " : "=f" (*(out)) : "b" (in) )
#define OSu16tof32(in,out)  asm volatile ("psq_l   %0, 0(%1), 1, 3  " : "=f" (*(out)) : "b" (in) )
#define OSs8tof32(in,out)   asm volatile ("psq_l   %0, 0(%1), 1, 4  " : "=f" (*(out)) : "b" (in) )
#define OSs16tof32(in,out)  asm volatile ("psq_l   %0, 0(%1), 1, 5  " : "=f" (*(out)) : "b" (in) )

#else

static inline f32 __OSu8tof32(register u8* in)
{
    register f32   r;
    asm
    {
        psq_l      r, 0(in), 1, OS_FASTCAST_U8
    }
    return r;
}

static inline void OSu8tof32(register u8* in, volatile register f32* out)
{
    *out = __OSu8tof32(in);
}


static inline f32 __OSu16tof32(register u16* in)
{
    register f32   r;
    asm
    {
        psq_l      r, 0(in), 1, OS_FASTCAST_U16
    }
    return r;
}

static inline void OSu16tof32(register u16* in, volatile register f32* out)
{
    *out = __OSu16tof32(in);
}


static inline f32 __OSs8tof32(register s8* in)
{
    register f32   r;
    asm
    {
        psq_l      r, 0(in), 1, OS_FASTCAST_S8
    }
    return r;
}

static inline void OSs8tof32(register s8* in, volatile register f32* out)
{
    *out = __OSs8tof32(in);
}


static inline f32 __OSs16tof32(register s16* in)
{
    register f32   r;
    asm
    {
        psq_l      r, 0(in), 1, OS_FASTCAST_S16
    }
    return r;
}

static inline void OSs16tof32(register s16* in, volatile register f32* out)
{
    *out = __OSs16tof32(in);
}


#endif
/*---------------------------------------------------------------------------*
  float to int

  Note that due to a compiler bug, we need to use addresses for the FP
  value.  Theoretically, we can simply use psq_st to store out values, but
  the compiler does not recognize the psq_st as touching static values, and may
  compile out static values.
 *---------------------------------------------------------------------------*/
#ifdef __SN__

#define OSf32tou8(in,out)   asm volatile ("psq_st   %1, 0(%0), 1, 2 " : : "b" (out) , "f" (*(in)) : "memory")
#define OSf32tou16(in,out)  asm volatile ("psq_st   %1, 0(%0), 1, 3 " : : "b" (out) , "f" (*(in)) : "memory")
#define OSf32tos8(in,out)   asm volatile ("psq_st   %1, 0(%0), 1, 4 " : : "b" (out) , "f" (*(in)) : "memory")
#define OSf32tos16(in,out)  asm volatile ("psq_st   %1, 0(%0), 1, 5 " : : "b" (out) , "f" (*(in)) : "memory")

#else

static inline u8 __OSf32tou8(register f32 in)
{
    f32           a;
    register f32* ptr = &a;
    register u8   r;

    asm
    {
        psq_st  in, 0(ptr), 1, OS_FASTCAST_U8
        lbz     r, 0(ptr)
    }
    return r;
}

static inline void OSf32tou8(register f32* in, volatile register u8* out)
{
    *out = __OSf32tou8(*in);
}


static inline u16 __OSf32tou16(register f32 in)
{
    f32           a;
    register f32* ptr = &a;
    register u16  r;

    asm
    {
        psq_st  in, 0(ptr), 1, OS_FASTCAST_U16
        lhz     r, 0(ptr)
    }
    return r;
}

static inline void OSf32tou16(register f32* in, volatile register u16* out)
{
    *out = __OSf32tou16(*in);
}


static inline s8 __OSf32tos8(register f32 in)
{
    f32           a;
    register f32* ptr = &a;
    register s8   r;

    asm
    {
        psq_st  in, 0(ptr), 1, OS_FASTCAST_S8
        lbz     r, 0(ptr)
        extsb   r, r
    }
    return r;
}

static inline void OSf32tos8(register f32* in, volatile register s8* out)
{
    *out = __OSf32tos8(*in);
}


static inline s16 __OSf32tos16(register f32 in)
{
    f32           a;
    register f32* ptr = &a;
    register s16  r;

    asm
    {
        psq_st  in, 0(ptr), 1, OS_FASTCAST_S16
        lha     r, 0(ptr)
    }
    return r;
}

static inline void OSf32tos16(register f32* in, volatile register s16* out)
{
    *out = __OSf32tos16(*in);
}

#endif

#else
/*---------------------------------------------------------------------------*
  Non-Gekko code
 *---------------------------------------------------------------------------*/
static inline void OSInitFastCast ( void )
{
    while(0)
    {
    }
}


/*---------------------------------------------------------------------------*
  int to float
 *---------------------------------------------------------------------------*/
static inline void OSs16tof32(s16* in, f32* out)
{
    *out = (f32) *in;
}

static inline void OSs8tof32(s8* in, f32* out)
{
    *out = (f32) *in;
}


static inline void OSu16tof32(u16* in, f32* out)
{
    *out = (f32) *in;
}


static inline void OSu8tof32(u8* in, f32* out)
{
    *out = (f32) *in;
}

/*---------------------------------------------------------------------------*
  float to int
 *---------------------------------------------------------------------------*/

static inline void OSf32tou8(register f32* in, register u8* out)
{
    *out = (u8)*in;
}

static inline void OSf32tou16(register f32* in, register u16* out)
{
    *out = (u16)*in;
}

static inline void OSf32tos8(register f32* in, register s8* out)
{
    *out = (s8)*in;
}

static inline void OSf32tos16(register f32* in, register s16* out)
{
    *out = (s16)*in;
}


#endif // GEKKO

#ifdef __cplusplus
}
#endif
#endif

#endif // _WIN32
