/** Bootstrap, runs at startup and installs other hooks.
 */
#include "main.h"

u32 debugCheats = 0; //DebugCheat
u8 overrideFov = 60;
u8 furFxMode = 0; //FurFxMode
bool bRumbleBlur = false;
bool bDisableParticleFx = false;

//XXX move this
static BOOL (*gameBitHook_replaced)();
BOOL gameBitHook(int bit, int val) {
    //this doesn't work because the regs are overwritten by the compiler's
    //boilerplate code. we can probably work around that by using something
    //like attribute naked, but we don't need to, because r3 and r4 are still
    //intact, so we can just use parameters.
    //int bit, val;
    //GET_REGISTER(29, bit);
    //GET_REGISTER(30, val);
    //DPRINT("GameBit 0x%04X set to %d", bit, val);
    return gameBitHook_replaced(); //BOOL isSaveGameLoading()
}


void mainLoopHook() {
    //replaces a bl to a do-nothing subroutine

    //sanity check
    if(furFxMode >= NUM_FURFX_MODES) furFxMode = 0;
    if(backpackMode >= NUM_BACKPACK_MODES) backpackMode = 0;
    if(overridePlayerNo >= NUM_PLAYER_IDS) overridePlayerNo = 0;
    if(overrideMinimapSize >= NUM_MINIMAP_SIZES) overrideMinimapSize = 0;
    if(overrideFov == 0) overrideFov = 60;

    //do some overrides
    if(overrideFov != 60 && !CameraParamsViewfinder) {
        //override if not in first person
        fovY = overrideFov;
        firstPersonFovY = overrideFov;
        if(pCamera) pCamera->fov = overrideFov;
        if(cameraMtxVar57) cameraMtxVar57->mtx1[3][0] = overrideFov;
    }

    switch(furFxMode) {
        case FURFX_NORMAL:
            WRITE16(0x800414E2, 0);
            WRITE32(0x800414B4, 0x9421FFE0); //original opcode
            iCacheFlush((void*)0x800414B4, 0x40);
            break;
        case FURFX_ALWAYS:
            WRITE16(0x800414E2, 0x1312); //arbitrary constant
            WRITE32(0x800414B4, 0x9421FFE0); //original opcode
            iCacheFlush((void*)0x800414B4, 0x40);
            break;
        default: //case FURFX_NEVER:
            WRITE_BLR(0x800414B4);
            iCacheFlush((void*)0x800414B4, 0x40);
    }

    WRITE32(0x800a4df4, bDisableParticleFx ? 0x4E800020 : 0x9421FED0);
    iCacheFlush((void*)0x800a4df4, 4);

    WRITE32(0x80148bc8, (debugTextFlags & DEBUGTEXT_TRICKY) ? 0x4BFEED80 : 0x9421FF90);
    iCacheFlush((void*)0x80148bc8, 4);

    minimapMainLoopHook();
    mainLoopDebugPrint();
    runMenu();
    krystalMainLoop();
    saveUpdateHook();

    //correct aspect ratio
    if(renderFlags & RenderFlag_Widescreen) {
        viewportAspect = 16.0 / 9.6;
        viewportAspectWidescreen = 16.0 / 9.6;
        viewportAspectWidescreenShadows = 16.0 / 9.6;
    }
    else {
        viewportAspect = 5.0 / 4.0;
        viewportAspectNotWidescreen = 5.0 / 4.0;
        //shadows use same address as main viewport for non-widescreen
    }

    playerMainLoopHook();

    PlayerCharState *playerState =
        &saveData.curSaveGame.charState[saveData.curSaveGame.character];
    if(debugCheats & DBGCHT_INF_HP) {
        //XXX won't work in Arwing levels
        playerState->curHealth = playerState->maxHealth;
    }
    if(debugCheats & DBGCHT_INF_MP) {
        playerState->curMagic = playerState->maxMagic;
    }
    if(debugCheats & DBGCHT_INF_MONEY) {
        playerState->money = 255;
    }
    if(debugCheats & DBGCHT_INF_LIVES) {
        playerState->curBafomDads = playerState->maxBafomDads;
    }
    if(debugCheats & DBGCHT_ENEMY_FROZEN) {
        //patch objIsFrozen() to always return true
        WRITE32(0x8002b048, 0x38600001);
    }
    else WRITE32(0x8002b048, 0x540307FE);
}


static inline void _initSaveHacks() {
    hookBranch(0x800e7fb0, saveLoadHook, 1);
    hookBranch(0x80042ec4, saveMapLoadHook, 1);
    hookBranch(0x8007db50, saveShowMsgHook, 1);
    //disable "not same memory card you last saved with" check,
    //since save states trigger that.
    //XXX look into how this check works and find a less invasive
    //way of fixing this.
    WRITE32(0x8007EF5C, 0x3B200000);
    WRITE32(0x8007F15C, 0x3B200000);
}

static inline void _initDebugPrintHacks() {
    hookBranch(0x80137948, debugPrintfHook, 0);
    WRITE32   (0x801378A8, 0x480000A0); //restore debugPrintf
    WRITE8    (0x80137317, 6); //smaller text for fixed-width mode
    WRITE16   (0x803E23B8, 0x3FA0); //smaller text
    WRITE32   (0x80137CF4, 0x3BFF000C); //smaller text for debugPrintfxy
    if(consoleType & 0xF0000000) { //emulator
        //move debug print to edge of screen
        WRITE16(0x8013761A, 0); //min X at 320 screen width
        WRITE16(0x8013762E, 0); //min X at 640 screen width
        WRITE16(0x8013764A, 0); //min Y at 240 screen height
        WRITE16(0x8013765E, 0); //min Y at 480 screen height - causes a glitch at bottom when fading (or not?)
        //WRITE32(0x80137830, 0x38000000); //these two prevent fade glitch
        //WRITE32(0x80137688, 0x38000000);
    }
}

static inline void _initPauseMenuHacks() {
    WRITE32(0x8012A97C, 0x4BFBDD55); //disable save confirmation
    //disable some voices
    WRITE_NOP(0x8012A904);
    WRITE_NOP(0x8012A8C4);
    WRITE_NOP(0x8012A95C);
    WRITE_NOP(0x8012BA94);
    WRITE_NOP(0x8012B8DC);
    WRITE_NOP(0x8012B88C);
    WRITE_NOP(0x8012B8B4);
    WRITE_NOP(0x8012BD78);
}

static inline void _initCameraHacks() {
    hookBranch(0x8010328c,        cameraUpdateHook,         1);
    hookBranch((u32)padGetCX,     padGetCxHook,             0);
    hookBranch((u32)padGetCY,     padGetCyHook,             0);
    hookBranch((u32)padGetStickX, padGetStickXHook,         0);
    hookBranch((u32)padGetStickY, padGetStickYHook,         0);
    hookBranch(0x80133af0,        minimapButtonHeldHook,    1);
    hookBranch(0x80133afc,        minimapButtonPressedHook, 1);
    hookBranch(0x80108758,        viewFinderZoomHook,       1);

    //viWidth fix
    WRITE16(0x80049512, 704);
    WRITE16(0x80049526, -32);
}

static inline void _initPlayerHacks() {
    hookBranch(0x80021078, initPlayerStatesHook, 1);
}

static inline void _initControllerHacks() {
    //enable all four controllers, which enables at least one debug function
    WRITE8(0x80014B87, 4);
    WRITE8(0x80014BC7, 4);
    WRITE8(0x80014C1B, 4);
    WRITE8(0x80014C6F, 4);
    WRITE8(0x80014CC3, 4);
    WRITE8(0x80014D9F, 4);
    WRITE8(0x80014DDB, 4);
    WRITE8(0x80014E17, 4);
    WRITE8(0x80014E73, 4);
    WRITE8(0x80014EC7, 4);
    WRITE8(0x80014EEB, 4);
}

void _start(void) {
    DPRINT("Patch running!");

    //Install hooks
    gameBitHook_replaced = (BOOL(*)())hookBranch(0x8002010C, gameBitHook, 1);
    runLoadingScreens_replaced = (void(*)())hookBranch(0x80020f2c, runLoadingScreens_hook, 1);
    startMsg_initDoneHook_replaced = (void(*)())hookBranch(0x80021250, startMsg_initDoneHook, 1);
    hookBranch(0x80020D4C, mainLoopHook, 1);
    hookBranch(0x8005c45c, motionBlurHook, 1);
    hookBranch(0x800d9e2c, hudDrawHook, 1);
    hookBranch((u32)allocTagged, allocTaggedHook, 0);

    krystalInit();
    _initSaveHacks();
    _initDebugPrintHacks();
    _initPauseMenuHacks();
    _initCameraHacks();
    _initPlayerHacks();

    //debug stuff
    WRITE_NOP(0x80119D90); //chapter select only needs Z button

    //increase climbing speeds
    WRITEFLOAT(0x803E8000, 0.05); //wall climb (up and down)
    WRITE16(0x802A26BA, 0x1B70); //ladder climb (up)
    WRITE16(0x802A26A6, 0x1B70); //ladder climb (down)

    //hooks for fixed-width text
    hookBranch(0x80018414, textHook,     0);
    hookBranch(0x800183dc, textDrawHook, 1);
    //disable special case for spaces in text.
    //it's not necessary anyway and interferes
    //with our fixed width hack.
    WRITE32(0x80017C70, 0x2816ACAB);

    DPRINT("Hooks installed!");
}
