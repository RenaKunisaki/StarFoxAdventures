/** Debug Object List submenu.
 */
#include "main.h"
#include "menuDebugObjs.h"

void objSelMenu_draw(Menu *self) {
    //Draw function for Object List selected object submenu
    drawMenuBox(OBJ_INFO_XPOS, OBJ_INFO_YPOS, OBJ_INFO_WIDTH, OBJ_INFO_HEIGHT);
    genericMenu_drawAt(self, OBJ_MENU_XPOS, OBJ_MENU_YPOS,
        OBJ_MENU_WIDTH, OBJ_MENU_HEIGHT);
    objMenu_drawObjInfo(objMenuSelected);

    char title[16];
    sprintf(title, "<%s>", objMenuSelected->file->name);
    gameTextSetColor(0, 255, 255, 255);
    gameTextShowStr(title, MENU_TEXTBOX_ID,
        OBJ_MENU_XPOS+MENU_PADDING+8, OBJ_MENU_YPOS+MENU_PADDING);
}

void menuObjSelFocus_select(const MenuItem *self, int amount) {
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;

    //focusing the camera on an object with NULL state poiner
    //will crash the game, so don't do that.
    if(pCamera && objMenuSelected->state) {
        pCamera->focus = objMenuSelected;
        //since time is stopped we must manually update
        //the camera position.
        cameraUpdate(1);
    }
    else audioPlaySound(NULL, MENU_FAIL_SOUND);
    //XXX else just move camera to it?
}

void menuObjSelDelete_select(const MenuItem *self, int amount) {
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    objFree(objMenuSelected);
}

void menuObjSelMovePlayer_select(const MenuItem *self, int amount) {
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    if(pPlayer) {
        objDisableHitbox(pPlayer); //only for next tick
        pPlayer->pos.pos = objMenuSelected->pos.pos;
        //try to override hit detection...
        pPlayer->prevPos = objMenuSelected->pos.pos;
    }
    cameraUpdate(1);
}

void menuObjSelSetPlayer_select(const MenuItem *self, int amount) {
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    pPlayer = objMenuSelected;
}

Menu menuDebugObjSelected = {
    "", 0,
    genericMenu_run, objSelMenu_draw, objListSubmenu_close,
    "Set Camera Focus", genericMenuItem_draw, menuObjSelFocus_select,
    "Go To",            genericMenuItem_draw, menuObjSelMovePlayer_select,
    "Delete",           genericMenuItem_draw, menuObjSelDelete_select,
    "Set as Player",    genericMenuItem_draw, menuObjSelSetPlayer_select,
    NULL,
};
