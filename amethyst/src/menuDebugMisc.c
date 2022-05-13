#include "main.h"
#include "revolution/os.h"

void debugMiscSubMenu_close(const Menu *self) {
    //Close function for submenus of the debug misc menu
    curMenu = &menuDebugMisc;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void menuDebugMiscHexEdit_select(const MenuItem *self, int amount) {
    if(amount) return;
    hexEditPrevMenu = curMenu;
    curMenu = &menuDebugHexEdit;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuDebugMiscRNG_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    static const char *modes[] = {"Normal", "Zero", "One", "Max",
        "Increment", "P4 R-Trigger"};
    sprintf(str, self->fmt, T(self->name), T(modes[rngMode]));
    menuDrawText(str, x, y, selected);
}
void menuDebugMiscRNG_select(const MenuItem *self, int amount) {
    rngMode += amount;
    if(rngMode < 0) rngMode = NUM_RNG_MODES - 1;
    if(rngMode >= NUM_RNG_MODES) rngMode = 0;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}

void menuDebugMiscRNGTest_select(const MenuItem *self, int amount) {
    if(amount) return;

    u32 count[256], inval=0;
    memset(count, 0, sizeof(count));
    for(int i=0; i<100000000; i++) {
        u32 r = randomGetRange(1, 4);
        if(r > 255) inval++;
        else count[r]++;
    }
    char str[256], *buf;
    for(int i=0; i<256; i += 16) {
        buf = str + sprintf(str, "%3d: ", i);
        for(int j=0; j<16; j++) {
            buf = buf + sprintf(buf, "%5d ", count[i+j]);
        }
        OSReport("%s\n", str);
    }
    OSReport("Invalid: %d\n", inval);

    /*for(u32 i=0; i<0xFFFFFFFF; i++) {
        randomNumber = i;
        u32 val = randomGetRange(1, 4);
        if(val < 1 || val > 4) OSReport("%08X", i);
    }*/
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
    "Edit Memory", "%s", genericMenuItem_draw,  menuDebugMiscHexEdit_select,
    "RNG", "%s: %s",     menuDebugMiscRNG_draw, menuDebugMiscRNG_select,
    "RNG Test", "%s",    genericMenuItem_draw,  menuDebugMiscRNGTest_select,
    "Kill Player", "%s", genericMenuItem_draw,  menuDebugMiscDie_select,
    "Crash Game",  "%s", genericMenuItem_draw,  menuDebugMiscCrash_select,
    NULL,
};
