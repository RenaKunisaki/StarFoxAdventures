#include "main.h"

u32 rngCalls = 0; //reset each frame
u32 rngReseeds = 0;
s8 rngMode = RNG_MODE_NORMAL;

u32 rngHook() { //installed in perf.c
    rngCalls++;
    switch(rngMode) {
        case RNG_MODE_ZERO: return 0;
        case RNG_MODE_ONE:  return 1;
        case RNG_MODE_MAX:  return 0xFFFFFFFF;
        case RNG_MODE_INC:
            randomNumber++;
            return randomNumber;
        case RNG_MODE_ANALOG:
            return controllerStates[3].triggerRight << 24;
        default:
        //this is simple enough that it's easier to just duplicate here
        //rather than calling the original code
        randomNumber = randomNumber * 0x19660d + 0x3c6ef35f;
        //fixes a very rare (~1 in 100 million) chance where randomGetRange()
        //can actually return an out-of-bounds value
        if(randomNumber >= 0xFFFFFE80) randomNumber = 0xFFFFFE7F;
        return randomNumber;
    }
}

void rngSeedHook(u32 seed) {
    //80292de4
    rngReseeds++;
    //duplicate the code
    randomNumber = seed;
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
    debugPrintf("RNG" DPRINT_FIXED "%6d %08X %08X %08X %08X Reseeds: %d\n"
        DPRINT_NOFIXED, rngCalls,
        randomGetNext(), randomGetNext(), randomGetNext(), randomGetNext(),
        rngReseeds);
    randomNumber = start; //reset seed
}
