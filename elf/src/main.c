/** Bootstrap, runs at startup and installs other hooks.
 */
#include "main.h"

u8 overrideFov = 60;
u8 furFxMode = 0; //FurFxMode
bool bRumbleBlur = false;

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
    //OSReport("GameBit 0x%04X set to %d", bit, val);
    return gameBitHook_replaced(); //BOOL isSaveGameLoading()
}


bool motionBlurHook() {
    //replaces a bl to shouldForceMotionBlur()
    bool force = shouldForceMotionBlur();
    if(force) return force;
    if(!bRumbleBlur) return false;

    if(rumbleTimer > 0) {
        float rumble = (rumbleTimer + 48) * 2;
        if(rumble > 120) rumble = 120;
        motionBlurIntensity = rumble;
        return true;
    }
    return false;
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

    minimapMainLoopHook();
    mainLoopDebugPrint();
    runMenu();
    krystalMainLoop();
    saveUpdateHook();
}


void _start(void) {
    OSReport("Patch running!");

    //Install hooks
    gameBitHook_replaced = (BOOL(*)())hookBranch(0x8002010C, gameBitHook, 1);
    runLoadingScreens_replaced = (void(*)())hookBranch(0x80020f2c, runLoadingScreens_hook, 1);
    startMsg_initDoneHook_replaced = (void(*)())hookBranch(0x80021250, startMsg_initDoneHook, 1);
    hookBranch(0x80020D4C, mainLoopHook, 1);
    hookBranch(0x800e7fb0, saveLoadHook, 1);
    hookBranch(0x8005c45c, motionBlurHook, 1);

    krystalInit();

    //autosave
    hookBranch(0x80042ec4, saveMapLoadHook, 1);
    hookBranch(0x8007db50, saveShowMsgHook, 1);
    //disable "not same memory card you last saved with" check,
    //since save states trigger that.
    //XXX look into how this check works and find a less invasive
    //way of fixing this.
    WRITE32(0x8007EF5C, 0x3B200000);
    WRITE32(0x8007F15C, 0x3B200000);

    //debug print
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

    //debug stuff
    WRITE_NOP(0x80119D90); //chapter select only needs Z button

    //pause menu improvements
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

    OSReport("Hooks installed!");
}
