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
}


void _start(void) {
    OSReport("Patch running!");

    //Install hooks
    gameBitHook_replaced = (BOOL(*)())hookBranch(0x8002010C, gameBitHook, 1);
    runLoadingScreens_replaced = (void(*)())hookBranch(0x80020f2c, runLoadingScreens_hook, 1);
    startMsg_initDoneHook_replaced = (void(*)())hookBranch(0x80021250, startMsg_initDoneHook, 1);
    hookBranch(0x80020D4C, mainLoopHook, 1);
    hookBranch(0x80137948, debugPrintfHook, 0);

    OSReport("Hooks installed!");
}
