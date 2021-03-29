/** Bootstrap, runs at startup and installs other hooks.
 */
#include "main.h"

//hack to ensure nothing is at offset 0 because that makes relocation difficult.
int fart __attribute__((section(".offsetzero"))) = 0x52656E61;

u32 debugCheats = 0; //DebugCheat
s16 overrideColorScale = -1;
u8 overrideFov = 60;
u8 furFxMode = 0; //FurFxMode
u16 dayOfYear, curYear;
bool bRumbleBlur = false;
bool bDisableParticleFx = false;


static inline void checkTime() {
    u64 ticks = __OSGetSystemTime();
    //this is necessary to make gcc not try to use soft float here.
    u32 tHi = ticks >> 32;
    u32 tLo = ticks & 0xFFFFFFFF;
    float fTicks = (float)tLo + (float)(tHi * 4294967296.0f);

    //note timestamp here is seconds since 2000-01-01
    //everything says this should be / 4 but I only get anything
    //sensible with / 2.
    float secs = fTicks / (__OSBusClock / 2);
    int days  = secs / 86400.0f; //non-leap days
    curYear = secs / 31556908.8f; //approximate average
    dayOfYear = days % 365 - (curYear / 4);
    //bool leap = (curYear % 4) == 0;
    //debugPrintf("Y %d D %d L %d\n", curYear, dayOfYear, leap);
}


static inline void doPadMainLoop() {
    static u32 prevBtn3 = 0;
    u32 bHeld3 = controllerStates[2].button;
    u32 bPressed3 = bHeld3 & ~prevBtn3;
    prevBtn3 = bHeld3;

    if(bPressed3 & PAD_BUTTON_START) timeStop = !timeStop;
}

void mainLoopHook() {
    //replaces a bl to a do-nothing subroutine

    //sanity check
    if(furFxMode >= NUM_FURFX_MODES) furFxMode = 0;
    if(backpackMode >= NUM_BACKPACK_MODES) backpackMode = 0;
    if(overridePlayerNo >= NUM_PLAYER_IDS) overridePlayerNo = 0;
    if(overrideMinimapSize >= NUM_MINIMAP_SIZES) overrideMinimapSize = 0;
    if(overrideFov == 0) overrideFov = 60;

    checkTime();
    doPadMainLoop();

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
    doFreeMove();
    saveUpdateHook();
    gameBitHookUpdate();

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
        playerState->curHealth = playerState->maxHealth;
        ObjInstance *arwing = getArwing();
        if(arwing) {
            u8 *state = (u8*)arwing->state;
            state[0x468] = state[0x469]; //curHealth = maxHealth
        }
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
    if(debugCheats & DBGCHT_INF_TRICKY) {
        saveData.curSaveGame.trickyEnergy = saveData.curSaveGame.maxTrickyEnergy;
    }

    //move camera while time is stopped in debug mode
    if(timeStop && (debugCameraMode != CAM_MODE_NORMAL) && !menuState) {
        cameraUpdate(1);
    }

    if(overrideColorScale >= 0) colorScale = overrideColorScale;
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

void pdaHook() {
    openMainMenu();
}

void _pdaHook(void);
__asm__(
    "_pdaHook:                \n"
    ASM_FUNC_START(0x80)
    "bl    pdaHook            \n"
    ASM_FUNC_END(0x80)
    "lis    0,  0x8013        \n" //skip actual on/off code
    "ori    0,  0,  0x3A94    \n"
    "mtctr  0                 \n"
    "bctr                     \n"
);

void _start(void) {
    DPRINT("Patch running!");

    //Install hooks
    hookBranch(0x80137df8, bsodHook, 1);
    runLoadingScreens_replaced = (void(*)())hookBranch(0x80020f2c,
        runLoadingScreens_hook, 1);
    startMsg_initDoneHook_replaced = (void(*)())hookBranch(0x80021250,
        startMsg_initDoneHook, 1);
    hookBranch(0x80020D4C, mainLoopHook, 1);
    hookBranch(0x8005c45c, motionBlurHook, 1);
    hookBranch(0x800d9e2c, hudDrawHook, 1);
    hookBranch((u32)allocTagged, allocTaggedHook, 0);
    hookBranch(0x80105df8, firstPersonHook, 1);
    hookBranch(0x80133A54, _pdaHook, 0);
    hookBranch(0x8020d31c, worldMapHook, 1);

    staffFxInit();
    krystalInit();
    _initSaveHacks();
    _initDebugPrintHacks();
    _initPauseMenuHacks();
    _initCameraHacks();
    _initPlayerHacks();
    gameBitHookInit();

    //debug stuff
    WRITE_NOP(0x80119D90); //chapter select only needs Z button

    //increase climbing speeds
    WRITEFLOAT(0x803E8000, 0.05); //wall climb (up and down)
    WRITE16(0x802A26BA, 0x1B70); //ladder climb (up)
    WRITE16(0x802A26A6, 0x1B70); //ladder climb (down)

    hookBranch(0x80018414, textSizeHook, 0);
    //hooks for fixed-width text
    hookBranch(0x800183dc, textDrawHook, 1);
    //disable special case for spaces in text.
    //it's not necessary anyway and interferes
    //with our fixed width hack.
    WRITE32(0x80017C70, 0x2816ACAB);
    textHookInit();

    //remove useless items from C menu
    //WRITE16(0x8031b1c0, 0x0096); //water spellstone 1
    //WRITE16(0x8031b1d0, 0x0096); //spellstone ?
    //WRITE16(0x8031b1e0, 0x0096); //spellstone ?
    //WRITE16(0x8031b1f0, 0x0096); //spellstone ?
    //WRITE16(0x8031b360, 0x0096); //fire spellstone 1
    //WRITE16(0x8031b370, 0x0096); //spellstone ?
    //WRITE16(0x8031b380, 0x0096); //fire spellstone 2
    //WRITE16(0x8031b390, 0x0096); //water spellstone 2
    WRITE16(0x8031b3e0, 0x0096); //fuel cells
    //WRITE16(0x8031b440, 0x0096); //give scarabs
    //WRITE16(0x8031b450, 0x0096); //cheat token 0
    //WRITE16(0x8031b460, 0x0096); //cheat token 3
    //WRITE16(0x8031b470, 0x0096); //cheat token 2
    //WRITE16(0x8031b480, 0x0096); //cheat token 6
    //WRITE16(0x8031b490, 0x0096); //cheat token 4
    //WRITE16(0x8031b4a0, 0x0096); //cheat token 7
    //WRITE16(0x8031b4b0, 0x0096); //cheat token 1
    //WRITE16(0x8031b4c0, 0x0096); //cheat token 5
    //WRITE16(0x8031b530, 0x0096); //Open Portal spell
    //WRITE16(0x8031b540, 0x0096); //Staff Booster spell

    DPRINT("Hooks installed!");
}
