/** Bootstrap, runs at startup and installs other hooks.
 */
#include "main.h"

//hack to ensure nothing is at offset 0 because that makes relocation difficult.
int fart __attribute__((section(".offsetzero"))) = 0x52656E61;

void mainLoopHook() {
    //replaces a bl to a do-nothing subroutine
    OSReport("main loop");
}

void _start(void) {
    OSReport("Patch running!");

    //Install hooks
    hookBranch(0x800781fc, mainLoopHook, 1);

    OSReport("Hooks installed!");
}
