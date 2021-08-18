/** Game Settings submenu.
 */
#include "main.h"

void menuGameSpeed_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (int)((physicsTimeScale * 100) / 60));
    menuDrawText(str, x, y, selected);
}
void menuGameSpeed_select(const MenuItem *self, int amount) {
    physicsTimeScale += amount * 15;
    if(physicsTimeScale <  15) physicsTimeScale =  15;
    if(physicsTimeScale > 240) physicsTimeScale = 240;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuAutoSave_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bAutoSave ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuAutoSave_select(const MenuItem *self, int amount) {
    bAutoSave = !bAutoSave;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuSubtitles_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bSubtitlesEnabled ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuSubtitles_select(const MenuItem *self, int amount) {
    bSubtitlesEnabled = !bSubtitlesEnabled;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}

//could add flags here but the EN,JP flags are missing (and French is reversed)
//actually JP is present but inaccessible

void menuLanguage_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), languageNames[curLanguage]);
    menuDrawText(str, x, y, selected);
}
void menuLanguage_select(const MenuItem *self, int amount) {
    curLanguage += amount;
    if(curLanguage < 0) curLanguage = NUM_LANGUAGES - 1;
    if(curLanguage >= NUM_LANGUAGES) curLanguage = 0;
    setGameLanguage(curLanguage);
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuHpFlash_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (hudFlags & HUD_LOW_HP_FLASH) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuHpFlash_select(const MenuItem *self, int amount) {
    hudFlags ^= HUD_LOW_HP_FLASH;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}

void menuHpSound_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (hudFlags & HUD_LOW_HP_BEEP) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuHpSound_select(const MenuItem *self, int amount) {
    hudFlags ^= HUD_LOW_HP_BEEP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}

Menu menuGameSettings = {
    "Game Settings", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Game Speed",   "%s: %d%%", menuGameSpeed_draw, menuGameSpeed_select,
    "Autosave",     "%s: %s",   menuAutoSave_draw,  menuAutoSave_select,
    "Subtitles",    "%s: %s",   menuSubtitles_draw, menuSubtitles_select,
    "Language",     "%s: %s",   menuLanguage_draw,  menuLanguage_select,
    "Low HP Flash", "%s: %s",   menuHpFlash_draw,   menuHpFlash_select,
    "Low HP Sound", "%s: %s",   menuHpSound_draw,   menuHpSound_select,
    NULL,
};
