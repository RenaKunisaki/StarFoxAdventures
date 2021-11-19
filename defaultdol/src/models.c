#include "main.h"

int loadModelInstanceHook(int id) {
    int realId = model_lookupModelInd(id); //replaced
    OSReport("Load model instance 0x%X (0x%X)\n", id, realId);
    return realId;
    //return 1;
}

void* loadModelHook(int id) {
    void *model = model_load(id);
    u32 *table = dataFileBuffers[MODELS_TAB];
    u32  entry = table[id];

    OSReport("Loaded model 0x%X to 0x%08X (entry 0x%08X)\n", id, model, entry);
    OSReport("vertexPositions = 0x%08X\n", *(u32*)(((u32)model) + 0x28));
    OSReport("vertexNormals   = 0x%08X\n", *(u32*)(((u32)model) + 0x2C));
    OSReport("vertexColours   = 0x%08X\n", *(u32*)(((u32)model) + 0x30));
    OSReport("vertexTexCoords = 0x%08X\n", *(u32*)(((u32)model) + 0x34));
    return model;
}

void _decompModelHook(int file, uint offset, void *dest) {
    offset &= 0xFFFFFF;
    u32 *src = dataFileBuffers[file] + offset;
    OSReport("Decode model from file 0x%X offset 0x%X (0x%08X) to 0x%08X\n",
        file, offset, src, dest);
    u32 header = *src;
    u32 outLen = 0;
    int err;

    err = decompress(src, 0, dest, &outLen);
    if(!err) {
        OSReport("Decompressed model: 0x%X bytes\n", outLen);
    }
    else OSReport("Decompress model failed (%d)\n", err);
}

void decompModelHook(void);
__asm__(
    "decompModelHook:       \n"
    ASM_FUNC_START(0x100)
    "mr     3, 25           \n" //file
    "mr     4, 30           \n" //offset
    "mr     5, 22           \n" //dest
    "bl     _decompModelHook\n"
    ASM_FUNC_END(0x100)
    "lis    3, 0x8009       \n"
    "ori    3, 3, 0xADF4    \n" //jump back to original code
    "mtctr  3               \n"
    "bctr"
);

void initModelHooks() {
    hookBranch(0x8007dba8, loadModelInstanceHook, 1);
    hookBranch(0x8007dbf4, loadModelHook, 1);
    //WRITE_NOP(0x8007dc78); //don't store to modelsLoadedTable
    hookBranch(0x8009ab38, decompModelHook, 0);

    //increase buffer sizes for map buffers to match final version.
    WRITE16(0x800a70ca, 0x0100); //trk_cblocks
    WRITE16(0x800a70de, 0x0080); //trk_blknos
    WRITE16(0x800a70f2, 0x0040); //trk_blkusage
    //WRITE16(0x800a7106, 0x????); //trk_mapinfo (not in final)
    WRITE16(0x800a711a, 0x0D48); //trk_tempbuf
    WRITE16(0x800abed2, 0x0D48); //trk_tempbuf size check
    //others are unchanged
}
