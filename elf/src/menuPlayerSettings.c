/** Player Settings submenu.
 */
#include "main.h"

static const char *playerNames[] = {"Krystal", "Fox", "Auto"};
void menuPlayer_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, playerNames[overridePlayerNo]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuPlayer_select(const MenuItem *self, int amount) {
    overridePlayerNo += amount;
    if(overridePlayerNo < 0) overridePlayerNo = NUM_PLAYER_IDS - 1;
    if(overridePlayerNo >= NUM_PLAYER_IDS) overridePlayerNo = 0;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


static const char *backpackModeNames[] = {"Normal", "Off", "On"};
void menuBackpack_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, backpackModeNames[backpackMode]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuBackpack_select(const MenuItem *self, int amount) {
    backpackMode += amount;
    if(backpackMode < 0) backpackMode = NUM_BACKPACK_MODES - 1;
    if(backpackMode >= NUM_BACKPACK_MODES) backpackMode = 0;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


static const char *furFxModeNames[] = {"Normal", "Off", "On"};
void menuFurFx_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, furFxModeNames[furFxMode]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuFurFx_select(const MenuItem *self, int amount) {
    int mode = furFxMode + amount;
    if(mode < 0) mode = NUM_FURFX_MODES - 1;
    if(mode >= NUM_FURFX_MODES) mode = 0;
    furFxMode = mode;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuPlayerSettings = {
    "Player Settings", 0,
    genericMenu_run, mainSubMenu_close,
    "Player: %s",     menuPlayer_draw,   menuPlayer_select,
    "Backpack: %s",   menuBackpack_draw, menuBackpack_select,
    "Fur Effect: %s", menuFurFx_draw,    menuFurFx_select,
    NULL,
};
