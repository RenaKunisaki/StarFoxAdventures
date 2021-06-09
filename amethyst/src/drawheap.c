/** Draw heap usage on screen.
 */
#include "main.h"
#define NUM_TAGS 37

//extracted from default.dol
static const u32 allocTagColorTbl[] = {
    0x00000000, //"0"
    0xCECECEFE, //"LISTS_COL"
    0xEEEEEEFE, //"SCREEN_COL"
    0x7E7E7EFE, //"CODE_COL"
    0x8E8E8EFE, //"DLL_COL"
    0x00EE00FF, //"TRACK_COL"
    0xEE00EEFF, //"TEX_COL"
    0xEE00EECC, //"TRACKTEX_COL"
    0xEE00EE99, //"SPRITETEX_COL"
    0x00EEEEFF, //"MODELS_COL"
    0x008888FF, //"ANIMS_COL"
    0xEE0000FF, //"AUDIO_COL"
    0xEE0000DD, //"SEQ_COL"
    0xEE0000BB, //"SFX_COL"
    0x0000EEFF, //"OBJECTS_COL"
    0x88000099, //"CAM_COL"
    0x00880099, //"VOX_COL"
    0x4050FFFF, //"ANIMSEQ_COL"
    0xFFCC00FF, //"LFX_COL"
    0xFFAA00FF, //"GFX_COL"
    0xFF9900FF, //"EXPGFX_COL"
    0xFF7700FF, //"MODGFX_COL"
    0xFF5500FF, //"PROJGFX_COL"
    0xFF3300FF, //"SKY_COL"
    0x7F7F7F99, //"SHAD_COL"
    0x12345678, //"GAME_COL"
    0xFFDD00FF, //"TEST_COL"
    0x000000FF, //"BLACK"
    0xFF0000FF, //"RED"
    0x00FF00FF, //"GREEN"
    0x0000FFFF, //"BLUE"
    0x00FFFFFF, //"CYAN"
    0xFF00FFFF, //"MAGENTA"
    0xFFFF00FF, //"YELLOW"
    0xFFFFFFFF, //"WHITE"
    0x7F7F7FFF, //"GREY"
    0xFF7F7FFF, //"ORANGE"
    0xFFFFFFFF, //unknown (added here)
};

static const char *tagNames[] = {
    "0",
    "LISTS",
    "SCREEN",
    "CODE",
    "DLL",
    "TRACK",
    "TEX",
    "TRACKTEX",
    "SPRITETEX",
    "MODELS",
    "ANIMS",
    "AUDIO",
    "SEQ",
    "SFX",
    "OBJECTS",
    "CAM",
    "VOX",
    "ANIMSEQ",
    "LFX",
    "GFX",
    "EXPGFX",
    "MODGFX",
    "PROJGFX",
    "SKY",
    "SHAD",
    "GAME",
    "TEST",
    "BLACK",
    "RED",
    "GREEN",
    "BLUE",
    "CYAN",
    "MAGENTA",
    "YELLOW",
    "WHITE",
    "GREY",
    "ORANGE",
    "UNKNOWN",
    NULL
};

//last heap entry is total.
//last tag entry is unknown tags.
static int tagCounts[NUM_HEAPS+1][NUM_TAGS+1];
static u32 tagBytes [NUM_HEAPS+1][NUM_TAGS+1];

static void updateCounts() {
    //init counts
    memset(tagCounts,     0, sizeof(tagCounts));
    memset(tagBytes,      0, sizeof(tagBytes));

    for(int iHeap=0; iHeap<NUM_HEAPS; iHeap++) {
        SfaHeap *heap = &heaps[iHeap];
        u32 used = heap->used;
        for(u32 iEntry=0; iEntry<used; iEntry++) {
            SfaHeapEntry *entry = &heap->data[iEntry];
            if(!entry) break;

            u32 tag = entry->col;
            if(tag >= NUM_TAGS) {
                for(int iTag=0; iTag<NUM_TAGS; iTag++) {
                    if(allocTagColorTbl[iTag] == tag) {
                        tag = iTag;
                        break;
                    }
                }
                if(tag >= NUM_TAGS) tag = NUM_TAGS; //unknown tag
            }

            tagCounts[iHeap][tag]++;
            tagCounts[NUM_HEAPS][tag]++; //total
            tagBytes[iHeap][tag] += entry->size;
            tagBytes[NUM_HEAPS][tag] += entry->size; //total
        }
    }
}

static void drawCounts() {
    updateCounts();
    debugPrintf(DPRINT_FIXED "Tag      ");
    for(int iHeap=0; iHeap<NUM_HEAPS; iHeap++) {
        debugPrintf("|Blk%d KByte%d ", iHeap, iHeap);
    }
    debugPrintf("|Totals");

    for(int iTag=0; tagNames[iTag]; iTag++) {
        u32 color = allocTagColorTbl[iTag];
        u8 r = (color >> 24);
        u8 g = (color >> 16) & 0xFF;
        u8 b = (color >>  8) & 0xFF;
        u8 a =  color        & 0xFF;
        if(!r) r=1;
        if(!g) g=1;
        if(!b) b=1;
        if(!a) a=1;
        if(r+g+b < 0x40) {
            r |= 0x40;
            g |= 0x40;
            b |= 0x40;
        }

        //int y = iTag * 5;
        char fmt[] = {'\n',
            0x85, 1, 1, 1, 0xFF, //set BG color
            0x81, r, g, b, a, //set color
            //can't use this directly. need to write it into the buffer
            //manually since there are zeros.
            //default.dol had additional functions to do this.
            //0x82, (y >> 8) & 0xFF, y & 0xFF, 0, 0, //set position
            '%', '9', 's',
            0x81, 0xFF, 0xFF, 0xFF, 0xFF, //set color
            0x85, r, g, b, 0x40, //set BG color
            0};
        debugPrintf(fmt, tagNames[iTag]);
        for(int iHeap=0; iHeap<NUM_HEAPS+1; iHeap++) {
            debugPrintf("|%4d %6d ", tagCounts[iHeap][iTag],
                tagBytes[iHeap][iTag] / 1024);
        }
    }
    debugPrintf(DPRINT_NOFIXED
        DPRINT_BGCOLOR "\x01\x01\x01\xFF"
        "\n");
}

static void drawHeap(int iHeap) {
    int x = 16, y = 400 + (iHeap * 6);
    SfaHeap *heap = &heaps[iHeap];
    //OSReport("heap %d %d/%d\r\n", iHeap, heap->used, heap->avail);

    u32 used  = heap->used;
    u32 avail = heap->avail;
    int w = (avail > 300) ? 1 : 2;
    Color4b color = {0, 0, 0, 128};
    hudDrawRect(x, y, x+(avail*w), y+6, &color);

    //XXX an option to hide the unused slots entirely?
    //just means using i<used instead of i<avail
    for(u32 i=0; i<avail; i++) {
        SfaHeapEntry *entry = &heap->data[i];
        if(entry) {
            u32 col = entry->col;
            if(col <= 0x21) col = allocTagColorTbl[col];
            color.value = col;
            if(i >= used) color.a /= 8;
            hudDrawRect(x, y, x+w, y+6, &color);
        }
        x += w;
        if(x > SCREEN_WIDTH) break;
    }
}

void drawHeaps() {
    //drawCounts();
    for(int i=0; i<NUM_HEAPS; i++) drawHeap(i);
}
