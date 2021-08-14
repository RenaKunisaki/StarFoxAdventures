/** Debug Font Test submenu.
 */
#include "main.h"
#include "revolution/pad.h"
#define FONT_MENU_XPOS 16
#define FONT_MENU_YPOS 16
#define FONT_MENU_WIDTH  (SCREEN_WIDTH  - (FONT_MENU_XPOS * 2))
#define FONT_MENU_HEIGHT (SCREEN_HEIGHT - (FONT_MENU_YPOS * 2))

s16 lineNo = 0;

void fontMenu_draw(Menu *self) {
    //Draw function for font menu
    int LH = ((curLanguage == LANG_JAPANESE) ? LINE_HEIGHT_JAPANESE : LINE_HEIGHT) + 4;
    int FW = ((curLanguage == LANG_JAPANESE) ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH);
    drawMenuBox(FONT_MENU_XPOS, FONT_MENU_YPOS, FONT_MENU_WIDTH, FONT_MENU_HEIGHT);
    int x = FONT_MENU_XPOS + MENU_PADDING;
    int y = FONT_MENU_YPOS + MENU_PADDING + (LH * 2);
    float idScale = curLanguage == LANG_JAPANESE ? 0.5 : 0.75;

    char str[256];
    sprintf(str, "%s %d", languageNames[curLanguage], self->selected);
    drawSimpleText(str, x, y - (LH * 2));

    int lineCount = lineNo;
    int lineHeight = 0;
    GameTextFont *font = &gameTextFonts[self->selected];
    for(int iChr=0; iChr<font->numChars; iChr++) {
        GameTextCharacterStruct *chr = &font->chars[iChr];

        int cw = chr->width;
        int ch = chr->height;
        int tw = chr->width + chr->left + chr->right; //total width
        int th = chr->height + chr->top + chr->bottom; //total height
        int x2 = x + tw;
        int y2 = y + th;

        //measure the ID string
        Color4b col = {.value=0xFFFFFFFF};
        int cx = 0, cy = 0;
        sprintf(str, "%X\v", chr->character);
        drawText(str, x, y, &cx, &cy, TEXT_FIXED | TEXT_MEASURE, col, idScale);

        int rw = MAX(tw, (cx - x) + 4); //render width

        if((x + rw) >= (FONT_MENU_XPOS + FONT_MENU_WIDTH) - MENU_PADDING) {
            //this character won't fit here. move to next line.
            x   = FONT_MENU_XPOS + MENU_PADDING;
            x2  = x + tw;
            if(lineCount) lineCount--;
            else {
                y  += lineHeight + LH + 1;
                y2 += lineHeight + LH + 1;
                cy += lineHeight + LH + 1;
                if(y2 >= FONT_MENU_YPOS + FONT_MENU_HEIGHT) break; //can't fit any more on screen
            }
            lineHeight = 0;
        }
        lineHeight = MAX(lineHeight, th);

        if(!lineCount) {
            //draw the ID
            drawText(str, x, y, NULL, NULL, TEXT_FIXED, col, idScale);

            //outline the padding region
            Color4b colPad = {.r=255, .g=255, .b=0, .a=128};
            hudDrawRect(x, cy, x2, cy+th, &colPad);

            //outline the character region
            Color4b colChar = {.r=255, .g=0, .b=255, .a=128};
            hudDrawRect(x + chr->left, cy + chr->top,
                x + cw + chr->left, cy + ch + chr->top, &colChar);

            //draw the character
            drawPartialTexture(x + chr->left, cy + chr->top,
                font->texture[chr->texture], 255, 256,
                cw, ch, chr->xpos, chr->ypos);
        }
        x += rw+1;
    }
}

void fontMenu_run(Menu *self) {
    //Run function for font menu
    int sel = curMenu->selected;
    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        curLanguage++;
        if(curLanguage >= NUM_LANGUAGES) curLanguage = 0;
        setGameLanguage(curLanguage);
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        lineNo--;
        if(lineNo < 0) lineNo = 0;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        lineNo++;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer =
            (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(sel > 3) sel = 0;
        curMenu->selected = sel;
        lineNo = 0;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) sel = 3;
        curMenu->selected = sel - 1;
        lineNo = 0;
    }
}

Menu menuDebugFontTest = {
    "Font Test", 0,
    fontMenu_run, fontMenu_draw, debugMapSubMenu_close,
    NULL,
};
