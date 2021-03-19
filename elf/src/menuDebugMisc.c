#include "main.h"

void menuDebugMiscCrash_select(const MenuItem *self, int amount) {
    if(amount) return;
    (*(u32*)0xBAD0) = 0xDEADDEAD;
}

Menu menuDebugMisc = {
    "Misc", 0,
    genericMenu_run, genericMenu_draw, debugSubMenu_close,
    "Crash Game", genericMenuItem_draw, menuDebugMiscCrash_select,
    NULL,
};
