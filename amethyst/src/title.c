/** Title screen improvements.
 */
#include "main.h"
#include "revolution/os.h"

static int (*oldTitleHook)();

int titleHook() {
    //do this here due to memory starvation at startup
    enableKrystal = 1;
    krystal_loadAssets();

    //check current and previous frame
    u16 buttons = controllerStates[0].button | controllerStates[4].button;

    //debugPrintf("saveStatus = %d frameCount = %d\n", saveStatus, frameCount);
    //doing it too soon will crash
    if(frameCount > 20 && frameCount < 300
    && titleScreen_panAwayFromMovieTimer > 0 && buttons & PAD_TRIGGER_R) {
        //OSReport("Loading save 1\n");
        titleScreenActive = false; //load into the game
        titleScreen_panAwayFromMovieTimer = 0;
        titleLoadSaveFiles(); //to get the savegame settings

        //interesting: calling this during the game still works, and replaces your current save
        //data, so things like your items are reset, but you don't reload or respawn...
        saveGame_load(0); //load the actual save file
        loadSaveSettings(); //apply the settings

        //free some memory. XXX does this actually do any good?
        //mapUnload(0x3D, 0x2000);
    }

    return oldTitleHook();
}

/* void saveInfoHook(void *unused, u32 alpha) {
    saveSelect_drawText(unused, alpha);

    //we can try to show more info here, but we need to also patch saveGame_prepareAndWrite
    //or some such to actually preserve that info.
    SaveGame *save = &saveData.curSaveGame;
    debugPrintf("%f %f %f %d: %02X (%02X)\n",
        save->charPos->pos.x,
        save->charPos->pos.y,
        save->charPos->pos.z,
        save->charPos->mapLayer,
        save->charPos->mapId,
        mapCoordsToId((int)save->charPos->pos.x / MAP_CELL_SCALE,
            (int)save->charPos->pos.z / MAP_CELL_SCALE, save->charPos->mapLayer)
    );
} */

void titleHooksInit() {
    //hook into the run method of the title screen DLL
    OSReport("Install title hook...\n");
    oldTitleHook = *(u32*)0x8031a320;
    WRITE32(0x8031a320, titleHook);
    //hookBranch(0x8011ab74, saveInfoHook, 1);
}
