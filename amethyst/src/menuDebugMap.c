#include "main.h"
static int8_t loadMapNo = 0;

void debugMapMenu_run(Menu *self) {
    //Run function for map menu
    genericMenu_run(self);

    if(buttonsJustPressed == PAD_TRIGGER_Z) {
        if(self->selected == 11) { //Load Map
            DPRINT("Unload map 0x%02X", loadMapNo);
            mapUnload(loadMapNo, 0x20000000);
        }
    }
}

void debugMapSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug map menu
    curMenu = &menuDebugMap;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void menuDebugMapReload_select(const MenuItem *self, int amount) {
    if(amount) return;
    mapReload();
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapObjs_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugObjList;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapSpawnObj_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugSpawnObj;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapWarp_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugWarp;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapSetRestart_select(const MenuItem *self, int amount) {
    if(amount) return;
    gplayRestartPoint(&pPlayer->pos.pos, pPlayer->pos.rotation.x, curMapLayer, 0);
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapClearRestart_select(const MenuItem *self, int amount) {
    if(amount) return;
    gplayClearRestartPoint();
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapGotoRestart_select(const MenuItem *self, int amount) {
    if(amount) return;
    gplayGotoRestartPoint();
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapSetSavePos_select(const MenuItem *self, int amount) {
    if(amount) return;
    if(!pCurSaveGame) {
        audioPlaySound(NULL, MENU_FAIL_SOUND);
        return;
    }
    //these are the coords you'll actually spawn at when loading the save.
    PlayerCharPos *playerPos =
        //&saveData.curSaveGame.charPos[saveData.curSaveGame.character];
        &pCurSaveGame->charPos[pCurSaveGame->character];
    playerPos->pos.x = pPlayer->pos.pos.x;
    playerPos->pos.y = pPlayer->pos.pos.y;
    playerPos->pos.z = pPlayer->pos.pos.z;
    playerPos->mapLayer = curMapLayer;
    playerPos->mapId = curMapId;
    playerPos->rotX  = pPlayer->pos.rotation.x >> 8;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapGotoSave_select(const MenuItem *self, int amount) {
    if(amount) return;
    gplayGotoSavegame();
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapStates_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugMapStates;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapEnv_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugMapEnv;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMapLoad_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), loadMapNo, mapDirNames[loadMapNo]);
    menuDrawText(str, x, y, selected);
}
void menuDebugMapLoad_select(const MenuItem *self, int amount) {
    if(amount) {
        loadMapNo += amount;
        if(loadMapNo < 0) loadMapNo = NUM_MAP_DIRS - 1;
        if(loadMapNo >= NUM_MAP_DIRS) loadMapNo = 0;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
    }
    else {
        audioPlaySound(NULL, MENU_OPEN_SOUND);
        mapLoadDataFiles(loadMapNo);
        //mapLoadForObject(loadMapNo, pPlayer);
    }
}

Menu menuDebugMap = {
    "Map", 0,
    debugMapMenu_run, genericMenu_draw, debugSubMenu_close,
    "Warp",                "%s", genericMenuItem_draw, menuDebugMapWarp_select,
    "Objects",             "%s", genericMenuItem_draw, menuDebugMapObjs_select,
    "Spawn Object",        "%s", genericMenuItem_draw, menuDebugMapSpawnObj_select,
    "Reload Map",          "%s", genericMenuItem_draw, menuDebugMapReload_select,
    "Set Restart Point",   "%s", genericMenuItem_draw, menuDebugMapSetRestart_select,
    "Clear Restart Point", "%s", genericMenuItem_draw, menuDebugMapClearRestart_select,
    "Goto Restart Point",  "%s", genericMenuItem_draw, menuDebugMapGotoRestart_select,
    "Set Saved Pos",       "%s", genericMenuItem_draw, menuDebugMapSetSavePos_select,
    "Goto Last Save",      "%s", genericMenuItem_draw, menuDebugMapGotoSave_select,
    "Map States",          "%s", genericMenuItem_draw, menuDebugMapStates_select,
    "Environment",         "%s", genericMenuItem_draw, menuDebugMapEnv_select,
    "Load Map",            "%s: %02X %s", menuDebugMapLoad_draw, menuDebugMapLoad_select,
    NULL,
};
