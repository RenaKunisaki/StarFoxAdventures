/*---------------------------------------------------------------------------*
  Project:     Compress/uncompress library
  File:        CXUncompression.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: CXUncompression.h,v $
  Revision 1.14  2007/05/18 10:50:39  yasuh-to
  Rollback to SYSTEMMENU2_DEV_BRANCH(HEAD)

  Revision 1.11  2006/09/13 01:01:31  takano_makoto
  Expanded max file size.

  Revision 1.10  2006/07/14 05:23:56  takano_makoto
  Added CXUncompressAny()
  Bug fix in CXGetCompressionType()

  Revision 1.9  2006/07/06 09:19:14  takano_makoto
  Changed the include guard to the REVOLUTION_SDK format

  Revision 1.8  2006/07/06 05:29:48  takano_makoto
  Added CXUnfilterDiff

  Revision 1.7  2006/07/06 04:39:50  takano_makoto
  Revised duplicate definitions in CXGetCompressionType, CXGetUncompressedSize

  Revision 1.6  2006/07/06 04:13:04  takano_makoto
  Added CXGetCompressionType, CXGetUncompressedSize

  Revision 1.5  2006/07/05 11:14:43  takano_makoto
  Revised the include guard

  Revision 1.4  2006/07/05 08:13:03  takano_makoto
  Revised comments

  Revision 1.3  2006/07/04 08:38:39  takano_makoto
  Fixed copyright header

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __CX_UNCOMPRESSION_H__
#define __CX_UNCOMPRESSION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <dolphin/types.h>


//---- Compression type
typedef enum
{
    CX_COMPRESSION_LZ           = 0x10,     // LZ77
    CX_COMPRESSION_HUFFMAN      = 0x20,     // Huffman
    CX_COMPRESSION_RL           = 0x30,     // Run Length
    CX_COMPRESSION_DIFF         = 0x80,     // Differential filter

    CX_COMPRESSION_TYPE_MASK    = 0xF0,
    CX_COMPRESSION_TYPE_EX_MASK = 0xFF
}
CXCompressionType;


//----------------------------------------------------------------
// Compressed data header
//
typedef struct
{
    u8  compType;   // Compression type
    u8  compParam;  // Compression parameter
    u8  padding_[2];
    u32 destSize;   // Expanded size
}
CXCompressionHeader;


/*---------------------------------------------------------------------------*
  Name:         CXGetCompressionHeader

  Description:  Gets header information from the first four bytes in the compression data.

  Arguments:    data    Pointer to the first four bytes of data in the compressed data

  Returns:      None.
 *---------------------------------------------------------------------------*/
CXCompressionHeader CXGetCompressionHeader( const void *data );

/*---------------------------------------------------------------------------*
  Name:         CXGetCompressionType

  Description:  Gets compression type from the first one byte in the compressed data

  Arguments:    srcp :  Compressed data address

  Returns:      Compression type.
                CX_COMPREESION_LZ      : LZ77 compressed data
                CX_COMPREESION_HUFFMAN : Huffman compressed data
                CX_COMPREESION_RL      : Run-length compressed data
                CX_COMPRESSION_DIFF    : Data converted with a differential filter
 *---------------------------------------------------------------------------*/
static inline CXCompressionType   CXGetCompressionType( const void *data )
{
    return (CXCompressionType)( *(u8*)data & 0xF0 );
}

//======================================================================
//          Expanding compressed data
//======================================================================

/*---------------------------------------------------------------------------*
  Name:         CXGetUncompressedSize

  Description:  Gets the data size after decompression.
                This function can be used for data in all compression formats handled by CX.

  Arguments:    srcp :  Pointer to the first four bytes of data in the compressed data

  Returns:      Data size after decompression
 *---------------------------------------------------------------------------*/
u32 CXGetUncompressedSize( const void *srcp );


/*---------------------------------------------------------------------------*
  Name:         CXUncompressAny

  Description:  Determines the compression type from the data header and performs the appropriate decompression.
                 
                Since decompression processes for all types of compression are linked, it may be better to execute separate functions for each for each compression type when not using formats other than the specific compression formats.
                 
                 

  Arguments:    srcp    Source address
                destp   Destination address

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CXUncompressAny( const void* srcp, void* destp );


/*---------------------------------------------------------------------------*
  Name:         CXUncompressRL

  Description:  8-bit decompression of run-length compressed data

  - Decompresses run-length compressed data, writing in 8 bit units.
  - Use 4 byte alignment for the source address.

  - Data header
      u32 :4                  Reserved
          compType:4          Compression type( = 3)
          destSize:24         Data size after decompression

  - Flag data format
      u8  length:7            Decompressed data length - 1 (When not compressed)
                              Decompressed data length - 3 (only compress when the contiguous length is 3 bytes or greater)
          flag:1              (0, 1) = (not compressed, compressed)

  Arguments:    *srcp   Source address
                *destp  Destination address

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CXUncompressRL( const void *srcp, void *destp );


/*---------------------------------------------------------------------------*
  Name:         CXUncompressLZ

  Description:  8-bit decompression of LZ77 compressed data

  - Decompresses LZ77 compressed data, writing in 8 bit units.
  - Use 4 byte alignment for the source address.

  - Data header
      u32 :4                  Reserved
          compType:4          Compression type( = 1)
          destSize:24         Data size after decompression

  - Flag data format
      u8  flags               Compression/no compression flag
                              (0, 1) = (not compressed, compressed)
  - Code data format (Big Endian)
      u16 length:4            Decompressed data length - 3 (only compress when the match length is 3 bytes or greater)
          offset:12           Match data offset - 1

  Arguments:    *srcp   Source address
                *destp  Destination address

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CXUncompressLZ( const void *srcp, void *destp );


/*---------------------------------------------------------------------------*
  Name:         CXUncompressHuffman

  Description:  Decompression of Huffman compressed data

  - Decompresses Huffman compressed data, writing in 32 bit units.
  - Use 4 byte alignment for the source address.
  - Use 4 byte alignment for the destination address.
  - The target decompression buffer size must be prepared in 4 byte multiples.

  - Data header
      u32 bitSize:4           1 data bit size (Normally 4|8)
          compType:4          Compression type( = 2)
          destSize:24         Data size after decompression

  - Tree table
      u8           treeSize        Tree table size/2 - 1
      TreeNodeData nodeRoot        Root node

      TreeNodeData nodeLeft        Root left node
      TreeNodeData nodeRight       Root right node

      TreeNodeData nodeLeftLeft    Left left node
      TreeNodeData nodeLeftRight   Left right node

      TreeNodeData nodeRightLeft   Right left node
      TreeNodeData nodeRightRight  Right right node

              .
              .

    The compressed data itself follows

  - TreeNodeData structure
      u8  nodeNextOffset:6    Offset to the next node data - 1 (2 byte units)
          rightEndFlag:1      Right node end flag
          leftEndzflag:1      Left node end flag
                              When the end flag is set
                              There is data in next node

  Arguments:    *srcp   Source address
                *destp  Destination address

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CXUncompressHuffman( const void *srcp, void *destp );


/*---------------------------------------------------------------------------*
  Name:         CXUnfilterDiff

  Description:  8-bit decompression to restore differential filter conversion.

    - Restores a differential filter, writing in 8 bit units.
    - Cannot decompress directly into VRAM
    - If the size of compressed data is not a multiple of four, adjust as necessary by padding with 0s.
      
    - Use 4 byte alignment for the source address.

  Arguments:    *srcp   Source address
                *destp  Destination address

  Returns:      None.
 *---------------------------------------------------------------------------*/
void CXUnfilterDiff( register const void *srcp, register void *destp );


//================================================================================


/*---------------------------------------------------------------------------*
  Name:         CXiUncompressBackward

  Description:  Uncompress special archive for module compression

  Arguments:    bottom      = Bottom adrs of packed archive + 1
                bottom[-12] = offset for top    of compressed data
                                 inp_top = bottom + bottom[-12]
                bottom[ -8] = offset for bottom of compressed data
                                 inp     = bottom + bottom[ -8]
                bottom[ -4] = offset for bottom of original data
                                 outp    = bottom + bottom[ -4]

                typedef struct
                {
                    int         bufferTop;
                    int         compressBottom;
                    int         originalBottom;
                }   CompFooter;

  Returns:      None.
 *---------------------------------------------------------------------------*/
// !!!! Coded in libraries/init/ARM9/crt0.c
// void CXiUncompressBackward( void *bottom );




#ifdef __cplusplus
} /* extern "C" */
#endif

/* __CX_UNCOMPRESSION_H__ */
#endif
