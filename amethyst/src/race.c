/** Renders a timer and speedometer during Ice Mountain bike sequence.
 *  XXX do it for other bike sequences too.
 */
#include "main.h"
#include "revolution/os.h"
#include "revolution/pad.h"

#define TEXTBOX_ID 4
#define TEXT_XPOS (SCREEN_WIDTH - 150)
#define TEXT_YPOS (SCREEN_HEIGHT - 83)
static float disappearTimer = 0;
static bool active = false;

void raceTimerToggle(bool start) {
    if(start) {
        gameTimerInit(GAME_TIMER_FLAG_RUNNING | GAME_TIMER_FLAG_VISIBLE,
            (99*60*60) + (59*60) + 59); //min:sec:frames
        //OSReport("Started race timer!");
        if(getButtonsHeld(0) & PAD_BUTTON_X) bikeMoveScale = 0.875; //turbo mode
        //XXX add a switch or something to the map to toggle this

        gameTimerValue = -timeDelta; //start at 0 (we're about to add this again)
        //hudHidden = true; //causes weird tail glitching, other issues
        hudElementOpacity = 0;
    }
    else {
        gameTimerStop();
        //OSReport("Stopped race timer!");
        //hudHidden = false;
        hudElementOpacity = 255;
    }
}

static void drawTimer() {
    char str[64];
    if(gameTimerValue < 0) gameTimerValue = 0;
    double secs = gameTimerValue / 60.0;

    vec3f vel;
    if(pPlayer) vel = pPlayer->vel;
    vec3f zero = {0, 0, 0};
    //this scale gives a top speed of 64km/h which is nice.
    double vxz = vec3f_xzDistance(&vel, &zero) * bikeMoveScale * 21.5;

    sprintf(str, "\eF%02d:%02d:%02d\n%3d km/h",
        ((int)secs) / 60, // minutes
        ((int)secs) % 60, // seconds
        ((int)(secs * 100.0)) % 100, // 1/100 seconds
        (int)vxz);

    u8 alpha = MIN((int)(disappearTimer * 8.0), 255);
    drawHudBox(TEXT_XPOS, TEXT_YPOS, 100, 40, alpha, true);
    Color4b color = {.r = 0xC0, .g = bikeMoveScale > 0.5 ? 0x3F : 0xC0, .b = 0xC0, .a = alpha};
    drawColorText(str, TEXT_XPOS+5, TEXT_YPOS, color);
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
    else if(stateId == 0x18 //riding
    && (curMapId == 0x13 || curMapId == 0x1B || curMapId == 0x2B)) { //in DIM/DIM Inside/CR Race
        //this is a separate case so that we can make the timer not start until
        //you actually gain control of the bike.
        //XXX find how to do this for the other races.
        void *pState = pPlayer ? pPlayer->state : NULL;
        ObjInstance *ride = pState ? (*(ObjInstance**)(pState + 0x7F0)) : NULL;
        //don't start for SnowHorn
        start = ride && ride->catId == ObjCatId_bike;
    }

    //start = true; stateId = 0x18; //for testing
    if(start && !active) { //start the timer
        raceTimerToggle(true);
        active = true;
    }

    if(active && pState && stateId != 0x18) { //player warped or something; stop the timer
        //mainSetBits(0xC8, 0);
        raceTimerToggle(false);
        active = false;
    }

    if(active) {
        //we don't actually start the timer because then it makes a ticking noise.
        //and we don't use its own render function because we want to move the
        //text a bit to make room for the speedometer.
        if(!timeStop) gameTimerValue += timeDelta;
        disappearTimer = 300.0; //frames, ie 5 seconds
        drawTimer();
    }
    else if(disappearTimer > 0) {
        disappearTimer -= timeDelta;
        if(disappearTimer < 0) disappearTimer = 0; //don't flicker
        drawTimer();
        if(disappearTimer <= 0) {
            bikeMoveScale = 0.5; //reset to default
        }
    }
}
