#include "main.h"

static void (*getEnvfxAct_replaced)(void *buf, DataFileEnum32 fileId, uint idx, int size);
static void (*getEnvfxActImm_replaced)(void *buf, DataFileEnum32 fileId, uint idx, int size);

void getEnvFxActHook(void *buf, DataFileEnum32 fileId, uint idx, int size) {
    DPRINT("GetEnvFxAct(0x%04X)\n", idx / 0x60);
    getTabEntry(buf, fileId, idx, size);
}

void envHooksInit() {
    getEnvfxAct_replaced =
        (void(*)(void *buf, DataFileEnum32 fileId, uint idx, int size))
        hookBranch(0x80008cfc, getEnvFxActHook, 1);
    getEnvfxActImm_replaced =
        (void(*)(void *buf, DataFileEnum32 fileId, uint idx, int size))
        hookBranch(0x80008bb4, getEnvFxActHook, 1);
}
