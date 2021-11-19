#include "main.h"
#include "zlib.h"
//int uncompress (dest, destLen, source, sourceLen);
//unsigned long crc32(crc, buf, len);

void* zalloc(void *stream, size_t nitems, size_t size);
void zfree(void *stream, void *ptr);

int decompress_zlib(byte *compr, int comprLen, byte *uncompr, int uncomprLen) {
    z_stream infstream;
    infstream.zalloc = zalloc;
    infstream.zfree = zfree;
    infstream.opaque = Z_NULL;
    infstream.avail_in = comprLen; // size of input
    infstream.next_in = (Bytef *)compr; // input char array
    infstream.avail_out = (uInt)uncomprLen; // size of output
    infstream.next_out = (Bytef *)uncompr; // output char array

    int err = inflateInit(&infstream);
    if(err < 0) {
        OSReport("inflateInit: %d\n", err);
        return err;
    }

    err = inflate(&infstream, Z_NO_FLUSH);
    if(err < 0) {
        OSReport("inflate: %d\n", err);
        return err;
    }

    err = inflateEnd(&infstream);
    if(err < 0) {
        OSReport("inflateEnd: %d\n", err);
        return err;
    }

    return 0;
}

int decompress(byte *src, int compLen, byte *dest, int *outLen) {
    //decompress data in LZO, ZLB or DIR format.
    //src points to the signature.
    int resLen = 0;
    int result = -1;
    u32 sig = *(u32*)src;
    OSReport("decompress sig = 0x%08X at 0x%08X\n", sig, src);
    switch(sig) {
        case CHARS_TO_U32('D', 'I', 'R', 0): //not compressed
        case CHARS_TO_U32('D', 'I', 'R', 'n'): {
            int len = *(int*)(src + 8);
            //OSReport("Copy DIR (len 0x%X) from 0x%08X to 0x%08X\n", len, src, dest);
            memcpy(dest, src+0x10, len);
            resLen = len;
            result = 0;
            break;
        }

        case CHARS_TO_U32('L', 'Z', 'O', 0): { //old compression
            //there's already a built-in routine for this.
            //OSReport("Decompress LZO (len 0x%X) from 0x%08X to 0x%08X\n",
            //    compLen, src, dest);
            if(!compLen) {
                OSReport("Can't decode LZO with unknown length\n");
                return -1;
            }
            result = lzoDecompress(src+0x10, compLen, dest, &resLen);
            break;
        }

        case CHARS_TO_U32('Z', 'L', 'B', 0): { //Zlib (used in newer versions)
            int len = *(int*)(src + 0x08);
            if(outLen) *outLen = len;

            int cLen = *(int*)(src + 0x0C);
            if(compLen != cLen & compLen != 0) {
                OSReport("ZLB compLen=0x%X but got 0x%X\n", cLen, compLen);
            }
            compLen = cLen;

            //unsigned long crc = crc32(0, src, compLen);
            //OSReport("Decompress ZLB (len 0x%X -> 0x%X) from 0x%08X to 0x%08X CRC 0x%08X\n",
            //    compLen, len, src, dest, crc);
            //memset(dest, 0xAAAAAAAA, len);
            result = decompress_zlib(src+0x10, compLen, dest, len);
            resLen = len;
            break;
        }

        case 0xE0E0E0E0: { //not compressed
            int len = *(int*)(src + 0x04);
            int offs = *(int*)(src + 0x08) + 0x18;
            memcpy(dest, src+offs, len);
            resLen = len;
            result = 0;
            break;
        }

        case 0xF0F0F0F0: { //old version of FACEFEED header?
            int offs = *(int*)(src + 0x08) + 0x28;
            int cLen = *(int*)(src + 0x0C);
            if(compLen != cLen & compLen != 0) {
                OSReport("F0F0 compLen=0x%X but got 0x%X\n", cLen, compLen);
            }
            compLen = cLen;
            result = lzoDecompress(src+offs, compLen, dest, &resLen);
            break;
        }

        case 0xFACEFEED: { //prefix before some model data
            int len  = *(int*)(src + 0x04);
            int ext  = *(int*)(src + 0x08);
            int cLen = *(int*)(src + 0x0C);
            if(outLen) *outLen = len;
            if(compLen != cLen & compLen != 0) {
                OSReport("FACEFEED compLen=0x%X but got 0x%X\n", cLen, compLen);
            }
            //OSReport("FACEFEED ext=0x%X\n", ext);
            compLen = cLen;
            //there can be various formats here...
            result = decompress(src+((ext-3)*4), compLen-0x10, dest, &resLen);
            break;
        }

        default:
            OSReport("Unrecognized compression signature: %08X\n", sig);
            return -1;
    }

    if(outLen) *outLen = resLen;
    return result;
}

int tex0loadHook(byte *src, int compLen, byte *dest, int *outLen) {
    //replaces a call to lzoDecompres()
    //OSReport("TEX0: Decompress %08X (len 0x%X) to %08X\n", src, compLen, dest);
    int r = decompress(&src[-16], compLen, dest, outLen);
    //OSReport("Result: %d\n", r);
    dCacheStore(dest, r);
    return r;
}

int _tex0loadHook(int fileId, int offset, byte *dest) {
    int len;
    void *file = dataFileBuffers[fileId] + offset;
    u32 sig = *(u32*)(file);
    if(sig == CHARS_TO_U32('D', 'I', 'R', 0)) {
        //OSReport("TEX1: Return DIR %08X\n", file + 0x20);
        return file + 0x20;
    }

    //OSReport("TEX1: Decompress %08X to %08X sig %c%c%c%c\n", file, dest,
    //    sig >> 24, (sig >> 16) & 0xFF, (sig >> 8) & 0xFF, sig & 0xFF);
    int r = decompress(file, *(u32*)(file+0xC), dest, &len);
    //OSReport("Result: %d\n", r);
    dCacheStore(dest, len);
    return r;
}

void tex1loadHook(void);
__asm__(
    "tex1loadHook:        \n"
    ASM_RFUNC_START(0x100)
    "mr     3, 25         \n" //r3 = file ID
    "mr     4, 30         \n" //r4 = offset
    "lwz    5, 0x24(1)    \n" //r5 = dest
    "bl     _tex0loadHook \n"
    ASM_RFUNC_END(0x100)
    "blr" //return from the parent function
);

void initCompressionHooks() {
    hookBranch(0x8009ac58, tex0loadHook, 1);
    hookBranch(0x8009ac80, tex1loadHook, 0);
}
