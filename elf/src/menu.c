/** In-game menu engine.
 */
#include "main.h"

Menu *curMenu = NULL;
u8  menuState = MENU_NOT_OPEN;
u8  menuAnimFrame = 0;
u8  menuInputDelayTimer = 0;
u8  menuPrevTimerFlags; //for game timer
u8  menuPrevGameFlags; //MenuGameStateFlags

void drawMenuBox(int xpos, int ypos, int width, int height) {
    //Draw the menu's box
    if(menuState == MENU_OPEN) {
        drawBox(xpos, ypos, width, height, 255);
    }
    else { //animate
        float anim = (float)menuAnimFrame / (float)MENU_ANIM_NUM_FRAMES;
        float cx   = xpos + (width  / 2.0);
        float cy   = ypos + (height / 2.0);
        float w    = (width /2.0) * anim;
        float h    = (height/2.0) * anim;
        drawBox(cx-w, cy-h, width * anim, height * anim, anim * 255);
    }
}

void genericMenu_draw(const Menu *self) {
    //Draw function for most menus
    menuAnimFrame++;
    //debugPrintf("Menu flags %02X delay %d\n", menuPrevGameFlags, menuInputDelayTimer);
    //GameTextDrawFunc prevDrawFunc = menuGameTextDrawFunc;
    //gameTextDrawFunc = menuGameTextDrawFunc;

    int x = MENU_XPOS + MENU_PADDING, y = MENU_YPOS + MENU_PADDING;

    drawMenuBox(MENU_XPOS, MENU_YPOS, MENU_WIDTH, MENU_HEIGHT);

    //Draw title
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr(self->title, 0, x, y-40);

    for(int i=0; self->items[i].name; i++) {
        y += MENU_LINE_HEIGHT;
        bool selected = i == self->selected;
        if(selected) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else gameTextSetColor(255, 255, 255, 255);
        self->items[i].draw(&self->items[i], x, y, selected);
    }
    //gameTextDrawFunc = prevDrawFunc;
}

void genericMenu_run(const Menu *self) {
    //Run function for most menus
    int sel = curMenu->selected;
    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_A) {
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        curMenu->items[sel].select(&curMenu->items[sel], 0);
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) {
            while(curMenu->items[sel].name) sel++;
        }
        curMenu->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(!curMenu->items[sel].name) sel = 0;
        curMenu->selected = sel;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
            ? MENU_INPUT_DELAY_ADJUST : MENU_INPUT_DELAY_ADJUST_FAST;
        curMenu->items[sel].select(&curMenu->items[sel], 1);
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
            ? MENU_INPUT_DELAY_ADJUST : MENU_INPUT_DELAY_ADJUST_FAST;
        curMenu->items[sel].select(&curMenu->items[sel], -1);
    }
    /* else if(controllerStates[0].triggerLeft > 40) { //L
        menuInputDelayTimer = 0;
        curMenu->items[sel].select(&curMenu->items[sel], -1);
    }
    else if(controllerStates[0].triggerRight > 40) { //R
        menuInputDelayTimer = 0;
        curMenu->items[sel].select(&curMenu->items[sel], 1);
    } */
}

void genericMenuItem_draw(const MenuItem *self, int x, int y, bool selected) {
    //Draw function for menu items that are only text
    gameTextShowStr(self->name, MENU_TEXTBOX_ID, x, y);
}


static void drawMenu() {
    curMenu->draw(curMenu);
}

static void doMenuInputs() {
    //Handle the controller while the menu is open
    if(menuInputDelayTimer) { //don't react to input too quickly
        menuInputDelayTimer--;
        return;
    }
    curMenu->run(curMenu);
}

void openMainMenu() {
    menuState = MENU_OPENING;
    menuAnimFrame = 0;
    curMenu = &menuMain;
    menuInputDelayTimer = 0;
    menuPrevGameFlags =
        (timeStop ? MENU_FLAG_TIME_STOPPED : 0) |
        (pauseDisabled ? MENU_FLAG_PAUSE_DISABLED : 0);
    timeStop = 1;
    pauseDisabled = 1;
    //inhibit game timer
    menuPrevTimerFlags = activeTimerFlags;
    activeTimerFlags = 0;
}

void runMenu() {
    //Main loop callback for the menu
    switch(menuState) {
        case MENU_NOT_OPEN:
            //Hold L+Z and press B to open the menu
            if((controllerStates[0].button & (PAD_TRIGGER_L | PAD_TRIGGER_Z))
            == (PAD_TRIGGER_L | PAD_TRIGGER_Z)) {
                if(buttonsJustPressed == PAD_BUTTON_B) openMainMenu();
            }
            break;

        case MENU_OPENING:
            if(!menuAnimFrame) audioPlaySound(NULL, 0x3E5);
            drawMenuBox(MENU_XPOS, MENU_YPOS, MENU_WIDTH, MENU_HEIGHT);
            if(++menuAnimFrame == MENU_ANIM_NUM_FRAMES) {
                menuState = MENU_OPEN;
                menuAnimFrame = 0;
            }
            shouldCloseCMenu = 1;
            break;

        case MENU_OPEN:
            drawMenu();
            doMenuInputs();
            shouldCloseCMenu = 1;
            break;

        case MENU_CLOSING:
            //if(!menuAnimFrame) audioPlaySound(NULL, 0x3E5);
            drawMenuBox(MENU_XPOS, MENU_YPOS, MENU_WIDTH, MENU_HEIGHT);
            if(--menuAnimFrame == 0) menuState = MENU_NOT_OPEN;
            break;
    }
}
