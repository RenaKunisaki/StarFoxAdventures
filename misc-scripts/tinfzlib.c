//int lzoDecompress(byte *src,int compLen,byte *dest,int *outLen)
#include <stdint.h>
#define TINF_DATA_ERROR -1
int tinf_zlib_uncompress(
    const uint8_t *src,
    unsigned int sourceLen,
    uint8_t *dst,
    unsigned int *destLen)
{
	unsigned int a32;
	int res;
	unsigned char cmf, flg;

	/* -- Check header -- */

	/* Check room for at least 2 byte header and 4 byte trailer */
	if (sourceLen < 6) {
		return TINF_DATA_ERROR;
	}

	/* Get header bytes */
	cmf = src[0];
	flg = src[1];

	/* Check checksum */
	if ((256 * cmf + flg) % 31) {
		return TINF_DATA_ERROR;
	}

	/* Check method is deflate */
	if ((cmf & 0x0F) != 8) {
		return TINF_DATA_ERROR;
	}

	/* Check window size is valid */
	if ((cmf >> 4) > 7) {
		return TINF_DATA_ERROR;
	}

	/* Check there is no preset dictionary */
	if (flg & 0x20) {
		return TINF_DATA_ERROR;
	}

	/* -- Get Adler-32 checksum of original data -- */

	a32 = read_be32(&src[sourceLen - 4]);

	/* -- Decompress data -- */

	res = tinf_uncompress(dst, destLen, src + 2, sourceLen - 6);

	if (res != TINF_OK) {
		return TINF_DATA_ERROR;
	}

	/* -- Check Adler-32 checksum -- */

	if (a32 != tinf_adler32(dst, *destLen)) {
		return TINF_DATA_ERROR;
	}

	return TINF_OK;
}
