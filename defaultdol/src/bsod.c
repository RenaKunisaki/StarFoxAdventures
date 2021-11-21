#include "main.h"

static const char *getExceptionName(int code) {
    switch(code) {
        case 0x0: return (const char*)0x8030e25c; //System Reset
        case 0x1: return (const char*)0x8030e26c; //Machine Check
        case 0x2: return (const char*)0x80397a88; //DSI
        case 0x3: return (const char*)0x80397a8c; //ISI
        case 0x5: return (const char*)0x8030e27c; //Alignment
        case 0xB: return (const char*)0x8030e288; //Performance monitor
        case 0xD: return (const char*)0x8030e29c; //System management interrupt
        default:  return (const char*)0x8030e2b8; //Unknown error
    }
}

void bsodHook() {
    OSReport("\n *** FATAL ERROR %d: %s\n",
        exceptionCode, getExceptionName(exceptionCode));
    OSReport("PC=%08X LR=%08X\nTrace:\n", bsodCtx->srr0, bsodCtx->lr);
    u32 *sp = bsodCtx->gpr[1];
    while((u32)sp >= 0x80000000 && (s32)sp < 0x81800000) {
        OSReport("  %08X\n", sp[1]);
        sp = (u32*)*sp;
    }
    for(int i=0; i<32; i += 4) {
        OSReport("GPR %2d: %08X %08X %08X %08X\n", i,
            bsodCtx->gpr[i],   bsodCtx->gpr[i+1],
            bsodCtx->gpr[i+2], bsodCtx->gpr[i+3]);
    }
    while(1);
}

/* void gxSetPixelHook(int x, int y, u32 color) {
    //replace broken framebuffer draw function
    static const u32 *pFrameBuf = 0x80398b4c;
    u32 buf = ((*pFrameBuf) & 0x0FFFFFFF) | 0xC0000000;

    // *(u32*)(((buf & 0xfffff003 | (x & 0xffff) << 2) & 0xffc00fff | (y & 0xffff) << 0xc) & 0xff3fffff) =
    // color;

    u16 *pixels = (u16*)buf;
    if(x < 0) x = 0;
    if(x > 640) x = 640;
    if(y < 0) y = 0;
    if(y > 480) y = 480;
    pixels[(y*640)+x] = color ? 0xC080 : 0;
    //pixels[(y*640)+x] = (x & 0xFF) | ((y << 8) & 0xFF00);
    //dCacheStore(&pixels[(y*640)+x], 2);
} */

/* void bsodFrameBufHook(void *buf, int param2) {
    dCacheStore(buf, 640*480*2);
    GXCopyDisp2(buf, param2);
} */

/* void debugPrintfxyHook(void);
__asm__(
    "debugPrintfxyHook:     \n"
    "lis     3, 0x8000      \n"
    "ori     3, 3, 0x9968   \n" //OSReport
    "mtctr   3              \n"
    "mr      3,  5          \n"
    "mr      4,  6          \n"
    "mr      5,  7          \n"
    "mr      6,  8          \n"
    "mr      7,  9          \n"
    "mr      8, 10          \n"
    "mr      9, 11          \n"
    "bctr"
); */

void initBsodHooks() {
    hookBranch(0x80191058, bsodHook, 0);
    //hookBranch(0x800201a4, gxSetPixelHook, 0);
    //hookBranch(0x80191a90, debugPrintfxyHook, 0);
    //hookBranch(0x80191e14, bsodFrameBufHook, 1);
}
