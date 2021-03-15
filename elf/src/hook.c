/** Utility functions to hook game code.
 */
#include "main.h"

uint32_t hookBranch(uint32_t addr, void *target, int isBl) {
    /** Replace a `b` or `bl` instruction in memory.
     *  @param addr Address to hook.
     *  @param target Function to branch to.
     *  @param isBl whether to insert a `b` or a `bl`.
     *  @return The original destination address.
     *  @note The target address should be a `b` or `bl` instruction.
     *  Otherwise, the return value is not useful, and special care needs to
     *  be taken to prevent the injected code from interfering with the
     *  original code's register state.
     */
    uint32_t oldOp = *(uint32_t*)addr;
    //make b or bl opcode
    uint32_t relDest = (uint32_t)target - addr;
    *(uint32_t*)addr = (relDest & 0x07FFFFFC) | (isBl ? 1 : 0) | 0x48000000;

    //decode original instruction and return original destination
    return addr + (oldOp & 0x07FFFFFC);
}
