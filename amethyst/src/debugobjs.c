//This hook detects when an object's render method is about to be used but is
//a stub, and instead uses the default render method.
//The effect is that more debug objects will be visible when displaying them
//is enabled (as well as potentially a few that weren't meant to be).
//This isn't perfect since there could still be some whose render method isn't
//a one-instruction stub but still does nothing.
//This hook has no effect if debug objects aren't enabled.
//XXX we still need a way to toggle them.
void _debugObjsHook(void);
__asm__(
    "_debugObjsHook:               \n"
    "lwz     9,  0(12)             \n" //get first instr of render method
    "lis     8,  0x4E80            \n"
    "ori     8,  8,  0x0020        \n"
    "cmpw    9,  8                 \n" //is it a blr instruction?
    "beq     .debugObjsHook_stub   \n"
    //not a stub, so call it like normal.
    "or      8,  31, 31            \n" //replaced
    "mtctr   12                    \n"
    "bctrl                         \n"

    ".debugObjsHook_done:          \n" //return to game code
    "lis     4,  0x8003            \n"
    "ori     4,  4,  0xBB00        \n"
    "mtctr   4                     \n"
    "bctr                          \n"

    ".debugObjsHook_stub:          \n" //render method is a stub
    "lis     9,  0x8003            \n"
    "ori     9,  9,  0xBAC8        \n"
    "mtctr   9                     \n"
    "bctr                          \n" //jump to default render method
);

void debugObjsInit() {
    hookBranch(0x8003ba28, _debugObjsHook, 1);
}
