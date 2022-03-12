/** Control Settings submenu.
 */
#include "main.h"

void menuRumble_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), enableRumble ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuRumble_select(const MenuItem *self, int amount) {
    enableRumble = !enableRumble;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    if(enableRumble) doRumble(60);
    updateSaveData();
}


void menuRumbleBlur_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bRumbleBlur ? T("On") : T("Off"));
    menuDrawText(str, x, y, selected);
}
void menuRumbleBlur_select(const MenuItem *self, int amount) {
    bRumbleBlur = !bRumbleBlur;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuCamCtrl_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), T("Controller"), (cameraFlags & CAM_FLAG_PAD3) ? 3 : 1);
    menuDrawText(str, x, y, selected);
}
void menuCamCtrl_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_PAD3;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuCamInvX_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (cameraFlags & CAM_FLAG_INVERT_X) ?
        T("Invert") : T("Normal"));
    menuDrawText(str, x, y, selected);
}
void menuCamInvX_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_INVERT_X;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuCamInvY_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (cameraFlags & CAM_FLAG_INVERT_Y) ?
        T("Invert") : T("Normal"));
    menuDrawText(str, x, y, selected);
}
void menuCamInvY_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_INVERT_Y;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuCamAxis_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), (cameraFlags & CAM_FLAG_PLAYER_AXIS) ?
        T("Player") : T("Camera"));
    menuDrawText(str, x, y, selected);
}
void menuCamAxis_select(const MenuItem *self, int amount) {
    cameraFlags ^= CAM_FLAG_PLAYER_AXIS;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuAimSnap_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bNoAimSnap ? T("Off") : T("On"));
    menuDrawText(str, x, y, selected);
}
void menuAimSnap_select(const MenuItem *self, int amount) {
    bNoAimSnap = !bNoAimSnap;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}


void menuAimSensitive_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];
    sprintf(str, self->fmt, T(self->name), bSensitiveAim ?
        T("High") : T("Normal"));
    menuDrawText(str, x, y, selected);
}
void menuAimSensitive_select(const MenuItem *self, int amount) {
    bSensitiveAim = !bSensitiveAim;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
    updateSaveData();
}

Menu menuControlSettings = {
    "Control Settings", 0,
    genericMenu_run, genericMenu_draw, mainSubMenu_close,
    "Rumble",         "%s: %s",    menuRumble_draw,       menuRumble_select,
    "Rumble Blur",    "%s: %s",    menuRumbleBlur_draw,   menuRumbleBlur_select,
    "Camera Control", "%s: %s %d", menuCamCtrl_draw,      menuCamCtrl_select,
    "Camera X",       "%s: %s",    menuCamInvX_draw,      menuCamInvX_select,
    "Camera Y",       "%s: %s",    menuCamInvY_draw,      menuCamInvY_select,
    //"Camera Axis",    "%s: %s",    menuCamAxis_draw,      menuCamAxis_select, //broken
    "Aim Snapback",   "%s: %s",    menuAimSnap_draw,      menuAimSnap_select,
    "Aim Sensitivity","%s: %s",    menuAimSensitive_draw, menuAimSensitive_select,
    NULL,
};
