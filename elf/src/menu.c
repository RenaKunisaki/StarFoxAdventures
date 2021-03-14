#include "main.h"

#define MENU_ANIM_NUM_FRAMES 15
#define MENU_PADDING 8
#define MENU_WIDTH (SCREEN_WIDTH * 0.7)
#define MENU_HEIGHT (SCREEN_HEIGHT * 0.7)
#define MENU_XPOS ((SCREEN_WIDTH/2)  - (MENU_WIDTH  / 2))
#define MENU_YPOS ((SCREEN_HEIGHT/2) - (MENU_HEIGHT / 2))
#define MENU_TEXTBOX_ID 0x93 //same as game uses for HUD
#define MENU_LINE_HEIGHT 18

typedef enum {
    MENU_NOT_OPEN,
    MENU_OPENING,
    MENU_OPEN,
    MENU_CLOSING,
} MenuState;

static u8  menuState = MENU_NOT_OPEN;
static u8  menuAnimFrame = 0;
static u16 menuSelItem = 1;

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

    int x = MENU_XPOS + MENU_PADDING, y = MENU_YPOS + MENU_PADDING;

    //Draw title
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr("Super Cool Menu", 0, x, y-40);

    //Draw items
    //LOL guess what, these don't get put in the GOT because lol!
    //that's probably also why we had to subtract 4 from the offset,
    //because some other shit is getting put there too, immediately
    //before the GOT instead of in it
    //we need to look at .data.rel.local and .fixup too
    //can we hack the link script to put them all in one section?
    static const char *items[] = {
        "The First Item!",
        "A second item",
        "Wow, a third item",
        "and yet more items here",
        NULL,
    };

    for(int i=0; items[i]; i++) {
        y += MENU_LINE_HEIGHT;
        OSReport("Item %d: %08X", i, items[i]);
        if(i == menuSelItem) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else gameTextSetColor(255, 255, 255, 255);
        gameTextShowStr(items[i], MENU_TEXTBOX_ID, x, y);
    }
}

void runMenu() {
    //Main loop callback for the menu
    switch(menuState) {
        case MENU_NOT_OPEN:
            //Hold L+Z and press B to open the menu
            if(controllerStates[0].button & (PAD_TRIGGER_L | PAD_TRIGGER_Z)) {
                if(buttonsJustPressed == PAD_BUTTON_B) {
                    OSReport("Opening menu!");
                    menuState = MENU_OPENING;
                    menuAnimFrame = 0;
                }
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
            if(buttonsJustPressed == PAD_BUTTON_B) {
                OSReport("Closing menu!");
                menuState = MENU_CLOSING;
                menuAnimFrame = MENU_ANIM_NUM_FRAMES;
            }
            break;

        case MENU_CLOSING:
            //if(!menuAnimFrame) audioPlaySound(NULL, 0x3E5);
            drawMenuBox();
            if(--menuAnimFrame == 0) menuState = MENU_NOT_OPEN;
            break;
    }
}
