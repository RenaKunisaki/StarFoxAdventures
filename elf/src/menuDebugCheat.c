#include "main.h"


void menuDebugCheatHP_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugCheats & DBGCHT_INF_HP) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugCheatHP_select(const MenuItem *self, int amount) {
    debugCheats ^= DBGCHT_INF_HP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugCheatMP_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugCheats & DBGCHT_INF_MP) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugCheatMP_select(const MenuItem *self, int amount) {
    debugCheats ^= DBGCHT_INF_MP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugCheatMoney_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugCheats & DBGCHT_INF_MONEY) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugCheatMoney_select(const MenuItem *self, int amount) {
    debugCheats ^= DBGCHT_INF_MONEY;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugCheatLives_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugCheats & DBGCHT_INF_LIVES) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugCheatLives_select(const MenuItem *self, int amount) {
    debugCheats ^= DBGCHT_INF_LIVES;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugCheatFrozen_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugCheats & DBGCHT_ENEMY_FROZEN) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugCheatFrozen_select(const MenuItem *self, int amount) {
    debugCheats ^= DBGCHT_ENEMY_FROZEN;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuDebugCheat = {
    "Cheats", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Infinite HP: %s",    menuDebugCheatHP_draw, menuDebugCheatHP_select,
    "Infinite MP: %s",    menuDebugCheatMP_draw, menuDebugCheatMP_select,
    "Infinite Money: %s", menuDebugCheatMoney_draw, menuDebugCheatMoney_select,
    "Infinite Lives: %s", menuDebugCheatLives_draw, menuDebugCheatLives_select,
    "Frozen Baddies: %s", menuDebugCheatFrozen_draw, menuDebugCheatFrozen_select,
    NULL,
};
