/** Control Settings submenu.
 */
#include "main.h"

void menuRumble_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, enableRumble ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuRumble_select(const MenuItem *self, int amount) {
    enableRumble = !enableRumble;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    if(enableRumble) doRumble(60);
    updateSaveData();
}


void menuRumbleBlur_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bRumbleBlur ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuRumbleBlur_select(const MenuItem *self, int amount) {
    bRumbleBlur = !bRumbleBlur;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuCamCtrl_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (cameraFlags & CAM_FLAG_PAD3) ? 3 : 1);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuCamCtrl_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_PAD3;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuCamInvX_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (cameraFlags & CAM_FLAG_INVERT_X) ? "Invert" : "Normal");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuCamInvX_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_INVERT_X;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuCamInvY_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, (cameraFlags & CAM_FLAG_INVERT_Y) ? "Invert" : "Normal");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuCamInvY_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_INVERT_Y;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


Menu menuControlSettings = {
    "Control Settings", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Rumble: %s",                    menuRumble_draw,     menuRumble_select,
    "Rumble Blur: %s",               menuRumbleBlur_draw, menuRumbleBlur_select,
    "Camera Control: Controller %d", menuCamCtrl_draw,    menuCamCtrl_select,
    "Camera X: %s",                  menuCamInvX_draw,    menuCamInvX_select,
    "Camera Y: %s",                  menuCamInvY_draw,    menuCamInvY_select,
    NULL,
};
