/** Debug Font Test submenu.
 */
#include "main.h"
#include "revolution/pad.h"

#define TEXT_MENU_XPOS 16
#define TEXT_MENU_YPOS (SCREEN_HEIGHT - 64)
#define TEXT_MENU_WIDTH  (SCREEN_WIDTH  - (TEXT_MENU_XPOS * 2))

GameLanguageEnum prevLanguage = -1;
u8 curPhrase = 0;
u8 cursor = 3; //digit (left to right)
bool useWindow = true;

static bool isTextValid(gametextStruct *text) {
    //return text && text->phrases && text->phrases[0] && text->phrases[0][0];
    return text && text->window != 0xFF;
}

void textMenu_draw(Menu *self) {
    //Draw function for text menu
    int LH = ((curLanguage == LANG_JAPANESE) ? LINE_HEIGHT_JAPANESE : LINE_HEIGHT) + 4;
    int FW = ((curLanguage == LANG_JAPANESE) ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH);
    int x = TEXT_MENU_XPOS + MENU_PADDING;
    int y = TEXT_MENU_YPOS + MENU_PADDING;
    float idScale = curLanguage == LANG_JAPANESE ? 0.5 : 0.75;

    drawMenuBox(TEXT_MENU_XPOS, TEXT_MENU_YPOS, TEXT_MENU_WIDTH, LH*3);

    char str[256];
    gametextStruct *text = gameTextGet(self->selected);
    if(isTextValid(text)) {
        gameTextSetColor(255, 255, 255, 255);
        sprintf(str, "Txt:\eF%04X (%5d)\eF Phr:\eF%2d/%2d\eF Win:%02X (%s) Lng:%d %s Alg:%d %d (%s)",
            self->selected & 0xFFFF, self->selected, curPhrase+1, text->numPhrases,
            text->window, useWindow ? "On" : "Off", text->language,
            text->language < NUM_LANGUAGES ? languageNamesShort[text->language] : "??",
            text->alignH, text->alignV,
            languageNamesShort[curLanguage]);

        const char *phrase = NULL;
        if(curPhrase < text->numPhrases) phrase = text->phrases[curPhrase];
        if(PTR_VALID(phrase)) {
            if(useWindow) gameTextShowStr(phrase, text->window, 0, 0);
            else {
                int tx=x, ty=32+LH, rx=0, ry=0;
                Color4b color = {0xFF, 0xFF, 0xFF, 0xFF};
                drawText(phrase, tx, ty, &rx, &ry, TEXT_COLORED | TEXT_SHADOW, color, 1.0f);
                drawMenuBox(tx-4, ty-4, (rx-tx)+8, (ry-ty)+LH+8);
            }
        }
        else drawSimpleText("(no text)", x+100, 32+LH);
    }
    else sprintf(str, "Txt:\eF%04X (%5d)\eF (not found) (%s)", self->selected & 0xFFFF,
        self->selected, languageNamesShort[curLanguage]);
    sprintf(str, "%s\nX:+ Y:- L:Prev R:Next S:Window Z:Lang", str); //we don't have strcat
    drawSimpleText(str, x, y);

    //draw cursor
    drawMenuBox(x + (FW * 3) + (cursor * FW), y, FW*2, LH);
}

void textMenu_run(Menu *self) {
    //Run function for text menu
    if(prevLanguage == -1) prevLanguage = curLanguage;

    int sel = curMenu->selected;
    if(buttonsJustPressed == PAD_BUTTON_B) { //close menu
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
        setGameLanguage(prevLanguage);
    }
    else if(buttonsJustPressed == PAD_TRIGGER_Z) { //switch language
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        curLanguage++;
        if(curLanguage >= NUM_LANGUAGES) curLanguage = 0;
        setGameLanguage(curLanguage);
    }
    //adjust selected digit
    else if(buttonsJustPressed == PAD_BUTTON_X) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        curMenu->selected += 1 << ((3-cursor) * 4);
    }
    else if(buttonsJustPressed == PAD_BUTTON_Y) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        curMenu->selected -= 1 << ((3-cursor) * 4);
    }
    //skip to prev/next existing text
    else if(buttonsJustPressed == PAD_TRIGGER_L) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        if(self->selected == 0) self->selected = MAX_GAMETEXT_ID;
        while(self->selected > 0) {
            self->selected--;
            if(isTextValid(gameTextGet(self->selected))) break;
        }
    }
    else if(buttonsJustPressed == PAD_TRIGGER_R) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        if(self->selected >= MAX_GAMETEXT_ID) self->selected = 0;
        while(self->selected < MAX_GAMETEXT_ID) {
            self->selected++;
            if(isTextValid(gameTextGet(self->selected))) break;
        }
    }
    else if(buttonsJustPressed == PAD_BUTTON_MENU) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        useWindow = !useWindow;
    }

    //move cursor to select text ID
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer =
            (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        cursor++;
        if(cursor > 3) cursor = 0;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        cursor--;
        if(cursor > 3) cursor = 3;
    }

    //select phrase
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        curPhrase--;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        curPhrase++;
    }
}

Menu menuDebugViewText = {
    "View Texts", 0,
    textMenu_run, textMenu_draw, debugRenderSubMenu_close,
    NULL,
};
