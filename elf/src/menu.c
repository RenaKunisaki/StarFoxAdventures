/** In-game menu engine.
 */
#include "main.h"

Menu *curMenu = NULL;
u8  menuState = MENU_NOT_OPEN;
u8  menuAnimFrame = 0;
u8  menuInputDelayTimer = 0;
u8  menuPrevTimerFlags; //for game timer
u8  menuPrevGameFlags; //MenuGameStateFlags

static void drawMenuBox() {
    //Draw the menu's box
    int cx = SCREEN_WIDTH / 2, cy = SCREEN_HEIGHT / 2;
    if(menuState == MENU_OPEN) {
        float w = MENU_WIDTH  / 2;
        float h = MENU_HEIGHT / 2;
        drawBox(cx - w, cy - h, w*2, h*2, 255);
    }
    else { //animate
        float anim = (float)menuAnimFrame / (float)MENU_ANIM_NUM_FRAMES;
        float w = (MENU_WIDTH /2) * anim;
        float h = (MENU_HEIGHT/2) * anim;
        debugPrintf("Menu C=%d,%d S=%f, %f A=%d (%f)\n",
            cx, cy, w, h, menuAnimFrame, anim);
        drawBox(cx - w, cy - h, w*2, h*2, anim * 255);
    }
}

static void drawMenu() {
    //Draw the menu's text
    menuAnimFrame++;
    debugPrintf("Menu flags %02X delay %d\n", menuPrevGameFlags, menuInputDelayTimer);
    //GameTextDrawFunc prevDrawFunc = menuGameTextDrawFunc;
    //gameTextDrawFunc = menuGameTextDrawFunc;

    int x = MENU_XPOS + MENU_PADDING, y = MENU_YPOS + MENU_PADDING;

    //Draw title
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr(curMenu->title, 0, x, y-40);

    for(int i=0; curMenu->items[i].name; i++) {
        y += MENU_LINE_HEIGHT;
        bool selected = i == curMenu->selected;
        if(selected) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else gameTextSetColor(255, 255, 255, 255);
        curMenu->items[i].draw(&curMenu->items[i], x, y, selected);
    }
    //gameTextDrawFunc = prevDrawFunc;
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
            if(controllerStates[0].button & (PAD_TRIGGER_L | PAD_TRIGGER_Z)) {
                if(buttonsJustPressed == PAD_BUTTON_B) openMainMenu();
            }
            break;

        case MENU_OPENING:
            if(!menuAnimFrame) audioPlaySound(NULL, 0x3E5);
            drawMenuBox();
            if(++menuAnimFrame == MENU_ANIM_NUM_FRAMES) {
                menuState = MENU_OPEN;
                menuAnimFrame = 0;
            }
            break;

        case MENU_OPEN:
            drawMenuBox();
            drawMenu();
            doMenuInputs();
            break;

        case MENU_CLOSING:
            //if(!menuAnimFrame) audioPlaySound(NULL, 0x3E5);
            drawMenuBox();
            if(--menuAnimFrame == 0) menuState = MENU_NOT_OPEN;
            break;
    }
}
