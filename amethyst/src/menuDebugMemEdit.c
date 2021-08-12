/** Edit Memory submenu.
 */
#include "main.h"
#define HEXEDIT_XPOS  30
#define HEXEDIT_YPOS 100
#define HEXEDIT_WIDTH  560
#define HEXEDIT_HEIGHT 328
#define HEXEDIT_NUM_LINES ((HEXEDIT_HEIGHT / MENU_LINE_HEIGHT) - 3)
#define HEXEDIT_NUM_COLS 8
static u8 cursorX = 0;
u32 hexEditAddr = RAM_START;
u32 hexEditPrevAddr = RAM_START;
Menu *hexEditPrevMenu = NULL;

void hexEdit_close(const Menu *self) {
    //Close function for memory editor
    if(hexEditPrevMenu) {
        curMenu = hexEditPrevMenu;
        hexEditPrevMenu = NULL;
    }
    else {
        OSReport("hexEditPrevMenu is NULL!");
        curMenu = &menuDebugMisc;
    }
    audioPlaySound(NULL, MENU_CLOSE_SOUND);
}

void hexEdit_draw(Menu *self) {
    //Draw function for memory editor
    char str[256];

    drawMenuBox(HEXEDIT_XPOS, HEXEDIT_YPOS, HEXEDIT_WIDTH, HEXEDIT_HEIGHT);

    int x = HEXEDIT_XPOS + MENU_PADDING, y = HEXEDIT_YPOS + MENU_PADDING;
    sprintf(str, "\eFAddr %08X", hexEditAddr);
    drawSimpleText(str, x, y);

    if(!PTR_VALID(hexEditAddr)) hexEditAddr = RAM_START;

    u8 *addr = (u8*)hexEditAddr;
    for(int i=0; i < HEXEDIT_NUM_LINES; i++) {
        y += MENU_LINE_HEIGHT;
        sprintf(str, "\eF%04X ", addr);

        for(int j=0; j < HEXEDIT_NUM_COLS; j++) {
            sprintf(&str[strlen(str)], "%02X ", addr[j]);
        }
        int idx = strlen(str);
        for(int j=0; j < HEXEDIT_NUM_COLS; j++) {
            u8 val = addr[j];
            str[idx+j] = (val >= 0x20 && val <= 0x7E) ? val : '.';
            str[idx+j+1] = 0;
        }

        drawSimpleText(str, x, y);
        addr += HEXEDIT_NUM_COLS;
    }

    //draw instructions
    y += MENU_LINE_HEIGHT;
    drawSimpleText("J:Move C:Fast X:+ Y:- S:FollowPtr Z:Back B:Exit", x, y);

    //draw cursor
    if(self->selected == 0) {
        x = HEXEDIT_XPOS + ((cursorX & 7) * MENU_FIXED_WIDTH) +
            (MENU_FIXED_WIDTH * 5) + 4;
    }
    else {
        x = HEXEDIT_XPOS + (cursorX * MENU_FIXED_WIDTH) +
            ((cursorX / 2) * MENU_FIXED_WIDTH) + (MENU_FIXED_WIDTH * 9) + 4;
    }
    y = HEXEDIT_YPOS + 5 + (MENU_LINE_HEIGHT * self->selected);
    drawBox(x, y, 20, 24, 255, false);
}

void hexEdit_run(Menu *self) {
    //Run function for memory editor
    int sel = curMenu->selected;
    u32 addr = hexEditAddr + (MAX(0, (self->selected-1)) * HEXEDIT_NUM_COLS) +
        (cursorX / 2);

    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_X
    ||      buttonsJustPressed == PAD_BUTTON_Y) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        if(self->selected == 0) { //edit address
            addr = hexEditAddr;
            u32 mask = 0xF << ((7-(cursorX & 7)) * 4);
            int adj  = (buttonsJustPressed == PAD_BUTTON_Y) ? -1 : 1;
            adj <<= ((7-(cursorX & 7)) * 4);
            addr = (addr & ~mask) | ((addr + adj) & mask);
            hexEditAddr = addr;
        }
        else { //edit data
            u8  val = *(u8*)addr;
            u8  mask = (cursorX & 1) ? 0x0F : 0xF0;
            int adj  = (buttonsJustPressed == PAD_BUTTON_Y) ? -1 : 1;
            adj <<= ((cursorX & 1) ? 0 : 4);
            val = (val & ~mask) | ((val + adj) & mask);
            *(u8*)addr = val;
        }
    }
    else if(buttonsJustPressed == PAD_BUTTON_MENU) {
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        u32 val = *(u32*)(addr & ~3);
        if(PTR_VALID(val)) {
            hexEditPrevAddr = hexEditAddr;
            hexEditAddr = val;
        }
        else audioPlaySound(NULL, MENU_FAIL_SOUND);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) {
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        hexEditAddr = hexEditPrevAddr;
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) hexEditAddr -= HEXEDIT_NUM_COLS;
        else sel--;
        curMenu->selected = sel;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel+1 >= HEXEDIT_NUM_LINES+1) hexEditAddr += HEXEDIT_NUM_COLS;
        else sel++;
        curMenu->selected = sel;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(cursorX > 0) cursorX--;
        else cursorX = (HEXEDIT_NUM_COLS * 2) - 1;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        cursorX++;
        if(cursorX >= (HEXEDIT_NUM_COLS * 2)) cursorX = 0;
    }
    /* else if(controllerStates[0].triggerLeft > MENU_TRIGGER_THRESHOLD) { //L
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
    } */
}

Menu menuDebugHexEdit = {
    "Edit Memory", 0,
    hexEdit_run, hexEdit_draw, hexEdit_close,
    NULL,
};
