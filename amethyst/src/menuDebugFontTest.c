/** Debug Font Test submenu.
 */
#include "main.h"
#define FONT_MENU_XPOS 16
#define FONT_MENU_YPOS 16
#define FONT_MENU_WIDTH  (SCREEN_WIDTH  - (FONT_MENU_XPOS * 2))
#define FONT_MENU_HEIGHT (SCREEN_HEIGHT - (FONT_MENU_YPOS * 2))

void fontMenu_draw(Menu *self) {
    //Draw function for font menu
    int LH = (curLanguage == LANG_JAPANESE) ? LINE_HEIGHT_JAPANESE : LINE_HEIGHT;
    drawMenuBox(FONT_MENU_XPOS, FONT_MENU_YPOS, FONT_MENU_WIDTH, FONT_MENU_HEIGHT);
    int x = FONT_MENU_XPOS + MENU_PADDING;
    int y = FONT_MENU_YPOS + MENU_PADDING + (LH * 2);

    //Draw title
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr(T(self->title), 0, x, y-(40+(LH * 2)));

    char str[256];
    sprintf(str, "%d", self->selected);
    drawSimpleText(str, FONT_MENU_XPOS + FONT_MENU_WIDTH - 64, FONT_MENU_YPOS + MENU_PADDING);

    int lineHeight = 0;
    GameTextFont *font = &gameTextFonts[self->selected];
    for(int iChr=0; iChr<font->numChars; iChr++) {
        GameTextCharacterStruct *chr = &font->chars[iChr];

        int x2 = x + chr->width + chr->left + chr->right;
        int y2 = y + chr->height + chr->top + chr->bottom;
        if(x2 >= (FONT_MENU_XPOS + FONT_MENU_WIDTH) - MENU_PADDING) {
            x   = FONT_MENU_XPOS + MENU_PADDING;
            x2  = x + chr->width + chr->left + chr->right;
            y  += lineHeight + 1;
            y2 += lineHeight + 1;
            if(y2 >= FONT_MENU_YPOS + FONT_MENU_HEIGHT) break;
            lineHeight = 0;
        }
        lineHeight = MAX(lineHeight, chr->height + chr->top + chr->bottom);

        //outline the character+padding region
        Color4b colPad = {.r=255, .g=255, .b=0, .a=128};
        hudDrawRect(x, y, x2, y2, &colPad);

        //outline the character region
        Color4b colChar = {.r=255, .g=0, .b=255, .a=128};
        hudDrawRect(x + chr->left, y + chr->top,
            x + chr->width + chr->left, y + chr->height + chr->top, &colChar);

        //draw the character
        drawPartialTexture(x+chr->left, y+chr->top, font->texture[chr->texture], 255, 256,
            chr->width, chr->height, chr->xpos, chr->ypos);

        x = x2 + 1;
        //XXX display the character ID, be able to scroll
        //be able to change language from here?
    }
}

void fontMenu_run(Menu *self) {
    //Run function for font menu
    int sel = curMenu->selected;
    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) sel = 3;
        curMenu->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(sel > 3) sel = 0;
        curMenu->selected = sel;
    }
}

Menu menuDebugFontTest = {
    "Font Test", 0,
    fontMenu_run, fontMenu_draw, debugMapSubMenu_close,
    NULL,
};
