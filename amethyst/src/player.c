#include "main.h"

PlayerStateFunc origClimbWallFn = NULL;

void initPlayerStatesHook(void) {
    playerInitFuncPtrs();
    origClimbWallFn = playerStateFuncs[PlayerStateEnum_ClimbWall];
    playerStateFuncs[PlayerStateEnum_ClimbWall] = playerStateClimbWallHook;
}

PlayerStateEnum playerStateClimbWallHook(double dT, ObjInstance *player, void *state) {
    //replaces the function pointer for the "climbing wall" player state.
    PlayerStateEnum result = origClimbWallFn(dT, player, state);

    aButtonIcon = 3; //jump

    if(buttonsJustPressed & PAD_BUTTON_A) { //jump down from the wall
        result = 1; //idle
        //prevent immediately regrabbing the wall.
        //this flag actually means we're in the water, so this can cause
        //occasionally switching into a swimming animation.
        //more importantly, it causes a glitch in areas where we're
        //climbing above flowing water - the current will actually push
        //us while we climb, even through walls.
        //hopefully only applying it while we drop will prevent that.
        (*(u8*)(state + 0x3F0)) |= 0x20;
    }
    return result;
}

void playerMainLoopHook() {
    //hold Z on controller 3 to fast forward
    static float prevSpeed = 0;
    u16 buttons = controllerStates[2].button;
    if(buttons & PAD_TRIGGER_Z) {
        if(prevSpeed == 0) {
            prevSpeed = physicsTimeScale;
        }
        physicsTimeScale = 180; //300% speed
    }
    else if(prevSpeed != 0) {
        physicsTimeScale = prevSpeed;
        prevSpeed = 0;
    }

    //limit aiming angle to prevent glitching when snapback is off
    void *playerState = pPlayer ? pPlayer->state : NULL;
    if(playerState) {
        u16 stateNo = *(u16*)(playerState + 0x274);
        if(stateNo == 0x2C) { //staff aiming
            float aimY = *(float*)(playerState + 0x7BC);
            if(aimY < -2.1) aimY = -2.1;
            if(aimY >  2.1) aimY =  2.1;
            *(float*)(playerState + 0x7BC) = aimY;
        }
    }

    //disable aiming snapback/interpolation as desired
    WRITE32(0x8029B0E8, bNoAimSnap    ? 0xFFE01090 : 0xEFE20028);
    WRITE32(0x8029B100, bNoAimSnap    ? 0xFFE01090 : 0xEFE0102A);
    WRITE32(0x8029B068, bNoAimSnap    ? 0x60000000 : 0xEC210028);
    WRITE32(0x8029B074, bSensitiveAim ? 0x60000000 : 0x4BD862FD);
    WRITE32(0x8029B0C4, bSensitiveAim ? 0x60000000 : 0x4BD862AD);
    iCacheFlush((void*)0x8029B0E8, 4);
    iCacheFlush((void*)0x8029B100, 4);
    iCacheFlush((void*)0x8029B068, 4);
    iCacheFlush((void*)0x8029B074, 4);
    iCacheFlush((void*)0x8029B0C4, 4);
}

void firstPersonHook(void *param1, void *param2) {
    //replaces a call to cameraCheckEnterFirstPerson()
    if(!bFreeMove) cameraCheckEnterFirstPerson(param1, param2);
}
