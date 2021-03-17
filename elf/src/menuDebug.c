/** Debug submenu.
 */
#include "main.h"

void debugSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug menu
    curMenu = &menuDebug;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
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


Menu menuDebug = {
    "Debug", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Debug Text",     genericMenuItem_draw, menuDebugText_select,
    "Map",            genericMenuItem_draw, menuDebugMap_select,
    NULL,
};
