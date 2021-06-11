#include "main.h"

bool bFreeMove = false;
vec3f freeMoveCoords;
static u16 startFlags;
static u16 startPlayerFlags;
static bool bWasOn = false;
static bool bWasTimeStopped;
static u8 freeMoveToggleDelay = 0;

#define FREE_MOVE_SCALE_XZ        0.07 //for X and Z axes (control stick)
#define FREE_MOVE_SCALE_XZ_FAST   0.35 //when Z held
#define FREE_MOVE_SCALE_Y         0.07 //for Y axis (L/R triggers)
#define FREE_MOVE_SCALE_Y_FAST    0.35 //when Z held
#define FREE_MOVE_SCALE_ROT       10.0 //for rotating (C stick)
#define FREE_MOVE_SCALE_ROT_FAST  50.0 //when Z held
#define FREE_MOVE_CAMERA_DISTANCE 15.0

void doFreeMove() {
    if(menuState != MENU_NOT_OPEN) return;
    if(freeMoveToggleDelay) freeMoveToggleDelay--;
    else {
        if(controllerStates[2].button & PAD_BUTTON_X) {
            freeMoveToggleDelay = 60;
            bFreeMove = !bFreeMove;
        }
        else if(buttonsJustPressed == PAD_BUTTON_B) {
            freeMoveToggleDelay = 60;
            bFreeMove = false;
        }
    }

    if(!bFreeMove) {
        if(bWasOn) {
            if(pPlayer) pPlayer->flags_0xb0 = startPlayerFlags;
            ObjInstance *obj = pCamera->focus;
            if(obj) obj->flags_0xb0 = startFlags;
            timeStop = bWasTimeStopped;
        }
        bWasOn = false;
        return;
    }

    //get camera focused object
    if(!pCamera) return;
    ObjInstance *obj = pCamera->focus;
    if(!obj) return;

    shouldCloseCMenu = 1;

    if(!bWasOn) {
        DPRINT("Start free move for obj %08X %s", obj, obj->file->name);
        freeMoveCoords = obj->pos.pos;
        startFlags  = obj->flags_0xb0;
        obj->flags_0xb0 = (obj->flags_0xb0 |
            ObjInstance_FlagsB0_DontUpdate | ObjInstance_FlagsB0_DontMove) &
            ~(ObjInstance_FlagsB0_Invisible);

        //even if we aren't moving the player, still stop them
        if(pPlayer) {
            startPlayerFlags = pPlayer->flags_0xb0;
            pPlayer->flags_0xb0 = (pPlayer->flags_0xb0 |
                ObjInstance_FlagsB0_DontUpdate | ObjInstance_FlagsB0_DontMove) &
                ~(ObjInstance_FlagsB0_Invisible);
        }
        bWasTimeStopped = timeStop;
        bWasOn = true;
        DPRINT("Start obj pos: %f %f %f", freeMoveCoords.x, freeMoveCoords.y, freeMoveCoords.z);
    }

    //get angle as X, Z multipliers
    float fwdX, fwdZ, sideX, sideZ;
    angleToVec2(obj->pos.rotation.x,          &fwdX,  &fwdZ);
    angleToVec2(obj->pos.rotation.x + 0x4000, &sideX, &sideZ);

    float dx =  (float)controllerStates[0].stickX;
    float dz = -(float)controllerStates[0].stickY;
    float rx = -(float)controllerStates[0].substickX;
    float ry = -(float)controllerStates[0].substickY;
    float dy =  (float)controllerStates[0].triggerRight -
        (float)controllerStates[0].triggerLeft;

    if(controllerStates[0].button & PAD_TRIGGER_Z) {
        dx *= FREE_MOVE_SCALE_XZ_FAST;
        dz *= FREE_MOVE_SCALE_XZ_FAST;
        dy *= FREE_MOVE_SCALE_Y_FAST;
        rx *= FREE_MOVE_SCALE_ROT_FAST;
        ry *= FREE_MOVE_SCALE_ROT_FAST;
    }
    else {
        dx *= FREE_MOVE_SCALE_XZ;
        dz *= FREE_MOVE_SCALE_XZ;
        dy *= FREE_MOVE_SCALE_Y;
        rx *= FREE_MOVE_SCALE_ROT;
        ry *= FREE_MOVE_SCALE_ROT;
    }

    //rotate dx and dz
    float mx = (dx * sideX) + (dz * fwdX);
    float mz = (dz * fwdZ ) + (dx * sideZ);
    //debugPrintf("MX (%f * %f) + (%f * %f) = %f\n", dx, sideX, dz, fwdX,  mx);
    //debugPrintf("MZ (%f * %f) + (%f * %f) = %f\n", dz, fwdZ,  dx, sideZ, mz);

    //update saved coords
    //this keeps the object from moving on its own
    //XXX is this needed since we set the flags?
    freeMoveCoords.x += mx;
    freeMoveCoords.z += mz;
    freeMoveCoords.y += dy;
    obj->pos.pos = freeMoveCoords;
    obj->pos.rotation.x += (s16)rx;
    obj->pos.rotation.y += (s16)ry;
    //OSReport("Move by %f %f %f to %f %f %f", dx, dy, dz,
    //    freeMoveCoords.x, freeMoveCoords.y, freeMoveCoords.z);

    //update object state.
    //why yes, this IS nuts. a lot of this can probably be removed.
    if(obj->catId == ObjCatId_Player) {
        void *state = obj->state;
        *(vec3f*)(state + 0x000C) = freeMoveCoords;
        *(vec3f*)(state + 0x0018) = freeMoveCoords;
        *(vec3f*)(state + 0x0024) = freeMoveCoords;
        *(vec3f*)(state + 0x003C) = freeMoveCoords;
        *(vec3f*)(state + 0x0048) = freeMoveCoords;
        *(vec3f*)(state + 0x0054) = freeMoveCoords;
        *(vec3f*)(state + 0x00B4) = freeMoveCoords;
        *(vec3f*)(state + 0x00E8) = freeMoveCoords;
        *(vec3f*)(state + 0x0118) = freeMoveCoords;
        *(float*)(state + 0x01BC) = freeMoveCoords.y;
        *(float*)(state + 0x01F4) = freeMoveCoords.y;
        *(s16*)  (state + 0x0478) = obj->pos.rotation.x;
    }
    obj->prevPos  = freeMoveCoords; //0x18
    obj->oldPos   = freeMoveCoords; //0x80
    obj->pos_0x8c = freeMoveCoords; //0x8C

    //force camera behind object
    vec3f vCam;
    vCam.y = obj->pos.pos.y;
    angleToVec2(obj->pos.rotation.x, &vCam.x, &vCam.z);
    vec3f_scale(FREE_MOVE_CAMERA_DISTANCE, &vCam, &vCam);
    pCamera->pos.pos = vCam;

    //force player state even if target object isn't player,
    //so that the player isn't running around while we're
    //moving some other object.
    if(pPlayer) {
        void *state = pPlayer->state;
        *(u16*)(state + 0x274) = 1;
    }

    //the camera doesn't update when time is stoped.
    //we need it to still follow us, though.
    if(timeStop) cameraUpdate(1);
}
