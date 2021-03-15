/** PDA Settings submenu.
 */
#include "main.h"

static const char *pdaModeNames[] = {
    "Map", "Fuel Cell Compass", "Information", "Off"};
void menuPdaMode_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    int mode = minimapMode;
    if(!pdaOn) mode = 3;
    sprintf(str, self->name, pdaModeNames[mode]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuPdaMode_select(const MenuItem *self, int amount) {
    if(pdaOn) {
        minimapMode += amount;
        if(minimapMode > 2) {
            minimapMode = 0;
            pdaOn = false;
        }
    }
    else {
        pdaOn = true;
        if(amount < 0) minimapMode = 2;
        else minimapMode = 0;
    }
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


static const char *mapSizeNames[] = {"Small", "Normal", "Big"};
void menuPdaSize_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, mapSizeNames[overrideMinimapSize]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuPdaSize_select(const MenuItem *self, int amount) {
    int size = overrideMinimapSize + amount;
    if(size < 0) size = MINIMAP_NUM_SIZES - 1;
    if(size >= MINIMAP_NUM_SIZES) size = 0;
    overrideMinimapSize = size;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuPdaOpacity_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)((overrideMinimapAlpha / 255.0) * 100));
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuPdaOpacity_select(const MenuItem *self, int amount) {
    int alpha = overrideMinimapAlpha + (amount * 25);
    if(alpha <   0) alpha =   0;
    if(alpha > 255) alpha = 255;
    overrideMinimapAlpha = alpha;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuPdaZoom_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)(minimapZoom * 100));
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuPdaZoom_select(const MenuItem *self, int amount) {
    minimapZoom += (amount * 0.05);
    if(minimapZoom < 0.05) minimapZoom = 0.05;
    if(minimapZoom > 2.00) minimapZoom = 2.00;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuPdaSettings = {
    "PDA Settings", 0,
    genericMenu_run, mainSubMenu_close,
    "Mode: %s",          menuPdaMode_draw,    menuPdaMode_select,
    "Map Size: %s",      menuPdaSize_draw,    menuPdaSize_select,
    "Map Opacity: %d%%", menuPdaOpacity_draw, menuPdaOpacity_select,
    "Map Zoom: %d%%",    menuPdaZoom_draw,    menuPdaZoom_select,
    NULL,
};
