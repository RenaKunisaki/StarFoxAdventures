/** Renders a timer and speedometer during Ice Mountain bike sequence.
 *  XXX do it for other bike sequences too.
 */
#include "main.h"
#include "revolution/os.h"
#include "revolution/pad.h"

#define TEXTBOX_ID 4
#define TEXT_XPOS (SCREEN_WIDTH - 210)
#define TEXT_YPOS (SCREEN_HEIGHT - 173)
static float disappearTimer = 0;
static bool active = false;

//backup for text window
static float oldTextScale = 0;
static s16 oldWidth;
static s16 oldHeight;
static u8 oldJustify;

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
    double secs = gameTimerValue / 60.0;
    sprintf(str, "%02d:%02d:%02d",
        ((int)secs) / 60, // minutes
        ((int)secs) % 60, // seconds
        ((int)(secs * 100.0)) % 100); // 1/100 seconds

    //the timer code does this, but slightly different. (and uses box 0xD)
    GameTextBox *box = gameTextGetBox(TEXTBOX_ID);
    if(oldTextScale == 0) {
        oldTextScale = box->textScale;
        oldWidth     = box->width;
        oldHeight    = box->height;
        oldJustify   = box->justify;
    }
    //box->x = TEXT_XPOS;
    //box->y = TEXT_YPOS;
    box->height = SCREEN_HEIGHT;
    box->width = SCREEN_WIDTH;
    box->textScale = 1.0;
    box->justify = GameTextJustify_Full;

    u8 alpha = MIN((int)(disappearTimer * 8.0), 255);
    drawHudBox(TEXT_XPOS+20, TEXT_YPOS+99, 120, 40, alpha, true);
    gameTextSetColor(0xFF, bikeMoveScale > 0.5 ? 0x3F : 0xFF, 0xFF, alpha);
    gameTextShowStr(str, TEXTBOX_ID, TEXT_XPOS+5, TEXT_YPOS);

    //draw speed
    vec3f vel;
    if(pPlayer) vel = pPlayer->vel;
    vec3f zero = {0, 0, 0};
    //this scale gives a top speed of 64km/h which is nice.
    double vxz = vec3f_xzDistance(&vel, &zero) * bikeMoveScale * 21.5;
    sprintf(str, "%3d km/h", (int)vxz);
    gameTextSetColor(0xFF, 0xFF, 0xFF, alpha);
    gameTextShowStr(str, TEXTBOX_ID, TEXT_XPOS, TEXT_YPOS+18);
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
        start = true;
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
            if(oldTextScale != 0) {
                GameTextBox *box = gameTextGetBox(TEXTBOX_ID);
                box->textScale   = oldTextScale;
                box->width       = oldWidth;
                box->height      = oldHeight;
                box->justify     = oldJustify;
                oldTextScale     = 0;
            }
        }
    }
}
