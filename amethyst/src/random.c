#include "main.h"

u32 rngCalls = 0; //reset each frame
u32 rngReseeds = 0;
s8 rngMode = RNG_MODE_NORMAL;
u32(*randIntHookOld)(u32 min, u32 max);

u32 rngHook() { //installed in perf.c
    rngCalls++;
    switch(rngMode) {
        case RNG_MODE_ZERO: return 0;
        case RNG_MODE_ONE:  return 1;
        case RNG_MODE_HALF: return 0x7FFFFFFF;
        case RNG_MODE_MAX:  return 0xFFFFFFFF;
        case RNG_MODE_INC:
            randomNumber++;
            return randomNumber;
        case RNG_MODE_FRAME: return frameCount;
        case RNG_MODE_ANALOG:
            return controllerStates[3].triggerRight << 24;
        default:
        //this is simple enough that it's easier to just duplicate here
        //rather than calling the original code
        randomNumber = randomNumber * 0x19660d + 0x3c6ef35f;
        //fixes a very rare (~1 in 112 million) chance where randomGetRange()
        //can actually return an out-of-bounds value
        if(randomNumber >= 0xFFFFFE80) {
            OSReport("Correcting bad RNG value 0x%08X", randomNumber);
            randomNumber = 0xFFFFFE7F;
        }
        return randomNumber;
    }
}

u32 randIntHook(u32 min, u32 max) {
    rngCalls++;
    switch(rngMode) {
        case RNG_MODE_ZERO: return min;
        case RNG_MODE_ONE:  return min+1;
        case RNG_MODE_HALF: return min + (((max+1)-min)/2);
        case RNG_MODE_MAX:  return max;
        case RNG_MODE_INC:
            return ((++randomNumber) % ((max+1) - min)) + min;
        case RNG_MODE_FRAME:
            return (frameCount % ((max+1) - min)) + min;
        case RNG_MODE_ANALOG: {
            if(max <= min) return min;
            u32 val = controllerStates[3].triggerRight;
            if(val > max) val = max;
            if(val < min) val = min;
            return val;
        }
        default: {
            if(max <= min) return min;
            randomNumber = randomNumber * 0x19660d + 0x3c6ef35f;
            //much simpler algorithm which gives a perfect uniform distribution
            //at least, for randomGetRange(0,255)
            u32 val = (randomNumber % ((max+1) - min)) + min;
            //game's algorithm which isn't quite perfectly uniform
            //and suffers from rounding errors
            //u32 val = ((float)randomNumber / 4294967296.0) * ((1.0 + (float)max) - (float)min) + min;
            //OSReport("rand(%6d, %6d) = %6d", min, max, val);
            return val;
        }
    }
}

void rngSeedHook(u32 seed) {
    rngReseeds++;
    //duplicate the code
    randomNumber = seed;
}

void drawRNG() {
    //graph the next several RNG values on-screen
    static const int w = 1, h = 100;
    int x = 20, y = SCREEN_HEIGHT - (h + 20) + (h/2);
    u32 start = randomNumber;
    u32 startCalls = rngCalls;
    for(int i=0; i<(SCREEN_WIDTH - 40) / w; i++) {
        u32 rnd = randomGetRange(0, h);
        if(rnd) {
            int half = h/2;
            int y2, c;
            if(rnd >= half) {
                y2 = y + (rnd-half);
                c  = (int)(((float)(y2 - y) / (float)half) * 255.0);
            }
            else {
                y2 = y - rnd;
                c  = (int)(((float)(y - y2) / (float)half) * 255.0);
            }
            Color4b col = {c, c, c, 0xFF};
            //hudDrawRect(x, y+h, x+w, (y+h)-rnd, &col);
            hudDrawRect(x, y, x+w, y2, &col);
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
