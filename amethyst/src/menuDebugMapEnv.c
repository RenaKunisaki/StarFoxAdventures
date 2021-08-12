#include "main.h"

void menuDebugMapEnvTime_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    debugPrintf("pSkyStruct=%08X", pSkyStruct);
    if(pSkyStruct) {
        int now = (int)pSkyStruct->timeOfDay;
        sprintf(str, self->name,
            (int)(now / 3600),
            (int)(now /   60) % 60,
            (int)(now)        % 60);
    }
    else strcpy(str, "Time: N/A");
    menuDrawText(str, x, y, selected);
}
void menuDebugMapEnvTime_select(const MenuItem *self, int amount) {
    if(pSkyStruct) {
        pSkyStruct->timeOfDay += amount * 60;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
    }
    else audioPlaySound(NULL, MENU_FAIL_SOUND);
}

void menuDebugMapEnvLights_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugMapEnvLights;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

Menu menuDebugMapEnv = {
    "Environment", 0,
    genericMenu_run, genericMenu_draw, debugMapSubMenu_close,
    "Time: %02d:%02d:%02d", menuDebugMapEnvTime_draw, menuDebugMapEnvTime_select,
    "Lights", genericMenuItem_draw, menuDebugMapEnvLights_select,
    NULL,
};
