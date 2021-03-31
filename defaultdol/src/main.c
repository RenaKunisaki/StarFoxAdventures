/** Bootstrap, runs at startup and installs other hooks.
 */
#include "main.h"

//hack to ensure nothing is at offset 0 because that makes relocation difficult.
int fart __attribute__((section(".offsetzero"))) = 0x52656E61;

void mainLoopHook() {
    //replaces a bl to a do-nothing subroutine
    //OSReport("main loop");
}

void _start(void) {
    OSReport("Patch running!");

    //Install hooks
    hookBranch(0x800781fc, mainLoopHook, 1);

    //WRITE_NOP(0x80009a60); //prevent disabling interrupts
    //WRITE_BLR(0x80006cf8); //disable PPCHalt
    WRITE32(0x80082e88, 0x3AA00001); //only load one frame of object (else we get sparsearray crash)
    WRITE32(0x80082d90, 0xA01E0050); //fix objData DLL offset
    WRITE32(0x80082d94, 0x2800FFFF); //don't load DLL -1
    WRITE32(0x80082d9c, 0xA07E0050); //fix objData DLL offset
    WRITE32(0x800d83ec, 0x38600022); //hack newstars
    WRITE32(0x8007b7ec, 0x8081FFE0); //clear memory on alloc
    WRITE32(0x8007b7f0, 0x4BF87A60); //clear memory on alloc

    initCompressionHooks();

    OSReport("Hooks installed!");
}
