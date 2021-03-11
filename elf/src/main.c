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

void _start(void) {
    OSReport("Patch running!");
    gameBitHook_replaced = (BOOL(*)())hookBranch(0x8002010C, gameBitHook, 1);
    OSReport("Hook installed!");
}
