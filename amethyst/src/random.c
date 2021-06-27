#include "main.h"

u32 rngCalls = 0; //reset each frame

u32 rngHook() { //installed in perf.c
    rngCalls++;
    //this is simple enough that it's easier to just duplicate here
    //rather than calling the original code
    randomNumber = randomNumber * 0x19660d + 0x3c6ef35f;
    return randomNumber;
    //return 0;
}

void drawRNG() {
    //graph the next several RNG values on-screen
    static const int w = 1, h = 50;
    int x = 20, y = SCREEN_HEIGHT - (h + 20);
    u32 start = randomNumber;
    u32 startCalls = rngCalls;
    for(int i=0; i<(SCREEN_WIDTH - 40) / w; i++) {
        //u32 rnd = randomGetNext();
        //XXX this scale discards a lot of detail. any better way?
        //u32 sz = ((double)rnd / 4294967295.0) * (double)h;
        u32 rnd = randomGetRange(0, h);
        u32 sz = rnd;
        if(sz) {
            //Color4b col = {(rnd >> 24) & 0xFF, (rnd >> 16) & 0xFF, (rnd >> 8) & 0xFF, 0xFF};
            u8 c = (u8)(((double)rnd / (double)h) * 255.0);
            Color4b col = {c, c, c, 0xFF};
            hudDrawRect(x, y+h, x+w, (y+h)-sz, &col);
        }
        x += w;
    }
    randomNumber = start; //reset seed
    rngCalls = startCalls; //don't count these
}

void printRNG() {
    //print RNG call rate and next few values
    u32 start = randomNumber;
    debugPrintf("RNG" DPRINT_FIXED "%6d %08X %08X %08X %08X\n" DPRINT_NOFIXED, rngCalls,
        randomGetNext(), randomGetNext(), randomGetNext(), randomGetNext());
    randomNumber = start; //reset seed
}