#include "main.h"

int decompress(byte *src, int compLen, byte *dest, int *outLen) {
    //decompress data in LZO, ZLB or DIR format.
    u32 sig = *(u32*)(src - 0x10);
    switch(sig) {
        case CHARS_TO_U32('D', 'I', 'R', 0): {
            int len = *(int*)(src - 8);
            //OSReport("Copy DIR (len %d) from %08X to %08X\n", len, src, dest);
            memcpy(dest, src, len);
            return 0;
        }

        case CHARS_TO_U32('L', 'Z', 'O', 0):
            //OSReport("Decompress LZO (len %d) from %08X to %08X\n",
            //    compLen, src, dest);
            return lzoDecompress(src, compLen, dest, outLen);

        case CHARS_TO_U32('Z', 'L', 'B', 0): {
            int len = *(int*)(src - 8);
            if(outLen) *outLen = len;
            //OSReport("Decompress ZLB (len %d -> %d) from %08X to %08X\n",
            //    compLen, len, src, dest);
            return zlbDecompress(src, compLen, dest);
        }

        default:
            OSReport("Unrecognized compression signature: %08X\n", sig);
            return -1;
    }
}

int tex0loadHook(byte *src, int compLen, byte *dest, int *outLen) {
    //replaces a call to lzoDecompres()
    return decompress(src, compLen, dest, outLen);
}

int _tex0loadHook(int fileId, int offset, byte *dest) {
    int len;
    void *file = dataFileBuffers[fileId] + offset;
    u32 sig = *(u32*)(file);
    if(sig == CHARS_TO_U32('D', 'I', 'R', 0)) {
        //OSReport("TEX1: Return DIR %08X\n", file + 0x20);
        return file + 0x20;
    }

    //OSReport("TEX1: Decompress %08X to %08X\n", file, dest);
    int r = decompress(file + 0x10, *(u32*)(file+0xC), dest, &len);
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
