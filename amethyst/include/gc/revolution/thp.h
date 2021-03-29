/*---------------------------------------------------------------------------*
  Project:  THP - fast picture and motion-picture decoding for Revolution
  File:     thp.h

  Copyright (C)2002-2006 Nintendo  All Rights Reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: thp.h,v $
  Revision 1.2  02/09/2006 04:50:20  aka
  Changed copyright.

  Revision 1.1.1.1  2005/05/12 02:41:07  yasuh-to
  Imported from Dolphin tree.
    
    2     03/09/15 9:38a Akagi
    
    1     02/01/16 12:02p Akagi
    moved from tests. cleanup.
    
    3     8/14/01 5:28p Tian
    Support for any multiple-of-16 resolution
    
    2     4/16/01 4:03p Tian
    Added support for both 640x480 and 512x448 decoding in library via
    separate APIs.  NOTE: THPSimple is only compiled for 512x448.  "#define
    DOING640480" to make THPSimple work for 640x480.  Updated MHTP APIs.
    
    2     4/13/01 4:25p Tian
    All copying eliminated, with locked cache for IDCT output, outer loop
    optimizations, even more Huffman decoding optimizations. 
    
    1     4/12/01 11:01p Tian
    Move to THP tree.  Massive optimizations : Hirose-san's new IDCT with
    swizzle, table-driven Huffman decoding, speculative execution of common
    case code, etc.
    
    5     4/10/01 2:58p Tian
    Locked cache Y-I8 texture output optimizations.
    
    4     4/09/01 11:22a Tian
    Huffman optimizations.  Integrated Hirose-san's IDCT.
    
    3     3/30/01 2:21a Tian
    Major optimizations for TEV-based YUV conversion (code ported from hvqm
    tree), 640x480 decode in one pass for all render modes, TEV constant
    calibration, zoom/pan controls tweaked.
    
    2     3/29/01 4:44p Tian
    Cleanup.  Optimizations.  More stats.
    
    1     3/28/01 12:16a Tian
    Initial check-in.
  $NoKeywords: $  

 *---------------------------------------------------------------------------*/

#ifndef __THP_H__
#define __THP_H__
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Public Constants...
 *---------------------------------------------------------------------------*/

// work size for THPVideoDecode()
#define THP_WORK_SIZE              0x1000
  
// this enum describes the used errorcodes...
enum 
{
    THP_OK = 0,                                   //  0

    THP_RESOURCE_NOT_FOUND,                       //  1
    THP_GET_DATA_ERROR,                           //  2
    THP_BAD_SYNTAX,                               //  3
    THP_BAD_VERSION,                              //  4
    THP_UNSUPPORTED_FORMAT,                       //  5
    THP_NO_MEMORY,                                //  6
    THP_HAS_THUMBNAIL,                            //  7
    THP_CORRUPT_HEADER,                           //  8
    THP_UNSUPPORTED_QUANTIZATION,                 //  9
    THP_UNSUPPORTED_PRECISION,                    // 10
    THP_UNSUPPORTED_MARKER,                       // 11
    THP_UNSUPPORTED_NUM_COMP,                     // 12
    THP_UNSUPPORTED_NUM_HUFF,                     // 13
    THP_BAD_SCAN_HEADER,                          // 14
    THP_INVALID_HUFFTAB,                          // 15
    THP_BAD_HUFFTAB,                              // 16
    THP_BAD_MCU_SIZE,                             // 17
    THP_OPTIONS_NOT_SUPPORTED,                    // 18
    THP_UNSUPPORTED_COMPS,                        // 19
    THP_UNSUPPORTED_IMAGESIZE,                    // 20
    THP_IMAGE_VS_TILE_MISMATCH,                   // 21
    THP_UNSUPPORTED_OUTPUTMODE,                   // 22
    THP_UNSUPPORTED_SAMPLING_FACTORS,             // 23
    THP_NUMCOMP_VS_OUTPUTMODE_MISMATCH,           // 24
    
    THP_NO_INPUT_FILE,                            // 25
    THP_NO_WORK_AREA,                             // 26
    
    THP_NO_OUTPUT_BUFFER,                         // 27
    THP_LC_NOT_ENABLED,                           // 28
    THP_NOT_INITIALIZED,                          // 29

    THP_NUM_ERRORS
};

// flag value of THPAudioDecode()
#define THP_AUDIO_INTERLEAVE    0x00
#define THP_AUDIO_NO_INTERLEAVE 0x01

/*---------------------------------------------------------------------------*
    Public Prototypes...
 *---------------------------------------------------------------------------*/

// graphics stuff
BOOL   THPInit        (void);
s32    THPVideoDecode (void *file, void *tileY, void *tileU, void *tileV, void *work);

// sound stuff
u32    THPAudioDecode (s16 *buffer, u8 *audioFrame, s32 flag);
  
#ifdef __cplusplus
}
#endif

#endif
