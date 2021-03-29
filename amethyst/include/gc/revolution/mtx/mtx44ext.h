/*---------------------------------------------------------------------------*
  Project: Matrix Vector Library
  File:    mtx44ext.h

  Copyright 1998-2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.


  $Log: mtx44ext.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    4     2002/04/11 13:08 Hirose
    const type specifier support. (worked by hiratsu@IRD)
    
    3     2001/08/30 10:37p Hirose
    Added MTX44Inverse.
    
    2     2001/07/30 10:17p Hirose
    Added model matrix and matrix-vector stuff.
    
    1     2001/07/24 5:58p Hirose
    Initial check in. This file will contain function prototypes for 4x4
    matrix extensions.
    
  $NoKeywords: $
  
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    Matrix-Vector Library : 4x4 matrix extension
 *---------------------------------------------------------------------------*/

#ifndef __MTX44EXT_H__
#define __MTX44EXT_H__


#include <revolution/mtx.h>


#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Default function binding configuration
 *---------------------------------------------------------------------------*/
/*
(Defined in mtx.h)

// [Binding Rule]
//
// "MTX_USE_PS" -> When this flag is specified, it uses PS* (Paired-Single
//                 assembler code) functions for non-prefixed function calls.
// "MTX_USE_C " -> When this flag is specified, it uses C_* (C code) functions
//                 for non-prefixed function calls.
//
// If both are specified, it will be treated as MTX_USE_PS.
// If nothing is specified, NDEBUG build refers PS* functions and
// DEBUG build uses C_* functions.

// For non-Gekko HW (e.g. emulator)
#ifndef GEKKO
#define MTX_USE_C
#undef  MTX_USE_PS
#endif

#if ( !defined(MTX_USE_PS) && !defined(MTX_USE_C) )
#ifndef _DEBUG
#define MTX_USE_PS
#endif
#endif
*/

/*---------------------------------------------------------------------------*
    4x4 GENERAL MATRIX SECTION
 *---------------------------------------------------------------------------*/
// C version
void    C_MTX44Identity         ( Mtx44 m );
void    C_MTX44Copy             ( const Mtx44 src, Mtx44 dst );
void    C_MTX44Concat           ( const Mtx44 a, const Mtx44 b, Mtx44 ab );
void    C_MTX44Transpose        ( const Mtx44 src, Mtx44 xPose );
u32     C_MTX44Inverse          ( const Mtx44 src, Mtx44 inv );

// PS assembler version
#ifdef GEKKO
void    PSMTX44Identity         ( Mtx44 m );
void    PSMTX44Copy             ( const Mtx44 src, Mtx44 dst );
void    PSMTX44Concat           ( const Mtx44 a, const Mtx44 b, Mtx44 ab );
void    PSMTX44Transpose        ( const Mtx44 src, Mtx44 xPose );
#endif

// Bindings
#ifdef MTX_USE_PS
#define MTX44Identity           PSMTX44Identity
#define MTX44Copy               PSMTX44Copy
#define MTX44Concat             PSMTX44Concat
#define MTX44Transpose          PSMTX44Transpose
#else // MTX_USE_C
#define MTX44Identity           C_MTX44Identity
#define MTX44Copy               C_MTX44Copy
#define MTX44Concat             C_MTX44Concat
#define MTX44Transpose          C_MTX44Transpose
#endif

#define MTX44Inverse            C_MTX44Inverse

/*---------------------------------------------------------------------------*
    MODEL MATRIX SECTION
 *---------------------------------------------------------------------------*/
// C version
void    C_MTX44Trans            ( Mtx44 m, f32 xT, f32 yT, f32 zT );
void    C_MTX44TransApply       ( const Mtx44 src, Mtx44 dst, f32 xT, f32 yT, f32 zT );
void    C_MTX44Scale            ( Mtx44 m, f32 xS, f32 yS, f32 zS );
void    C_MTX44ScaleApply       ( const Mtx44 src, Mtx44 dst, f32 xS, f32 yS, f32 zS );

void    C_MTX44RotRad           ( Mtx44 m, char axis, f32 rad );
void    C_MTX44RotTrig          ( Mtx44 m, char axis, f32 sinA, f32 cosA );
void    C_MTX44RotAxisRad       ( Mtx44 m, const Vec *axis, f32 rad);

// PS assembler version
#ifdef GEKKO
void    PSMTX44Trans            ( Mtx44 m, f32 xT, f32 yT, f32 zT );
void    PSMTX44TransApply       ( const Mtx44 src, Mtx44 dst, f32 xT, f32 yT, f32 zT );
void    PSMTX44Scale            ( Mtx44 m, f32 xS, f32 yS, f32 zS );
void    PSMTX44ScaleApply       ( const Mtx44 src, Mtx44 dst, f32 xS, f32 yS, f32 zS );

void    PSMTX44RotRad           ( Mtx44 m, char axis, f32 rad );
void    PSMTX44RotTrig          ( Mtx44 m, char axis, f32 sinA, f32 cosA );
void    PSMTX44RotAxisRad       ( Mtx44 m, const Vec *axis, f32 rad);
#endif

// Bindings
#ifdef MTX_USE_PS
#define MTX44Trans              PSMTX44Trans
#define MTX44TransApply         PSMTX44TransApply
#define MTX44Scale              PSMTX44Scale
#define MTX44ScaleApply         PSMTX44ScaleApply

#define MTX44RotRad             PSMTX44RotRad
#define MTX44RotTrig            PSMTX44RotTrig
#define MTX44RotAxisRad         PSMTX44RotAxisRad

#else // MTX_USE_C
#define MTX44Trans              C_MTX44Trans
#define MTX44TransApply         C_MTX44TransApply
#define MTX44Scale              C_MTX44Scale
#define MTX44ScaleApply         C_MTX44ScaleApply

#define MTX44RotRad             C_MTX44RotRad
#define MTX44RotTrig            C_MTX44RotTrig
#define MTX44RotAxisRad         C_MTX44RotAxisRad
#endif

/*---------------------------------------------------------------------------*
    MATRIX-VECTOR SECTION
 *---------------------------------------------------------------------------*/
// C version
void    C_MTX44MultVec          ( const Mtx44 m, const Vec *src, Vec *dst );
void    C_MTX44MultVecArray     ( const Mtx44 m, const Vec *srcBase, Vec *dstBase, u32 count );
void    C_MTX44MultVecSR        ( const Mtx44 m, const Vec *src, Vec *dst );
void    C_MTX44MultVecArraySR   ( const Mtx44 m, const Vec *srcBase, Vec *dstBase, u32 count );

// PS assembler version
#ifdef GEKKO
void    PSMTX44MultVec          ( const Mtx44 m, const Vec *src, Vec *dst );
void    PSMTX44MultVecArray     ( const Mtx44 m, const Vec *srcBase, Vec *dstBase, u32 count );
void    PSMTX44MultVecSR        ( const Mtx44 m, const Vec *src, Vec *dst );
void    PSMTX44MultVecArraySR   ( const Mtx44 m, const Vec *srcBase, Vec *dstBase, u32 count );
#endif

// Bindings
#ifdef MTX_USE_PS
#define MTX44MultVec            PSMTX44MultVec
#define MTX44MultVecArray       PSMTX44MultVecArray
#define MTX44MultVecSR          PSMTX44MultVecSR
#define MTX44MultVecArraySR     PSMTX44MultVecArraySR
#else // MTX_USE_C
#define MTX44MultVec            C_MTX44MultVec
#define MTX44MultVecArray       C_MTX44MultVecArray
#define MTX44MultVecSR          C_MTX44MultVecSR
#define MTX44MultVecArraySR     C_MTX44MultVecArraySR
#endif


/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif // __MTX44EXT_H__

/*===========================================================================*/

