#include "main.h"

u8 cameraFlags = 0; //CameraFlags
s8 debugCameraMode = CAM_MODE_NORMAL; //CameraMode

void _camUpdateNormal() {
    int pad = (cameraFlags & CAM_FLAG_PAD3) ? 2 : 0;
    s8 stickX = controllerStates[pad].substickX & 0xFC;
    s8 stickY = controllerStates[pad].substickY & 0xFC;

    if(cameraFlags & CAM_FLAG_INVERT_X) stickX = -stickX;
    if(cameraFlags & CAM_FLAG_INVERT_Y) stickY = -stickY;

    if(!bFreeMove) {
        pCamera->pos.rotation.x += stickX * 128;
        pCamera->pos.rotation.y += stickY *  16;
    }
}

void _drawDebugInfo() {
    float px=pCamera->pos.pos.x, py=pCamera->pos.pos.y, pz=pCamera->pos.pos.z;
    s16 rx=pCamera->pos.rotation.x, ry=pCamera->pos.rotation.y,
        rz=pCamera->pos.rotation.z;
    debugPrintfxy(24, 400, "X\t%6d\t%3d", (int)px, (int)(rx / (65535.0 / 360.0)));
    debugPrintfxy(24, 411, "Y\t%6d\t%3d", (int)py, (int)(ry / (65535.0 / 360.0)));
    debugPrintfxy(24, 422, "Z\t%6d\t%3d", (int)pz, (int)(rz / (65535.0 / 360.0)));
}

void cameraUpdateHook() {
    //replaces a call to the update method of the current camera DLL.
    //r12 is the address it was about to jump to, which depends on the mode.
    void (*origFunc)(Camera *self);
    GET_REGISTER(12, origFunc);

    //debugPrintf("Cam mode 0x%02X\n", cameraMode);

    //do this in all modes so that we don't carry over stale button states
    //when changing modes
    static u32 prevBtn4 = 0;
    u32 bHeld4 = controllerStates[3].button;
    u32 bPressed4 = bHeld4 & ~prevBtn4;
    prevBtn4 = bHeld4;

    if(debugCameraMode == CAM_MODE_NORMAL) {
        origFunc(pCamera);
        _camUpdateNormal();
    }
    else {
        if(debugCameraMode == CAM_MODE_STAY || bHeld4 & PAD_BUTTON_Y) {
            //turn to look at target
            vec3f targetPos;
            float targetXZ;
            cameraGetFocusObjDistance(
                cameraMtxVar57 ? cameraMtxVar57->targetHeight : 0,
                pCamera, &targetPos.x, &targetPos.y,
                &targetPos.z, &targetXZ, false);
            pCamera->pos.rotation.x = -0x8000 - atan2(targetPos.x, targetPos.z);

            //# tilt to point to player
            //(Y rotation value, even though it's local X-axis rotation...)
            pCamera->pos.rotation.y = atan2(targetPos.y, targetXZ);
        }

        int scale = (bHeld4 & PAD_TRIGGER_Z) ? 2 : 1;
        float srx = scale * 5 * (cameraFlags & CAM_FLAG_INVERT_X) ? -1 : 1;
        float sry = scale * 5 * (cameraFlags & CAM_FLAG_INVERT_Y) ? -1 : 1;
        float spx = scale *  0.25;
        float spy = scale *  0.05;
        float spz = scale * -0.25;

        pCamera->pos.rotation.x += srx * controllerStates[3].substickX;
        pCamera->pos.rotation.y += sry * controllerStates[3].substickY;
        pCamera->pos.pos.y += spy *
            (controllerStates[3].triggerRight - controllerStates[3].triggerLeft);
        pCamera->pos.pos.x += spx * controllerStates[3].stickX;
        pCamera->pos.pos.z += spz * controllerStates[3].stickY;
        //debugPrintf(DPRINT_FIXED "S %d %d C %d %d L %d R %d\n" DPRINT_NOFIXED,
        //    controllerStates[3].stickX,      controllerStates[3].stickY,
        //    controllerStates[3].substickX,   controllerStates[3].substickY,
        //    controllerStates[3].triggerLeft, controllerStates[3].triggerRight);

        cameraUpdateViewMtx(pCamera);
        _drawDebugInfo();
    }
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
    u16 buttons = controllerStates[pad + (whichControllerFrame * 4)].button;
    //XXX mode 0x52 is both holding L button and aiming with staff.
    //find a better way to distinguish the two than checking for L button.
    if((cameraFlags & CAM_FLAG_INVERT_X) && !(buttons & PAD_TRIGGER_L)) {
        if(cameraMode == 0x52    //staff aiming
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
    u16 buttons = controllerStates[pad + (whichControllerFrame * 4)].button;
    if(cameraMode == 0x44) { //viewfinder
        //already inverted by default, so do opposite
        if(!(cameraFlags & CAM_FLAG_INVERT_Y)) return -result;
    }
    else if((cameraFlags & CAM_FLAG_INVERT_Y) && !(buttons & PAD_TRIGGER_L) && (
    cameraMode == 0x52 || cameraMode == 0x51)) {
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
