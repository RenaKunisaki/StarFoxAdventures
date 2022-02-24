#include "main.h"
#include "revolution/os.h"

#define BIT_LOG_TIME 120
#define MAX_BIT_LOG 16
struct {
    u32 val;
    u32 time;
    u16 bit;
} bitLog[MAX_BIT_LOG];

static uint8_t *ignoreList = NULL;

static BOOL (*gameBitHook_replaced)();
BOOL gameBitHook(uint bit, int val) {
    //this doesn't work because the regs are overwritten by the compiler's
    //boilerplate code. we can probably work around that by using something
    //like attribute naked, but we don't need to, because r3 and r4 are still
    //intact, so we can just use parameters.
    //int bit, val;
    //GET_REGISTER(29, bit);
    //GET_REGISTER(30, val);

    if(mainGetBit(bit) == val || bit >= 0x7FFF) { //don't log if not changed
        return gameBitHook_replaced();
    }

    if(!ignoreList) {
        ignoreList = allocTagged(NUM_GAMEBITS, ALLOC_TAG_TEST_COL,
            "gamebit:ignore");
        registerFreeablePtr((void**)&ignoreList, "gamebit:ignore");
        if(ignoreList) {
            memset(ignoreList, 0, NUM_GAMEBITS);
        }
    }

    if(ignoreList) {
        if(ignoreList[bit] == 0xFF) {
            //don't log
            return gameBitHook_replaced();
        }
        else ignoreList[bit]++;
    }

    DPRINT("GameBit 0x%04X (%s) set to %d", bit, getBitName(bit), val);
    /* if(bit == 0x94E) {
        OSReport("BREAKPOINT");
        while(1) {
            waitNextFrame();
        }
    } */

    if(ignoreList && ignoreList[bit] == 0xFF) {
        DPRINT(" - ignoring further changes to this bit!");
    }

    //log the change
    bool found = false;
    for(int i=0; i<MAX_BIT_LOG; i++) {
        if(bitLog[i].bit == bit || bitLog[i].bit == 0xFFFF) { //update the entry
            bitLog[i].bit  = bit;
            bitLog[i].val  = val;
            bitLog[i].time = frameCount;
            found = true;
            break;
        }
    }

    if(!found) { //replace the oldest entry
        u32 oldest = 0xFFFFFFFF;
        int idx = 0;
        for(int i=0; i<MAX_BIT_LOG; i++) {
            if(bitLog[i].time < oldest) {
                oldest = bitLog[i].time;
                idx = i;
            }
        }
        bitLog[idx].bit  = bit;
        bitLog[idx].val  = val;
        bitLog[idx].time = frameCount;
    }

    return gameBitHook_replaced(); //BOOL isSaveGameLoading()
}

void gameBitHookInit() {
    gameBitHook_replaced = (BOOL(*)())hookBranch(0x8002010C, gameBitHook, 1);
    for(int i=0; i<MAX_BIT_LOG; i++) {
        bitLog[i].time = 0xFFFFFFFF;
        bitLog[i].bit  = 0xFFFF;
    }
}

void gameBitHookUpdate() {
    if(!(debugTextFlags & DEBUGTEXT_GAMEBIT_LOG)) return;
    u32 now = frameCount;
    for(int i=0; i<MAX_BIT_LOG; i++) {
        if(bitLog[i].bit != 0xFFFF && now - bitLog[i].time < BIT_LOG_TIME) {
            u8 t = 255 - ((now - bitLog[i].time) * 2);
            if(t < 32) t = 32;
            char color[6] = {0x81, t, t, t, t, 0};
            debugPrintf("%sBit " DPRINT_FIXED "%03X = %X %s"
                "\x81\xFF\xFF\xFF\xFF\n" DPRINT_NOFIXED,
                color, bitLog[i].bit, bitLog[i].val, getBitName(bitLog[i].bit));
        }
    }
}
