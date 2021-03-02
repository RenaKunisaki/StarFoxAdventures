#include "main.h"

uint32_t hookBranch(uint32_t addr, void *target, int isBl) {
    uint32_t oldOp = *(uint32_t*)addr;
    //make b or bl opcode
    uint32_t relDest = (uint32_t)target - addr;
    *(uint32_t*)addr = (relDest & 0x07FFFFFC) | (isBl ? 1 : 0) | 0x48000000;

    //decode original instruction and return original destination
    return addr + (oldOp & 0x07FFFFFC);
}
