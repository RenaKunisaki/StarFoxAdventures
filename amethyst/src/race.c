/** Renders a timer and speedometer during Ice Mountain bike sequence.
 *  XXX do it for other bike sequences too.
 */
#include "main.h"
#include "revolution/os.h"
#include "revolution/pad.h"
static float disappearTimer = 0;
static bool active = false;

void raceTimerToggle(bool start) {
    if(start) {
        gameTimerInit(GAME_TIMER_FLAG_RUNNING | GAME_TIMER_FLAG_VISIBLE,
            (99*60*60) + (59*60) + 59); //min:sec:frames
        OSReport("Started race timer!");
        if(getButtonsHeld(0) & PAD_TRIGGER_Z) bikeMoveScale = 0.875; //turbo mode
        //XXX add a switch or something to the map to toggle this
    }
    else {
        gameTimerStop();
        OSReport("Stopped race timer!");
    }
}

static void drawTimer() {
    char str[64];
    double secs = gameTimerValue / 60.0;
    sprintf(str, "%02d:%02d:%02d",
        ((int)secs) / 60, // minutes
        ((int)secs) % 60, // seconds
        ((int)(secs * 100.0)) % 100); // 1/100 seconds

    //the timer code does this, but slightly different. (and uses box 0xD)
    //XXX does this break other timers after the race?
    //or whatever else uses this box. should restore it after the race.
    int boxId = 4, x = SCREEN_WIDTH - 210, y = SCREEN_HEIGHT - 173;
    GameTextBox *box = gameTextGetBox(boxId);
    //box->x = x;
    //box->y = y;
    box->height = SCREEN_HEIGHT;
    box->width = SCREEN_WIDTH;
    box->textScale = 1.0;
    box->justify = GameTextJustify_Full;

    u8 alpha = MIN((int)(disappearTimer * 8.0), 255);
    drawHudBox(450, 406, 120, 40, alpha, true);
    gameTextSetColor(0xFF, bikeMoveScale > 0.5 ? 0x3F : 0xFF, 0xFF, alpha);
    gameTextShowStr(str, boxId, x+5, y);

    //draw speed
    vec3f vel;
    if(pPlayer) vel = pPlayer->vel;
    vec3f zero = {0, 0, 0};
    //this scale gives a top speed of 64km/h which is nice.
    double vxz = vec3f_xzDistance(&vel, &zero) * bikeMoveScale * 21.5;
    sprintf(str, "%3d km/h", (int)vxz);
    gameTextSetColor(0xFF, 0xFF, 0xFF, 0xFF);
    gameTextShowStr(str, boxId, x, y+18);
}

void raceTimerUpdate() {
    char str[64];

    void *pState = (pPlayer && pPlayer->catId == 1) ? pPlayer->state : NULL;
    u16 stateId = pState ? *(u16*)((u32)pState + 0x274) : 0;

    //if(pState) debugPrintf("STATE %02X\n", stateId);

    bool start = false;
    if(mainGetBit(0xC8)) { //IM race has actually started
        start = true;
    }
    else if(stateId == 0x18 && curMapId != 0x17) { //on bike, not Ice Mountain
        start = true;
    }

    //start = true; stateId = 0x18; //for testing
    if(start && !active) { //start the timer
        active = true;
        gameTimerValue = -timeDelta; //start at 0 (we're about to add this again)
        //hudHidden = true; //causes weird tail glitching
        hudElementOpacity = 0;
    }

    if(active && pState && stateId != 0x18) { //player warped or something; stop the timer
        mainSetBits(0xC8, 0);
        active = false;
        //hudHidden = false;
        hudElementOpacity = 255;
    }

    if(active) {
        //we don't actually start the timer because then it makes a ticking noise.
        //and we don't use its own render function because we want to move the
        //text a bit to make room for the speedometer.
        gameTimerValue += timeDelta;
        disappearTimer = 300.0; //frames, ie 5 seconds
        drawTimer();
    }
    else if(disappearTimer > 0) {
        disappearTimer -= timeDelta;
        drawTimer();
        if(disappearTimer <= 0) bikeMoveScale = 0.5; //reset to default
    }
}
