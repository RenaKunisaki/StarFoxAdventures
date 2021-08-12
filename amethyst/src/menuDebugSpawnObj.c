/** Debug Spawn Object submenu.
 */
#include "main.h"
#include "revolution/os.h"
#define SPAWN_MENU_XPOS 15
#define SPAWN_MENU_YPOS 50
#define SPAWN_MENU_WIDTH  600
#define SPAWN_MENU_HEIGHT 405
#define SPAWN_MENU_NUM_LINES ((SPAWN_MENU_HEIGHT / MENU_LINE_HEIGHT) - 1)
#define SPAWN_MENU_MAX_PARAMS 15
#define SPAWN_MENU_CURSOR_X_OFFS (SPAWN_MENU_XPOS + MENU_PADDING + \
    (MENU_FIXED_WIDTH * 11))

enum {
    SPAWN_MENU_ITEM_OBJECT = 0,
    SPAWN_MENU_ITEM_NUM_PARAMS,
    SPAWN_MENU_ITEM_SPAWN_FLAGS,
    SPAWN_MENU_ITEM_LOAD_FLAGS,
    SPAWN_MENU_ITEM_MAP_ACTS,
    SPAWN_MENU_ITEM_BOUNDS,
    SPAWN_MENU_ITEM_UNK7,
    SPAWN_MENU_ITEM_OBJID,
    SPAWN_MENU_ITEM_MAPID,
    SPAWN_MENU_ITEM_OBJNO,
    SPAWN_MENU_ITEM_XPOS,
    SPAWN_MENU_ITEM_YPOS,
    SPAWN_MENU_ITEM_ZPOS,
    SPAWN_MENU_ITEM_PARAMS,
    SPAWN_MENU_NUM_ITEMS
};

struct {
    ObjDef def;
    u32 params[SPAWN_MENU_MAX_PARAMS];
} spawnObjDef;
s32 spawnCoords[3];
u32 spawnFlags     =  5;
s16 spawnObjNo     = -1; //ObjNo, not spawnObjDef.def.objType (ObjDefEnum)
u16 spawnActs      =  4; //copied to mapActs1 and mapActs2, easier to edit this way
s8  spawnMapId     = -1;
s8  spawnNumparams =  0;
u8  spawnCursorX   =  0;
bool spawnMenuIsInit = false;

enum {
    ObjSpawnListSortType,
    ObjSpawnListSortName,
    ObjSpawnListSortCategory,
    NumObjSpawnListSortMethods
} ObjSpawnListSortMethods;
static const char *sortModeNames[] = {"DefNo", "Name", "Category"};
static u8 sortMode = ObjSpawnListSortType;
static int *objIdList = NULL;
static int numItems = 0;


void spawnList_draw(Menu *self) {
    //Draw function for Spawn Object list
    char str[256];
    menuAnimFrame++;

    drawMenuBox(SPAWN_MENU_XPOS, SPAWN_MENU_YPOS, SPAWN_MENU_WIDTH, SPAWN_MENU_HEIGHT);
    gameTextSetColor(255, 255, 255, 255);

    int x = SPAWN_MENU_XPOS + MENU_PADDING, y = SPAWN_MENU_YPOS + MENU_PADDING;
    int iObj    = MAX(0, self->selected - (SPAWN_MENU_NUM_LINES-2));
    int nLines  = 0;

    if(!objIdList) {
        drawSimpleText("Sorting...", x, y);
        return;
    }

    while(nLines < (SPAWN_MENU_NUM_LINES-1) && iObj < numItems) {
        char name[12];
        int defNo = objIdList[iObj];
        int realId = getObjRealId(defNo);
        ObjectFileStruct *file = getObjFile(defNo);
        if(!file) break;
        getObjFileName(name, file);
        sprintf(str, "%04X %04X %s", defNo & 0xFFFF, realId & 0xFFFF, name);

        menuDrawText(str, x, y, iObj == self->selected);
        y += MENU_LINE_HEIGHT;
        nLines++;
        iObj++;
    }

    sprintf(str, "Z:Sort:%s", sortModeNames[sortMode]);
    drawSimpleText(str, x+30, y);
}

int compareFilesByName(const void *fileA, const void *fileB) {
    char nameA[12], nameB[12];
    getObjFileName(nameA, getObjFile((int)fileA));
    getObjFileName(nameB, getObjFile((int)fileB));
    return strcmpi(nameA, nameB);
}
int compareFilesByCategory(const void *fileA, const void *fileB) {
    ObjectFileStruct *A = getObjFile((int)fileA);
    ObjectFileStruct *B = getObjFile((int)fileB);
    if(A && B) return (int)(A->clsId) - (int)(B->clsId);
    else if(A) return -1;
    else return 1;
}

void rebuildList() {
    //copy ObjectFileStruct pointers to list in order
    s16  *objIndex = dataFileBuffers[FILE_OBJINDEX_BIN];
    u32  *objsTab  = dataFileBuffers[FILE_OBJECTS_TAB];
    void *objsBin  = dataFileBuffers[FILE_OBJECTS_BIN];

    //build list, skipping objindex duplicates
    u8 seenId[objIndexCount]; //XXX there must be a better way
    memset(seenId, 0, objIndexCount);
    numItems = 0;
    for(int i=0; i<objIndexCount; i++) {
        int id = objIndex[i];
        if(id < 0) id = i;
        if(PTR_VALID(getObjFile(id))) {
            if(!seenId[id]) {
                objIdList[numItems++] = i;
                seenId[id] = 1;
            }
        }
    }
    numItems--;

    //sort the list
    CompareFunc func = NULL;
    switch(sortMode) {
        case ObjSpawnListSortName: func = compareFilesByName; break;
        case ObjSpawnListSortCategory: func = compareFilesByCategory; break;
        default: return; //list is already sorted by defNo
    }
    quicksort((const void**)objIdList, 0, numItems, func);
}

void changeSort() {
    sortMode++;
    if(sortMode >= NumObjSpawnListSortMethods) sortMode = 0;
    rebuildList();
}

void spawnList_run(Menu *self) {
    //Run function for Spawn Object list

    if(!objIdList) {
        objIdList = (int*)allocTagged(NUM_OBJECTS * sizeof(ObjectFileStruct*),
            ALLOC_TAG_LISTS_COL, "debug:objIds");
        if(!objIdList) { //out of memory
            if(buttonsJustPressed == PAD_BUTTON_A || buttonsJustPressed == PAD_BUTTON_B) {
                menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
                curMenu->close(curMenu);
            }
            return;
        }
        else rebuildList();
    }

    if(buttonsJustPressed == PAD_BUTTON_A || buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);

        //find selected defNo
        spawnObjDef.def.objType = objIdList[self->selected];
        if(objIdList) free(objIdList);
        objIdList = NULL;
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) changeSort();
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected -= 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected += 1;
    }
    else if(controllerStates[0].triggerLeft > MENU_TRIGGER_THRESHOLD) { //L
        self->selected -= 0x100;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
    else if(controllerStates[0].triggerRight > MENU_TRIGGER_THRESHOLD) { //R
        self->selected += 0x100;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }

    if(self->selected < 0) self->selected = numItems - 1;
    if(self->selected >= numItems) self->selected = 0;
}

void spawnList_close(const Menu *self) {
    //Close function for Spawn Object list
    curMenu = &menuDebugSpawnObj;
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void spawnMenu_draw(Menu *self) {
    //Draw function for Spawn Object menu
    char str[256];
    menuAnimFrame++;

    drawMenuBox(SPAWN_MENU_XPOS, SPAWN_MENU_YPOS, SPAWN_MENU_WIDTH, SPAWN_MENU_HEIGHT);
    gameTextSetColor(255, 255, 255, 255);

    int x = SPAWN_MENU_XPOS + MENU_PADDING, y = SPAWN_MENU_YPOS + MENU_PADDING;
    s16  *objIndex = dataFileBuffers[FILE_OBJINDEX_BIN];
    u32  *objsTab  = dataFileBuffers[FILE_OBJECTS_TAB];
    void *objsBin  = dataFileBuffers[FILE_OBJECTS_BIN];

    s16 realId = objIndex[spawnObjDef.def.objType];
    if(realId == -1) realId = spawnObjDef.def.objType;
    u32 offs = objsTab[realId];
    ObjectFileStruct *file = NULL;
    if(offs < dataFileSize[FILE_OBJECTS_BIN]) {
        file = (ObjectFileStruct*)(objsBin + offs);
    }

    //copy the name, filtering out any control codes.
    char name[12];
    getObjFileName(name, file);


    sprintf(str, "\eFObject:      %04X (%04X) %s",
        spawnObjDef.def.objType & 0xFFFF, realId & 0xFFFF, name);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFParams:      %X", spawnNumparams);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFSpawn Flags: %08X", spawnFlags);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFLoad  Flags: %02X ", spawnObjDef.def.loadFlags & 0xFF);
    bin2str(&str[strlen(str)], spawnObjDef.def.loadFlags, 8);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFMap Acts:    %04X", spawnActs & 0xFFFF);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFBounds:      %02X", spawnObjDef.def.bound & 0xFF);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFUnk07:       %02X", spawnObjDef.def.unk7 & 0xFF);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFObj ID:      %08X", spawnObjDef.def.id);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFMap ID:      %02X", spawnMapId & 0xFF);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFObj No.:     %04X", spawnObjNo & 0xFFFF);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFX Position:  %08X", spawnCoords[0]);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFY Position:  %08X", spawnCoords[1]);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "\eFZ Position:  %08X", spawnCoords[2]);
    menuDrawText(str, x, y, false);
    y += MENU_LINE_HEIGHT;

    int start = MAX(0, self->selected - (SPAWN_MENU_NUM_LINES-1));
    for(int i=0; i < (SPAWN_MENU_NUM_LINES - SPAWN_MENU_ITEM_PARAMS); i++) {
        int pIdx = i + start;
        if(pIdx >= spawnNumparams) break;
        sprintf(str, "\eFParam %02X:    %08X", pIdx*4, spawnObjDef.params[pIdx]);
        menuDrawText(str, x, y, false);
        y += MENU_LINE_HEIGHT;
    }

    drawSimpleText("Start:Spawn A:List B:Exit X:+ Y:- Z:Player Coords",
        x, SPAWN_MENU_YPOS + SPAWN_MENU_HEIGHT -
        (MENU_LINE_HEIGHT + MENU_PADDING));

    //draw cursor
    drawBox(SPAWN_MENU_XPOS + 4 + (spawnCursorX * MENU_FIXED_WIDTH) +
        SPAWN_MENU_CURSOR_X_OFFS,
        SPAWN_MENU_YPOS + 5 + (MENU_LINE_HEIGHT * (self->selected - start)),
        20, 24, 255, false);
}

static void adjustHex8(u8 *val, int adj) {
    int shift = (((7-spawnCursorX) & 1) * 4);
    u8 mask = 0xF << shift;
    adj <<= shift;
    *val = (*val & ~mask) | ((*val + adj) & mask);
}
static void adjustHex16(u16 *val, int adj) {
    int shift = (((7-spawnCursorX) & 3) * 4);
    u16 mask = 0xF << shift;
    adj <<= shift;
    *val = (*val & ~mask) | ((*val + adj) & mask);
}
static void adjustHex32(u32 *val, int adj) {
    int shift = ((7-spawnCursorX) * 4);
    u32 mask = 0xF << shift;
    adj <<= shift;
    *val = (*val & ~mask) | ((*val + adj) & mask);
}

static void doAdjust(Menu *self, int amount) {
    switch(self->selected) {
        case SPAWN_MENU_ITEM_OBJECT:
            adjustHex16(&spawnObjDef.def.objType, amount);
            break;

        case SPAWN_MENU_ITEM_NUM_PARAMS:
            spawnNumparams += amount;
            if(spawnNumparams > SPAWN_MENU_MAX_PARAMS) spawnNumparams = 0;
            if(spawnNumparams < 0) spawnNumparams = SPAWN_MENU_MAX_PARAMS;
            break;

        case SPAWN_MENU_ITEM_SPAWN_FLAGS:
            adjustHex32(&spawnFlags, amount);
            break;

        case SPAWN_MENU_ITEM_LOAD_FLAGS:
            adjustHex8(&spawnObjDef.def.loadFlags, amount);
            break;

        case SPAWN_MENU_ITEM_MAP_ACTS:
            adjustHex16(&spawnActs, amount);
            break;

        case SPAWN_MENU_ITEM_BOUNDS:
            adjustHex8(&spawnObjDef.def.bound, amount);
            break;

        case SPAWN_MENU_ITEM_UNK7:
            adjustHex8(&spawnObjDef.def.unk7, amount);
            break;

        case SPAWN_MENU_ITEM_OBJID:
            adjustHex32(&spawnObjDef.def.id, amount);
            break;

        case SPAWN_MENU_ITEM_MAPID:
            adjustHex8(&spawnMapId, amount);
            break;

        case SPAWN_MENU_ITEM_OBJNO:
            adjustHex16(&spawnObjNo, amount);
            break;

        case SPAWN_MENU_ITEM_XPOS:
            adjustHex32(&spawnCoords[0], amount);
            break;

        case SPAWN_MENU_ITEM_YPOS:
            adjustHex32(&spawnCoords[1], amount);
            break;

        case SPAWN_MENU_ITEM_ZPOS:
            adjustHex32(&spawnCoords[2], amount);
            break;

        default: //params
            adjustHex32(&spawnObjDef.params[
                self->selected - SPAWN_MENU_ITEM_PARAMS], amount);
    }
}

void spawnMenu_run(Menu *self) {
    //Run function for Spawn Object menu
    if(!spawnMenuIsInit) {
        spawnMenuIsInit = true;
        spawnObjDef.def.loadFlags = 0x08;
        spawnObjDef.def.bound     = 0x64;
        spawnObjDef.def.unk7      = 0x96;
        spawnObjDef.def.id        = -1;
        if(pPlayer) {
            spawnCoords[0] = (int)pPlayer->pos.pos.x;
            spawnCoords[1] = (int)pPlayer->pos.pos.y;
            spawnCoords[2] = (int)pPlayer->pos.pos.z;
        }
    }

    int sel = self->selected;
    int selMax = SPAWN_MENU_ITEM_PARAMS + spawnNumparams;
    ObjInstance *obj = loadedObjects[sel];

    if(buttonsJustPressed == PAD_BUTTON_B) { //close menu
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        self->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_A) { //open obj list
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        curMenu = &menuDebugSpawnObjList;
        audioPlaySound(NULL, MENU_OPEN_SOUND);
    }
    else if(buttonsJustPressed == PAD_BUTTON_X
    ||      buttonsJustPressed == PAD_BUTTON_Y) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        doAdjust(self, (buttonsJustPressed == PAD_BUTTON_X) ? 1 : -1);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) { //copy player coords
        if(pPlayer) {
            spawnCoords[0] = (int)pPlayer->pos.pos.x;
            spawnCoords[1] = (int)pPlayer->pos.pos.y;
            spawnCoords[2] = (int)pPlayer->pos.pos.z;
        }
        else audioPlaySound(NULL, MENU_FAIL_SOUND);
    }
    else if(buttonsJustPressed == PAD_BUTTON_MENU) { //do spawn
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        spawnObjDef.def.mapActs1 = spawnActs >> 8;
        spawnObjDef.def.mapActs2 = spawnActs & 0xFF;
        spawnObjDef.def.pos.x    = spawnCoords[0];
        spawnObjDef.def.pos.y    = spawnCoords[1];
        spawnObjDef.def.pos.z    = spawnCoords[2];
        spawnObjDef.def.allocatedSize = spawnNumparams+5;
        ObjInstance* obj = objInstantiateCharacter((ObjDef*)&spawnObjDef,
            spawnFlags, spawnMapId, spawnObjNo, NULL);
        if(obj) {
            OSReport("Spawned obj %08X at %d, %d, %d: %s", obj,
                spawnCoords[0], spawnCoords[1], spawnCoords[2],
                obj->file->name);
            //some objects like to spawn invisible
            objSetModel(obj, 0);
            obj->pos.flags &= ~ObjInstance_Flags06_Invisible;
            obj->flags_0xaf &= ~(ObjInstance_FlagsAF_ModelDisableFlag08 |
                ObjInstance_FlagsAF_ModelDisableFlag20);
        }
        else OSReport("Spawn failed");
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) sel = selMax;
        self->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(sel >= selMax) sel = 0;
        self->selected = sel;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        spawnCursorX = (spawnCursorX - 1) & 7;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        spawnCursorX = (spawnCursorX + 1) & 7;
    }
}

Menu menuDebugSpawnObjList = {
    "Objects", 0,
    spawnList_run, spawnList_draw, spawnList_close,
    NULL,
};

Menu menuDebugSpawnObj = {
    "Spawn Object", 0,
    spawnMenu_run, spawnMenu_draw, debugMapSubMenu_close,
    NULL,
};
