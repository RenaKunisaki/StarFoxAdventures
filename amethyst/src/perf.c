#include "main.h"
#include "revolution/os.h"

u64 tLoopStart;   //start time of main loop
u64 tLoopEnd;     //end   time of main loop
u64 tLogicStart;  //start time of game logic section
u64 tLogicEnd;    //end   time of game logic section, start of audio
u64 tAudioEnd;    //end   time of audio section
u64 tAudioIrq;    //time spent between sndBegin/sndEnd
u64 tRenderStart; //start time of render section
u64 tRenderEnd;   //end   time of render section
u64 tLoop, tLogic, tAudio, tRender; //durations
//u32 audioIrqCnt;
float pctLogic, pctRender, pctAudio, pctTotal;

//if we can find an unused 8-byte field in ObjInstance (or just alloc a buffer)
//we could track logic/render times per object

void renderPerfMeters() {
    //draw meters
    static const Color4b colLogic  = {  0, 255,   0, 128};
    static const Color4b colAudio  = {  0,   0, 255, 128};
    static const Color4b colRender = {255,   0,   0, 128};
    static const Color4b colIdle   = {  0,   0,   0, 128};
    int x = 40;
    int y = SCREEN_HEIGHT - 50;
    int w = SCREEN_WIDTH - (x * 2);
    int h = 4;

    begin2D(NULL);
    draw2Dbox(x, y, w*pctLogic, h, &colLogic);
    x += (w * pctLogic);
    if(x < w) draw2Dbox(x, y, w*pctRender, h, &colRender);
    x += (w * pctRender);
    if(x < w) draw2Dbox(x, y, w*pctAudio, h, &colAudio);
    x += (w * pctAudio);
    if(x < w) draw2Dbox(x, y, w, h, &colIdle);

    //draw scale
    Color4b colScale[] = {
        {  0, 255, 255, 128}, // 0- 24%
        {  0, 255,   0, 128}, //25- 49%
        {255, 255,   0, 128}, //50- 74%
        {255,   0,   0, 128}, //75-100%
    };
    x = 40;
    y += h;
    draw2Dbox(x,          y, (w*0.25) - 1, h, &colScale[0]);
    draw2Dbox(x+(w*0.25), y, (w*0.25) - 1, h, &colScale[1]);
    draw2Dbox(x+(w*0.50), y, (w*0.25) - 1, h, &colScale[2]);
    draw2Dbox(x+(w*0.75), y, (w*0.25) - 1, h, &colScale[3]);
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
    tAudio  = tAudioEnd  - tLogicEnd; //tLogicEnd is also tAudioStart
    tAudio += tAudioIrq;
    tAudioIrq = 0;

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
    double sAudio  = ticksToSecs(tAudio);
    pctLogic  = sLogic   / frame;
    pctRender = sRender  / frame;
    pctAudio  = sAudio   / frame;
    pctTotal  = (sLogic + sRender + sAudio) / frame;
}

void gameLogicStartHook() {
    tLogicStart = __OSGetSystemTime();
    padUpdate(); //replaced
}

void gameLogicEndHook() {
    tLogicEnd = __OSGetSystemTime();
    mainLoopAudioUpdate(); //replaced
    mainLoopDoQueuedSounds(); //replaced
    tAudioEnd = __OSGetSystemTime();
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

static u64 sndBeginTime;
void sndBeginHook() {
    //replaced
    s16  sVar2 = irqDisableDepth + 1;
    bool bVar1 = irqDisableDepth == 0;
    irqDisableDepth = sVar2;
    if(bVar1) {
        audioPrevIrqFlags = OSDisableInterrupts();
        //don't use __OSGetSystemTime with interrupts disabled.
        sndBeginTime = OSGetTime();
        //audioIrqCnt++;
    }
}
void sndEndHook() {
    //replaced
    irqDisableDepth += -1;
    if(irqDisableDepth == 0) {
        tAudioIrq += (OSGetTime() - sndBeginTime);
        OSRestoreInterrupts(audioPrevIrqFlags);
    }
}

void perfMonInit() {
    hookBranch(0x80020c40, loopStartHook, 1);
    hookBranch(0x80020d70, loopEndHook, 1);
    hookBranch(0x80020c50, gameLogicStartHook, 1);
    WRITE_NOP (0x80020c68); //remove mainLoopAudioUpdate (we call it ourselves)
    hookBranch(0x80020c6c, gameLogicEndHook, 1);
    hookBranch(0x80020c74, renderStartHook, 1);
    hookBranch(0x80020d60, renderEndHook, 1);
    hookBranch((u32)sndBegin, sndBeginHook, 0);
    hookBranch((u32)sndEnd,   sndEndHook, 0);
}
