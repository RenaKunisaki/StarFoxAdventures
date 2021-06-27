#include "main.h"
static float disappearTimer = 0;

void IM_ToggleRaceTimer(bool start) {
    if(start) {
        gameTimerInit(GAME_TIMER_FLAG_RUNNING | GAME_TIMER_FLAG_VISIBLE, 
            (99*60*60) + (59*60) + 59); //min:sec:frames
        OSReport("Started IM race timer!");
        //bikeMoveScale = 0.875; //turbo mode
    }
    else {
        gameTimerStop();
        OSReport("Stopped IM race timer!");
    }
}

static void drawTimer() {
    char str[64];
    double secs = gameTimerValue / 60.0;
    sprintf(str, "%02d:%02d:%02d",
        ((int)secs) / 60, // minutes
        ((int)secs) % 60, // seconds
        ((int)(secs * 100.0)) % 100); // 1/100 seconds

    //the timer code does this, but slightly different
    GameTextBox *box = gameTextGetBox(0xD);
    box->y = 400;
    box->height = 400;
    box->textScale = 1.0;
    box->justify = GameTextJustify_Full;

    u8 alpha = MIN((int)(disappearTimer * 8.0), 255);
    drawHudBox(50, 346, 120, 40, alpha, true);
    gameTextSetColor(0xFF, 0xFF, 0xFF, alpha);
    //gameTextShowStr(str, 0, 0, 300);
    gameTextShowStr(str, 0xD, 10, 90);
}

void IM_UpdateRaceTimer() {
    char str[64];

    if(mainGetBit(0xC8)) {
        //we don't actually start the timer because then it makes a ticking noise.
        //and we don't use its own render function because we want to move the
        //text a bit to make room for the speedometer.
        gameTimerValue += timeDelta;
        disappearTimer = 300.0; //frames, ie 5 seconds
        drawTimer();
        
        vec3f vel = pPlayer->vel;
        vec3f zero = {0, 0, 0};
        double vxz = vec3f_xzDistance(&vel, &zero) * bikeMoveScale * 20.0;
        sprintf(str, "%3d km/h", (int)vxz);
        gameTextSetColor(0xFF, 0xFF, 0xFF, 0xFF);
        gameTextShowStr(str, 0xD, 5, 111);
    }
    else if(disappearTimer - timeDelta > 0) {
        disappearTimer -= timeDelta;
        drawTimer();
    }
}
