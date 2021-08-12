/** Debug submenu.
 */
#include "main.h"

void debugSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug menu
    curMenu = &menuDebug;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void menuDebugFreeMove_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bFreeMove ? "On" : "Off");
    menuDrawText(str, x, y, selected);
}
void menuDebugFreeMove_select(const MenuItem *self, int amount) {
    bFreeMove = !bFreeMove;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

void menuDebugText_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugText;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMap_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugMap;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugCam_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugCamera;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugGame_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugGameState;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugCheat_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugCheat;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugRender_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugRender;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMisc_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugMisc;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}


Menu menuDebug = {
    "Debug", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Free Move: %s",  menuDebugFreeMove_draw, menuDebugFreeMove_select,
    "Debug Text",     genericMenuItem_draw,   menuDebugText_select,
    "Map",            genericMenuItem_draw,   menuDebugMap_select,
    "Camera",         genericMenuItem_draw,   menuDebugCam_select,
    "Game State",     genericMenuItem_draw,   menuDebugGame_select,
    "Cheats",         genericMenuItem_draw,   menuDebugCheat_select,
    "Render",         genericMenuItem_draw,   menuDebugRender_select,
    "Misc",           genericMenuItem_draw,   menuDebugMisc_select,
    NULL,
};
