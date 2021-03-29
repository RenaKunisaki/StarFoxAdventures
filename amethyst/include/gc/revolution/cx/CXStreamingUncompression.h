/*---------------------------------------------------------------------------*
  Project:     Compress/uncompress library
  File:        CXStreamingUncompression.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: CXStreamingUncompression.h,v $
  Revision 1.15  2007/05/30 04:19:52  takano_makoto
  Added CXiInitUncompContextXXFront()

  Revision 1.14  2007/05/18 10:50:39  yasuh-to
  Rolled back to SYSTEMMENU2_DEV_BRANCH(HEAD)

  Revision 1.11  2006/07/27 07:40:19  takano_makoto
  Added const modifier to CXIsFinishedUncompXXX parameter.

  Revision 1.10  2006/07/27 04:25:12  takano_makoto
  Changed Condition of CXIsFinishedUncompXXX()

  Revision 1.9  2006/07/27 04:12:14  takano_makoto
  Changed Interface of CXInitUncompContextXXX()
  Changed Parameter type u8* to void*

  Revision 1.8  2006/07/12 01:11:41  takano_makoto
  Fixed comment

  Revision 1.7  2006/07/10 08:38:17  takano_makoto
  Added CXIsFinishedUncompXXX() to determine when streaming decompression is complete
  Changed the spec for CXReadUncompXXX() to include header in passed data
  (for a simplified interface)

  Revision 1.6  2006/07/06 09:19:14  takano_makoto
  Changed the include guard to the REVOLUTION_SDK format

  Revision 1.5  2006/07/06 04:40:14  takano_makoto
  Changed RL8 and LZ8 to RL and LZ

  Revision 1.4  2006/07/05 11:14:43  takano_makoto
  Revised the include guard

  Revision 1.3  2006/07/05 08:13:36  takano_makoto
  Revised comments
  Deleted context members only needed for 16 bit access functions

  Revision 1.2  2006/07/04 08:26:20  takano_makoto
  Changed the prefix from the DS version

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __CX_STREAMING_UNCOMPRESSION_STREAM_H__
#define __CX_STREAMING_UNCOMPRESSION_STREAM_H__

#include <dolphin/types.h>
#include <revolution/os.h> // For ASSERT use.

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    u8 *destp;                         // Write-destination pointer                     4B
    s32 destCount;                     // Remaining size to write                     4B
    s32 forceDestCount;                // Forcibly set the decompression size             4B
    u16 length;                        // Remaining size of continuous write      2B
    u8  flags;                         // Compression flag                      1B
    u8  headerSize;                    // Size of header being read             1B
                                       //                         Total is 16B
}
CXUncompContextRL;


typedef struct
{
    u8 *destp;                         // Write-destination pointer                     4B
    s32 destCount;                     // Remaining size to write                     4B
    s32 forceDestCount;                // Forcibly set the decompression size             4B
    s32 length;                        // Remaining length of continuous write              4B
    u8  lengthFlg;                     // Length-acquired flag              1B
    u8  flags;                         // Compression flag                      1B
    u8  flagIndex;                     // Current compression flag index  1B
    u8  headerSize;                    // Size of header being read             1B
    u8  exFormat;                      // LZ77 compression extension option          1B
    u8  padding_[3];                   //                                 3B
                                       //                             Total is 24B
}
CXUncompContextLZ;


typedef struct
{
    u8 *destp;                         // Write-destination pointer                     4B
    s32 destCount;                     // Remaining size to write                     4B
    s32 forceDestCount;                // Forcibly set the decompression size             4B
    u8 *treep;                         // Huffman encoding table, current pointer 4B
    u32 srcTmp;                        // Data being read                   4B
    u32 destTmp;                       // Data being decoded                     4B
    s16 treeSize;                      // Size of Huffman encoding table             2B
    u8  srcTmpCnt;                     // Size of data being read             1B
    u8  destTmpCnt;                    // Number of bits that have been decoded                     1B
    u8  bitSize;                       // Size of encoded bits                     1B
    u8  headerSize;                    // Size of header being read             1B
    u8  padding_[2];                   //                                        2B
    u8  tree[0x200];                   // Huffman encoding table                 512B (32B is enough for 4bit encoding, but allocated enough for 8bit encoding)
                                       //                                   Total = 544B  (60B sufficient if 4bit encoding)
}
CXUncompContextHuffman;

/*---------------------------------------------------------------------------*
  Name        : CXInitUncompContextRL

  Description : Initializes the streaming decompression context for run-length compressed data.
                

  Arguments   : context     Pointer to the run-length uncompressed context
                dest        Destination address for uncompressed data

  Returns     : Can get the data size after decompression.

 *---------------------------------------------------------------------------*/
void CXInitUncompContextRL( CXUncompContextRL          *context, 
                            void                       *dest );


/*---------------------------------------------------------------------------*
  Name        : CXInitUncompContextLZ

  Description : Initializes the streaming decompression context for LZ compressed data.

  Arguments   : context     Pointer to the LZ uncompressed context
                dest        Destination address for uncompressed data

 *---------------------------------------------------------------------------*/
void CXInitUncompContextLZ( CXUncompContextLZ          *context,
                            void                       *dest );

/*---------------------------------------------------------------------------*
  Name        : CXInitUncompContextHuffman

  Description : Initializes the streaming decompression context for Huffman compressed data.

  Arguments   : context     Pointer to the Huffman uncompressed context
                dest        Destination address for uncompressed data

 *---------------------------------------------------------------------------*/
void CXInitUncompContextHuffman( CXUncompContextHuffman    *context, 
                                 void                      *dest );


/*---------------------------------------------------------------------------*
  Name        : CXReadUncompRL

  Description : This function performs streaming decompression of run-length compressed data.

  Arguments   : context Pointer to the run-length uncompressed context
                data    Pointer to the next data
                len     Data size

  Returns     : Size of remaining uncompressed data.

 *---------------------------------------------------------------------------*/
s32 CXReadUncompRL( CXUncompContextRL *context, const void *data, u32 len );

/*---------------------------------------------------------------------------*
  Name        : CXReadUncompLZ

  Description : This function performs streaming decompression of LZ compressed data.

  Arguments   : context Pointer to the LZ uncompressed context
                data    Pointer to the next data
                len     Data size

  Returns     : Size of remaining uncompressed data.

 *---------------------------------------------------------------------------*/
s32 CXReadUncompLZ( CXUncompContextLZ *context, const void *data, u32 len );

/*---------------------------------------------------------------------------*
  Name        : CXReadUncompHuffman

  Description : This function performs streaming decompression of Huffman compressed data.

  Arguments   : context Pointer to the Huffman uncompressed context
                data    Pointer to the next data
                len     Data size

  Returns     : Size of remaining uncompressed data.

 *---------------------------------------------------------------------------*/
s32 CXReadUncompHuffman( CXUncompContextHuffman *context, const void *data, u32 len );


/*---------------------------------------------------------------------------*
  Name:         CXIsFinishedUncompRL

  Description:  This function determines whether streaming decompression for run-length compression has completed or not.

  Arguments:    *context    Pointer to the run-length decompressed context

  Returns:      TRUE when decompression is complete
                FALSE when data needing to be read still remains
 *---------------------------------------------------------------------------*/
static inline BOOL CXIsFinishedUncompRL( const CXUncompContextRL *context )
{
    return (context->destCount > 0 || context->headerSize > 0)? FALSE : TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         CXIsFinishedUncompLZ

  Description:  This function determines whether streaming decompression for LZ compression has completed or not.

  Arguments:    *context Pointer to the LZ decompressed context

  Returns:      TRUE when decompression is complete
                FALSE when data needing to be read still remains
 *---------------------------------------------------------------------------*/
static inline BOOL CXIsFinishedUncompLZ( const CXUncompContextLZ *context )
{
    return (context->destCount > 0 || context->headerSize > 0)? FALSE : TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         CXIsFinishedUncompHuffman

  Description:  This function determines whether streaming decompression for Huffman compression has completed or not.

  Arguments:    *context pointer to the Huffman decompressed context

  Returns:      TRUE when decompression is complete
                FALSE when data needing to be read still remains
 *---------------------------------------------------------------------------*/
static inline BOOL CXIsFinishedUncompHuffman( const CXUncompContextHuffman *context )
{
    return (context->destCount > 0 || context->headerSize > 0)? FALSE : TRUE;
}



/*---------------------------------------------------------------------------*
  Name:         CXInitUncompContextXXFront

  Description:  Initializes the streaming decompression context for compressed data. (Includes specification for decompression size)

  Arguments:    *context    Pointer to the run-length decompressed context
                *dest       Destination address for decompressed data
                destSize    Decompressed size setting

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void CXiInitUncompContextRLFront( CXUncompContextRL *context, void *dest, s32 destSize )
{
    ASSERT( destSize > 0 );
    CXInitUncompContextRL( context, dest );
    context->forceDestCount = destSize;
}

static inline void CXiInitUncompContextLZFront( CXUncompContextLZ *context, void *dest, s32 destSize )
{
    ASSERT( destSize > 0 );
    CXInitUncompContextLZ( context, dest );
    context->forceDestCount = destSize;
}

static inline void CXiInitUncompContextHuffmanFront( CXUncompContextHuffman *context, void *dest, s32 destSize )
{
    ASSERT( destSize > 0 );
    CXInitUncompContextHuffman( context, dest );
    context->forceDestCount = destSize;
}




#ifdef __cplusplus
} /* extern "C" */
#endif

/* __CX_STREAMING_UNCOMPRESSION_STREAM_H__ */
#endif
