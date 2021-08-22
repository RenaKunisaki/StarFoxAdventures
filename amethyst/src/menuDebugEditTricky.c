/** Edit Tricky State submenu.
 */
#include "main.h"

void menuEditTrickyCurFood_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), saveData.curSaveGame.trickyEnergy);
    menuDrawText(str, x, y, selected);
}
void menuEditTrickyCurFood_select(const MenuItem *self, int amount) {
    if(amount) saveData.curSaveGame.trickyEnergy += amount;
    else saveData.curSaveGame.trickyEnergy = saveData.curSaveGame.maxTrickyEnergy;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

void menuEditTrickyMaxFood_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), saveData.curSaveGame.maxTrickyEnergy);
    menuDrawText(str, x, y, selected);
}
void menuEditTrickyMaxFood_select(const MenuItem *self, int amount) {
    if(amount) saveData.curSaveGame.maxTrickyEnergy += amount;
    else saveData.curSaveGame.maxTrickyEnergy = 20; //default
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

void menuEditTrickyPlayCount_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), saveData.curSaveGame.trickyPlayCount);
    menuDrawText(str, x, y, selected);
}
void menuEditTrickyPlayCount_select(const MenuItem *self, int amount) {
    saveData.curSaveGame.trickyPlayCount += amount;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

#define _FMT "%s:\eX250"
Menu menuDebugEditTricky = {
    "Edit Tricky State", 0,
    genericMenu_run, genericMenu_draw, debugGameSubMenu_close,
    "Cur Food", _FMT "\eF%3d",   menuEditTrickyCurFood_draw,     menuEditTrickyCurFood_select,
    "Max Food", _FMT "\eF%3d",   menuEditTrickyMaxFood_draw,     menuEditTrickyMaxFood_select,
    "Play Count", _FMT "\eF%3d", menuEditTrickyPlayCount_draw,   menuEditTrickyPlayCount_select,
    NULL,
};
#undef _FMT
