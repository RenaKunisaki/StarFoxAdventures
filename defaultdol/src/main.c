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
    //WRITE32(0x80082e88, 0x3AA00001); //only load one frame of object (else we get sparsearray crash)
    //WRITE32(0x80082d90, 0xA01E0050); //fix objData DLL offset
    //WRITE32(0x80082d9c, 0xA07E0050); //fix objData DLL offset
    //WRITE32(0x80082d94, 0x2800FFFF); //don't load DLL -1
    WRITE32(0x800d83ec, 0x38600022); //hack newstars
    WRITE32(0x8007b7ec, 0x8081FFE0); //clear memory on alloc
    WRITE32(0x8007b7f0, 0x4BF87A60); //clear memory on alloc
    WRITE_BLR(0x800a72ec); //do not update environment
    WRITE32(0x800a7758, 0x38000001); //disable sky

    WRITE32(0x80084a44, 0x38600000); //objGetControlNo return 0
    WRITE_BLR(0x80084a48); //objGetControlNo return 0

    //WRITE32(0x80181bc0, 0x38600001); //isModelAnimDisabled return 1
    //WRITE_BLR(0x80181bc4); //isModelAnimDisabled return 1
    WRITE_BLR(0x8008045c); //disable a model anim function
    WRITE_NOP(0x800947f4); //disable another anim function

    //fix ObjData modline offset and size (XXX correct for DP?)
    //WRITE32(0x800847e4, 0x881F005C); //lha r0,0x78(r31) -> lbz r0,0x5C(r31)
    //WRITE32(0x800847fc, 0x88BF005C); //lha r5,0x78(r31) -> lbz r5,0x5C(r31)
    //WRITE32(0x80084808, 0x887F005C); //lha r3,0x78(r31) -> lbz r3,0x5C(r31)
    //WRITE32(0x8008480c, 0x389F005D); //addi r4,r31,0x76 -> addi r4,r31,0x5D
    //WRITE32(0x800849e0, 0x9B630000); //sth r27,0x0(out) -> stb r27,0x0(out)
    //WRITE32(0x800847e4, 0x3800FFFF); //don't load modlines

    WRITE_BLR(0x8007cc94); //disable makeModelAnimation
    WRITE_BLR(0x8007cfa4); //disable modelSetupAnims

    initBsodHooks();
    initCompressionHooks();
    initTextureHooks();
    initModelHooks();
    initLoggingHooks();
    OSReport("Flushing cache...\n");
    __flush_cache(0x80000000, 0x81800000);

    OSReport("Hooks installed!\n");
}
