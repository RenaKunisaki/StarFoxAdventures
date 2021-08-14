/** Video Settings submenu.
 */
#include "main.h"

void menuWidescreen_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];;
    sprintf(str, self->fmt, T(self->name), (renderFlags & RenderFlag_Widescreen) ?
        T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuWidescreen_select(const MenuItem *self, int amount) {
    setWidescreen(!(renderFlags & RenderFlag_Widescreen));
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuParticleFx_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];;
    sprintf(str, self->fmt, T(self->name), bDisableParticleFx ? T("Off") : T("On"));
    menuDrawText(str, x, y, selected);
}
void menuParticleFx_select(const MenuItem *self, int amount) {
    bDisableParticleFx = !bDisableParticleFx;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuFOV_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];;
    sprintf(str, self->fmt, T(self->name), (int)overrideFov);
    menuDrawText(str, x, y, selected);
}
void menuFOV_select(const MenuItem *self, int amount) {
    overrideFov += amount;
    if(overrideFov <   5) overrideFov =   5;
    if(overrideFov > 175) overrideFov = 175; //most the game can do
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuHudOnOff_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];;
    sprintf(str, self->fmt, T(self->name), (cameraFlags & CAM_FLAG_NO_HUD) ? T("Off") : T("On"));
    menuDrawText(str, x, y, selected);
}
void menuHudOnOff_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_NO_HUD;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


Menu menuVideoSettings = {
    "Video Settings", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Widescreen",  "%s: %s", menuWidescreen_draw, menuWidescreen_select,
    "Particle FX", "%s: %s", menuParticleFx_draw, menuParticleFx_select,
    "FOV",         "%s: %d", menuFOV_draw,        menuFOV_select,
    "HUD",         "%s: %s", menuHudOnOff_draw,   menuHudOnOff_select,
    NULL,
};
