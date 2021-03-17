#include "main.h"

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

void menuDebugMapWarp_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugWarp;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

Menu menuDebugMap = {
    "Map", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Warp",           genericMenuItem_draw, menuDebugMapWarp_select,
    "Reload Map",     genericMenuItem_draw, menuDebugMapReload_select,
    NULL,
};
