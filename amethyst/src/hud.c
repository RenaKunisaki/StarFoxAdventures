#include "main.h"
#include <stdint.h>

uint8_t hudFlags = 0; //HudFlag
uint8_t beepTimer = 0;

void doHudHacks() {
    PlayerCharState *playerState =
        &saveData.curSaveGame.charState[saveData.curSaveGame.character];

    u8 health = playerState->curHealth;
    ObjInstance *arwing = getArwing();
    if(arwing) {
        u8 *state = (u8*)arwing->state;
        health = state[0x468];
    }

    if(health <= 4) {
        if(hudFlags & HUD_LOW_HP_FLASH) {
            //XXX doesn't work for Arwing
            if(hudHealthBlinkTimer < 30) hudHealthBlinkTimer = 150;
        }
        if(hudFlags & HUD_LOW_HP_BEEP) {
            if(!beepTimer) {
                audioPlaySound(NULL, 0x38D);
                beepTimer = 255;
            }
            else beepTimer--;
        }
    }
    //debugPrintf("Blink %f\n", hudHealthBlinkTimer);
}
