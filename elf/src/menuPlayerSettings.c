/** Player Settings submenu.
 */
#include "main.h"

static const char *playerNames[] = {"Auto", "Krystal", "Fox"};
void menuPlayer_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, "Not Implemented");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuPlayer_select(const MenuItem *self, int amount) {
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


static const char *backpackModeNames[] = {"Normal", "On", "Off"};
void menuBackpack_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, "Not Implemented");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuBackpack_select(const MenuItem *self, int amount) {
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


static const char *furFxNames[] = {"Normal", "Always"};
void menuFurFx_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, "Not Implemented");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuFurFx_select(const MenuItem *self, int amount) {
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
