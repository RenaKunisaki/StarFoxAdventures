/** Main in-game menu.
 */
#include "main.h"

void mainMenu_close(const Menu *self) {
    //Close function for main menu
    DPRINT("Close main menu");
    menuState = MENU_CLOSING;
    menuAnimFrame = MENU_ANIM_NUM_FRAMES;
    if(!(menuPrevGameFlags & MENU_FLAG_TIME_STOPPED)) timeStop = 0;
    if(!(menuPrevGameFlags & MENU_FLAG_PAUSE_DISABLED)) pauseDisabled = 0;
    activeTimerFlags = menuPrevTimerFlags;
}

void mainSubMenu_close(const Menu *self) {
    //Close function for submenus of the main menu
    curMenu = &menuMain;
}

void genericMenu_run(const Menu *self) {
    //Run function for most menus
    int sel = curMenu->selected;
    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = 8;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_A) {
        menuInputDelayTimer = 8;
        curMenu->items[sel].select(&curMenu->items[sel], 0);
    }
    else if(controllerStates[0].stickY > 40) { //up
        menuInputDelayTimer = 8;
        if(sel == 0) {
            while(curMenu->items[sel].name) sel++;
        }
        curMenu->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -40) { //down
        menuInputDelayTimer = 8;
        sel++;
        if(!curMenu->items[sel].name) sel = 0;
        curMenu->selected = sel;
    }
    else if(controllerStates[0].stickX > 40) { //right
        menuInputDelayTimer = 8;
        curMenu->items[sel].select(&curMenu->items[sel], 1);
    }
    else if(controllerStates[0].stickX < -40) { //left
        menuInputDelayTimer = 8;
        curMenu->items[sel].select(&curMenu->items[sel], -1);
    }
    else if(controllerStates[0].triggerLeft > 40) { //L
        menuInputDelayTimer = 0;
        curMenu->items[sel].select(&curMenu->items[sel], -1);
    }
    else if(controllerStates[0].triggerRight > 40) { //R
        menuInputDelayTimer = 0;
        curMenu->items[sel].select(&curMenu->items[sel], 1);
    }
}

void genericMenuItem_draw(const MenuItem *self, int x, int y, bool selected) {
    //Draw function for menu items that are only text
    gameTextShowStr(self->name, MENU_TEXTBOX_ID, x, y);
}

void menuMainGameSettings_select(const MenuItem *self, int amount) {
    if(amount) return; //no adjustment
    curMenu = &menuGameSettings;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuMainControlSettings_select(const MenuItem *self, int amount) {
    if(amount) return; //no adjustment
    curMenu = &menuControlSettings;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuMainPlayerSettings_select(const MenuItem *self, int amount) {
    if(amount) return; //no adjustment
    curMenu = &menuPlayerSettings;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuMainVideoSettings_select(const MenuItem *self, int amount) {
    if(amount) return; //no adjustment
    curMenu = &menuVideoSettings;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuMainAudioSettings_select(const MenuItem *self, int amount) {
    if(amount) return; //no adjustment
    curMenu = &menuAudioSettings;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

void menuMainPdaSettings_select(const MenuItem *self, int amount) {
    if(amount) return; //no adjustment
    curMenu = &menuPdaSettings;
    audioPlaySound(NULL, MENU_OPEN_SOUND);
}

Menu menuMain = {
    "Main Menu", 0,
    genericMenu_run, mainMenu_close,
    "Game Settings",    genericMenuItem_draw, menuMainGameSettings_select,
    "Control Settings", genericMenuItem_draw, menuMainControlSettings_select,
    "Player Settings",  genericMenuItem_draw, menuMainPlayerSettings_select,
    "Video Settings",   genericMenuItem_draw, menuMainVideoSettings_select,
    "Audio Settings",   genericMenuItem_draw, menuMainAudioSettings_select,
    "PDA Settings",     genericMenuItem_draw, menuMainPdaSettings_select,
    NULL,
};
