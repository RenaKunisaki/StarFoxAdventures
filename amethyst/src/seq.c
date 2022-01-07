#include "main.h"

void hook_objRunSeq(int seqIdx, ObjInstance *obj, u32 flags) {
    char name[16];
    getObjName(name, obj);
    OSReport("objRunSeq(0x%X, 0x%08X \"%s\", 0x%08X)\n",
        seqIdx, obj, name, flags);
}
void _hook_objRunSeq(void);
__asm__(
    "_hook_objRunSeq:        \n"
    ASM_FUNC_START(0x100)
    "bl    hook_objRunSeq    \n"
    ASM_FUNC_END(0x100)
    "lis      20, 0x8008     \n"
    "ori      20, 20, 0x0E18 \n"
    "mtctr    20             \n"
    "or       20, 3, 3       \n" //replaced
    "bctr                    \n"
);


void hook_endObjSequence(int seqIdx) {
    OSReport("endObjSequence(0x%X)\n", seqIdx);
}
void _hook_endObjSequence(void);
__asm__(
    "_hook_endObjSequence:        \n"
    ASM_FUNC_START(0x100)
    "bl    hook_endObjSequence    \n"
    ASM_FUNC_END(0x100)
    "lis      28, 0x8008          \n"
    "ori      28, 28, 0x0C30      \n"
    "mtctr    28                  \n"
    "or       28, 3, 3            \n" //replaced
    "bctr                         \n"
);


void hook_seqFn0D(ObjInstance *obj) {
    char name[16];
    getObjName(name, obj);
    OSReport("seqFn0D(0x%08X \"%s\")\n", obj, name);
}
void _hook_seqFn0D(void);
__asm__(
    "_hook_seqFn0D:               \n"
    ASM_FUNC_START(0x100)
    "bl    hook_seqFn0D           \n"
    ASM_FUNC_END(0x100)
    "lis      30, 0x8008          \n"
    "ori      30, 30, 0x1984      \n"
    "mtctr    30                  \n"
    "or       30, 3, 3            \n" //replaced
    "bctr                         \n"
);

void seqHooksInit() {
    hookBranch(0x80080E14, _hook_objRunSeq, 0);
    hookBranch(0x80080C2C, _hook_endObjSequence, 0);
    hookBranch(0x80081980, _hook_seqFn0D, 0);
}
