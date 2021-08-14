/** Debug Object List submenu for selected object.
 */
#include "main.h"
#include "menuDebugObjs.h"
static s8 selectedSeq = 0;

void objSelMenu_draw(Menu *self) {
    //Draw function for Object List selected object submenu
    drawMenuBox(OBJ_INFO_XPOS, OBJ_INFO_YPOS, OBJ_INFO_WIDTH, OBJ_INFO_HEIGHT);
    genericMenu_drawAt(self, OBJ_MENU_XPOS, OBJ_MENU_YPOS,
        OBJ_MENU_WIDTH, OBJ_MENU_HEIGHT);
    objMenu_drawObjInfo(objMenuSelected);

    char title[16];
    sprintf(title, "<%s>", objMenuSelected->file->name);
    gameTextSetColor(0, 255, 255, 255);
    Color4b color = {.r=0, .g=255, .b=255, .a=255};
    drawColorText(title, OBJ_MENU_XPOS+MENU_PADDING+8, OBJ_MENU_YPOS+MENU_PADDING, color);
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
        freeMoveCoords.x = objMenuSelected->pos.pos.x;
        freeMoveCoords.y = objMenuSelected->pos.pos.y;
        freeMoveCoords.z = objMenuSelected->pos.pos.z;
    }
    cameraUpdate(1);
}
void menuObjSelSummon_select(const MenuItem *self, int amount) {
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    if(pPlayer) {
        objDisableHitbox(objMenuSelected); //only for next tick
        objMenuSelected->pos.pos = pPlayer->pos.pos;
        //try to override hit detection...
        objMenuSelected->prevPos = pPlayer->pos.pos;
    }
    cameraUpdate(1);
}
void menuObjSelSetPlayer_select(const MenuItem *self, int amount) {
    //this will just crash 99% of the time
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    pPlayer = objMenuSelected;
}
void menuObjSelRide_select(const MenuItem *self, int amount) {
    //this will also crash 99% of the time
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    void *pState = pPlayer ? pPlayer->state : NULL;
    (*(ObjInstance**)(pState + 0x7F0)) = objMenuSelected;
    (*(u16*)(pState + 0x274)) = PlayerStateEnum_RideBike;
}
void menuObjSelHold_select(const MenuItem *self, int amount) {
    //doesn't always crash but not very useful
    if(amount) return;
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    playerSetHeldObject(pPlayer, objMenuSelected);
}
void menuObjSelEditObj_select(const MenuItem *self, int amount) {
    if(amount) return;
    hexEditPrevMenu = curMenu;
    hexEditAddr = (u32)objMenuSelected;
    curMenu = &menuDebugHexEdit;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}
void menuObjSelEditDef_select(const MenuItem *self, int amount) {
    if(amount) return;
    if(!objMenuSelected->objDef) {
        audioPlaySound(NULL, MENU_FAIL_SOUND);
        return;
    }
    hexEditPrevMenu = curMenu;
    hexEditAddr = (u32)objMenuSelected->objDef;
    curMenu = &menuDebugHexEdit;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}
void menuObjSelEditFile_select(const MenuItem *self, int amount) {
    if(amount) return;
    if(!objMenuSelected->file) {
        audioPlaySound(NULL, MENU_FAIL_SOUND);
        return;
    }
    hexEditPrevMenu = curMenu;
    hexEditAddr = (u32)objMenuSelected->file;
    curMenu = &menuDebugHexEdit;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}
void menuObjSelEditState_select(const MenuItem *self, int amount) {
    if(amount) return;
    if(!objMenuSelected->state) {
        audioPlaySound(NULL, MENU_FAIL_SOUND);
        return;
    }
    hexEditPrevMenu = curMenu;
    hexEditAddr = (u32)objMenuSelected->state;
    curMenu = &menuDebugHexEdit;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuObjSelCallSeq_draw(const MenuItem *self, int x, int y, bool selected) {
    char str[256];

    //do this here so it applies when a different object is selected
    if(selectedSeq < 0) selectedSeq = objMenuSelected->file->numSeqs - 1;
    if(selectedSeq >= objMenuSelected->file->numSeqs) selectedSeq = 0;

    u16 seqNo = 0xFFFF;
    if(selectedSeq < objMenuSelected->file->numSeqs) {
        //if object has no sequences the selected seq will still be invalid.
        //this doesn't matter for calling because it will just do nothing.
        seqNo = objMenuSelected->file->pSeq[selectedSeq];
    }
    sprintf(str, self->fmt, T(self->name), selectedSeq, seqNo);
    menuDrawText(str, x, y, selected);
}
void menuObjSelCallSeq_select(const MenuItem *self, int amount) {
    if(amount) {
        selectedSeq += amount;
        audioPlaySound(NULL, MENU_ADJUST_SOUND);
        return;
    }
    objRunSeq(selectedSeq, objMenuSelected, 0xFFFFFFFF);
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

Menu menuDebugObjSelected = {
    "", 0, //blank title
    genericMenu_run, objSelMenu_draw, objListSubmenu_close,
    "Set Camera Focus",   "%s", genericMenuItem_draw, menuObjSelFocus_select,
    "Go To",              "%s", genericMenuItem_draw, menuObjSelMovePlayer_select,
    "Bring To Player",    "%s", genericMenuItem_draw, menuObjSelSummon_select,
    "Delete",             "%s", genericMenuItem_draw, menuObjSelDelete_select,
    "Edit",               "%s", genericMenuItem_draw, menuObjSelEditObj_select,
    "Edit ObjDef",        "%s", genericMenuItem_draw, menuObjSelEditDef_select,
    "Edit File",          "%s", genericMenuItem_draw, menuObjSelEditFile_select,
    "Edit State",         "%s", genericMenuItem_draw, menuObjSelEditState_select,
    "Run Seq",   "%s %d: %04X", menuObjSelCallSeq_draw, menuObjSelCallSeq_select,
    //these will probably crash, so add a little warning icon
    "Set as Player",  "[!] %s", genericMenuItem_draw, menuObjSelSetPlayer_select,
    "Ride",           "[!] %s", genericMenuItem_draw, menuObjSelRide_select,
    "Pick Up",        "[!] %s", genericMenuItem_draw, menuObjSelHold_select,
    NULL,
};
