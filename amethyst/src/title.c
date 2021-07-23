/** Title screen improvements.
 */
#include "main.h"
#include "revolution/os.h"

static int (*oldTitleHook)();

int titleHook() {
    //do this here due to memory starvation at startup
    krystal_loadAssets();

    //check current and previous frame
    u16 buttons = controllerStates[0].button | controllerStates[4].button;

    //debugPrintf("saveStatus = %d frameCount = %d\n", saveStatus, frameCount);
    //doing it too soon will crash
    if(frameCount > 20 && titleScreen_panAwayFromMovieTimer > 0 && buttons & PAD_TRIGGER_L) {
        //OSReport("Loading save 1\n");
        titleScreenActive = false;
        titleScreen_panAwayFromMovieTimer = 0;
        DPRINT("Title: load card");
        titleLoadSaveFiles();
        DPRINT("Title: load file");
        saveGame_load(0);
        DPRINT("Title: load settings");
        loadSaveSettings();
        DPRINT("Title: load OK");
        //interesting: calling this during the game still works, and replaces your current save
        //data, so things like your items are reset, but you don't reload or respawn...

        //WRITE8(0x803dd5d1, 4);
        //WRITE8(0x803dd5d2, 4);
        mapUnload(0x3D, 0x2000);
    }

    return oldTitleHook();
}

void titleHooksInit() {
    //hook into the run method of the title screen DLL
    OSReport("Install title hook...\n");
    oldTitleHook = *(u32*)0x8031a320;
    WRITE32(0x8031a320, titleHook);
}
