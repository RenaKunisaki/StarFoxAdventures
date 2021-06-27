#include "main.h"

u64 tLoopStart;   //start time of main loop
u64 tLoopEnd;     //end   time of main loop
u64 tLogicStart;  //start time of game logic section
u64 tLogicEnd;    //end   time of game logic section
u64 tRenderStart; //start time of render section
u64 tRenderEnd;   //end   time of render section
u64 tLoop, tLogic, tRender; //durations
float pctLogic;
float pctRender;
float pctTotal;

//if we can find an unused 8-byte field in ObjInstance (or just alloc a buffer)
//we could track logic/render times per object

void renderPerfMeters() {
    //draw meters
    //colors can't be const because hudDrawRect is insane and changes them
    Color4b colLogic  = {  0, 255,   0, 128};
    Color4b colRender = {255,   0,   0, 128};
    Color4b colIdle   = {  0,   0,   0, 128};
    int x = 40;
    int y = SCREEN_HEIGHT - 50;
    int w = SCREEN_WIDTH - (x * 2);
    int h = 4;
    hudDrawRect(x, y, x+(w*pctLogic), y+h, &colLogic);
    x += (w * pctLogic);
    if(x < w) hudDrawRect(x, y, x+(w*pctRender), y+h, &colRender);
    x += (w * pctRender);
    if(x < w) hudDrawRect(x, y, w, y+h, &colIdle);

    //draw scale
    Color4b colScale[] = {
        {  0, 255, 255, 128}, // 0- 24%
        {  0, 255,   0, 128}, //25- 49%
        {255, 255,   0, 128}, //50- 74%
        {255,   0,   0, 128}, //75-100%
    };
    x = 40;
    y += h;
    hudDrawRect(x,          y, (x + (w*0.25)) - 1, y+h, &colScale[0]);
    hudDrawRect(x+(w*0.25), y, (x + (w*0.50)) - 1, y+h, &colScale[1]);
    hudDrawRect(x+(w*0.50), y, (x + (w*0.75)) - 1, y+h, &colScale[2]);
    hudDrawRect(x+(w*0.75), y,  x +  w,            y+h, &colScale[3]);
}

void loopStartHook() {
    waitNextFrame(); //replaced
    tLoopStart = __OSGetSystemTime();
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

    //this will give percent of main thread time, not percent of frame time
    //double dTotal    = u64toDouble(tLoop);
    //double dLogic    = u64toDouble(tLogic);
    //double dRender   = u64toDouble(tRender);
    //double pctLogic  = dLogic  / dTotal;
    //double pctRender = dRender / dTotal;
    //double pctTotal  = pctLogic + pctRender;
    double totalSecs = ticksToSecs(tLoop);

    static const double frame = 1.0 / 60.0; //frame duration in seconds
    double sLogic  = ticksToSecs(tLogic);
    double sRender = ticksToSecs(tRender);
    pctLogic  = sLogic   / frame;
    pctRender = sRender  / frame;
    pctTotal  = (sLogic + sRender) / frame;
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
    hookBranch(0x80292dc0, rngHook, 0);
}