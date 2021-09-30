/** Debug Font Test submenu.
 */
#include "main.h"
#include "revolution/pad.h"
#include <stdio.h>

#define TEXTFILE_MENU_XPOS 16
#define TEXTFILE_MENU_YPOS 48
#define TEXTFILE_MENU_WIDTH SCREEN_WIDTH - (TEXTFILE_MENU_XPOS * 2)
#define TEXTFILE_MENU_HEIGHT SCREEN_HEIGHT - (TEXTFILE_MENU_YPOS + 16)

extern GameLanguageEnum prevLanguage;

void textFileMenu_draw(Menu *self) {
    //Draw function for text file menu
    int LH = ((curLanguage == LANG_JAPANESE) ? LINE_HEIGHT_JAPANESE : LINE_HEIGHT) + 3;
    int FW = ((curLanguage == LANG_JAPANESE) ? FIXED_CHR_WIDTH_JAPANESE : FIXED_CHR_WIDTH);
    int x = TEXTFILE_MENU_XPOS + MENU_PADDING;
    int y = TEXTFILE_MENU_YPOS + MENU_PADDING;
    float idScale = curLanguage == LANG_JAPANESE ? 0.5 : 0.75;

    drawMenuBox(TEXTFILE_MENU_XPOS, TEXTFILE_MENU_YPOS, TEXTFILE_MENU_WIDTH, TEXTFILE_MENU_HEIGHT);

    char str[256];
    GameTextFileData *file = &curGameTexts[self->selected];
    sprintf(str,
        "Slot \eF%d\eF: \eF%d %s\eF, font #\eF%d\eF: %02X %s\n"
        "File ID \eF%02X\eF @ \eF%08X\eF size \eF%06X\eF\n"
        "ID\t\t%02X\n"
        "Pad\t\t%d, %d\n"
        "ChrSize\t%d, %d\n"
        "Font\t%02X\n"
        "Texture\t%02X\n"
        "State\t%02X\n"
        "RefCnt\t%d\n",
        self->selected, file->lang,
        file->lang < NUM_LANGUAGES ? languageNamesShort[file->lang] : "?",
        file->fontNo, file->dir, gameTextMapNames[file->dir],
        file->fileIdx, file->textFile, file->fileSize,
        file->id,
        file->leftPad, file->topPad,
        file->charW, file->charH,
        file->font,
        file->textureIdx,
        file->state,
        file->refCount);
    drawSimpleText(str, x, y);
    x += 300;

    //hex dump
    u8 *data = (u8*)file;
    for(int offs=0; offs<sizeof(GameTextFileData); offs += 8) {
        sprintf(str, "\eF%02X:  %02X %02X %02X %02X  %02X %02X %02X %02X\n", offs,
            data[offs+0], data[offs+1], data[offs+2], data[offs+3],
            data[offs+4], data[offs+5], data[offs+6], data[offs+7]);
        drawSimpleText(str, x, y);
        y += LH;
    }
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

    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer =
            (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected++;
        if(self->selected >= MAX_LOADED_TEXTS) self->selected = 0;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        self->selected--;
        if(self->selected < 0) self->selected = MAX_LOADED_TEXTS - 1;
    }
}

Menu menuDebugViewTextFiles = {
    "View Loaded Text Files", 0,
    textFileMenu_run, textFileMenu_draw, debugTextSubMenu_close,
    NULL,
};
