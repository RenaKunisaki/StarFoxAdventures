#include "main.h"

void debugMiscSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug misc menu
    curMenu = &menuDebugMisc;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void menuDebugMiscHexEdit_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugHexEdit;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMiscDie_select(const MenuItem *self, int amount) {
    if(amount) return;
    playerDie(pPlayer);
}

void menuDebugMiscCrash_select(const MenuItem *self, int amount) {
    if(amount) return;
    (*(u32*)0xBAD0) = 0xDEADDEAD;
}

Menu menuDebugMisc = {
    "Misc", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Edit Memory", genericMenuItem_draw, menuDebugMiscHexEdit_select,
    "Kill Player", genericMenuItem_draw, menuDebugMiscDie_select,
    "Crash Game",  genericMenuItem_draw, menuDebugMiscCrash_select,
    NULL,
};
