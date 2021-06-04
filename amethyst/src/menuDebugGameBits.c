/** Debug GameBits submenu.
 */
#include "main.h"
#define BIT_MENU_XPOS 32
#define BIT_MENU_YPOS 32
#define BIT_MENU_WIDTH  (SCREEN_WIDTH  - (BIT_MENU_XPOS * 2))
#define BIT_MENU_HEIGHT (SCREEN_HEIGHT - (BIT_MENU_YPOS * 2))
#define BIT_MENU_NUM_LINES ((BIT_MENU_HEIGHT / MENU_LINE_HEIGHT) - 3)
s8 *bitNames = NULL;
static u8 bitMenuCursorX = 7;
static s8 bitMenuTable = -1;

BitTableEntry* getBitTableEntry(int bit) {
    BitTableEntry *bitTable = (BitTableEntry*)dataFileBuffers[FILE_BITTABLE_BIN];
    return &bitTable[bit];
}

const char* getBitName(int bit) {
    if(!bitNames) {
        //try to load bitnames.dat
        bitNames = loadFileByPath("bitnames.dat", NULL);
        if(!bitNames) return "";
    }

    //this is hilariously inefficient time-wise, but saves a ton of memory.
    const char *res = bitNames;
    while(bit > 0) {
        if(res[0] == 0) bit--;
        res++;
    }
    return res;
}

void bitMenu_draw(Menu *self) {
    //Draw function for GameBit menu
    menuAnimFrame++;

    drawMenuBox(BIT_MENU_XPOS, BIT_MENU_YPOS, BIT_MENU_WIDTH, BIT_MENU_HEIGHT);
    int x = BIT_MENU_XPOS + MENU_PADDING, y = BIT_MENU_YPOS + MENU_PADDING;

    //Draw title and header
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr(self->title, 0, x, y-40);
    gameTextShowStr("Bit  T Sz Value", MENU_TEXTBOX_ID, x, y);

    int start  = MAX(0, self->selected - (BIT_MENU_NUM_LINES-1));
    int bitIdx = start - 1;
    int nLines = 0;
    int cursorY = 0;
    while(nLines < BIT_MENU_NUM_LINES && bitIdx < NUM_GAMEBITS) {
        bitIdx++;
        BitTableEntry *entry = getBitTableEntry(bitIdx);
        int tbl = (entry->flags >> 6) & 3;
        if(bitMenuTable >= 0 && bitMenuTable != tbl) continue;
        y += MENU_LINE_HEIGHT;
        nLines++;
        //debugPrintf(DPRINT_FIXED "Bit %04X t %d sz %2d offs %04X\n" DPRINT_NOFIXED,
        //    bitIdx, tbl, entry->flags & GameBitFlags_Size, entry->offset);

        bool selected = bitIdx == self->selected;
        if(selected) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
            cursorY = y;
        }
        else gameTextSetColor(255, 255, 255, 255);

        char str[256];
        sprintf(str, "%04X %d %2d %08X %s", bitIdx, tbl,
            (entry->flags & GameBitFlags_Size) + 1,
            mainGetBit(bitIdx), getBitName(bitIdx));
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    }

    //draw instructions
    y += MENU_LINE_HEIGHT;
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr("B:Exit X:+ Y:- L/R:Page Z:Table", MENU_TEXTBOX_ID, x, y);

    //draw cursor
    drawBox(BIT_MENU_XPOS + 4 + ((bitMenuCursorX + 10) * MENU_FIXED_WIDTH),
        cursorY - 4, 20, 24, 255, false);
}

static void moveCursorY(Menu *self, int amount) {
    int sel = self->selected + amount;

    //skip past bits in a hidden table
    amount = (amount < 0) ? -1 : 1;
    while(true) {
        if(sel < 0) sel = NUM_GAMEBITS - 1;
        if(sel >= NUM_GAMEBITS) sel = 0;

        BitTableEntry *entry = getBitTableEntry(sel);
        int tbl = (entry->flags >> 6) & 3;
        if(bitMenuTable == tbl || bitMenuTable < 0) break;
        sel += amount;
    }
    self->selected = sel;
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
        bitMenuTable++;
        if(bitMenuTable > 3) bitMenuTable = -1;
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        moveCursorY(self, 0); //move if on a now-hidden bit
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        moveCursorY(self, -1);
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        moveCursorY(self, 1);
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
        moveCursorY(self, -0x100);
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
    else if(controllerStates[0].triggerRight > MENU_TRIGGER_THRESHOLD) { //R
        moveCursorY(self, 0x100);
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
}

Menu menuDebugGameBits = {
    "GameBits", 0,
    bitMenu_run, bitMenu_draw, debugGameSubMenu_close,
    NULL,
};
