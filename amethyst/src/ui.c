#include "main.h"

bool motionBlurHook() {
    //replaces a bl to shouldForceMotionBlur()
    bool force = shouldForceMotionBlur();
    if(force) return force;
    if(!bRumbleBlur) return false;

    if(rumbleTimer > 0) {
        float rumble = (rumbleTimer + 48) * 2;
        if(rumble > 120) rumble = 120;
        motionBlurIntensity = rumble;
        return true;
    }
    return false;
}

void hudDrawHook(int p1, int p2, int p3) {
     if(curGameText != -1 || !(cameraFlags & CAM_FLAG_NO_HUD)) {
         GameUI_hudDraw(p1, p2, p3);
     }
     else { //still manually render a few things
         pauseMenuDraw(p1, p2, p3);
         pauseMenuDrawText();
         if(mapScreenVisible) mapScreenDrawHud();
         if(timeListVisible) timeListDraw();
         hudDrawAirMeter();
         if(cMenuEnabled && cMenuFadeCounter) hudDrawButtons(p1, p2, p3);
         fearTestMeterDraw();
     }
}
