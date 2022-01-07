#include "main.h"
#include "revolution/os.h"

void initEndHook(const char *msg) {
    //msg = "finished init"
    u32 totalBlocks, totalBytes, usedBlocks, usedBytes;
    int blocksPct, bytesPct;
    getFreeMemory(&totalBlocks, &totalBytes, &usedBlocks, &usedBytes,
        &blocksPct, &bytesPct);
    OSReport("Init done; bytes used %d/%d, blocks %d/%d, took %f sec\n",
        usedBytes, totalBytes, usedBlocks, totalBlocks,
        ticksToSecs(__OSGetSystemTime() - tBootStart));
}

void initBootHacks() {
    hookBranch(0x80021320, initEndHook, 1);
}
