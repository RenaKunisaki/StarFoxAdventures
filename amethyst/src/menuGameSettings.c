/** Game Settings submenu.
 */
#include "main.h"

void menuGameSpeed_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)((physicsTimeScale * 100) / 60));
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
    char str[64];
    sprintf(str, self->name, bAutoSave ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuAutoSave_select(const MenuItem *self, int amount) {
    bAutoSave = !bAutoSave;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuSubtitles_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bSubtitlesEnabled ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuSubtitles_select(const MenuItem *self, int amount) {
    bSubtitlesEnabled = !bSubtitlesEnabled;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}

//could add flags here but the EN,JP flags are missing (and French is reversed)
static const char *languages[NUM_LANGUAGES] = {
    "English", //English
    "français", //French
    "Deutsch", //German
    "Italiano", //Italian
    "日本", //Japanese
    "Español" //Spanish
};
void menuLanguage_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, languages[curLanguage]);
    menuDrawText(str, x, y, selected);
}
void menuLanguage_select(const MenuItem *self, int amount) {
    curLanguage += amount;
    if(curLanguage < 0) curLanguage = NUM_LANGUAGES - 1;
    if(curLanguage >= NUM_LANGUAGES) curLanguage = 0;
    GameTextDir32 dir = curGameTextDir;
    gameTextLoadDir(GAMETEXT_DIR_Link); //load HUD texts
    gameTextLoadDir(dir); //then load current map's texts
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuHpFlash_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (hudFlags & HUD_LOW_HP_FLASH) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuHpFlash_select(const MenuItem *self, int amount) {
    hudFlags ^= HUD_LOW_HP_FLASH;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}

void menuHpSound_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (hudFlags & HUD_LOW_HP_BEEP) ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuHpSound_select(const MenuItem *self, int amount) {
    hudFlags ^= HUD_LOW_HP_BEEP;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}

//maybe for options we split into two strings, like:
//"Game Speed", "%s: %d%%", menuGameSpeed_draw, menuGameSpeed_select,
//where the first is the string which gets looked up in translation table
//and the second is the format string where the translated string is arg 0.
//maybe need to use a special placeholder instead of %s, or just always concat
//the format string to the end of it?
Menu menuGameSettings = {
    "Game Settings", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Game Speed: %d%%", menuGameSpeed_draw, menuGameSpeed_select,
    "Autosave: %s",     menuAutoSave_draw,  menuAutoSave_select,
    "Subtitles: %s",    menuSubtitles_draw, menuSubtitles_select,
    "Language: %s",     menuLanguage_draw,  menuLanguage_select,
    "Low HP Flash: %s", menuHpFlash_draw,   menuHpFlash_select,
    "Low HP Sound: %s", menuHpSound_draw,   menuHpSound_select,
    NULL,
};
