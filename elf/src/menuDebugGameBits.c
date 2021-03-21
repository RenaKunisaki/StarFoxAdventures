/** Debug GameBits submenu.
 */
#include "main.h"
#define BIT_MENU_XPOS 32
#define BIT_MENU_YPOS 32
#define BIT_MENU_WIDTH  (SCREEN_WIDTH  - (BIT_MENU_XPOS * 2))
#define BIT_MENU_HEIGHT (SCREEN_HEIGHT - (BIT_MENU_YPOS * 2))
#define BIT_MENU_NUM_LINES ((BIT_MENU_HEIGHT / MENU_LINE_HEIGHT) - 3)
static u8 bitMenuCursorX = 7;

static BitTableEntry* getBitTableEntry(int bit) {
    BitTableEntry *bitTable = (BitTableEntry*)dataFileBuffers[FILE_BITTABLE_BIN];
    return &bitTable[bit];
}

void bitMenu_draw(Menu *self) {
    //Draw function for GameBit menu
    menuAnimFrame++;

    drawMenuBox(BIT_MENU_XPOS, BIT_MENU_YPOS, BIT_MENU_WIDTH, BIT_MENU_HEIGHT);
    int x = BIT_MENU_XPOS + MENU_PADDING, y = BIT_MENU_YPOS + MENU_PADDING;

    //Draw title
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr(self->title, 0, x, y-40);

    int start = MAX(0, self->selected - (BIT_MENU_NUM_LINES-1));

    for(int i=0; i < BIT_MENU_NUM_LINES; i++) {
        int bitIdx = i + start;
        if(bitIdx >= NUM_GAMEBITS) break;
        y += MENU_LINE_HEIGHT;

        bool selected = bitIdx == self->selected;
        if(selected) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else gameTextSetColor(255, 255, 255, 255);

        BitTableEntry *entry = getBitTableEntry(bitIdx);
        const char *hint = NULL;
        //this doesn't work, ghidra is being a butt so I can't find why
        /* if(!(entry->flags & GameBitFlags_IsHintText)) {
            int tid = -1, sid = -1;
            ((void (*)(int,int*,int*))0x80015d70)(bitIdx, &sid, &tid);
            if(tid == 0x29) {
                void *r = ((void* (*)(int,int))0x8001a420)(tid,sid);
                OSReport("bit %04X tid %08X %04X sid %08X -> %08X", bitIdx, tid,
                    entry->text, sid, r);
            }
            gametextStruct *text = gameTextGet(entry->text + 0xF4);
            //the text always has 16 bytes worth of control codes at the start.
            //these contain nulls which won't carry through sprintf,
            //so skip over them.
            hint = text ? &text->phrases[0][16] : NULL;
            //DPRINT("Bit 0x%X Text 0x%X: %08X %s", bitIdx,
            //    entry->text + 0xF4, text, hint ? hint : "-");
        } */

        char str[256];
        sprintf(str, "%04X %d %08X %s", bitIdx, (entry->flags >> 6) & 3,
            mainGetBit(bitIdx), hint ? hint : "");
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    }

    //draw instructions
    y += MENU_LINE_HEIGHT;
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr("B:Exit X:+ Y:- L/R:Page", MENU_TEXTBOX_ID, x, y);

    //draw cursor
    drawBox(BIT_MENU_XPOS + 4 + ((bitMenuCursorX + 5) * MENU_FIXED_WIDTH),
        BIT_MENU_YPOS + 5 + (MENU_LINE_HEIGHT * ((self->selected - start) + 1)),
        20, 24, 255, false);
}

void bitMenu_run(Menu *self) {
    //Run function for GameBit menu
    textForceFixedWidth = MENU_FIXED_WIDTH;
    int sel = curMenu->selected;

    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_X
    ||      buttonsJustPressed == PAD_BUTTON_Y) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        u32 val = mainGetBit(self->selected);
        u32 mask = 0xF << ((7-bitMenuCursorX) * 4);
        int adj  = (buttonsJustPressed == PAD_BUTTON_Y) ? -1 : 1;
        adj <<= ((7-bitMenuCursorX) * 4);
        val = (val & ~mask) | ((val + adj) & mask);
        mainSetBits(self->selected, val);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) {

    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) sel = NUM_GAMEBITS;
        curMenu->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(sel >= NUM_GAMEBITS) sel = 0;
        curMenu->selected = sel;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        bitMenuCursorX = (bitMenuCursorX - 1) & 7;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        bitMenuCursorX = (bitMenuCursorX + 1) & 7;
    }
    else if(controllerStates[0].triggerLeft > MENU_TRIGGER_THRESHOLD) { //L
        sel -= 0x100;
        if(sel <= 0) sel = NUM_GAMEBITS;
        curMenu->selected = sel - 1;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
    else if(controllerStates[0].triggerRight > MENU_TRIGGER_THRESHOLD) { //R
        sel += 0x100;
        if(sel >= NUM_GAMEBITS) sel = 0;
        curMenu->selected = sel;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
}

Menu menuDebugGameBits = {
    "GameBits", 0,
    bitMenu_run, bitMenu_draw, debugGameSubMenu_close,
    NULL,
};
