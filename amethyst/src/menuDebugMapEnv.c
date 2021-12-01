#include "main.h"

void menuDebugMapEnvDay_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, "%s: %d", T("Day"), mainGetBit(0x2BA));
    menuDrawText(str, x, y, selected);
}
void menuDebugMapEnvDay_select(const MenuItem *self, int amount) {
    int day = mainGetBit(0x2BA) + amount;
    if(day < 0) day = 27;
    if(day > 27) day = 0;
    mainSetBits(0x2BA, day);
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugMapEnvTime_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    debugPrintf("pSkyStruct=%08X", pSkyStruct);
    if(pSkyStruct) {
        int now = (int)pSkyStruct->timeOfDay;
        sprintf(str, self->fmt, T(self->name),
            (int)(now / 3600),
            (int)(now /   60) % 60,
            (int)(now)        % 60);
    }
    else sprintf(str, "%s: %s", T("Time"), T("N/A"));
    menuDrawText(str, x, y, selected);
}
void menuDebugMapEnvTime_select(const MenuItem *self, int amount) {
    if(pSkyStruct) {
        pSkyStruct->timeOfDay += amount * 60;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
    }
    else audioPlaySound(NULL, MENU_FAIL_SOUND);
}

static s16 envFxId = 0; //XXX find current ID?
void menuDebugMapEnvGet_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, "%s: \eF0x%04X", T("Load EnvFX"), envFxId);
    menuDrawText(str, x, y, selected);
}
void menuDebugMapEnvGet_select(const MenuItem *self, int amount) {
    if(amount) envFxId += amount;
    else {
        //getEnvfxActImmediately(NULL, NULL, envFxId, 0);
        getEnvfxAct(NULL, NULL, envFxId, 0);
    }
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuDebugMapEnvLights_select(const MenuItem *self, int amount) {
    if(amount) return;
    curMenu = &menuDebugMapEnvLights;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

Menu menuDebugMapEnv = {
    "Environment", 0,
    genericMenu_run, genericMenu_draw, debugMapSubMenu_close,
    "Day",   "%s: %s", menuDebugMapEnvDay_draw, menuDebugMapEnvDay_select,
    "Time",   "%s: \eF%02d:%02d:%02d", menuDebugMapEnvTime_draw, menuDebugMapEnvTime_select,
    "Load EnvFX", "%s: \eF0x%04X", menuDebugMapEnvGet_draw, menuDebugMapEnvGet_select,
    "Lights", "%s", genericMenuItem_draw, menuDebugMapEnvLights_select,
    NULL,
};
