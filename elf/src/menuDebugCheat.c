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


void menuDebugCheatTricky_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (debugCheats & DBGCHT_INF_TRICKY) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuDebugCheatTricky_select(const MenuItem *self, int amount) {
    debugCheats ^= DBGCHT_INF_TRICKY;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


static u16 unlockAllBits[] = {
    //GameBits which get set to max
    0x0025, //HaveTrickysBall
    0x002D, //HaveFireballSpell
    0x0040, //HaveSharpClawDisguise
    0x0075, //HaveStaff
    0x00C1, //NumTrickyFoods
    0x00DD, //HaveCallTricky
    0x0107, //HaveGroundQuake
    0x013D, //NumFireflies
    0x013E, //HaveFireflyLantern
    0x0245, //HaveTrickyFlame
    0x03F5, //NumFuelCells
    0x03F9, //WorldMapDarkIce
    0x03FA, //WorldMapCloudFort
    0x03FB, //WorldMapWallCity
    0x03FC, //WorldMapDragRock
    0x04E4, //CanUseTricky
    0x059D, //HaveVolcanoMap
    0x059E, //HaveDarkIceMap
    0x05A0, //HaveSnowWastesMap
    0x05A1, //HaveCloudFortMap
    0x05A2, //HaveLightFootMap
    0x05A3, //HaveHollowMap
    0x05BD, //HaveOpenPortal
    0x05CE, //HaveIceBlast
    0x05D6, //NumFirefliesNotShown
    0x066C, //NumBombSpores
    0x07DD, //HaveDragRockMap
    0x07E5, //HaveKrazoaMap
    0x07E9, //HaveOceanMap
    0x082E, //HaveWallCityMap
    0x082F, //HaveCapeClawMap
    0x0835, //HaveMoonPassMap
    0x086A, //NumMoonSeeds
    0x090D, //DidCollectMagic
    0x090E, //DidCollectBigHealth
    0x090F, //DidCollectApple
    0x0912, //DidSeeWarpPad
    0x0919, //Have50ScarabBag
    0x091A, //Have100ScarabBag
    0x091B, //Have200ScarabBag
    0x0957, //HaveStaffBooster
    0x0958, //HaveLaserSpell
    0x0C55, //HaveSuperQuake
    0x0C64, //HaveViewfinder
    0x0CC0, //DidCollectBafomdad
    0x0EB1, //HaveMagic
    0x0EB2, //HaveBafomdadHolder
    0xFFFF, //end
};
void menuDebugCheatUnlock_select(const MenuItem *self, int amount) {
    if(amount) return;
    for(int i=0; unlockAllBits[i] != 0xFFFF; i++) {
        mainSetBits(unlockAllBits[i], 0xFFFFFFFF);
    }
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuDebugCheat = {
    "Cheats", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Infinite HP: %s",    menuDebugCheatHP_draw, menuDebugCheatHP_select,
    "Infinite MP: %s",    menuDebugCheatMP_draw, menuDebugCheatMP_select,
    "Infinite Money: %s", menuDebugCheatMoney_draw, menuDebugCheatMoney_select,
    "Infinite Lives: %s", menuDebugCheatLives_draw, menuDebugCheatLives_select,
    "Infinite Tricky Energy: %s", menuDebugCheatTricky_draw, menuDebugCheatTricky_select,
    "Frozen Baddies: %s", menuDebugCheatFrozen_draw, menuDebugCheatFrozen_select,
    "Unlock Everyting", genericMenuItem_draw, menuDebugCheatUnlock_select,
    NULL,
};
