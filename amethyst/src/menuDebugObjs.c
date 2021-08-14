/** Debug Object List submenu.
 */
#include "main.h"
#include "menuDebugObjs.h"

//store list of all obj pointers which we can then sort
static ObjInstance **objList = NULL;
ObjInstance *objMenuSelected = NULL;

enum {
    ObjListSortNone,
    ObjListSortId,
    ObjListSortType,
    ObjListSortName,
    ObjListSortDistance,
    ObjListSortAddr,
    NumObjListSortMethods
} ObjListSortMethods;
static const char *sortModeNames[] = {
    "None", "UniqueID", "DefNo", "Name", "Distance", "Address",
};

static u8 sortMode = ObjListSortNone;

static void sortObjs(int method) {
    u32 size = sizeof(ObjInstance*) * numLoadedObjs;
    if(objList) free(objList);
    objList = allocTagged(size, ALLOC_TAG_LISTS_COL, "debug:objList");
    if(!objList) return;
    memcpy(objList, loadedObjects, size);

    switch(method) {
        case ObjListSortId:
            quicksort((const void**)objList, 0, numLoadedObjs-1,
                compareObjsById);
            break;

        case ObjListSortType:
            quicksort((const void**)objList, 0, numLoadedObjs-1,
                compareObjsByType);
            break;

        case ObjListSortName:
            quicksort((const void**)objList, 0, numLoadedObjs-1,
                compareObjsByName);
            break;

        case ObjListSortDistance:
            quicksort((const void**)objList, 0, numLoadedObjs-1, compareObjsByDistance);
            break;

        case ObjListSortAddr:
            quicksort((const void**)objList, 0, numLoadedObjs-1, compareObjsByAddr);
            break;

        default: break;
    }
}


void objListSubmenu_close(const Menu *self) {
    //Close function for submenus of the object list menu
    curMenu = &menuDebugObjList;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}


void objMenu_drawObjInfo(ObjInstance *obj) {
    //Draw the right side pane (object info).

    int x = OBJ_INFO_XPOS + MENU_PADDING;
    int y = OBJ_INFO_YPOS + MENU_PADDING;
    char str[256];

    sprintf(str, "\eF%s  %d, %d, %d", T("POS"),
        (int)obj->pos.pos.x, (int)obj->pos.pos.y, (int)obj->pos.pos.z);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    if(obj->objDef) {
        sprintf(str, "\eF%s %d, %d, %d", T("ORIG"),
            (int)obj->objDef->pos.x, (int)obj->objDef->pos.y,
            (int)obj->objDef->pos.z);
        drawSimpleText(str, x, y);
        y += MENU_LINE_HEIGHT;

        sprintf(str, "\eF%s   %08X %s %08X", T("ADDR"), obj, T("ID"), obj->objDef->id);
        drawSimpleText(str, x, y);
        y += MENU_LINE_HEIGHT;
    }
    else {
        sprintf(str, "\eF%s", T("NO OBJDEF"));
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;

        sprintf(str, "\eF%s   %08X %s --", T("ADDR"), obj, T("ID"));
        drawSimpleText(str, x, y);
        y += MENU_LINE_HEIGHT;
    }

    sprintf(str, "\eF%s   %08X %s %04X", T("FILE"), obj->file, T("DEFNO"), obj->defNo);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s  %04X %s %04X", T("CATID"), obj->catId, T("OBJNO"), obj->objNo);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s   %04X %02X %02X %02X %02X", T("FLAG"), obj->pos.flags,
        obj->flags_0xaf, obj->flags_0xb0, obj->flags_e3, obj->flags_0xf8);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s   %02X %s %02X %02X", T("SLOT"), obj->slot, T("MAP"), obj->mapId & 0xFF,
        obj->map & 0xFF);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s %08X %s %d", T("OBJDEF"), obj->objDef, T("SEQ"), obj->curSeq);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s  %08X", T("EVENT"), obj->pEventName);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    Model **models = obj->models = obj->models;
    Model  *model = models ? models[obj->curModel] : NULL;
    u16     mdlId = (model && model->header) ? model->header->modelId : 0xFFFF;
    sprintf(str, "\eF%s  %08X(%X) %04X", T("MODEL"), obj->models, obj->curModel, mdlId);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s  %08X", T("STATE"), obj->state);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s   %08X", T("HITS"), obj->hitstate);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s    %08X %04X", T("DLL"), obj->dll,
        (obj->file ? obj->file->dll_id : -1) & 0xFFFF);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s   %08X %X/%X", T("MSGS"), obj->seqMsgs,
        obj->seqMsgs ? obj->seqMsgs->nMsgs : 0,
        obj->seqMsgs ? obj->seqMsgs->length : 0);
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eF%s %08X %s", T("PARENT"), obj->parent,
        (obj->parent && obj->parent->file) ? obj->parent->file->name : "-");
    drawSimpleText(str, x, y);
    y += MENU_LINE_HEIGHT;

    for(int i=0; i<3; i++) {
        sprintf(str, "\eF%s  %08X %s", T("CHILD"), obj->child[i],
            (obj->child[i] && obj->child[i]->file) ?
                obj->child[i]->file->name : "-");
        drawSimpleText(str, x, y);
        y += MENU_LINE_HEIGHT;
    }
}

void objMenu_draw(Menu *self) {
    //Draw function for Object List menu
    menuAnimFrame++;

    drawMenuBox(OBJ_MENU_XPOS, OBJ_MENU_YPOS, OBJ_MENU_WIDTH, OBJ_MENU_HEIGHT);
    drawMenuBox(OBJ_INFO_XPOS, OBJ_INFO_YPOS, OBJ_INFO_WIDTH, OBJ_INFO_HEIGHT);

    int x = OBJ_MENU_XPOS + MENU_PADDING, y = OBJ_MENU_YPOS + MENU_PADDING;
    int start = MAX(0, self->selected - (OBJ_MENU_NUM_LINES-1));

    if(!objList) {
        gameTextShowStr(T("Sorting..."), MENU_TEXTBOX_ID, x, y);
        return;
    }

    char str[256];
    for(int i=0; i < OBJ_MENU_NUM_LINES; i++) {
        int objIdx = i + start;
        if(objIdx >= numLoadedObjs) break;

        ObjInstance *obj = objList[objIdx];
        if(!PTR_VALID(obj)) break;

        Color4b color = {.r=192, .g=192, .b=192, .a=255};
        bool selected = objIdx == self->selected;
        if(selected) {
            objMenu_drawObjInfo(obj);
            color = hsv2rgb(menuTextAnimFrame*2, 192, 192, 128);
        }
        else if(pCamera && pCamera->focus == obj) {
            color.r=255; color.g=0; color.b=255; //pink
        }
        else if(pCamera && pCamera->target == obj) {
            color.r=255; color.g=255; color.b=0; //yellow
        }

        sprintf(str, "\eF%04X %08X %s",
            (obj->objDef ? obj->objDef->objType : -1) & 0xFFFF,
            (obj->objDef ? obj->objDef->id      : -1),
            (obj->file ? obj->file->name : "?"));
        drawColorText(str, x, y, color);
        y += MENU_LINE_HEIGHT;
    }

    //draw instructions in right pane
    //done here so they don't stay when submenu is open
    sprintf(str, "S:%s  Y:%s\nZ:%s: %s", T("Player"), T("Show"), T("Sort"), sortModeNames[sortMode]);
    drawSimpleText(str,
        OBJ_INFO_XPOS + MENU_PADDING,
        OBJ_INFO_YPOS + OBJ_INFO_HEIGHT - (MENU_LINE_HEIGHT*2) - MENU_PADDING);
}

static bool objMenuCheckClose(Menu *self) {
    if(buttonsJustPressed == PAD_BUTTON_B) { //close menu
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        self->close(curMenu);
        if(objList) free(objList);
        objList = NULL;
        return true;
    }
    return false;
}

static void objMenuSelectPlayer(Menu *self) {
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
    for(int i=0; i<numLoadedObjs; i++) {
        if(objList[i] == pPlayer) {
            self->selected = i;
            break;
        }
    }
}

static void objMenuShow(Menu *self) {
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;

    //the object keeps a pointer to the ObjDef and frees it
    //so it's important that it be allocated, not local.
    //manually setting camera position doesn't work no matter how
    //aggressively. we can either bypass it entirely and modify the
    //view matrix and do the math ourselves, or this.
    ObjDef_Override *obj = (ObjDef_Override*)objAlloc(9*4, ObjDefEnum_Override);
    if(!obj) {
        OSReport("Failed to alloc object");
        return;
    }
    obj->def.pos.x = objMenuSelected->pos.pos.x;
    obj->def.pos.y = objMenuSelected->pos.pos.y;
    obj->def.pos.z = objMenuSelected->pos.pos.z;
    ObjInstance* target = objInstantiateCharacter((ObjDef*)obj,
        5, -1, -1, NULL);
    if(!target) {
        OSReport("Failed to spawn override");
        free(obj);
        return;
    }

    ObjInstance *oldTarget = pCamera->focus;
    pCamera->focus = target;
    cameraUpdate(1);
    //sceneRender();
    pCamera->focus = oldTarget;
    objFree(target);
    //free(obj); //objFree does it for us

    //refresh list
    if(objList) free(objList);
    objList = NULL;
}

static void objMenuChangeSort(Menu *self) {
    ObjInstance *obj = objList[self->selected];
    menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;

    sortMode++;
    if(sortMode >= NumObjListSortMethods) sortMode = 0;
    sortObjs(sortMode);
    //re-select previously selected obj
    if(objList) {
        for(int i=0; i<numLoadedObjs; i++) {
            if(objList[i] == obj) {
                self->selected = i;
                break;
            }
        }
    }
}


void objMenu_run(Menu *self) {
    //Run function for Object List menu
    int sel = self->selected;

    objMenuSelected = NULL;
    if(!objList) sortObjs(sortMode);
    if(!objList) { //out of memory
        objMenuCheckClose(self);
        return;
    }

    ObjInstance *obj = objList[sel];
    objMenuSelected = obj;

    if(objMenuCheckClose(self)) {
        //nothing to do
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) { //change sort mode
        objMenuChangeSort(self);
    }
    else if(buttonsJustPressed == PAD_BUTTON_A) { //open submenu
        curMenu = &menuDebugObjSelected;
        audioPlaySound(NULL, MENU_OPEN_SOUND);
        //ensure the list is refreshed when we return
        if(objList) free(objList);
        objList = NULL;
    }
    else if(buttonsJustPressed == PAD_BUTTON_Y) { //view object
        objMenuShow(self);
    }
    else if(buttonsJustPressed == PAD_BUTTON_MENU) { //jump to player in list
        objMenuSelectPlayer(self);
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
    objMenu_run, objMenu_draw, debugMapSubMenu_close,
    NULL,
};
