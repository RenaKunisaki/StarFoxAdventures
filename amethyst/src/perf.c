#include "main.h"

u64 tLoopStart;   //start time of main loop
u64 tLoopEnd;     //end   time of main loop
u64 tLogicStart;  //start time of game logic section
u64 tLogicEnd;    //end   time of game logic section
u64 tRenderStart; //start time of render section
u64 tRenderEnd;   //end   time of render section
u64 tLoop, tLogic, tRender; //durations

void loopStartHook() {
    tLoopStart = __OSGetSystemTime();
    waitNextFrame(); //replaced
}

void loopEndHook() {
    doQueuedLoads(); //replaced
    tLoopEnd = __OSGetSystemTime();

    //if(tLoopEnd < tLoopStart || tLogicEnd < tLogicStart || tRenderEnd < tRenderStart) {
    //    OSReport("ERROR: TIME TRAVEL DETECTED");
    //}
    tLoop   = tLoopEnd   - tLoopStart;
    tLogic  = tLogicEnd  - tLogicStart;
    tRender = tRenderEnd - tRenderStart;
}

void gameLogicStartHook() {
    tLogicStart = __OSGetSystemTime();
    padUpdate(); //replaced
}

void gameLogicEndHook() {
    mainLoopDoQueuedSounds(); //replaced
    tLogicEnd = __OSGetSystemTime();
}

void renderStartHook() {
    tRenderStart = __OSGetSystemTime();
    debugPrintDraw(); //replaced
        //actually is debugPrintDraw(0) but param is unused
}

void renderEndHook() {
    GXFlush_(1, 1); //replaced
    tRenderEnd = __OSGetSystemTime();
}

void perfMonInit() {
    hookBranch(0x80020c40, loopStartHook, 1);
    hookBranch(0x80020d70, loopEndHook, 1);
    hookBranch(0x80020c50, gameLogicStartHook, 1);   
    hookBranch(0x80020c6c, gameLogicEndHook, 1);   
    hookBranch(0x80020c74, renderStartHook, 1);   
    hookBranch(0x80020d60, renderEndHook, 1);   
}