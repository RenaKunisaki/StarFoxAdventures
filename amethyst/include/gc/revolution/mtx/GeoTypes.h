/*---------------------------------------------------------------------------*
  Project: Matrix Vector Library, Graphics library
  File:    GeoTypes.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
  
  
  $Log: GeoTypes.h,v $
  Revision 1.2  2006/02/04 11:56:47  hashida
  (none)

  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.
NoKeywords: $
    
    2     2000/01/27 4:53p Tian
    Added S16Vec type and ROMtx (reordered matrix) type.
    
    6     1999/11/16 11:45a Yasu
    Changed Quat -> Qtrn
    
    5     1999/11/15 6:44p Yasu
    Added Quat, QuatPtr
   
    
 *---------------------------------------------------------------------------*/



#include <revolution/types.h>



#ifndef __GEOTYPES_H__
#define __GEOTYPES_H__



#ifdef __cplusplus
extern "C" {
#endif


// 3D vector, point
typedef struct
{

	f32 x, y, z;

} Vec, *VecPtr, Point3d, *Point3dPtr;



// Signed 16bit 3D vector
typedef struct
{   

    s16 x;
    s16 y;
    s16 z;

}S16Vec, *S16VecPtr;



// Quaternion
typedef struct
{

	f32 x, y, z, w;

} Quaternion, *QuaternionPtr, Qtrn, *QtrnPtr;


// 3x4 matrix, pointer
typedef f32 Mtx[3][4];

typedef f32 (*MtxPtr)[4];

// 4x3 reordered matrix, pointer
typedef f32 ROMtx[4][3];

typedef f32 (*ROMtxPtr)[3];



// 4x4 matrix , pointer
// Used for projection matrix
typedef f32 Mtx44[4][4];

typedef f32 (*Mtx44Ptr)[4];




#ifdef __cplusplus
}
#endif

#endif  // __GEOTYPES_H__


