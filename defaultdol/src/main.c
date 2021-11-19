/** Bootstrap, runs at startup and installs other hooks.
 */
#include "main.h"

//hack to ensure nothing is at offset 0 because that makes relocation difficult.
int fart __attribute__((section(".offsetzero"))) = 0x52656E61;

void mainLoopHook() {
    //replaces a bl to a do-nothing subroutine
    //OSReport("main loop");
}

void gxSetPixelHook(int x, int y, u32 color) {
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
}

void bsodFrameBufHook(void *buf, int param2) {
    dCacheStore(buf, 640*480*2);
    GXCopyDisp2(buf, param2);
}

void debugPrintfxyHook(void);
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
);

void _start(void) {
    OSReport("Patch running!");

    //Install hooks
    hookBranch(0x800781fc, mainLoopHook, 1);
    hookBranch(0x800201a4, gxSetPixelHook, 0);
    //hookBranch(0x80191a90, debugPrintfxyHook, 0);
    hookBranch(0x80191e14, bsodFrameBufHook, 1);

    //WRITE_NOP(0x80009a60); //prevent disabling interrupts
    //WRITE_BLR(0x80006cf8); //disable PPCHalt
    //WRITE32(0x80082e88, 0x3AA00001); //only load one frame of object (else we get sparsearray crash)
    WRITE32(0x80082d90, 0xA01E0050); //fix objData DLL offset
    WRITE32(0x80082d94, 0x2800FFFF); //don't load DLL -1
    WRITE32(0x80082d9c, 0xA07E0050); //fix objData DLL offset
    WRITE32(0x800d83ec, 0x38600022); //hack newstars
    WRITE32(0x8007b7ec, 0x8081FFE0); //clear memory on alloc
    WRITE32(0x8007b7f0, 0x4BF87A60); //clear memory on alloc
    WRITE_BLR(0x800a72ec); //do not update environment
    WRITE32(0x800a7758, 0x38000001); //disable sky

    initCompressionHooks();
    initTextureHooks();
    initModelHooks();
    initLoggingHooks();

    OSReport("Hooks installed!");
}
