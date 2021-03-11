/*---------------------------------------------------------------------------*
  Project:     Compress/uncompress library
  File:        CXSecureUncompression.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: CXSecureUncompression.h,v $
  Revision 1.5  2007/05/30 04:20:25  takano_makoto
  Minor fixes.

  Revision 1.4  2007/05/18 10:50:39  yasuh-to
  Rolled back to SYSTEMMENU2_DEV_BRANCH(HEAD)

  Revision 1.1.2.1  2006/12/06 09:55:16  yasuh-to
  Initial commit

  Revision 1.1  2006/12/04 13:34:46  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __CX_SECURE_UNCOMPRESSION_H__
#define __CX_SECURE_UNCOMPRESSION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <dolphin/types.h>
#include <revolution/cx/CXUncompression.h>

#define CX_ERR_SUCCESS              0
#define CX_ERR_UNSUPPORTED          -1
#define CX_ERR_SRC_SHORTAGE         -2
#define CX_ERR_SRC_REMAINDER        -3
#define CX_ERR_DEST_OVERRUN         -4
#define CX_ERR_ILLEGAL_TABLE        -5

//======================================================================
//          Expanding compressed data
//======================================================================

/*---------------------------------------------------------------------------*
  Name:         CXSecureUncompressAny

  Description:  Determines the compression type from the data header and performs the appropriate decompression process.
                 
                Since decompression processes for all types of compression are linked, it may be better to execute separate functions for each for each compression type when not using formats other than the specific compression formats.
                
                

  Arguments:    srcp    Source address
                srcSize Source data size
                destp   Destination address

  Returns:      Returns 0 when conversion succeeds
                Returns a negative error code if failed.
 *---------------------------------------------------------------------------*/
s32 CXSecureUncompressAny( const void* srcp, u32 srcSize, void* destp );


/*---------------------------------------------------------------------------*
  Name:         CXSecureUncompressRL

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
                srcSize Source data size
                *destp  Destination address

  Returns:      Returns 0 when conversion succeeds
                Returns a negative error code if failed.
 *---------------------------------------------------------------------------*/
s32 CXSecureUncompressRL( const void *srcp, u32 srcSize, void *destp );


/*---------------------------------------------------------------------------*
  Name:         CXSecureUncompressLZ

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
                srcSize Source data size
                *destp  Destination address

  Returns:      Returns 0 when conversion succeeds
                Returns a negative error code if failed.
 *---------------------------------------------------------------------------*/
s32 CXSecureUncompressLZ( const void *srcp, u32 srcSize, void *destp );


/*---------------------------------------------------------------------------*
  Name:         CXSecureUncompressHuffman

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
                              When end flag is set
                              There is data in next node

  Arguments:    *srcp   Source address
                srcSize Source data size
                *destp  Destination address

  Returns:      Returns 0 when conversion succeeds
                Returns a negative error code if failed.
 *---------------------------------------------------------------------------*/
s32 CXSecureUncompressHuffman( const void *srcp, u32 srcSize, void *destp );


/*---------------------------------------------------------------------------*
  Name:         CXSecureUnfilterDiff

  Description:  8-bit decompression to restore differential filter conversion.

    - Restores a differential filter, writing in 8 bit units.
    - Cannot decompress directly into VRAM
    - If the size of compressed data is not a multiple of four, adjust as necessary by padding with 0s.
       
    - Use 4 byte alignment for the source address.

  Arguments:    *srcp   Source address
                srcSize Source size
                *destp  Destination address

  Returns:      Returns 0 when conversion succeeds
                Returns a negative error code if failed.
 *---------------------------------------------------------------------------*/
s32 CXSecureUnfilterDiff( register const void *srcp, u32 srcSize, register void *destp );



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

  Returns:      Error Codes
 *---------------------------------------------------------------------------*/
// !!!! Coded in libraries/init/ARM9/crt0.c
// void CXiUncompressBackward( void *bottom );




#ifdef __cplusplus
} /* extern "C" */
#endif

/* __CX_SECURE_UNCOMPRESSION_H__ */
#endif
