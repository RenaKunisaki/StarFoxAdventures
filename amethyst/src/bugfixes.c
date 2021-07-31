//Patches to fix bugs in the original game.
#include "main.h"

//Fix a crash that happens when trying to report that the object message queue is full.
void fixObjMsgFull(void);
__asm__(
    "fixObjMsgFull:                   \n"
    "cmpwi    7,  0                   \n"
    "beq      .fixObjMsgFull_dontCrash\n"
    "lha      7,  0x46(7)             \n" //replaced
    "b        .fixObjMsgFull_end      \n"
    ".fixObjMsgFull_dontCrash:        \n"
    "li       7,  -1                  \n"
    ".fixObjMsgFull_end:              \n"
    "lis      8,  0x8003              \n"
    "ori      8,  8,  0x7948          \n"
    "mtctr    8                       \n"
    "bctr                             \n"
);

void initBugFixes() {
    hookBranch(0x80037944, fixObjMsgFull, 0);
}
