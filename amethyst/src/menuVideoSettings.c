/** Video Settings submenu.
 */
#include "main.h"

void menuWidescreen_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (renderFlags & RenderFlag_Widescreen) ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuWidescreen_select(const MenuItem *self, int amount) {
    setWidescreen(!(renderFlags & RenderFlag_Widescreen));
    saveData.saveSettings.bWidescreen = (renderFlags & RenderFlag_Widescreen) ? 1 : 0;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuParticleFx_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bDisableParticleFx ? "Off" : "On");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuParticleFx_select(const MenuItem *self, int amount) {
    bDisableParticleFx = !bDisableParticleFx;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuFOV_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (int)overrideFov);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuFOV_select(const MenuItem *self, int amount) {
    overrideFov += amount;
    if(overrideFov <   5) overrideFov =   5;
    if(overrideFov > 175) overrideFov = 175; //most the game can do
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuHudOnOff_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (cameraFlags & CAM_FLAG_NO_HUD) ? "Off" : "On");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuHudOnOff_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_NO_HUD;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuVideoSettings = {
    "Video Settings", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Widescreen: %s",  menuWidescreen_draw, menuWidescreen_select,
    "Particle FX: %s", menuParticleFx_draw, menuParticleFx_select,
    "FOV: %d",         menuFOV_draw,        menuFOV_select,
    "HUD: %s",         menuHudOnOff_draw,   menuHudOnOff_select,
    NULL,
};
