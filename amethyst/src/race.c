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
    if(start && !active) {
        gameTimerInit(GAME_TIMER_FLAG_RUNNING | GAME_TIMER_FLAG_VISIBLE,
            (99*60*60) + (59*60) + 59); //min:sec:frames
        OSReport("Started race timer! bits %d %d", mainGetBit(0x72), mainGetBit(0xC8));
        if(getButtonsHeld(0) & PAD_BUTTON_X) bikeMoveScale = 0.875; //turbo mode

        gameTimerValue = -timeDelta; //start at 0 (we're about to add this again)
        //hudHidden = true; //causes weird tail glitching, other issues
        hudElementOpacity = 0;
        active = true;
    }
    else if(active && !start) {
        gameTimerStop();
        OSReport("Stopped race timer! bits %d %d", mainGetBit(0x72), mainGetBit(0xC8));
        //hudHidden = false;
        hudElementOpacity = 255;
        active = false;
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
    ObjInstance *ride = pState ? (*(ObjInstance**)(pState + 0x7F0)) : NULL;
    u16 stateId = pState ? *(u16*)((u32)pState + 0x274) : 0;
    bool start = false, stop = false;

    //if(pState) debugPrintf("STATE %02X\n", stateId);

    switch(curMapId) {
        case 0x17: { //Ice Mountain
            if(!mainGetBit(0x378)) stop = true;
            else if(mainGetBit(0x72) && mainGetBit(0xC8)) start = true;
            break;
        }

        case 0x13: //DarkIce Mines
        case 0x1B: //DIM inside
        case 0x2B: { //CloudRunner Fortress race
            if(pState) {
                //don't start for SnowHorn
                if(stateId == 0x18 && ride && ride->catId == ObjCatId_bike) start = true;
                else stop = true;
            }
            break;
        }

        default:
            stop = true; //in case player warped out of race
    }

    //start = true; stateId = 0x18; //for testing
    if(start) raceTimerToggle(true);
    else if(stop) raceTimerToggle(false);

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
