/** Debug submenu.
 */
#include "main.h"

void debugSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug menu
    curMenu = &menuDebug;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void menuDebugFreeMove_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bFreeMove ? T("On") : T("Off"));
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

void menuDebugGameText_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugGameText;
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
    "Free Move",  "%s: %s", menuDebugFreeMove_draw, menuDebugFreeMove_select,
    "Debug Text", "%s",     genericMenuItem_draw,   menuDebugText_select,
    "Map",        "%s",     genericMenuItem_draw,   menuDebugMap_select,
    "Camera",     "%s",     genericMenuItem_draw,   menuDebugCam_select,
    "Game State", "%s",     genericMenuItem_draw,   menuDebugGame_select,
    "Cheats",     "%s",     genericMenuItem_draw,   menuDebugCheat_select,
    "Render",     "%s",     genericMenuItem_draw,   menuDebugRender_select,
    "GameText",   "%s",     genericMenuItem_draw,   menuDebugGameText_select,
    "Misc",       "%s",     genericMenuItem_draw,   menuDebugMisc_select,
    NULL,
};
