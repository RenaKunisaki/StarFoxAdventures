#include "main.h"

u8 cameraFlags = 0; //CameraFlags

float cameraUpdateHook() {
    //replaces a call to the update method of the current camera DLL.
    //r12 is the address it was about to jump to, which depends on the mode.
    void (*origFunc)(Camera *self);
    GET_REGISTER(12, origFunc);
    origFunc(pCamera);

    int pad = (cameraFlags & CAM_FLAG_PAD3) ? 2 : 0;
    s8 stickX = controllerStates[pad].substickX & 0xFC;
    s8 stickY = controllerStates[pad].substickY & 0xFC;

    if(cameraFlags & CAM_FLAG_INVERT_X) stickX = -stickX;
    if(cameraFlags & CAM_FLAG_INVERT_Y) stickY = -stickY;

    pCamera->pos.rotation.x += stickX * 128;
    pCamera->pos.rotation.y += stickY *  16;
    //debugPrintf("Cam mode 0x%02X\n", cameraMode);
}

int padGetCxHook(int pad) {
    //replaces the body of padGetCX().
    if(joypadDisable || isDvdDriveBusy) return 0;
    if((pad == 0) && !(cameraFlags & CAM_FLAG_PAD3)) {
        //we use C stick to move camera
        //this means we don't want other things to react to the C stick
        u16 buttons = controllerStates[pad + (whichControllerFrame * 4)].button;
        if(buttons & PAD_BUTTON_LEFT)  return -16;
        if(buttons & PAD_BUTTON_RIGHT) return  16;
        return 0;
    }
    return controllerStates[pad + (whichControllerFrame * 4)].substickX;
}

int padGetCyHook(int pad) {
    //replaces the body of padGetCY().
    if(joypadDisable || isDvdDriveBusy) return 0;
    if((pad == 0) && !(cameraFlags & CAM_FLAG_PAD3)) {
        u16 buttons = controllerStates[pad + (whichControllerFrame * 4)].button;
        if(buttons & PAD_BUTTON_UP)   return  16;
        if(buttons & PAD_BUTTON_DOWN) return -16;
        return 0;
    }
    return controllerStates[pad + (whichControllerFrame * 4)].substickY;
}

//XXX check if cannon is inverted by default like viewfinder is.
int padGetStickXHook(int pad) {
    //replaces the body of padGetStickX().
    if(joypadDisable || isDvdDriveBusy) return 0;
    int result = controllerStates[pad + (whichControllerFrame * 4)].stickX;
    if(cameraFlags & CAM_FLAG_INVERT_X) {
        if(cameraMode == 0x43    //staff aiming
        || cameraMode == 0x44    //viewfinder
        || cameraMode == 0x51) { //aim cannon
            return -result;
        }
    }
    return result;
}

int padGetStickYHook(int pad) {
    //replaces the body of padGetStickY().
    if(joypadDisable || isDvdDriveBusy) return 0;
    int result = controllerStates[pad + (whichControllerFrame * 4)].stickY;
    if(cameraMode == 0x44) { //viewfinder
        //already inverted by default, so do opposite
        if(!(cameraFlags & CAM_FLAG_INVERT_Y)) return -result;
    }
    else if((cameraFlags & CAM_FLAG_INVERT_Y) && (
    cameraMode == 0x43 || cameraMode == 0x51)) {
        return -result;
    }
    return result;
}

u32 minimapButtonHeldHook(int pad) {
    //replaces a call to getButtonsHeld() in minimap update routine.
    if(cameraFlags & CAM_FLAG_PAD3) {
        //we don't need to change anything
        return getButtonsHeld(pad);
    }
    return 0;
}
u32 minimapButtonPressedHook(int pad) {
    //replaces a call to getButtonsJustPressed() in minimap update routine.
    if(cameraFlags & CAM_FLAG_PAD3) {
        //we don't need to change anything
        return getButtonsJustPressed(pad);
    }
    return 0;
}

int viewFinderZoomHook(int pad) {
    //replaces a call to padGetCY() in viewfinder routine.
    if(cameraFlags & CAM_FLAG_PAD3) {
        //we don't need to change anything
        return padGetCY(pad);
    }
    //remap to L/R, since D-pad is slow and digital.
    return controllerStates[pad].triggerLeft -  controllerStates[pad].triggerRight;
}
