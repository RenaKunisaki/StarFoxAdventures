#include "main.h"
#include "revolution/gx/GXEnum.h"

u32 rngCalls = 0; //reset each frame
u32 rngReseeds = 0;
u32 badVals = 0;
u32(*randIntHookOld)(u32 min, u32 max);
u16 *callCounts = NULL;
s8 rngMode = RNG_MODE_NORMAL;
static const int graphW = (SCREEN_WIDTH-40);

static void _logCall() {
    rngCalls++;
    if(!callCounts) return;
    callCounts[frameCount % 600]++;
}

u32 rngHook() {
     _logCall();
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
            badVals++;
            randomNumber = 0xFFFFFE7F;
        }
        return randomNumber;
    }
}

u32 randIntHook(u32 min, u32 max) {
    _logCall();
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
            if(randomNumber >= 0xFFFFFE80) badVals++;
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

void rngHooksInit() {
    hookBranch((u32)randomGetNext, rngHook, 0);
    hookBranch((u32)srand, rngSeedHook, 0);
    hookBranch((u32)randomGetRange, randIntHook, 0);
    registerFreeablePtr((void**)&callCounts, "rng:callCounts");
}

void drawRngRate() {
    //expects that begin2D() was already called
    if(!callCounts) {
        callCounts = allocTagged(600 * sizeof(u16),
            ALLOC_TAG_TEST_COL,  "rng:callCounts");
        if(!callCounts) return;
        memset(callCounts, 0, 600 * sizeof(u16));
    }

    Color4b col = {0xFF, 0x00, 0xFF, 0xFF};
    GXSetTevKColor_(0, &col);

    GXBegin(GX_LINESTRIP, 1, 599);
    for(int i=0; i<599; i++) {
        int idx = (frameCount-600)+i+1;
        if(idx < 0) idx += 600;
        float iY = SCREEN_HEIGHT - ((callCounts[idx%600]/3.0) + 100);
        if(iY < 20) iY = 20;
        write2Dvtx(i+20, iY);
    }
    callCounts[frameCount % 600] = 0;
}

void drawRNG() {
    //graph the next several RNG values on-screen
    static const int w = 1, h = 100;
    int x = 20, y = SCREEN_HEIGHT - (h + 20) + (h/2);

    Color4b col = {0xFF, 0xFF, 0xFF, 0xFF};
    begin2D(&col);
    GXBegin(GX_LINESTRIP, 1, graphW/w);

    u32 start = randomNumber;
    u32 startCalls = rngCalls;
    for(int i=0; i<graphW / w; i++) {
        u32 rnd = randomGetRange(0, h);
        int half = h/2;
        int y2;
        if(rnd >= half) y2 = y + (rnd-half);
        else y2 = y - rnd;
        write2Dvtx(x, y2);
        x += w;
    }
    randomNumber = start; //reset seed
    rngCalls = startCalls; //don't count these
    drawRngRate();
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
