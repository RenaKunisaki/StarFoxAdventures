/*---------------------------------------------------------------------------*
  Project:  Revolution Audio encode/decode library for controller speaker
  File:     wenc.h

  Copyright (C) 2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wenc.h,v $
  Revision 1.4  09/07/2006 08:29:04  aka
  Modified WENCGetEncodeBufferSize().

  Revision 1.3  08/10/2006 02:29:54  aka
  Changed WENCGetEncodeBufferSize().

  Revision 1.2  08/09/2006 00:39:55  aka
  Changed an argument.

  Revision 1.1  07/18/2006 05:44:31  aka
  Initial check-in.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __WENC_H__
#define __WENC_H__

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------
// encode flag definition
//----------------------------------------------------------------------------
#define WENC_FLAG_FIRST  0x00000000
#define WENC_FLAG_CONT   0x00000001
#define WENC_FLAG_LAST   0x00000002

//----------------------------------------------------------------------------
// encode information structure
//----------------------------------------------------------------------------
typedef struct
{
    u8   data[32];
    
} WENCInfo;
    
//----------------------------------------------------------------------------
// Function:
//              WENCGetEncodeBufferSize()
// Input:
//              nSampleNum:                   number of PCM data.
// Return:
//              output buffer size [byte].
// Description:
//              calculate output buffer size (byte) which application need to
//              alloc before calling WENCGetEncodeData().
//----------------------------------------------------------------------------
#define WENCGetEncodeBufferSize( nSampleNum )  (((nSampleNum) + 1) / 2)
    
//----------------------------------------------------------------------------
// Function:
//              WENCGetEncodeData()
// Input:
//              WENCInfo*  info:              pointer to WENCInfo structure
//              u32        flag:              encode control flag
//              const s16* pbyPcmData:        PCM data
//              s32        nSampleNum:        number of PCM data.
// Output:
//              u8*        pbyEncData:        Encoded data
// Return:
//              encoded samples
//----------------------------------------------------------------------------
s32 WENCGetEncodeData( WENCInfo*  info,
                       u32        flag,
                       const s16* pbyPcmData,
                       s32        nSampleNum,
                       u8*        pbyEncData );
    
#ifdef __cplusplus
}
#endif

#endif // __WENC_H__
