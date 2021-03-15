/** Display some debug information on-screen.
 */
#include "main.h"

static void printCoords() {
    //Display player coords
    //debugPrintf doesn't support eg '%+7.2f' so we'll just convert
    //to int to get rid of excess digits
    if(pPlayer) {
        debugPrintf("P:" DPRINT_FIXED "%6d %6d %6d %08X" DPRINT_NOFIXED,
            (int)pPlayer->pos.pos.x, (int)pPlayer->pos.pos.y, (int)pPlayer->pos.pos.z,
            pPlayer);
    }

    //Display map coords
    int mapNo = curMapId;
    if(mapNo >= NUM_MAP_DIRS) mapNo = mapActLut[mapNo];
    debugPrintf(" M:" DPRINT_FIXED "%3d,%3d,%d #%02X T%X %08X" DPRINT_NOFIXED,
        mapCoords.mapX, mapCoords.mapZ, curMapLayer, curMapId,
        mainGetBit(mapActBitIdx[mapNo]),
        mainGetBit(mapObjGroupBit[mapNo]));

    //Display locked map IDs
    debugPrintf(" L:"DPRINT_FIXED "%02X%02X %02X%02X\n" DPRINT_NOFIXED,
        loadedFileMapIds[FILE_BLOCKS_BIN] & 0xFF,
        loadedFileMapIds[FILE_BLOCKS_BIN2] & 0xFF,
        //buckets are int, but we really only need lowest byte.
        levelLockBuckets[0] & 0xFF, levelLockBuckets[1] & 0xFF);
}

static void printRestartPoint() {
    //Display restart point
    static int restartPointFrameCount = 0;
    static vec3f prevRestartPoint = {0, 0, 0};
    static u8 prevRestartPointLayer = 0;
    static u8 prevRestartPointMap = 0;

    SaveGame *restart = pRestartPoint;
    if(!restart) restart = pLastSavedGame;
    if(restart) {
        //PlayerCharState *chrState = restart->charState[restart->character];
        PlayerCharPos   *chrPos   = &restart->charPos  [restart->character];

        //if restart point has changed, highlight it
        if(chrPos->pos.x    != prevRestartPoint.x
        || chrPos->pos.y    != prevRestartPoint.y
        || chrPos->pos.z    != prevRestartPoint.z
        || chrPos->mapLayer != prevRestartPointLayer
        || chrPos->mapId    != prevRestartPointMap) {
            restartPointFrameCount = 60;
            prevRestartPoint.x    = chrPos->pos.x;
            prevRestartPoint.y    = chrPos->pos.y;
            prevRestartPoint.z    = chrPos->pos.z;
            prevRestartPointLayer = chrPos->mapLayer;
            prevRestartPointMap   = chrPos->mapId;
        }
        else if(restartPointFrameCount > 0) restartPointFrameCount--;

        u8 color = 255 - (restartPointFrameCount * 4);
        if(!color) color = 1; //don't break sprintf
        char sColor[6] = {0x81, color, 0xFF, 0xFF, 0xFF, 0};

        //map ID is 0xFF sometimes, no idea why
        debugPrintf("%sR:" DPRINT_FIXED "%6d %6d %6d %d M%02X" DPRINT_NOFIXED
            DPRINT_COLOR "\xFF\xFF\xFF\xFF\n", sColor,
            (int)chrPos->pos.x, (int)chrPos->pos.y, (int)chrPos->pos.z,
            chrPos->mapLayer, chrPos->mapId & 0xFF);
    }
}

static void printCamera() {
    //Display camera coords
    if(pCamera) {
        debugPrintf("C:" DPRINT_FIXED "%6d %6d %6d " DPRINT_NOFIXED,
            (int)pCamera->pos.pos.x, (int)pCamera->pos.pos.y, (int)pCamera->pos.pos.z);
    }
}

static void printObjCount() {
    //Display number of objects
    debugPrintf("Obj:" DPRINT_FIXED "%3d" DPRINT_NOFIXED " ", numLoadedObjs);
}

static void printMemory() {
    //Display memory used percentages
    u32 totalBlocks=0, totalBytes=0, usedBlocks=0, usedBytes=0;
    for(int i=0; i<NUM_HEAPS; i++) {
        totalBytes  += heaps[i].dataSize;
        totalBlocks += heaps[i].avail;
        usedBytes   += heaps[i].size;
        usedBlocks  += heaps[i].used;
    }
    int bytesPct  = (usedBytes  * 100) / totalBytes;
    int blocksPct = (usedBlocks * 100) / totalBlocks;
    u32 color = 0xFFFFFFFF; //rgba
    if     (bytesPct >= 90 || blocksPct >= 90) color = 0xFF0000FF;
    else if(bytesPct >= 85 || blocksPct >= 85) color = 0xFF0000FF;
    char sColor[6] = {0x81, (color >> 24), (color >> 16), (color >> 8), color, 0};
    debugPrintf("%sMem " DPRINT_FIXED "%3d %3d" DPRINT_COLOR "\xFF\xFF\xFF\xFF"
        DPRINT_NOFIXED "\n", sColor, bytesPct, blocksPct);
}

static void printTarget() {
    //Display target that player is focused on
    if(pCamera && pCamera->target) {
        debugPrintf("Target: " DPRINT_FIXED "%08X %08X %04X " DPRINT_NOFIXED "%s\n",
            pCamera->target,
            pCamera->target->objDef->id,
            pCamera->target->defNo,
            pCamera->target->file->name);
    }
}

void mainLoopDebugPrint() {
    debugPrintf(DPRINT_COLOR "\xFF\xFF\xFF\xFF"); //reset color
    /* debugPrintf("Player=%08X: ", pPlayer);
    if(pPlayer) {
        debugPrintf("%s\n", pPlayer->file->name);
    }
    else debugPrintf("-\n"); */

    printCoords();
    printCamera();
    printObjCount();
    printMemory();
    printRestartPoint();
    printTarget();

    debugPrintf("\n"); //for game's own messages
}
