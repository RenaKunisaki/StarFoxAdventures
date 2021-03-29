/** Debug Texture Viewer submenu.
 */
#include "main.h"
#define TEXTURE_MENU_XPOS      15
#define TEXTURE_MENU_YPOS      40
#define TEXTURE_MENU_WIDTH    620
#define TEXTURE_MENU_HEIGHT    60
#define TEXTURE_IMAGE_XPOS     15
#define TEXTURE_IMAGE_YPOS    105
#define TEXTURE_IMAGE_PADDING   7

static s8 textureMenuFrame = 0;
static bool textureMenuAnimate = false;
static u8 textureMenuAnimTimer = 0;

void textureMenu_draw(Menu *self) {
    //Draw function for Texture Viewer menu
    menuAnimFrame++;
    char str[256];

    drawMenuBox(TEXTURE_MENU_XPOS, TEXTURE_MENU_YPOS,
        TEXTURE_MENU_WIDTH, TEXTURE_MENU_HEIGHT);
    gameTextSetColor(0xFF,0xFF,0xFF,0xFF);

    int x = TEXTURE_MENU_XPOS + MENU_PADDING;
    int y = TEXTURE_MENU_YPOS + MENU_PADDING;

    if(self->selected >= numLoadedTextures) {
        self->selected = numLoadedTextures - 1;
    }
    LoadedTexture *ltex = &loadedTextures[self->selected];
    Texture *tex = ltex ? ltex->texture : NULL;

    int id = ltex->id;
    sprintf(str, "Tex %04X.%02X: ID %c%04X Unk %02X @%08X",
        self->selected, textureMenuFrame, id >= 0 ? ' ' : '-',
        ABS(id), ltex->unk08, tex);
    gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
    y += MENU_LINE_HEIGHT;

    if(tex) {
        sprintf(str, "Flags: %08X RefCnt: %3d Fmt: %02X; %d x %d",
            tex->flags, tex->refCount, tex->format, tex->width, tex->height);
        gameTextShowStr(str, MENU_TEXTBOX_ID, x, y);
        y += MENU_LINE_HEIGHT;

        //get frame
        if(textureMenuAnimate) {
            if(textureMenuAnimTimer < 10) textureMenuAnimTimer++;
            else {
                textureMenuAnimTimer = 0;
                textureMenuFrame++;
            }
        }

        int frm = textureMenuFrame;
        Texture *pFrm = tex;
        while(pFrm && frm) {
            pFrm = pFrm->next;
            frm--;
        }
        if(!pFrm) { //passed last frame
            pFrm = tex;
            textureMenuFrame = 0;
        }
        tex = pFrm;

        if(tex) {
            drawMenuBox(TEXTURE_IMAGE_XPOS, TEXTURE_IMAGE_YPOS,
                tex->width  + (TEXTURE_IMAGE_PADDING*2),
                tex->height + (TEXTURE_IMAGE_PADDING*2));
            drawTexture(TEXTURE_IMAGE_XPOS + TEXTURE_IMAGE_PADDING,
                TEXTURE_IMAGE_YPOS + TEXTURE_IMAGE_PADDING, tex, 255, 0x100);
        }
    }
}

void textureMenu_run(Menu *self) {
    //Run function for Texture Viewer menu
    textForceFixedWidth = MENU_FIXED_WIDTH;
    int sel = self->selected;
    LoadedTexture *ltex = &loadedTextures[sel];

    if(buttonsJustPressed == PAD_BUTTON_B) { //close menu
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        self->close(self);
    }
    else if(buttonsJustPressed == PAD_BUTTON_X) { //next frame
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        textureMenuFrame++;
    }
    else if(buttonsJustPressed == PAD_BUTTON_Y) { //prev frame
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        textureMenuFrame--;
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) { //toggle animation
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        textureMenuAnimate = !textureMenuAnimate;
        textureMenuAnimTimer = 0;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer =
            (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) sel = numLoadedTextures;
        self->selected = sel - 1;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(sel >= numLoadedTextures) sel = 0;
        self->selected = sel;
    }
    else if(controllerStates[0].triggerLeft > MENU_TRIGGER_THRESHOLD) { //L
        sel -= 0x10;
        if(sel <= 0) sel = numLoadedTextures;
        self->selected = sel - 1;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }
    else if(controllerStates[0].triggerRight > MENU_TRIGGER_THRESHOLD) { //R
        sel += 0x10;
        if(sel >= numLoadedTextures) sel = 0;
        self->selected = sel;
        menuInputDelayTimer = MENU_INPUT_DELAY_MOVE;
    }

    if(self->selected >= numLoadedTextures) self->selected = numLoadedTextures - 1;
}

Menu menuDebugTextureView = {
    "View Textures", 0,
    textureMenu_run, textureMenu_draw, debugRenderSubMenu_close,
    NULL,
};
