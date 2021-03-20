/** Debug Object List submenu.
 */
#include "main.h"
#define OBJ_MENU_XPOS 2
#define OBJ_MENU_YPOS 40
#define OBJ_MENU_WIDTH  310
#define OBJ_MENU_HEIGHT 438
#define OBJ_INFO_XPOS (OBJ_MENU_XPOS + OBJ_MENU_WIDTH)
#define OBJ_INFO_YPOS 40
#define OBJ_INFO_WIDTH  330
#define OBJ_INFO_HEIGHT 438
#define OBJ_MENU_NUM_LINES ((OBJ_MENU_HEIGHT / MENU_LINE_HEIGHT) - 1)

static void objMenu_drawObjInfo(ObjInstance *obj) {
    int x = OBJ_INFO_XPOS + MENU_PADDING;
    int y = OBJ_INFO_YPOS + MENU_PADDING;
    char str[256];

    sprintf(str, "POS  %d, %d, %d",
        (int)obj->pos.pos.x, (int)obj->pos.pos.y, (int)obj->pos.pos.z);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    if(obj->objDef) {
        sprintf(str, "ORIG %d, %d, %d",
            (int)obj->objDef->pos.x, (int)obj->objDef->pos.y,
            (int)obj->objDef->pos.z);
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;

        sprintf(str, "ADDR   %08X ID %08X", obj, obj->objDef->id);
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;
    }
    else {
        gameTextShowStr("NO OBJDEF", MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;

        sprintf(str, "ADDR   %08X ID --", obj);
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;
    }

    sprintf(str, "FILE   %08X DEFNO %04X", obj->file, obj->defNo);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "CATID  %04X OBJNO %04X", obj->catId, obj->objNo);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "FLAG   %04X %02X %02X %02X %02X", obj->pos.flags,
        obj->flags_0xaf, obj->flags_0xb0, obj->flags_e3, obj->flags_0xf8);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "SLOT   %02X MAP %02X %02X", obj->slot, obj->mapId & 0xFF,
        obj->map & 0xFF);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "OBJDEF %08X SEQ %d", obj->objDef, obj->curSeq);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "EVENT  %08X", obj->pEventName);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    Model **models = obj->models = obj->models;
    Model  *model = models ? models[obj->curModel] : NULL;
    u16     mdlId = (model && model->header) ? model->header->modelId : 0xFFFF;
    sprintf(str, "MODEL  %08X(%X) %04X", obj->models, obj->curModel, mdlId);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "STATE  %08X", obj->state);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "HITS   %08X", obj->hitstate);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "DLL    %08X %04X", obj->dll,
        (obj->file ? obj->file->dll_id : -1) & 0xFFFF);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "MSGS   %08X %X/%X", obj->seqMsgs,
        obj->seqMsgs ? obj->seqMsgs->nMsgs : 0,
        obj->seqMsgs ? obj->seqMsgs->length : 0);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "PARENT %08X %s", obj->parent,
        (obj->parent && obj->parent->file) ? obj->parent->file->name : "-");
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    for(int i=0; i<3; i++) {
        sprintf(str, "CHILD  %08X %s", obj->child[i],
            (obj->child[i] && obj->child[i]->file) ?
                obj->child[i]->file->name : "-");
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;
    }

    gameTextShowStr("Z:Focus S:Player", MENU_TEXTBOX_ID,
        OBJ_INFO_XPOS + MENU_PADDING,
        OBJ_INFO_YPOS + OBJ_INFO_HEIGHT - (MENU_LINE_HEIGHT*2) - MENU_PADDING);
    gameTextShowStr("Y:GoTo  X:Delete", MENU_TEXTBOX_ID,
        OBJ_INFO_XPOS + MENU_PADDING,
        OBJ_INFO_YPOS + OBJ_INFO_HEIGHT - (MENU_LINE_HEIGHT*1) - MENU_PADDING);
}

void objMenu_draw(Menu *self) {
    //Draw function for Object List menu
    menuAnimFrame++;

    drawMenuBox(OBJ_MENU_XPOS, OBJ_MENU_YPOS, OBJ_MENU_WIDTH, OBJ_MENU_HEIGHT);
    drawMenuBox(OBJ_INFO_XPOS, OBJ_INFO_YPOS, OBJ_INFO_WIDTH, OBJ_INFO_HEIGHT);

    int x = OBJ_MENU_XPOS + MENU_PADDING, y = OBJ_MENU_YPOS + MENU_PADDING;
    int start = MAX(0, self->selected - (OBJ_MENU_NUM_LINES-1));

    for(int i=0; i < OBJ_MENU_NUM_LINES; i++) {
        int objIdx = i + start;
        if(objIdx >= numLoadedObjs) break;

        ObjInstance *obj = loadedObjects[objIdx];
        bool selected = objIdx == self->selected;
        if(selected) {
            gameTextSetColor(255, 255, 255, 255);
            objMenu_drawObjInfo(obj);
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else if(pCamera && pCamera->focus == obj) {
            gameTextSetColor(255, 0, 255, 255); //pink
        }
        else if(pCamera && pCamera->target == obj) {
            gameTextSetColor(255, 255, 0, 255); //yellow
        }
        else gameTextSetColor(255, 255, 255, 255);

        char str[256];
        sprintf(str, "%04X %08X %s",
            (obj->objDef ? obj->objDef->objType : -1) & 0xFFFF,
            (obj->objDef ? obj->objDef->id      : -1),
            (obj->file ? obj->file->name : "?"));
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;
    }
}

void objMenu_run(Menu *self) {
    //Run function for Object List menu
    textForceFixedWidth = MENU_FIXED_WIDTH;
    int sel = self->selected;
    ObjInstance *obj = loadedObjects[sel];

    if(buttonsJustPressed == PAD_BUTTON_B) { //close menu
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        self->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_X) { //delete object
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        objFree(obj);
    }
    else if(buttonsJustPressed == PAD_BUTTON_Y) { //move player to object
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        if(pPlayer) {
            objDisableHitbox(pPlayer); //only for next tick
            pPlayer->pos.pos = obj->pos.pos;
            //try to override hit detection...
            pPlayer->prevPos = obj->pos.pos;
        }
        cameraUpdate(1);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) { //focus camera on object
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        //focusing the camera on an object with NULL state poiner
        //will crash the game, so don't do that.
        if(pCamera && obj->state) {
            pCamera->focus = obj;
            //since time is stopped we must manually update
            //the camera position.
            cameraUpdate(1);
        }
        else audioPlaySound(NULL, 0xFD); //"can't use that item"
    }
    else if(buttonsJustPressed == PAD_BUTTON_MENU) { //jump to player in list
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        for(int i=0; i<numLoadedObjs; i++) {
            if(loadedObjects[i] == pPlayer) {
                self->selected = i;
                break;
            }
        }
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) sel = numLoadedObjs;
        self->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(sel >= numLoadedObjs) sel = 0;
        self->selected = sel;
    }
    else if(controllerStates[0].triggerLeft > MENU_TRIGGER_THRESHOLD) { //L
        sel -= OBJ_MENU_NUM_LINES;
        if(sel <= 0) sel = numLoadedObjs;
        self->selected = sel - 1;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
    else if(controllerStates[0].triggerRight > MENU_TRIGGER_THRESHOLD) { //R
        sel += OBJ_MENU_NUM_LINES;
        if(sel >= numLoadedObjs) sel = 0;
        self->selected = sel;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }

    //object list can change while we're looking at it.
    if(self->selected >= numLoadedObjs) self->selected = numLoadedObjs - 1;
}

Menu menuDebugObjList = {
    "Objects", 0,
    objMenu_run, objMenu_draw, debugGameSubMenu_close,
    NULL,
};
