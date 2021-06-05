/** Display some debug information on-screen.
 */
#include "main.h"

u32 debugTextFlags = DEBUGTEXT_PLAYER_COORDS | //DEBUGTEXT_PLAYER_STATE |
    DEBUGTEXT_CAMERA_COORDS | DEBUGTEXT_MEMORY_INFO |
    DEBUGTEXT_INTERACT_OBJ_INFO;

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

        u8 color = pRestartPoint ? 0xFF : 0x7F;
        char sColor[6] = {0x81, color, color, color, 0xFF, 0};

        //map ID is 0xFF sometimes, no idea why
        debugPrintf("%sR:" DPRINT_FIXED "%6d %6d %6d %d M%02X" DPRINT_NOFIXED,
            sColor,
            (int)chrPos->pos.x, (int)chrPos->pos.y, (int)chrPos->pos.z,
            chrPos->mapLayer, chrPos->mapId & 0xFF);
        debugPrintf("%s\n" DPRINT_COLOR "\xFF\xFF\xFF\xFF",
            restartPointFrameCount == 0 ? "" : " CHANGED");
    }
    else debugPrintf(DPRINT_FIXED "R: none" DPRINT_NOFIXED "\n");
}

static void printCamera() {
    //Display camera coords
    if(pCamera) {
        debugPrintf("C:" DPRINT_FIXED "%6d %6d %6d M%02X" DPRINT_NOFIXED "\n",
            (int)pCamera->pos.pos.x, (int)pCamera->pos.pos.y,
            (int)pCamera->pos.pos.z, cameraMode);
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

static void printHeapInfo() {
    //Display detailed heap info
    debugPrintf(DPRINT_FIXED "Free Blocks   Free KBytes\n" DPRINT_NOFIXED);
    for(int i=0; i<NUM_HEAPS; i++) {
        debugPrintf(DPRINT_FIXED "%5d/%5d %6d/%6d\n" DPRINT_NOFIXED,
            heaps[i].avail - heaps[i].used, heaps[i].avail, //blocks
            (heaps[i].dataSize - heaps[i].size) / 1024, heaps[i].dataSize / 1024 //bytes
        );
    }

    debugPrintf("Emerg frees: %d; alloc fails: %d\n", emergFreeCount, allocFailCount);
    if(allocFailCount > 0) {
        debugPrintf(DPRINT_FIXED "FailSize FailTag" DPRINT_NOFIXED "\n");
        for(int i=0; i<MIN(allocFailCount, ALLOC_FAIL_LOG_SIZE); i++) {
            if(i == allocFailLogIdx) debugPrintf(DPRINT_COLOR "\x00\xFF\x00\xFF");
            else debugPrintf(DPRINT_COLOR "\xFF\xFF\xFF\xFF");
            debugPrintf(DPRINT_FIXED "%08X %08X" DPRINT_NOFIXED "\n",
                allocFailLog[i].size, allocFailLog[i].tag);
        }
        debugPrintf(DPRINT_COLOR "\xFF\xFF\xFF\xFF");
    }
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

static void printPlayerObj(const char *name, ObjInstance *obj) {
    if(!(PTR_VALID(obj) && PTR_VALID(obj->file))) return;
    debugPrintf("%s " DPRINT_FIXED "%08X %s %08X ",
        name, obj, obj->file->name, obj->objDef->id);
    debugPrintf("%d, %d, %d" DPRINT_NOFIXED "\n",
        (int)obj->pos.pos.x,
        (int)obj->pos.pos.y,
        (int)obj->pos.pos.z);
}

static void printPlayerState() {
    debugPrintf("Player=%08X: ", pPlayer);
    if(pPlayer && pPlayer->file) {
        debugPrintf("%s\n", pPlayer->file->name);
    }
    else {
        debugPrintf("-\n");
        return;
    }
    if(pPlayer->catId != 1) return; //don't apply to title screen Fox

    void *pState = pPlayer ? pPlayer->state : NULL;
    if(!pState) return;
    PlayerCharState *cState = *(PlayerCharState**)(pState + 0x35C);

    //most of these are unknown
    debugPrintf(DPRINT_FIXED "%02X %08X %08X %08X %08X ",
        *(u16*)(pState + 0x274),
        *(u32*)(pState + 0x310),
        *(u32*)(pState + 0x314),
        *(u32*)(pState + 0x318),
        *(u32*)(pState + 0x31C));

    debugPrintf("%08X %08X %08X %04X %02X" DPRINT_NOFIXED "\n",
        *(u32*)(pState + 0x360),
        *(u32*)(pState + 0x3F0),
        *(u32*)(pState + 0x3F4),
        cState ? ((cState->flags02 << 8) | cState->field_03) : 0,
        cState ? cState->field_0B : 0);

    printPlayerObj("Death obj",   *(ObjInstance**)(pState + 0x46C));
    printPlayerObj("Obj4C4",      *(ObjInstance**)(pState + 0x4C4));
    printPlayerObj("Collect obj", *(ObjInstance**)(pState + 0x684));
    printPlayerObj("Obj7B0",      *(ObjInstance**)(pState + 0x7B0));
    printPlayerObj("Ride obj",    *(ObjInstance**)(pState + 0x7F0));
    //XXX confirm 7F4 is an object
    printPlayerObj("Obj7F4",      *(ObjInstance**)(pState + 0x7F4));
    printPlayerObj("Hold obj",    *(ObjInstance**)(pState + 0x7F8));
}

void mainLoopDebugPrint() {
    debugPrintf(DPRINT_COLOR "\xFF\xFF\xFF\xFF"); //reset color

    if(debugTextFlags & DEBUGTEXT_PLAYER_COORDS) printCoords();
    if(debugTextFlags & DEBUGTEXT_CAMERA_COORDS) printCamera();
    if(debugTextFlags & DEBUGTEXT_RESTART_POINT) printRestartPoint();
    if(debugTextFlags & DEBUGTEXT_PLAYER_STATE)  printPlayerState();
    if(debugTextFlags & DEBUGTEXT_MEMORY_INFO) {
        printObjCount();
        printMemory();
    }
    if(debugTextFlags & DEBUGTEXT_HEAP_STATE) printHeapInfo();
    if(debugTextFlags & DEBUGTEXT_INTERACT_OBJ_INFO) printTarget();

    debugPrintf("\n"); //for game's own messages

    if(debugTextFlags & DEBUGTEXT_HEAP_GRAPH) drawHeaps();
    if(debugTextFlags & DEBUGTEXT_WORLD_MAP) drawMapGrid();
}
