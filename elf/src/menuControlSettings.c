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
}


void menuRumbleBlur_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, bRumbleBlur ? "On" : "Off");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuRumbleBlur_select(const MenuItem *self, int amount) {
    bRumbleBlur = !bRumbleBlur;
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuCamCtrl_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, 69); //XXX
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuCamCtrl_select(const MenuItem *self, int amount) {
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuCamInvX_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, "XXX");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuCamInvX_select(const MenuItem *self, int amount) {
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


void menuCamInvY_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[64];
    sprintf(str, self->name, "XXX");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
}
void menuCamInvY_select(const MenuItem *self, int amount) {
    audioPlaySound(NULL, MENU_ADJUST_SOUND);
}


Menu menuControlSettings = {
    "Control Settings", 0,
    genericMenu_run, mainSubMenu_close,
    "Rumble: %s",                    menuRumble_draw,     menuRumble_select,
    "Rumble Blur: %s",               menuRumbleBlur_draw, menuRumbleBlur_select,
    "Camera Control: Controller %d", menuCamCtrl_draw,    menuCamCtrl_select,
    "Camera X: %s",                  menuCamInvX_draw,    menuCamInvX_select,
    "Camera Y: %s",                  menuCamInvY_draw,    menuCamInvY_select,
    NULL,
};
