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
    u32 sig = *(u32*)(src - 0x10);
    switch(sig) {
        case CHARS_TO_U32('D', 'I', 'R', 0): {
            int len = *(int*)(src - 8);
            //OSReport("Copy DIR (len 0x%X) from 0x%08X to 0x%08X\n", len, src, dest);
            memcpy(dest, src, len);
            return 0;
        }

        case CHARS_TO_U32('L', 'Z', 'O', 0):
            //OSReport("Decompress LZO (len 0x%X) from 0x%08X to 0x%08X\n",
            //    compLen, src, dest);
            return lzoDecompress(src, compLen, dest, outLen);

        case CHARS_TO_U32('Z', 'L', 'B', 0): {
            int len = *(int*)(src - 8);
            if(outLen) *outLen = len;
            unsigned long crc = crc32(0, src, compLen);
            //OSReport("Decompress ZLB (len 0x%X -> 0x%X) from 0x%08X to 0x%08X CRC 0x%08X\n",
            //    compLen, len, src, dest, crc);
            //memset(dest, 0xAAAAAAAA, len);
            return decompress_zlib(src, compLen, dest, len);
        }

        default:
            OSReport("Unrecognized compression signature: %08X\n", sig);
            return -1;
    }
}

int tex0loadHook(byte *src, int compLen, byte *dest, int *outLen) {
    //replaces a call to lzoDecompres()
    //OSReport("TEX0: Decompress %08X (len 0x%X) to %08X\n", src, compLen, dest);
    int r = decompress(src, compLen, dest, outLen);
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
    int r = decompress(file + 0x10, *(u32*)(file+0xC), dest, &len);
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
