/** Bootstrap, runs at startup and installs other hooks.
 */
#include "main.h"
#include "revolution/os.h"
#include "revolution/pad.h"

//hack to ensure nothing is at offset 0 because that makes relocation difficult.
int fart __attribute__((section(".offsetzero"))) = 0x52656E61;

static u32 prevBtn3 = 0;
static u32 bHeld3;
static u32 bPressed3;
static bool showDetails = true;

//Fix a crash that happens when trying to report that the object
//message queue is full.
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

void getFreeMemory(u32 *outTotalBlocks, u32 *outTotalBytes,
u32 *outUsedBlocks, u32 *outUsedBytes, int *outBlocksPct, int *outBytesPct) {
    u32 totalBlocks=0, totalBytes=0, usedBlocks=0, usedBytes=0;
    for(int i=0; i<NUM_HEAPS; i++) {
        totalBytes  += heaps[i].dataSize;
        totalBlocks += heaps[i].avail;
        usedBytes   += heaps[i].size;
        usedBlocks  += heaps[i].used;
    }
    if(outTotalBlocks) *outTotalBlocks = totalBlocks;
    if(outTotalBytes)  *outTotalBytes  = totalBytes;
    if(outUsedBlocks)  *outUsedBlocks  = usedBlocks;
    if(outUsedBytes)   *outUsedBytes   = usedBytes;

    if(outBlocksPct) *outBlocksPct = (usedBlocks * 100) / totalBlocks;
    if(outBytesPct)  *outBytesPct  = (usedBytes  * 100) / totalBytes;
}

static inline void printObj(const char *prefix, ObjInstance *obj) {
    if(!obj) return;

    char name[16];
    char *next = name;
    for(int i=0; i<12; i++) {
        char c = (obj && obj->file) ? obj->file->name[i] : 0;
        if(c >= 0x20 && c <= 0x7E) *(next++) = c;
    }
    *next = 0;

    char msg[256]; //debugPrintf doesn't support %7.2f modifier
    sprintf(msg, DPRINT_FIXED "%s %7.2f %7.2f %7.2f %6d %6d %6d" DPRINT_NOFIXED
        " %s\n",
        prefix, obj->pos.pos.x, obj->pos.pos.y, obj->pos.pos.z,
        obj->pos.rotation.x, obj->pos.rotation.y, obj->pos.rotation.z, name);
    debugPrintf(msg);
}


static inline void printWater(void *pState) {
    float waterCurrentRelX = *(float*)(pState + 0x43C); //water current, player-relative
    float waterCurrentRelZ = *(float*)(pState + 0x440);
    float waterCurrentAbsX = *(float*)(pState + 0x648); //water current, world-relative
    float waterCurrentAbsZ = *(float*)(pState + 0x64C);
    float waterDepth       = *(float*)(pState + 0x838); //how deep we are in it
    float waterHeight      = *(float*)(pState + 0x83C); //Y pos of top
    float waterBottomDist  = *(float*)(pState + 0x1B0); //distance to bottom
    float curAbsLen = sqrtf( //current's absolute-vector length
        (waterCurrentAbsX*waterCurrentAbsX)+
        (waterCurrentAbsZ*waterCurrentAbsZ));
    float curRelLen = sqrtf( //current's relative-vector length
        (waterCurrentRelX*waterCurrentRelX)+
        (waterCurrentRelZ*waterCurrentRelZ));
    if(waterDepth > 0) {
        debugPrintf("Water plane @" DPRINT_FIXED "%5d" DPRINT_NOFIXED ", "
            DPRINT_FIXED "%4d" DPRINT_NOFIXED " from bottom, "
            DPRINT_FIXED "%4d" DPRINT_NOFIXED " from surface\n",
            (int)waterHeight, (int)waterBottomDist, (int)waterDepth);
        debugPrintf("current: " DPRINT_FIXED "%f, %f (%f)" DPRINT_NOFIXED
            "\ncur rel: " DPRINT_FIXED "%f, %f (%f)" DPRINT_NOFIXED "\n",
            waterCurrentAbsX, waterCurrentAbsZ, curAbsLen,
            waterCurrentRelX, waterCurrentRelZ, curRelLen);
    }
}

static void doControls() {
    bHeld3 = controllerStates[2].button;
    bPressed3 = bHeld3 & ~prevBtn3;
    prevBtn3 = bHeld3;

    //Start: toggle time stop
    if(bPressed3 & PAD_BUTTON_START) timeStop = !timeStop;

    //B: abort cutscene
    if(bPressed3 & PAD_BUTTON_B) endObjSequence(curSeqNo);

    //X: toggle detail readout
    if(bPressed3 & PAD_BUTTON_X) showDetails = !showDetails;

    //Z: hold to fast forward; L: hold for slow motion
    if(bHeld3 & PAD_TRIGGER_Z) physicsTimeScale = 180;
    else if(bHeld3 & PAD_TRIGGER_L) physicsTimeScale = 15;
    else physicsTimeScale = 60;

    //Y: advance one tick while time is stopped
    static bool isStep = false;
    if(timeStop) {
        if(bPressed3 & PAD_BUTTON_Y) {
            isStep = true;
            timeStop = false;
        }
    }
    else if(isStep) {
        timeStop = true;
        isStep = false;
    }
}

void _loop(ObjInstance *player, int param_2, int param_3,
int param_4, int param_5, bool shouldRender) {
    playerRender(player, param_2, param_3, param_4, param_5, shouldRender);
    doControls();

    static const char *white = "\xFF\xFF\xFF\xFF";
    //reset color
    debugPrintf(DPRINT_COLOR "%s" DPRINT_BGCOLOR "\x01\x01\x01\x3F", white);

    //Display locked map IDs
    debugPrintf("L:"DPRINT_FIXED "%02X%02X %02X%02X " DPRINT_NOFIXED,
        loadedFileMapIds[FILE_BLOCKS_BIN] & 0xFF,
        loadedFileMapIds[FILE_BLOCKS_BIN2] & 0xFF,
        //buckets are int, but we really only need lowest byte.
        levelLockBuckets[0] & 0xFF, levelLockBuckets[1] & 0xFF);

    //display map act and objgroups
    int mapNo = curMapId;
    if(mapNo >= NUM_MAP_DIRS) mapNo = mapActLut[mapNo];
    debugPrintf("Act:" DPRINT_FIXED "%d " DPRINT_NOFIXED
        "Grp " DPRINT_FIXED "%08X" DPRINT_NOFIXED,
        mainGetBit(mapActBitIdx[mapNo]),
        mainGetBit(mapObjGroupBit[mapNo]));

    //display memory info, obj count, FPS
    int blocksPct=0, bytesPct=0;
    getFreeMemory(NULL, NULL, NULL, NULL, &blocksPct, &bytesPct);
    const char *color = white;
    if(blocksPct < 10 || bytesPct < 10) color = "\xFF\x01\x01\xFF";
    else if(blocksPct < 20 || bytesPct < 20) color = "\xFF\xFF\x01\xFF";
    debugPrintf("%sM:" DPRINT_FIXED "%3d %3d" DPRINT_NOFIXED
        DPRINT_COLOR "%s O:" DPRINT_FIXED "%3d" DPRINT_NOFIXED
        " FPS:%2d\n",
        color, blocksPct, bytesPct, white, numLoadedObjs,
        (int)(1000.0 / msecsThisFrame));

    if(!showDetails) return;

    //display player velocity
    //putting it here makes the layout a little nicer, not hiding mana
    vec3f vel = player->vel;
    vec3f zero = {0, 0, 0};
    vel.z = vec3f_xzDistance(&vel, &zero);
    debugPrintf("Vel " DPRINT_FIXED "%+7.3f %+7.3f" DPRINT_NOFIXED "\n",
        vel.z, vel.y);

    //display player and other objs' positions, rotations
    debugPrintf(DPRINT_FIXED "         X    PosY     PosZ   RotX   RotY   RotZ Name\n");
    printObj("C", (ObjInstance*)pCamera);
    printObj("P", player);

    void *pState = player ? player->state : NULL;
    if(!pState) return;

    printObj("R", *(ObjInstance**)(pState + 0x7F0)); //riding object
    printObj("H", *(ObjInstance**)(pState + 0x7F8)); //held object
    printObj("T", pCamera->target);

    //display misc player info
    float onFire = *(float*)(pState + 0x79C);
    if(onFire) debugPrintf("On fire %f\n", onFire);
    printWater(pState);
}

static inline void _initDebugPrintHacks() {
    hookBranch(0x80137948, debugPrintfHook, 0);
    WRITE32   (0x801378A8, 0x480000A0); //restore debugPrintf
    WRITE8    (0x80137317, 6); //smaller text for fixed-width mode
    WRITE16   (0x803E23B8, 0x3FA0); //smaller text
    WRITE32   (0x80137CF4, 0x3BFF000C); //smaller text for debugPrintfxy
    WRITE16   (0x801372AA, 8); //less line spacing
    //this doesn't work, probably because we do it too late.
    /* if(consoleType & 0xF0000000) { //emulator
        //move debug print to edge of screen
        WRITE16(0x8013761A, 0); //min X at 320 screen width
        WRITE16(0x8013762E, 0); //min X at 640 screen width
        WRITE16(0x8013764A, 0); //min Y at 240 screen height
        WRITE16(0x8013765E, 0); //min Y at 480 screen height - causes a glitch at bottom when fading (or not?)
        //WRITE32(0x80137830, 0x38000000); //these two prevent fade glitch
        //WRITE32(0x80137688, 0x38000000);
    } */
}

void _start(void) {
    //DPRINT("Patch running!");
    _initDebugPrintHacks();
    hookBranch(0x80037944, fixObjMsgFull, 0);

    //replace the playerRender hook we used to bootstrap with one
    //that will jump directly into our code.
    hookBranch(0x8003BAC0, _loop, 1);
}
