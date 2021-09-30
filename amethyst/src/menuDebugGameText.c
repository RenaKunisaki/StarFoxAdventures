#include "main.h"

void debugTextSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug GameText menu
    curMenu = &menuDebugGameText;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void menuDebugTextViewTexts_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugViewText;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}
void menuDebugTextViewFonts_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugFontTest;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}
void menuDebugTextViewFiles_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugViewTextFiles;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

#define _FMT "%s:\eX290%s"
Menu menuDebugGameText = {
    "GameText", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "View Texts",             "%s", genericMenuItem_draw, menuDebugTextViewTexts_select,
    "View Fonts",             "%s", genericMenuItem_draw, menuDebugTextViewFonts_select,
    "View Loaded Text Files", "%s", genericMenuItem_draw, menuDebugTextViewFiles_select,
    NULL,
};
#undef _FMT
