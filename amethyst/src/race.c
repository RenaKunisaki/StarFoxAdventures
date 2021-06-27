#include "main.h"
static float disappearTimer = 0;

void IM_ToggleRaceTimer(bool start) {
    if(start) {
        gameTimerInit(GAME_TIMER_FLAG_RUNNING | GAME_TIMER_FLAG_VISIBLE, 
            (99*60*60) + (59*60) + 59); //min:sec:frames
        OSReport("Started IM race timer!");
        WRITE32(0x803DE8B8, 0x3F600000); //turbo mode
    }
    else {
        gameTimerStop();
        OSReport("Stopped IM race timer!");
    }
}

void IM_UpdateRaceTimer() {
    if(mainGetBit(0xC8)) {
        //we don't actually start the timer because then it makes a ticking noise.
        gameTimerValue += timeDelta;
        disappearTimer = 300.0; //frames, ie 5 seconds
        gameTimerRun(); //display the timer
    }
    else if(disappearTimer - timeDelta > 0) {
        disappearTimer -= timeDelta;
        //the timer won't render during the end scene
        char str[64];
        double secs = gameTimerValue / 60.0;
        sprintf(str, "%02d:%02d:%02d",
            ((int)secs) / 60, // minutes
            ((int)secs) % 60, // seconds
            ((int)(secs * 100.0)) % 100); // 1/100 seconds
        gameTextSetColor(0xFF, 0xFF, 0xFF, MIN((int)(disappearTimer * 8.0), 255));
        gameTextShowStr(str, 0, 0, 300);
    }
}