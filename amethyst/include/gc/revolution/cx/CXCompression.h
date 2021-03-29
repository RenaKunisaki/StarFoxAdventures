/*---------------------------------------------------------------------------*
  Project:     Compress/uncompress library
  File:        CXCompression.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: CXCompression.h,v $
  Revision 1.9  2007/05/30 04:20:56  takano_makoto
  Added CXCompressLZEx.

  Revision 1.8  2007/05/18 10:50:39  yasuh-to
  Rolled back to SYSTEMMENU2_DEV_BRANCH(HEAD)

  Revision 1.5  2006/07/06 09:19:14  takano_makoto
  Changed the include guard to the REVOLUTION_SDK format

  Revision 1.4  2006/07/05 11:14:34  takano_makoto
  Changed MI_LZ_COMPRESS_WORK_SIZE to CX_LZ_COMPRESS_WORK_SIZE
  Revised the include guard

  Revision 1.3  2006/07/04 13:19:40  takano_makoto
  Changed LZ compression to the high-speed version using work buffer

  Revision 1.2  2006/07/04 08:25:25  takano_makoto
  Changed the prefix from the DS version

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __CX_COMPRESS_H__
#define __CX_COMPRESS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <dolphin/types.h>


#define CX_LZ_COMPRESS_WORK_SIZE   ( (4096 + 256 + 256) * sizeof(s16) )

/*---------------------------------------------------------------------------*
  Name:         CXCompressLZ

  Description:  Function that performs LZ77 compression

  Arguments:    srcp            Pointer to compression source data
                size            Size of compression source data
                dstp            Pointer to compressed data
                                Buffer must be larger than size of compression source data.
                work            Temporary buffer for compression
                                A region for the size of CX_LZ_COMPRESS_WORK_SIZE is necessary.

  Returns:      The data size after compression.
                If compressed data is larger than original data, compression is terminated and 0 is returned.
 *---------------------------------------------------------------------------*/
u32 CXCompressLZImpl(const u8 *srcp, u32 size, u8 *dstp, void *work, BOOL exFormat);

inline u32 CXCompressLZEx(const u8 *srcp, u32 size, u8 *dstp, void* work)
{
    return CXCompressLZImpl( srcp, size, dstp, work, TRUE );
}
inline u32 CXCompressLZ(const u8 *srcp, u32 size, u8 *dstp, void *work)
{
    return CXCompressLZImpl( srcp, size, dstp, work, FALSE );
}


/*---------------------------------------------------------------------------*
  Name:         CXCompressRL

  Description:  Function that performs run-length compression

  Arguments:    srcp            Pointer to compression source data
                size            Size of compression source data
                dstp            Pointer to compressed data
                                Buffer must be larger than size of compression source data.

  Returns:      The data size after compression.
                If compressed data is larger than original data, compression is terminated and 0 is returned.
 *---------------------------------------------------------------------------*/
u32 CXCompressRL( const u8 *srcp, u32 size, u8 *dstp );


#define CX_HUFFMAN_COMPRESS_WORK_SIZE   ( 12288 + 512 + 1536 )


/*---------------------------------------------------------------------------*
  Name:         CXCompressHuffman

  Description:  Function that performs Huffman compression

  Arguments:    srcp            Pointer to compression source data
                size            Size of compression source data
                dstp            Pointer to compressed data
                                Buffer must be larger than size of compression source data.
                huffBitSize     The number of bits to encode
                work            The work buffer for compression that requires a size of CX_HUFFMAN_COMPRESS_WORK_SIZE.

  Returns:      The data size after compression.
                If compressed data is larger than original data, compression is terminated and 0 is returned.
 *---------------------------------------------------------------------------*/
u32 CXCompressHuffman( const u8 *srcp, u32 size, u8 *dstp, u8 huffBitSize, void *work );


#ifdef __cplusplus
} /* extern "C" */
#endif

/* __CX_COMPRESS_H__ */
#endif
