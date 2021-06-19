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
        drawBox(xpos, ypos, width, height, 255, true);
    }
    else { //animate
        float anim = (float)menuAnimFrame / (float)MENU_ANIM_NUM_FRAMES;
        float cx   = xpos + (width  / 2.0);
        float cy   = ypos + (height / 2.0);
        float w    = (width /2.0) * anim;
        float h    = (height/2.0) * anim;
        drawBox(cx-w, cy-h, width * anim, height * anim, anim * 255, true);
    }
}

void genericMenu_drawAt(Menu *self, int xpos, int ypos, int width, int height) {
    menuAnimFrame++;

    //debugPrintf("Menu flags %02X delay %d\n", menuPrevGameFlags, menuInputDelayTimer);
    //GameTextDrawFunc prevDrawFunc = menuGameTextDrawFunc;
    //gameTextDrawFunc = menuGameTextDrawFunc;

    drawMenuBox(xpos, ypos, width, height);

    //Draw title
    //box type 0 is (center, y+40), no background
    if(self->title && self->title[0]) {
        gameTextSetColor(255, 255, 255, 255);
        gameTextShowStr(self->title, 0, xpos, ypos+MENU_PADDING-40);
    }

    //find Y position of current item
    int iStart = 0;
    int ySel = MENU_LINE_HEIGHT * (self->selected+2) + MENU_PADDING;
    //debugPrintf("ySel=%d/%d ", ySel, (int)height);
    while(ySel >= (int)(height-MENU_PADDING)) {
        ySel -= MENU_LINE_HEIGHT;
        iStart++;
    }
    //debugPrintf("iStart=%d\n", iStart);

    //draw items
    int y = ypos;
    bool drawBottomArrow = false;
    for(int i=iStart; self->items[i].name; i++) {
        y += MENU_LINE_HEIGHT;
        bool selected = i == self->selected;
        if(y + MENU_LINE_HEIGHT >= ypos + (height-(MENU_PADDING*2))) {
            drawBottomArrow = true;
            break;
        }
        if(selected) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else gameTextSetColor(255, 255, 255, 255);
        self->items[i].draw(&self->items[i],
            xpos+MENU_PADDING, y+MENU_PADDING, selected);
    }
    //gameTextDrawFunc = prevDrawFunc;

    //draw arrows if there are more items.
    //this is actually the location indicator for the world map,
    //but it's a vertical arrow that's always loaded.
    Texture *arrow = getLoadedTexture(0xBE8);
    if(drawBottomArrow) {
        drawScaledTexture(
            (float)(xpos + width - (MENU_PADDING * 3)),
            (float)(ypos + height - (MENU_PADDING*1)),
            arrow, 255, 256, 16, 12, DRAW_SCALED_TEXTURE_FLIP_V);
    }
    if(iStart > 0) { //draw top arrow
        drawScaledTexture(
            (float)(xpos + width - (MENU_PADDING * 3)),
            (float)(ypos - 4),
            arrow, 255, 256, 16, 12, 0);
    }
}

void genericMenu_draw(Menu *self) {
    //Draw function for most menus
    genericMenu_drawAt(self, MENU_XPOS, MENU_YPOS,
        MENU_WIDTH, MENU_HEIGHT);
}

void genericMenu_run(Menu *self) {
    //Run function for most menus
    int sel = self->selected;
    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        self->close(self);
        textForceFixedWidth = 0;
    }
    else if(buttonsJustPressed == PAD_BUTTON_A) {
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        self->items[sel].select(&self->items[sel], 0);
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) {
            while(self->items[sel].name) sel++;
        }
        self->selected = sel - 1;
        audioPlaySound(NULL, MENU_MOVE_SOUND);
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(!self->items[sel].name) sel = 0;
        self->selected = sel;
        audioPlaySound(NULL, MENU_MOVE_SOUND);
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
            ? MENU_INPUT_DELAY_ADJUST : MENU_INPUT_DELAY_ADJUST_FAST;
        self->items[sel].select(&self->items[sel], 1);
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
            ? MENU_INPUT_DELAY_ADJUST : MENU_INPUT_DELAY_ADJUST_FAST;
        self->items[sel].select(&self->items[sel], -1);
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
    //textForceFixedWidth = 0;
    //ensure this remains set after a warp
    timeStop = 1;
    pauseDisabled = 1;
    activeTimerFlags = 0;
    /* if(!curMenu) {
        OSReport("drawMenu: no menu!");
        (*(u32*)0) = 0;
    }
    OSReport("Draw menu %08X", curMenu); */
    curMenu->draw(curMenu);
}

static void doMenuInputs() {
    //Handle the controller while the menu is open
    if(menuInputDelayTimer) { //don't react to input too quickly
        menuInputDelayTimer--;
        return;
    }
    /* if(!curMenu) {
        OSReport("doMenuInputs: no menu!");
        (*(u32*)0) = 0;
    }
    OSReport("Run menu %08X", curMenu); */
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
                //DPRINT("Menu finished opening");
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
            textForceFixedWidth = 0;
            break;
    }
}
