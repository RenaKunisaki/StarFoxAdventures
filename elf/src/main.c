#include "main.h"

static BOOL (*gameBitHook_replaced)();
BOOL gameBitHook(int bit, int val) {
    //this doesn't work because the regs are overwritten by the compiler's
    //boilerplate code. we can probably work around that by using something
    //like attribute naked, but we don't need to, because r3 and r4 are still
    //intact, so we can just use parameters.
    //int bit, val;
    //GET_REGISTER(29, bit);
    //GET_REGISTER(30, val);
    OSReport("GameBit 0x%04X set to %d", bit, val);
    return gameBitHook_replaced(); //BOOL isSaveGameLoading()
}


void mainLoopHook() {
    //replaces a bl to a do-nothing subroutine
    static int count = 0;
    debugPrintf("Frame %d\n", count++);
    debugPrintf("Player=%08X: ", pPlayer);
    if(pPlayer) {
        debugPrintf("%s\n", pPlayer->file->name);
    }
    else debugPrintf("-\n");
}


void _start(void) {
    OSReport("Patch running!");

    //Install hooks
    gameBitHook_replaced = (BOOL(*)())hookBranch(0x8002010C, gameBitHook, 1);
    runLoadingScreens_replaced = (void(*)())hookBranch(0x80020f2c, runLoadingScreens_hook, 1);
    startMsg_initDoneHook_replaced = (void(*)())hookBranch(0x80021250, startMsg_initDoneHook, 1);
    hookBranch(0x80020D4C, mainLoopHook, 1);

    //debug print
    hookBranch(0x80137948, debugPrintfHook, 0);
    WRITE32(0x801378A8, 0x480000A0); //restore debugPrintf
    WRITE8 (0x80137317, 6); //smaller text for fixed-width mode
    WRITE16(0x803E23B8, 0x3FA0); //smaller text
    WRITE32(0x80137CF4, 0x3BFF000C); //smaller text for debugPrintfxy
    if(consoleType & 0xF0000000) { //emulator
        //move debug print to edge of screen
        WRITE16(0x8013761A, 0); //min X at 320 screen width
        WRITE16(0x8013762E, 0); //min X at 640 screen width
        WRITE16(0x8013764A, 0); //min Y at 240 screen height
        WRITE16(0x8013765E, 0); //min Y at 480 screen height - causes a glitch at bottom when fading
        //WRITE32(0x80137830, 0x38000000); //these two prevent fade glitch
        //WRITE32(0x80137688, 0x38000000);
    }

    //debug stuff
    WRITE32(0x80119D90, 0x60000000); //chapter select only needs Z button

    OSReport("Hooks installed!");
}
