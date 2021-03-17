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

    //press X on controller 3 to toggle free move
    static u16 prevButtons = 0;
    if((buttons & PAD_BUTTON_X) && !(prevButtons & PAD_BUTTON_X)) {
        OSReport("Toggle free move!");
    }
    prevButtons = buttons;
}
