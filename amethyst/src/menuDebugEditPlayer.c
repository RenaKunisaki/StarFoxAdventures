/** Edit Player State submenu.
 */
#include "main.h"

static u8 playerIdx = 0;

void menuEditPlayerWhich_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, (playerIdx == 0) ? "Krystal" : "Fox");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerWhich_select(const MenuItem *self, int amount) {
    playerIdx ^= 1;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerCurHp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, playerState->curHealth);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerCurHp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    if(amount) playerState->curHealth += amount;
    else playerState->curHealth = playerState->maxHealth;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMaxHp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, playerState->maxHealth);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerMaxHp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    playerState->maxHealth += amount;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerCurMp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, playerState->curMagic);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerCurMp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    if(amount) playerState->curMagic += amount;
    else playerState->curMagic = playerState->maxMagic;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMaxMp_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, playerState->maxMagic);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerMaxMp_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    playerState->maxMagic += amount;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerCurLives_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, playerState->curBafomDads);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerCurLives_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    if(amount) playerState->curBafomDads += amount;
    else playerState->curBafomDads = playerState->maxBafomDads;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMaxLives_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, playerState->maxBafomDads);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerMaxLives_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    playerState->maxBafomDads += amount;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerMoney_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    sprintf(str, self->name, playerState->money);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerMoney_select(const MenuItem *self, int amount) {
    PlayerCharState *playerState = &saveData.curSaveGame.charState[playerIdx];
    if(amount) playerState->money += amount;
    else playerState->money = 255;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuEditPlayerFood_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, saveData.curSaveGame.trickyEnergy);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuEditPlayerFood_select(const MenuItem *self, int amount) {
    if(amount) saveData.curSaveGame.trickyEnergy += amount;
    else saveData.curSaveGame.trickyEnergy = 127;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuDebugEditPlayer = {
    "Edit Player State", 0,
    genericMenu_run, genericMenu_draw, debugGameSubMenu_close,
    "Edit: %s",   menuEditPlayerWhich_draw, menuEditPlayerWhich_select,
    "Cur HP: %d", menuEditPlayerCurHp_draw, menuEditPlayerCurHp_select,
    "Max HP: %d", menuEditPlayerMaxHp_draw, menuEditPlayerMaxHp_select,
    "Cur MP: %d", menuEditPlayerCurMp_draw, menuEditPlayerCurMp_select,
    "Max MP: %d", menuEditPlayerMaxMp_draw, menuEditPlayerMaxMp_select,
    "Cur Lives: %d", menuEditPlayerCurLives_draw, menuEditPlayerCurLives_select,
    "Max Lives: %d", menuEditPlayerMaxLives_draw, menuEditPlayerMaxLives_select,
    "Money: %d", menuEditPlayerMoney_draw, menuEditPlayerMoney_select,
    "Tricky Food: %d", menuEditPlayerFood_draw, menuEditPlayerFood_select,
    NULL,
};
