/** Edit Player State submenu.
 */
#include "main.h"

u8 editPlayerIdx = 0;

void menuEditPlayerWhich_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), (editPlayerIdx == 0) ? T("Krystal") : T("Fox"));
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerWhich_select(const MenuItem *self, int amount) {
    editPlayerIdx ^= 1;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerCurHp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), playerState->curHealth);
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerCurHp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    if(amount) playerState->curHealth += amount;
    else playerState->curHealth = playerState->maxHealth;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMaxHp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), playerState->maxHealth);
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerMaxHp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    playerState->maxHealth += amount;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerCurMp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), playerState->curMagic);
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerCurMp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    if(amount) playerState->curMagic += amount;
    else playerState->curMagic = playerState->maxMagic;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMaxMp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), playerState->maxMagic);
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerMaxMp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    playerState->maxMagic += amount;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerCurLives_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), playerState->curBafomDads);
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerCurLives_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    if(amount) playerState->curBafomDads += amount;
    else playerState->curBafomDads = playerState->maxBafomDads;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMaxLives_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), playerState->maxBafomDads);
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerMaxLives_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    playerState->maxBafomDads += amount;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMoney_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    sprintf(str, self->fmt, T(self->name), playerState->money);
    menuDrawText(str, x, y, selected);
}
void menuEditPlayerMoney_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[editPlayerIdx];
    if(amount) playerState->money += amount;
    else playerState->money = 255;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


#define _FMT "%s:\eX250"
Menu menuDebugEditPlayer = {
    "Edit Player State", 0,
    genericMenu_run, genericMenu_draw, debugGameSubMenu_close,
    "Edit",        "%s: %s",      menuEditPlayerWhich_draw,    menuEditPlayerWhich_select,
    "Cur HP",      _FMT "\eF%3d", menuEditPlayerCurHp_draw,    menuEditPlayerCurHp_select,
    "Max HP",      _FMT "\eF%3d", menuEditPlayerMaxHp_draw,    menuEditPlayerMaxHp_select,
    "Cur MP",      _FMT "\eF%3d", menuEditPlayerCurMp_draw,    menuEditPlayerCurMp_select,
    "Max MP",      _FMT "\eF%3d", menuEditPlayerMaxMp_draw,    menuEditPlayerMaxMp_select,
    "Cur Lives",   _FMT "\eF%3d", menuEditPlayerCurLives_draw, menuEditPlayerCurLives_select,
    "Max Lives",   _FMT "\eF%3d", menuEditPlayerMaxLives_draw, menuEditPlayerMaxLives_select,
    "Money",       _FMT "\eF%3d", menuEditPlayerMoney_draw,    menuEditPlayerMoney_select,
    NULL,
};
#undef _FMT
