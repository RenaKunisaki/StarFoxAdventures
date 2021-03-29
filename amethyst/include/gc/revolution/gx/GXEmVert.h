/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXEmVert.h

  Copyright 1998, 1999 Nintendo. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: GXEmVert.h,v $
  Revision 1.1.1.1  2005/12/29 06:53:28  hiratsu
  Initial import.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Ported from dolphin source tree.

    
    1     1999/11/17 6:26p Carl
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

/*>*******************************(*)*******************************<*/
// This file is for the emulator.
// It is included from GXVert.h, and thus it must be placed in the
// same directory as that file.
/*>*******************************(*)*******************************<*/

/*>*******************************(*)*******************************<*/
// These calls are not implemented.  They're just dummies.
// They don't need to be implemented, in fact.  They're only
// defined here to make GXVert.h work.
/*>*******************************(*)*******************************<*/
void EmGXCmd1u8( u8 x );
void EmGXCmd1u16( u16 x );
void EmGXCmd1u32( u32 x );
void EmGXParam1u8( u8 x );
void EmGXParam1u16( u16 x );
void EmGXParam1u32( u32 x );
void EmGXParam1s8( s8 x );
void EmGXParam1s16( s16 x );
void EmGXParam1s32( s32 x );
void EmGXParam1f32( f32 x );
void EmGXParam3f32( f32 x, f32 y, f32 z );
void EmGXParam4f32( f32 x, f32 y, f32 z, f32 w );

/*>*******************************(*)*******************************<*/
// The rest of these calls are implemented in the emulator.
/*>*******************************(*)*******************************<*/
void EmGXPosition3f32         ( float x, float y, float z );
void EmGXPosition3u16         ( u16 x, u16 y, u16 z );
void EmGXPosition3s16         ( s16 x, s16 y, s16 z );
void EmGXPosition3u8          ( u8 x, u8 y, u8 z );
void EmGXPosition3s8          ( s8 x, s8 y, s8 z );
void EmGXPosition2f32         ( float x, float y );
void EmGXPosition2u16         ( u16 x, u16 y );
void EmGXPosition2s16         ( s16 x, s16 y );
void EmGXPosition2u8          ( u8 x, u8 y );
void EmGXPosition2s8          ( s8 x, s8 y );
void EmGXPosition1x16         ( u16 idx );
void EmGXPosition1x8          ( u8 idx );

/*>*******************************(*)*******************************<*/
void EmGXNormal3f32           ( f32 x, f32 y, f32 z );
void EmGXNormal3s16           ( s16 x, s16 y, s16 z );
void EmGXNormal3s8            ( s8 x, s8 y, s8 z );
void EmGXNormal1x16           ( u16 idx );
void EmGXNormal1x8            ( u8 idx );

/*>*******************************(*)*******************************<*/
void EmGXColor4u8             ( u8 r, u8 g, u8 b, u8 a );
void EmGXColor3u8             ( u8 r, u8 g, u8 b );
void EmGXColor1u32            ( u32 rgba );
void EmGXColor1u16            ( u16 rgba );
void EmGXColor1x16            ( u16 idx );
void EmGXColor1x8             ( u8 idx );

/*>*******************************(*)*******************************<*/
void EmGXTexCoord2f32         ( f32 s, f32 t );
void EmGXTexCoord2s16         ( s16 s, s16 t );
void EmGXTexCoord2u16         ( u16 s, u16 t );
void EmGXTexCoord2s8          ( s8 s, s8 t );
void EmGXTexCoord2u8          ( u8 s, u8 t );
void EmGXTexCoord1f32         ( f32 s );
void EmGXTexCoord1s16         ( s16 s );
void EmGXTexCoord1u16         ( u16 s );
void EmGXTexCoord1s8          ( s8 s );
void EmGXTexCoord1u8          ( u8 s );
void EmGXTexCoord1x16         ( u16 idx );
void EmGXTexCoord1x8          ( u8 idx );

/*>*******************************(*)*******************************<*/
void EmGXMatrixIndex1u8       ( u32 idx );

/********************************/
