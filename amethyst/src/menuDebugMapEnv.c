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
    debugPrintf("pSkyStruct=%08X\n", pSkyStruct);
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
        if(controllerStates[0].button & PAD_TRIGGER_Z) {
            pSkyStruct->timeOfDay += amount * 3600;
        }
        else pSkyStruct->timeOfDay += amount * 60;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
    }
    else audioPlaySound(NULL, MENU_FAIL_SOUND);
}

static s16 envFxId = 0; //XXX find current ID?
static const char *types[] = {
    "Rain/Snow", "?", "?", "Fog", "Rain/Snow", "Color", "Clouds"};
void menuDebugMapEnvGet_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, "%s: \eF0x%04X", T("Load EnvFX"), envFxId);
    menuDrawText(str, x, y, selected);

    //display envfx entry data
    EnvFxActEntry *entry = (EnvFxActEntry*)dataFileBuffers[FILE_ENVFXACT_BIN];
    if(entry) entry = &entry[envFxId];
    else return;

    debugPrintf("envfx=" DPRINT_FIXED "%08X" DPRINT_NOFIXED
        ": %02X (%s), intensity %d, speed %f, fade %d\n",
        entry, entry->type, entry->type < 7 ? types[entry->type] : "?",
        entry->intensity, entry->skyMoveSpeed, entry->fadeTimer);
    debugPrintf("cloud: " DPRINT_FIXED "%d %d vis %d" DPRINT_NOFIXED " flags "
        DPRINT_FIXED "%02X %02X" DPRINT_NOFIXED "\nColors: " DPRINT_FIXED,
        entry->cloudIdx, entry->cloudTimer2C, entry->visibility,
        entry->cloudFlags, entry->flags59);
    for(int i=0; i<8; i++) {
        debugPrintf("%02X%02X%02X ", entry->red[i], entry->green[i], entry->blue[i]);
    }
    debugPrintf(DPRINT_NOFIXED "\nUnk: %f %f\n", entry->unk00, entry->unk04);
}
void menuDebugMapEnvGet_select(const MenuItem *self, int amount) {
    if(amount) {
        envFxId += amount;
        if(envFxId < 0) envFxId += MAX_ENVFX_ID;
        if(envFxId >= MAX_ENVFX_ID) envFxId -= MAX_ENVFX_ID;
    }
    else {
        if(controllerStates[0].button & PAD_TRIGGER_Z) {
            getEnvfxActImmediately(pPlayer, pPlayer, envFxId, 0);
        }
        else getEnvfxAct(pPlayer, pPlayer, envFxId, 0);
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
