#include "main.h"

u8 cameraFlags = 0; //CameraFlags
s8 debugCameraMode = CAM_MODE_NORMAL; //CameraMode

static void (*origFunc)(Camera *self);

//position to override camera for free/stay modes
static float camOverrideX, camOverrideY, camOverrideZ, camOverrideDist = 300;
static s16 camOverrideRX, camOverrideRY, camOverrideRZ;
static u8 camOverrideValid; //bool

//distance and angle for orbit mode
static float camOrbitDist  = 50;
static s16   camOrbitAngXZ = 0;
static s16   camOrbitAngY  = 0;

//turn camera to look at target
void _lookAtTarget() {
    vec3f targetPos;
    float targetXZ;
    cameraGetFocusObjDistance(
        cameraMtxVar57 ? cameraMtxVar57->targetHeight : 0,
        pCamera, &targetPos.x, &targetPos.y,
        &targetPos.z, &targetXZ, false);
    pCamera->pos.rotation.x = -0x8000 - atan2fi(targetPos.x, targetPos.z);

    //tilt to point to player
    //(Y rotation value, even though it's local X-axis rotation...)
    pCamera->pos.rotation.y = atan2fi(targetPos.y, targetXZ);
}

void _camGetStickInput(s8 *outX, s8 *outY) {
    int pad = (cameraFlags & CAM_FLAG_PAD3) ? 2 : 0;
    s8 stickX = controllerStates[pad].substickX & 0xFC;
    s8 stickY = controllerStates[pad].substickY & 0xFC;

    if(cameraFlags & CAM_FLAG_INVERT_X) stickX = -stickX;
    if(cameraFlags & CAM_FLAG_INVERT_Y) stickY = -stickY;

    if(outX) *outX = stickX;
    if(outY) *outY = stickY;
}

void _camDoRotateAroundPlayer(u8 stickX, u8 stickY) {
    //here we want to take the position the camera would be in if
    //we didn't change it, add an additional rotation around the
    //player, and use that as the new position.
    //problem: by changing the camera's actual position we change
    //what the "position it would have been" is for the next frame...
    //to fix this maybe we need to revert it after the view matrix
    //has been calculated?
    //or hook places that read the position and give our new position
    //but that might be a lot of places.
    //this also isn't as simple as adding an offset to an angle in the
    //position calculation, because the camera isn't calculating its
    //position based on its angle, but the other way around. it moves
    //in the world like any object, rotating to point to the player.
    //it only tries to position itself at a certain angle behind you
    //when in certain modes (holding L, etc).
    //maybe check CameraModeForceBehind?

    //get the distance from camera to player
    float dist;
    objGetXZAngleDistance((ObjInstance*)pCamera, pCamera->focus, &dist);

    //get the angle in world coords
    s16 rx = pCamera->pos.rotation.x;
    debugPrintf("d=%f r=%d\n", dist, rx);

    //add stick angle
    rx += stickX * 64;

    //compute new position - same distance, new angle
    float height = cameraMtxVar57 ? cameraMtxVar57->targetHeight : 0;
    //s16 rx = ((s16)stickX) + 64 + pCamera->focus->pos.rotation.x;
    //s16 ry =  (s16)stickY       + pCamera->focus->pos.rotation.y;
    s16 ry = 0;
    float mz = sinf(pi * -rx);
    float mx = cosf(pi * -rx);
    float my = sinf(pi * -ry / 128.0);
    float dy = cosf(pi * -ry / 128.0) * dist;
    //pCamera->pos.pos.x = pCamera->focus->pos.pos.x + dy * mx;
    //pCamera->pos.pos.y = pCamera->focus->pos.pos.y + height + camOrbitDist * my;
    //pCamera->pos.pos.z = pCamera->focus->pos.pos.z + dy * mz;
    //_lookAtTarget();
}

void _camDoCStick() {
    s8 stickX, stickY;
    _camGetStickInput(&stickX, &stickY);
    if(!bFreeMove) {
        float scale = 1;
        if(debugCameraMode == CAM_MODE_BIRD
        || debugCameraMode == CAM_MODE_FIRST_PERSON) {
            scale = 8;
        }
        if(cameraFlags & CAM_FLAG_PLAYER_AXIS) {
            _camDoRotateAroundPlayer(stickX, stickY);
        }
        else {
            pCamera->pos.rotation.x += stickX * 128 * scale * timeDelta;
            pCamera->pos.rotation.y += stickY *  16 * scale * timeDelta;
        }
    }
    camOverrideValid = 0;
}

void _drawDebugInfo() {
    float px=pCamera->pos.pos.x, py=pCamera->pos.pos.y, pz=pCamera->pos.pos.z;
    s16 rx=pCamera->pos.rotation.x, ry=pCamera->pos.rotation.y,
        rz=pCamera->pos.rotation.z;
    debugPrintfxy(24, 400, "X\t%6d\t%3d", (int)px, (int)(rx / (65535.0 / 360.0)));
    debugPrintfxy(24, 411, "Y\t%6d\t%3d", (int)py, (int)(ry / (65535.0 / 360.0)));
    debugPrintfxy(24, 422, "Z\t%6d\t%3d", (int)pz, (int)(rz / (65535.0 / 360.0)));
}

void _applyOverride() {
    if(camOverrideValid) {
        pCamera->pos.pos.x = camOverrideX;
        pCamera->pos.pos.y = camOverrideY;
        pCamera->pos.pos.z = camOverrideZ;
        if(debugCameraMode == CAM_MODE_FREE) {
            pCamera->pos.rotation.x = camOverrideRX;
            pCamera->pos.rotation.y = camOverrideRY;
            pCamera->pos.rotation.z = camOverrideRZ;
        }
    }
}

void _updateOverride() {
    camOverrideX  = pCamera->pos.pos.x;
    camOverrideY  = pCamera->pos.pos.y;
    camOverrideZ  = pCamera->pos.pos.z;
    camOverrideRX = pCamera->pos.rotation.x;
    camOverrideRY = pCamera->pos.rotation.y;
    camOverrideRZ = pCamera->pos.rotation.z;
    camOverrideValid = 1;
}

//update camera in Stay and Free modes
void _doStayOrFree(u32 bHeld, u32 bPressed) {
    //calculate movement scale
    float scale = ((bHeld & PAD_TRIGGER_Z) ? 2 : 1) * timeDelta;
    float srx = scale * 10.0 * ((cameraFlags & CAM_FLAG_INVERT_X) ? -1.0 : 1.0);
    float sry = scale * 10.0 * ((cameraFlags & CAM_FLAG_INVERT_Y) ? -1.0 : 1.0);
    float spx = scale * -0.25;
    float spy = scale *  0.05;
    float spz = scale *  0.25;

    //apply deadzones
    int sx = controllerStates[3].stickX;
    int sy = controllerStates[3].stickY;
    int cx = controllerStates[3].substickX;
    int cy = controllerStates[3].substickY;
    int tl = controllerStates[3].triggerLeft;
    int tr = controllerStates[3].triggerRight;
    if(sx < CAMERA_LSTICK_DEADZONE && sx > -CAMERA_LSTICK_DEADZONE) sx = 0;
    if(sy < CAMERA_LSTICK_DEADZONE && sy > -CAMERA_LSTICK_DEADZONE) sy = 0;
    if(cx < CAMERA_RSTICK_DEADZONE && cx > -CAMERA_RSTICK_DEADZONE) cx = 0;
    if(cy < CAMERA_RSTICK_DEADZONE && cy > -CAMERA_RSTICK_DEADZONE) cy = 0;
    if(tl < CAMERA_TRIGGER_DEADZONE) tl = 0;
    if(tr < CAMERA_TRIGGER_DEADZONE) tr = 0;

    //apply rotation
    pCamera->pos.rotation.x += srx * cx;
    pCamera->pos.rotation.y += sry * cy;

    //apply Y translation (movement)
    pCamera->pos.pos.y += spy * (tr - tl);

    //calculate XZ movement vector
    float tx = spx * sx;
    float tz = spz * sy; //not sz, since it's analog stick input
    //X here is actually the rotation around the Y axis...
    float sinR = sinf((float)pCamera->pos.rotation.x * S16_TO_RADIANS);
    float cosR = cosf((float)pCamera->pos.rotation.x * S16_TO_RADIANS);

    pCamera->pos.pos.x += (tx * cosR) - (tz * sinR);
    pCamera->pos.pos.z += (tx * sinR) + (tz * cosR);
}

//update camera in Bird's Eye mode
void _doBird(u32 bHeld, u32 bPressed) {
    vec3f targetPos;
    float targetXZ;

    //follow the target
    if(pCamera->focus) {
        pCamera->pos.pos.x = pCamera->focus->pos.pos.x;
        pCamera->pos.pos.y = pCamera->focus->pos.pos.y + camOverrideDist;
        pCamera->pos.pos.z = pCamera->focus->pos.pos.z;
        if(getArwing()) {
            pCamera->pos.rotation.x = -pCamera->focus->pos.rotation.x;
            pCamera->pos.rotation.y = 0x4000;
        }
        else {
            pCamera->pos.rotation.x = -0x8000 - pCamera->focus->pos.rotation.x;
            pCamera->pos.rotation.y = 0x4000;
        }
        if(camOverrideDist < 0) pCamera->pos.rotation.y = -pCamera->pos.rotation.y;
    }
    _camDoCStick();

    //L/R to zoom
    int tl = controllerStates[3].triggerLeft;
    int tr = controllerStates[3].triggerRight;
    if(tl < CAMERA_TRIGGER_DEADZONE) tl = 0;
    if(tr < CAMERA_TRIGGER_DEADZONE) tr = 0;
    float scale = timeDelta * 0.01;
    camOverrideDist += (tr - tl) * ((bHeld & PAD_TRIGGER_Z) ? 2 : 1) * scale;
    if(tl != tr) debugPrintf("Dist %f\n", camOverrideDist);
}

//handle camera in First Person mode
void _doFirstPerson(u32 bHeld, u32 bPressed) {
    if(pCamera->focus) {
        float height = cameraMtxVar57 ? cameraMtxVar57->targetHeight : 0;
        pCamera->pos.pos.x = pCamera->focus->pos.pos.x;
        //fudge factor to be above character's head instead of inside it
        pCamera->pos.pos.y = pCamera->focus->pos.pos.y + height + 8;
        pCamera->pos.pos.z = pCamera->focus->pos.pos.z;
        if(getArwing()) {
            pCamera->pos.rotation.x = -pCamera->focus->pos.rotation.x;
            pCamera->pos.rotation.y = pCamera->focus->pos.rotation.y;
            pCamera->pos.rotation.z = pCamera->focus->pos.rotation.z;
        }
        else {
            pCamera->pos.rotation.x = -0x8000 - pCamera->focus->pos.rotation.x;
            pCamera->pos.rotation.y = -pCamera->focus->pos.rotation.y;
            pCamera->pos.rotation.z = -pCamera->focus->pos.rotation.z;
        }
        //this works but doesn't undo when you exit
        //XXX how does it work when you do the actual first person view?
        //if(pCamera->focus->newOpacity > 64) pCamera->focus->newOpacity = 64;
    }
    _camDoCStick();
}

//handle camera in Orbit mode
void _doOrbit(u32 bHeld, u32 bPressed) {
    if(!pCamera->focus) return;

    s8 stickX, stickY;
    _camGetStickInput(&stickX, &stickY);
    camOrbitAngXZ += stickX * timeDelta * 10;
    camOrbitAngY  += stickY * timeDelta * 10;
    camOrbitDist  += (controllerStates[3].triggerLeft -
        controllerStates[3].triggerRight) * timeDelta * 0.1;

    float height = cameraMtxVar57 ? cameraMtxVar57->targetHeight : 0;
    float mz = sinf(pi * -(camOrbitAngXZ - 0x4000) / 32768.0);
    float mx = cosf(pi * -(camOrbitAngXZ - 0x4000) / 32768.0);
    float my = sinf(pi * -camOrbitAngY / 32768.0);
    float dy = cosf(pi * -camOrbitAngY / 32768.0) * camOrbitDist;

    pCamera->pos.pos.x = pCamera->focus->pos.pos.x + dy * mx;
    pCamera->pos.pos.y = pCamera->focus->pos.pos.y + height + camOrbitDist * my;
    pCamera->pos.pos.z = pCamera->focus->pos.pos.z + dy * mz;
    _lookAtTarget();
}

//replaces a call to the update method of the current camera DLL.
void cameraUpdateHook() {
    //r12 is the address it was about to jump to, which depends on the mode.
    GET_REGISTER(12, origFunc);

    //debugPrintf("Cam mode 0x%02X\n", cameraMode);

    //do this in all modes so that we don't carry over stale button states
    //when changing modes
    static u32 prevBtn4 = 0;
    u32 bHeld = controllerStates[3].button;
    u32 bPressed = bHeld & ~prevBtn4;
    prevBtn4 = bHeld;

    if(debugCameraMode == CAM_MODE_NORMAL) {
        origFunc(pCamera);
        _camDoCStick();
        return;
    }

    _applyOverride();

    if(debugCameraMode == CAM_MODE_STAY || bHeld & PAD_BUTTON_Y) {
        _lookAtTarget();
    }

    if(bHeld & PAD_BUTTON_A) { //reset
        origFunc(pCamera);
        _camDoCStick();
    }

    switch(debugCameraMode) {
        case CAM_MODE_STAY:
        case CAM_MODE_FREE:
            _doStayOrFree(bHeld, bPressed);
            break;
        case CAM_MODE_ORBIT: _doOrbit(bHeld, bPressed); break;
        case CAM_MODE_BIRD: _doBird(bHeld, bPressed); break;
        case CAM_MODE_FIRST_PERSON: _doFirstPerson(bHeld, bPressed); break;
        default: break;
    }

    //debugPrintf(DPRINT_FIXED "S %d %d C %d %d L %d R %d\n" DPRINT_NOFIXED,
    //    controllerStates[3].stickX,      controllerStates[3].stickY,
    //    controllerStates[3].substickX,   controllerStates[3].substickY,
    //    controllerStates[3].triggerLeft, controllerStates[3].triggerRight);

    //update camera
    cameraUpdateViewMtx(pCamera);
    _drawDebugInfo();
    _updateOverride();
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
    //XXX mode 0x52 is both holding L button and aiming with staff,
    //and pushing blocks.
    //find a better way to distinguish the two than checking for L button.

    void *pState = pPlayer ? pPlayer->state : NULL;
    u16 stateNo = pState ? *(u16*)(pState + 0x274) : 0;
    //don't invert controls when pushing a block
    if(stateNo == 0x1D) return result;

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

    void *pState = pPlayer ? pPlayer->state : NULL;
    u16 stateNo = pState ? *(u16*)(pState + 0x274) : 0;
    //don't invert controls when pushing a block
    if(stateNo == 0x1D) return result;

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
    int tl = controllerStates[pad].triggerLeft;
    int tr = controllerStates[pad].triggerRight;
    if(tl < CAMERA_TRIGGER_DEADZONE) tl = 0;
    if(tr < CAMERA_TRIGGER_DEADZONE) tr = 0;
    return tl - tr;
}
