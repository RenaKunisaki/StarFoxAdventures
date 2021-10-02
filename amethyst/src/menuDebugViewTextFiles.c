/** Debug Font Test submenu.
 */
#include "main.h"
#include "revolution/os.h"
#include "revolution/pad.h"
#include <stdio.h>

#define TEXTFILE_MENU_XPOS 16
#define TEXTFILE_MENU_YPOS 48
#define TEXTFILE_MENU_WIDTH SCREEN_WIDTH - (TEXTFILE_MENU_XPOS * 2)
#define TEXTFILE_MENU_HEIGHT SCREEN_HEIGHT - (TEXTFILE_MENU_YPOS + 16)

extern GameLanguageEnum prevLanguage;
static s16 curTextIdx = 0;

static const char *stateNames[] = {
    "Empty",      //0x0
    "Loading",    //0x1
    "TexGen",     //0x2
    "Ready",      //0x3
    "Cancelling", //0x4
    "Cancelled",  //0x5
    "NeedFree",   //0x6
};

static void _drawSlot(Menu *self) {
    bool JP = curLanguage == LANG_JAPANESE;
    int LH = (JP ? LINE_HEIGHT_JAPANESE : LINE_HEIGHT) + 3;
    int FW = (JP ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH);
    int x = TEXTFILE_MENU_XPOS + MENU_PADDING;
    int y = TEXTFILE_MENU_YPOS + MENU_PADDING;
    Color4b color = {255, 255, 255, 255};
    float scale = JP ? 0.7 : 1.0;
    u32 flags = TEXT_FIXED | TEXT_SHADOW;

    char str[1024];
    GameTextFileData *file = &curGameTexts[self->selected-1];
    sprintf(str,
        "Slot \eF%d\eF: \eF%d %s\eF, font \eF%d\eF: %02X %s\eF\n\eF"
        "File ID \eF%02X\eF @ \eF%08X\eF size \eF%06X\n\eF"
        "Pad\t\t\eF%d, %d\n\eF"
        "ChrSize\t\eF%d, %d\n\eF"
        "Font\t\t\eF%02X\n\eF"
        "Texture\t\eF%02X\n\eF"
        "State\t%02X %s\eF\n\eF"
        "RefCnt\t\eF%d",
        self->selected-1,
        file->lang, file->lang < NUM_LANGUAGES ? languageNamesShort[file->lang] : "?",
        file->fontNo, file->dir, gameTextMapNames[file->dir],
        file->fileIdx, file->textFile, file->fileSize,
        file->leftPad, file->topPad,
        file->charW, file->charH,
        file->font,
        file->textureIdx,
        file->state, (file->state < 7) ? stateNames[file->state] : "?",
        file->refCount);
    drawText(str, x, y, NULL, NULL, flags, color, scale);
    x += JP ? 330 : 300;

    //hex dump
    sprintf(str, "%08X\n", file);
    //drawSimpleText(str, x, y);
    drawText(str, x, y, NULL, NULL, flags, color, scale);
    y += LH;
    u8 *data = (u8*)file;
    for(int offs=0; offs<sizeof(GameTextFileData); offs += 8) {
        sprintf(str, "%02X:  %02X %02X %02X %02X  %02X %02X %02X %02X\n", offs,
            data[offs+0], data[offs+1], data[offs+2], data[offs+3],
            data[offs+4], data[offs+5], data[offs+6], data[offs+7]);
        drawText(str, x, y, NULL, NULL, flags, color, scale);
        y += LH-1;
    }

    //draw selected text
    x -= JP ? 330 : 300;
    y -= LH * 2;
    if(PTR_VALID(file) && PTR_VALID((file->textFile))) {
        u32 numCharStructs = *(u32*)file->textFile;
        u16 *pTexts = (u16*)((u32)file->textFile + 4 + (numCharStructs * sizeof(GameTextCharacterStruct)));
        u16 nTexts = PTR_VALID(pTexts) ? *pTexts : 0;
        if(curTextIdx < nTexts && curTextIdx >= 0) {
            gametextStruct *text = (gametextStruct*)&pTexts[2];
            text = &text[curTextIdx];
            sprintf(str, "Text %d/%d: ", curTextIdx, nTexts);
            if(PTR_VALID(text)) {
                sprintf(str, "%s%d (0x%X)\n(\eF%s, %dph @ %08X )\n", str,
                    text->identifier, text->identifier,
                    text->language < NUM_LANGUAGES ? languageNamesShort[text->language] : "?",
                    text->numPhrases, text->phrases);
                //OSReport("%s", str);
                drawText(str, x, y, NULL, NULL, flags, color, scale);
                y += LH*2;

                //phrases can be 0 if it hasn't finished loading yet.
                int nPhrases = PTR_VALID(text->phrases) ? text->numPhrases : 0;
                for(int iPhrase=0; iPhrase<nPhrases; iPhrase++) {
                    if(PTR_VALID(text->phrases[iPhrase])) {
                        //OSReport("Phrase %d/%d: %08X", iPhrase, text->numPhrases, text->phrases[iPhrase]);
                        //OSReport("-> %s", text->phrases[iPhrase]);
                        drawMenuBox(x, y+11, 2, 2); //bullet point
                        drawText(text->phrases[iPhrase], x+8, y, NULL, NULL, flags, color, scale);
                        y += LH;
                        if(y >= SCREEN_HEIGHT) break;
                    }
                }
            }
            else {
                sprintf(str, "%sN/A", str);
                drawText(str, x, y, NULL, NULL, flags, color, scale);
            }
        }
        else {
            sprintf(str, "Text %d/%d: N/A ", curTextIdx, nTexts);
            drawText(str, x, y, NULL, NULL, flags, color, scale);
        }
    }
}

void textFileMenu_draw(Menu *self) {
    //Draw function for text file menu
    int LH = ((curLanguage == LANG_JAPANESE) ? LINE_HEIGHT_JAPANESE : LINE_HEIGHT) + 3;
    int FW = ((curLanguage == LANG_JAPANESE) ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH);
    int x = TEXTFILE_MENU_XPOS + MENU_PADDING;
    int y = TEXTFILE_MENU_YPOS + MENU_PADDING;
    float idScale = curLanguage == LANG_JAPANESE ? 0.5 : 0.75;

    drawMenuBox(TEXTFILE_MENU_XPOS, TEXTFILE_MENU_YPOS, TEXTFILE_MENU_WIDTH, TEXTFILE_MENU_HEIGHT);
    if(self->selected == 0) {
        char str[256];

        drawSimpleText("\eF#: Ln Dir\eX0250Status\eX0400Ref Texts\n", x, y); y += LH;

        for(int iSlot=0; iSlot<MAX_LOADED_TEXTS; iSlot++) {
            GameTextFileData *file = &curGameTexts[iSlot];
            int nTexts = -1;
            if(PTR_VALID(file) && PTR_VALID((file->textFile))) {
                u32 numCharStructs = *(u32*)file->textFile;
                u16 *pTexts = (u16*)((u32)file->textFile + 4 +
                    (numCharStructs * sizeof(GameTextCharacterStruct)));
                nTexts = PTR_VALID(pTexts) ? *pTexts : 0;
            }

            sprintf(str, "\eF%d: %2s %02X %s\eX0250%02X %s\eX0400%d %5d",
                iSlot, file->lang < NUM_LANGUAGES ? languageNamesShort[file->lang] : "?",
                file->dir, gameTextMapNames[file->dir],
                file->state, (file->state < 7) ? stateNames[file->state] : "?",
                file->refCount, nTexts);
            drawSimpleText(str, x, y);
            y += LH;
        }
    }
    else _drawSlot(self);
}

void textFileMenu_run(Menu *self) {
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
    else if(buttonsJustPressed == PAD_TRIGGER_L) { //page through texts
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        curTextIdx -= 100;
        if(curTextIdx < 0) curTextIdx = 0;
    }
    else if(buttonsJustPressed == PAD_TRIGGER_R) { //page through texts
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        curTextIdx += 100;
    }

    //left/right: select slot
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer =
            (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected++;
        //0 is overview
        if(self->selected > MAX_LOADED_TEXTS) self->selected = 0;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected--;
        if(self->selected < 0) self->selected = MAX_LOADED_TEXTS;
    }

    //up/down: select text
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        curTextIdx--;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        curTextIdx++;
    }
}

Menu menuDebugViewTextFiles = {
    "View Loaded Text Files", 0,
    textFileMenu_run, textFileMenu_draw, debugTextSubMenu_close,
    NULL,
};
