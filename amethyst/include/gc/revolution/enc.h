/*---------------------------------------------------------------------------*
  Project: ENC library
  File:    enc.h

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law. They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: enc.h,v $
  Revision 1.7  2007/02/05 23:34:45  yoshioka_yasuhiro
  Added table stripping.

  Revision 1.6  2006/11/04 08:14:34  yoshioka_yasuhiro
  Added windows-1252 support.

  Revision 1.5  2006/10/27 11:10:36  yoshioka_yasuhiro
  Added some character encodings and automatic converter.

  Revision 1.4  2006/08/14 04:30:37  yoshioka_yasuhiro
  Added ENCConvertStringJisToUnicode and ENCConvertStringUnicodeToJis.

  Revision 1.2  2006/08/09 10:36:55  yoshioka_yasuhiro
  Specification change.
  ENCConverString* functions returns a result code,
  and read/write dstlen and srclen.

  Revision 1.1  2006/08/07 09:13:42  yoshioka_yasuhiro
  Initial commit.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef REVOLUTION_ENC_H__
#define REVOLUTION_ENC_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef enum ENCResult
{
    ENC_OK = 0,
    ENC_ERR_NO_BUF_LEFT = -1,
    ENC_ERR_NO_MAP_RULE = -2,
    ENC_ERR_INVALID_PARAM = -3,
    ENC_ERR_INVALID_FORMAT = -4,
    ENC_ERR_UNKNOWN_ENCODING = -5,
    ENC_ERR_UNSUPPORTED = -6,
    ENC_ERR_NOT_LOADED = -7
} ENCResult;

typedef enum ENCBreakType
{
    ENC_BR_KEEP = 0,
    ENC_BR_CRLF,
    ENC_BR_CR,
    ENC_BR_LF
} ENCBreakType;

typedef struct ENCContext
{
    s32 encoding;
    ENCBreakType brtype;
    s32 state;
    u16 nomap;
    u16 invalid;
} ENCContext;

#define ENC_INTERNAL_CHAR_WIDTH sizeof(u16)

#define ENC_ENCODING_COUNT 23
#define ENC_ENCODING_NAME_LENGTH 16

#define ENC_CHECK_NOT_FOUND -1

#define ENC_ALT_STOP 0x0000
#define ENC_ALT_SKIP 0xFFFF

/* Internal from/to External */

ENCResult ENCInitContext(ENCContext *context);
ENCResult ENCDuplicateContext(ENCContext *duplicate, const ENCContext *original);
ENCResult ENCSetExternalEncoding(ENCContext* context, const u8* encoding);
ENCResult ENCGetExternalEncoding(const ENCContext* context, u8* encoding);
ENCResult ENCSetBreakType(ENCContext* context, ENCBreakType type);
ENCResult ENCSetAlternativeCharacter(ENCContext* context, u16 nomap, u16 invalid);
ENCResult ENCGetNextCharacterWidth(const ENCContext* context, const u8* src, u32* width);
ENCResult ENCGetExternalCharacterWidth(const ENCContext* context, u32* width);
ENCResult ENCConvertToInternalEncoding(ENCContext* context, u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertFromInternalEncoding(ENCContext* context, u8* dst, s32* dstlen, const u16* src, s32* srclen);


/* Check */

ENCResult ENCCheckEncoding(s32* index, const u8** encodings, s32 encsize, const u16* src, s32 srclen);


ENCResult ENCIs7BitEncoding(BOOL* is7bit, const u8* encoding);


/* BOM */

ENCResult ENCSetUnicodeBOM(u16* dst, s32 dstlen);
ENCResult ENCSetUnicodeBOM32(u32* dst, s32 dstlen);
ENCResult ENCSetUnicodeBOM16(u16* dst, s32 dstlen);
ENCResult ENCSetUnicodeBOM8(u8* dst, s32 dstlen);

/* ASCII */

ENCResult ENCConvertStringUnicodeToAscii(u8* dst, s32* dstlen, const u16* src, s32* srclen);
ENCResult ENCConvertStringAsciiToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);

/* UTF */

ENCResult ENCConvertStringUtf32ToUtf16(u16* dst, s32* dstlen, const u32* src, s32* srclen);
ENCResult ENCConvertStringUtf16ToUtf32(u32* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringUtf32ToUtf8(u8* dst, s32* dstlen, const u32* src, s32* srclen);
ENCResult ENCConvertStringUtf8ToUtf32(u32* dst, s32* dstlen, const u8* src, s32* srclen);

ENCResult ENCConvertStringUtf16ToUtf8(u8* dst, s32* dstlen, const u16* src, s32* srclen);
ENCResult ENCConvertStringUtf8ToUtf16(u16* dst, s32* dstlen, const u8* src, s32* srclen);

/* Japanese */

ENCResult ENCConvertStringSjisToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToSjis(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringJisToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToJis(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringJisToSjis(u8* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringSjisToJis(u8* dst, s32* dstlen, const u8* src, s32* srclen);

/* Latin */

ENCResult ENCConvertStringLatin1ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToLatin1(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringLatin2ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToLatin2(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringLatin3ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToLatin3(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringGreekToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToGreek(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringLatin6ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToLatin6(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringLatin9ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToLatin9(u8* dst, s32* dstlen, const u16* src, s32* srclen);

ENCResult ENCConvertStringWin1252ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringUnicodeToWin1252(u8* dst, s32* dstlen, const u16* src, s32* srclen);

/* one-way */

ENCResult ENCConvertStringUtf16ToUtf16(u16* dst, s32* dstlen, const u16* src, s32* srclen);
ENCResult ENCConvertStringUtf16LEToUtf16BE(u16* dst, s32* dstlen, const u16* src, s32* srclen);
ENCResult ENCConvertStringUtf7ToUtf16(u16* dst, s32* dstlen, const u8* src, s32* srclen);

ENCResult ENCConvertStringWin1250ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringWin1253ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);

ENCResult ENCConvertStringMacromanToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringMacgreekToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringMacceToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);

ENCResult ENCConvertStringIbm850ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);
ENCResult ENCConvertStringIbm852ToUnicode(u16* dst, s32* dstlen, const u8* src, s32* srclen);

/* strip table */

#define ENC_STRIP_TABLE_JP \
    BOOL enc_tbl_jp_loaded = FALSE; \
    const u8* enc_tbl_jp_wctomb = NULL;   \
    const u8* enc_tbl_jp_mbtowc = NULL;   \
    const u16* enc_offset_jp = NULL;

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* REVOLUTION_ENC_H__ */
