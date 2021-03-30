#include "main.h"

#define BSOD_LINE_HEIGHT 11

static void clearFrameBuffer(void *buf) {
    //framebuffer is YUYV
    //Y=29.07, U=255.5, V=107.26595 should be dark blue
    //but I have no idea so black it is
    u32 *_buf = (u32*)buf;
    for(int i=0; i<(640*480)>>1; i++) {
        _buf[i] = 0x10801080; //black
    }
}

static void drawPageGeneral() {
    debugPrintfxy(16, 30,
        "       --- FLAGRANT SYSTEM ERROR ---\n"
        "Please send photos/screenshots of each page to\n"
        "       hyperhacker at gmail dot com\n"
        "    along with your save file if possible.");

    //show error code and PC
    const char *errMsg = "";
    switch(bsodErrorCode) {
        //save a bit of memory by reusing existing strings in the game
        case 0x1: errMsg = (const char*)0x8031d1d0; break; //"machine check"
        case 0x2: errMsg = (const char*)0x803dbc18; break; //"DSI"
        case 0x3: errMsg = (const char*)0x803dbc1c; break; //"ISI"
        case 0x5: errMsg = (const char*)0x8031d1e0; break; //"alignment"
        case 0xB: errMsg = (const char*)0x8031d1ec; break; //"performance monitor"
        case 0xD: errMsg = (const char*)0x8031d200; break; //"system management interrupt"
        case 0xF: errMsg = (const char*)0x8031d21c; break; //"memory protection error"
        default:  errMsg = (const char*)0x8031d234; break; //"unknown error";
    }
    debugPrintfxy(26, 85, "PC %08X ERR %X %s", bsodCtx->srr0,
        bsodErrorCode, errMsg);

    //show player coords
    if(PTR_VALID(pPlayer)) {
        debugPrintfxy(26, 96, "P %08X %08X %08X %08X", pPlayer,
            (int)pPlayer->pos.pos.x, (int)pPlayer->pos.pos.y,
            (int)pPlayer->pos.pos.z);
    }

    //show map and camera info
    debugPrintfxy( 26, 107, "CM %02X ACT %02X OBJ %04X MAPS %02X %02X %02X %02X",
        cameraMode, curMapAct, numLoadedObjs,
        loadedFileMapIds[FILE_MODELS_BIN]  & 0xFF,
        loadedFileMapIds[FILE_MODELS_BIN2] & 0xFF,
        //buckets are int, but we really only need lowest byte.
        levelLockBuckets[0] & 0xFF, levelLockBuckets[1] & 0xFF);

    //show GPRs
    debugPrintfxy(26, 122, (const char*)0x8031d270); //"General purpose registers"
    for(int i=0; i<32; i += 4) {
        debugPrintfxy(26, 133 + (BSOD_LINE_HEIGHT*(i>>2)), "%08X %08X %08X %08X",
            bsodCtx->gpr[i  ], bsodCtx->gpr[i+1],
            bsodCtx->gpr[i+2], bsodCtx->gpr[i+3]);
    }

    //show stack dump
    debugPrintfxy(26, 228, "STACK DUMP");
    u32 *sp = (u32*)bsodCtx->gpr[1];
    for(int i=0; i<16; i++) {
        if(!PTR_VALID(sp)) break;
        debugPrintfxy(26, 239+(i*BSOD_LINE_HEIGHT), "%08X %08X %08X %08X",
            sp[0], sp[-1], sp[-2], sp[-3]);
        sp = &sp[-4];
    }

    //show stack trace
    debugPrintfxy(440, 122, (const char*)0x8031d24A); //"trace"
    sp = (u32*)bsodCtx->gpr[1];
    int y = 133;
    while(PTR_VALID(sp) && y < 480 - BSOD_LINE_HEIGHT) {
        debugPrintfxy(440, y, "%08X", *sp);
        sp = (u32*)*sp;
        y += BSOD_LINE_HEIGHT;
    }

    //show instructions
    debugPrintfxy(26, 430, "A:NEXT B:PREV START:RESET");
}

static void drawPageMemory() {
    //show heap info
    debugPrintfxy(26, 20, "HEAP ADR/B USED/B TOTL/BLKU/BLKT");
    for(int i=0; i<NUM_HEAPS; i++) {
        debugPrintfxy(26, 31+(i*BSOD_LINE_HEIGHT), "%08X %06X %06X %04X %04X",
            &heaps[i],
            heaps[i].size, heaps[i].dataSize, heaps[i].used, heaps[i].avail);
    }

    //show alloc fail log
    debugPrintfxy(26, 86,
        "Emerg frees: %d; alloc fails: %d\n", emergFreeCount, allocFailCount);
    debugPrintfxy(26, 97, "FailSize FailTag");
    for(int i=0; i<MIN(allocFailCount, ALLOC_FAIL_LOG_SIZE); i++) {
        debugPrintfxy(26, 108 + (i*BSOD_LINE_HEIGHT), "%c %08X %08X",
            (i == allocFailLogIdx) ? '*' : ' ',
            allocFailLog[i].size, allocFailLog[i].tag);
    }
}

static void drawPagePosLog() {
    debugPrintfxy(26, 20, "PLAYER POS LOG");
    int x=26, y=31, idx=playerPrevPosIdx;
    for(int i=0; i<PLAYER_POS_LOG_SIZE; i++) {
        debugPrintfxy(x, y, "%6d %6d %6d %06X",
            (int)playerPrevPositions[idx].pos.x,
            (int)playerPrevPositions[idx].pos.y,
            (int)playerPrevPositions[idx].pos.z,
            playerPrevPositions[idx].time);
        idx++;
        if(idx >= PLAYER_POS_LOG_SIZE) idx = 0;
        if(i == PLAYER_POS_LOG_SIZE / 2) {
            x = 340;
            y =  31;
        }
        else y += BSOD_LINE_HEIGHT;
    }
}

static void drawFileTable(int start) {
    int x=26, y=31;
    debugPrintfxy(26, 20, "FILE TABLE");
    for(int i=0; i<76; i++) {
        int idx = i + start;
        if(idx >= NUM_FILES) break;
        debugPrintfxy(x, y, "%02X %02X %08X %08X", idx,
            loadedFileMapIds[idx] & 0xFF, dataFileBuffers[idx], dataFileSize[idx]);
        y += BSOD_LINE_HEIGHT;
        if(i == 37) {
            x = 320;
            y =  31;
        }
    }
}

static void drawPageFiles1() {
    drawFileTable(0);
}

static void drawPageFiles2() {
    drawFileTable(76);
}

static void drawHexDump(void *data, int nBytes, int xPos, int yPos) {
    u32 *d = (u32*)data;
    for(int i=0; i<nBytes; i += 16) {
        debugPrintfxy(xPos, yPos, "%08X %08X %08X %08X", d[0], d[1], d[2], d[3]);
        d = &d[4];
        yPos += BSOD_LINE_HEIGHT;
    }
}

static void drawPageBits1() {
    debugPrintfxy(26, 20, "BIT TABLES");
    drawHexDump(tempGameBits, 128, 26,  31);
    drawHexDump(pLastSavedGame->gameBits1, 116, 26, 130);
    drawHexDump(pLastSavedGame->gameBits3, 172, 26, 229);
}

static void drawPageBits2() { //too big for other page
    debugPrintfxy(26, 20, "BIT TABLES");
    drawHexDump(pLastSavedGame->gameBits2, 324, 26, 31);
}

void bsodHook(void) {
    //replace BSOD thread main function.
    //this thread only runs when the game crashes.

    //XXX why is this causing a relocation error?
    //u32 msr = OSDisableInterrupts();
    debugFrameBuffer[0] = pFrameBuffer[0];
    debugFrameBuffer[1] = pFrameBuffer[1];

    //disable some video functions so we can use the framebuffer
    videoSetBufferSwapCb(NULL);
    setGpuErrorHandler(NULL);
    videoSetCallback_80256460(NULL);
    gxWaitFn_80258330();
    //OSRestoreInterrupts(msr);

    int page = 0;
    int padDelay = 60;
    while(true) {
        if(padDelay) padDelay--;
        else {
            //padReadControllers returns all zeros if done too often
            padDelay = 20;
            padReadControllers(controllerStates);
            if(controllerStates[0].button & PAD_BUTTON_A) page++;
            if(controllerStates[0].button & PAD_BUTTON_B) page--;
            if(controllerStates[0].button & PAD_BUTTON_MENU) {
                reset(0, 0x80000000, 0);
            }
        }

        clearFrameBuffer(debugFrameBuffer[0]);
        switch(page) {
            case  7: page = 0; //fall thru
            case  0: drawPageGeneral(); break;
            case  1: drawPageMemory(); break;
            case  2: drawPagePosLog(); break;
            case  3: drawPageFiles1(); break;
            case  4: drawPageFiles2(); break;
            case  5: drawPageBits1(); break;
            case -1: page = 6; //fall thru
            case  6: drawPageBits2(); break;
        }

        debugPrintfxy(610, 438, "P%d", page);

        //swap buffers
        dCacheStore(debugFrameBuffer[0], 640*480*2);
        void *buf = debugFrameBuffer[0];
        debugFrameBuffer[0] = debugFrameBuffer[1];
        debugFrameBuffer[1] = buf;
        VISetNextFrameBuffer(buf);
        viFn_8024d554(); //XXX what is this?
        VIWaitForRetrace();
    }
}
