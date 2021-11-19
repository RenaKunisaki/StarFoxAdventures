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

void initModelHooks() {
    hookBranch(0x8007dba8, loadModelInstanceHook, 1);
    hookBranch(0x8007dbf4, loadModelHook, 1);
    //WRITE_NOP(0x8007dc78); //don't store to modelsLoadedTable

    //increase buffer sizes for map buffers to match final version.
    WRITE16(0x800a70ca, 0x0100); //trk_cblocks
    WRITE16(0x800a70de, 0x0080); //trk_blknos
    WRITE16(0x800a70f2, 0x0040); //trk_blkusage
    //WRITE16(0x800a7106, 0x????); //trk_mapinfo (not in final)
    WRITE16(0x800a711a, 0x0D48); //trk_tempbuf
    WRITE16(0x800abed2, 0x0D48); //trk_tempbuf size check
    //others are unchanged
}
