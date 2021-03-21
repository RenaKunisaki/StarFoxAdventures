/** Debug Spawn Object submenu.
 */
#include "main.h"
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
s16 spawnObjNo     = -1;
u16 spawnActs      =  4; //copied to mapActs1 and mapActs2, easier to edit this way
s8  spawnMapId     = -1;
s8  spawnNumparams =  0;
u8  spawnCursorX   =  0;
bool spawnMenuIsInit = false;

/* typedef struct PACKED ObjDef {
	s16   objType;       //0x00 ObjDefEnum
	byte  allocatedSize; //0x02
	byte  mapActs1;      //0x03
	u8    loadFlags;     //0x04 RomListObjLoadFlags
	byte  mapActs2;      //0x05
	byte  bound;         //0x06
	byte  unk7;          //0x07
	vec3f pos;           //0x08
	u32   id;            //0x14 ObjUniqueId
} ObjDef;

typedef enum { //type:u32
	ObjSpawnFlags_KeepLoaded         = 0x1,
	ObjSpawnFlags_DontUseObjIndexBin = 0x2,
	ObjSpawnFlags_DontSave           = 0x4,
} ObjSpawnFlags;
*/

void spawnMenu_draw(Menu *self) {
    //Draw function for Spawn Object menu
    char str[256];
    menuAnimFrame++;

    drawMenuBox(SPAWN_MENU_XPOS, SPAWN_MENU_YPOS, SPAWN_MENU_WIDTH, SPAWN_MENU_HEIGHT);

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
    if(file) {
        int p = 0;
        for(int i=0; i<11; i++) {
            char c = file->name[i];
            if(c == 0) break;
            else if(c >= 0x20 && c <= 0x7E) name[p++] = c;
        }
        name[p] = 0;
    }
    else strcpy(name, "N/A");


    sprintf(str, "Object:      %04X (%04X) %s",
        spawnObjDef.def.objType & 0xFFFF, realId & 0xFFFF, name);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Params:      %X", spawnNumparams);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Spawn Flags: %08X", spawnFlags);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Load  Flags: %02X ", spawnObjDef.def.loadFlags & 0xFF);
    bin2str(&str[strlen(str)], spawnObjDef.def.loadFlags, 8);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Map Acts:    %04X", spawnActs & 0xFFFF);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Bounds:      %02X", spawnObjDef.def.bound & 0xFF);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Unk07:       %02X", spawnObjDef.def.unk7 & 0xFF);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Obj ID:      %08X", spawnObjDef.def.id);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Map ID:      %02X", spawnMapId & 0xFF);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Obj No.:     %04X", spawnObjNo & 0xFFFF);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "X Position:  %08X", spawnCoords[0]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Y Position:  %08X", spawnCoords[1]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    sprintf(str, "Z Position:  %08X", spawnCoords[2]);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    int start = MAX(0, self->selected - (SPAWN_MENU_NUM_LINES-1));
    for(int i=0; i < (SPAWN_MENU_NUM_LINES - SPAWN_MENU_ITEM_PARAMS); i++) {
        int pIdx = i + start;
        if(pIdx >= spawnNumparams) break;
        sprintf(str, "Param %02X:    %08X", pIdx*4, spawnObjDef.params[pIdx]);
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;
    }

    gameTextShowStr("Start:Spawn B:Exit X:+ Y:- Z:Player Coords",
        MENU_TEXTBOX_ID, x, SPAWN_MENU_YPOS + SPAWN_MENU_HEIGHT -
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
    textForceFixedWidth = MENU_FIXED_WIDTH;

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

Menu menuDebugSpawnObj = {
    "Spawn Object", 0,
    spawnMenu_run, spawnMenu_draw, debugMapSubMenu_close,
    NULL,
};
