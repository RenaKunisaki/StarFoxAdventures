/** Draw heap usage on screen.
 */
#include "main.h"
#define BYTE_SCALE 4096
#define GRAPH_HEIGHT 6
u8 heapDrawMode = HEAP_DRAW_NONE;

//extracted from default.dol
const u32 allocTagColorTbl[] = {
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
static int tagCounts[NUM_HEAPS+1][NUM_ALLOC_TAGS+1];
static u32 tagBytes [NUM_HEAPS+1][NUM_ALLOC_TAGS+1];

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
            if(tag >= NUM_ALLOC_TAGS) {
                for(int iTag=0; iTag<NUM_ALLOC_TAGS; iTag++) {
                    if(allocTagColorTbl[iTag] == tag) {
                        tag = iTag;
                        break;
                    }
                }
                if(tag >= NUM_ALLOC_TAGS) tag = ALLOC_TAG_UNKNOWN;
            }

            tagCounts[iHeap][tag]++;
            tagCounts[NUM_HEAPS][tag]++; //total
            tagBytes[iHeap][tag] += entry->size;
            tagBytes[NUM_HEAPS][tag] += entry->size; //total
        }
    }
}

static void drawCounts(bool all) {
    //draw color legend, block count, and Kbyte count of each tag.
    //takes up the whole damn screen.
    updateCounts();
    debugPrintf(DPRINT_FIXED "Tag      ");
    if(all) {
        for(int iHeap=0; iHeap<NUM_HEAPS; iHeap++) {
            debugPrintf("|Blk%d KByte%d ", iHeap, iHeap);
        }
    }
    debugPrintf("|BlkTotl KBtotl");

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
        if(r+g+b < 0x80) {
            r |= 0x80;
            g |= 0x80;
            b |= 0x80;
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
        for(int iHeap=all ? 0 : NUM_HEAPS; iHeap<NUM_HEAPS+1; iHeap++) {
            debugPrintf("|%4d %6d ", tagCounts[iHeap][iTag],
                tagBytes[iHeap][iTag] / 1024);
        }
    }
    debugPrintf(DPRINT_NOFIXED
        DPRINT_BGCOLOR "\x01\x01\x01\xFF"
        "\n");
}

static void drawHeapBlocks(int iHeap) {
    //draw graph showing tag color of each heap block.
    int x = 16, y = 400 + (iHeap * GRAPH_HEIGHT);
    SfaHeap *heap = &heaps[iHeap];
    //OSReport("heap %d %d/%d\r\n", iHeap, heap->used, heap->avail);

    u32 used  = heap->used;
    u32 avail = heap->avail;
    int w = (avail > 300) ? 1 : 2;
    Color4b color = {0, 0, 0, 128};
    begin2D(NULL);
    draw2Dbox(x, y, (avail*w), GRAPH_HEIGHT, &color);

    //XXX an option to hide the unused slots entirely?
    //just means using i<used instead of i<avail
    for(u32 iBlock=0; iBlock<avail; iBlock++) {
        SfaHeapEntry *entry = &heap->data[iBlock];
        if(entry) {
            u32 col = entry->col;
            if(col < NUM_ALLOC_TAGS) col = allocTagColorTbl[col];
            color.value = col;
            if(iBlock >= used) color.a /= 8;
            draw2Dbox(x, y, w, GRAPH_HEIGHT, &color);
        }
        x += w;
        if(x > SCREEN_WIDTH) break;
    }
}

static void drawHeapBytes(int iHeap) {
    //draw graph showing tag color of each heap block scaled to its size.
    if(iHeap == 0) return; //XXX game doesn't fill in size correctly...
    int x = 16, y = 400 + (iHeap * GRAPH_HEIGHT);
    SfaHeap *heap = &heaps[iHeap];
    //OSReport("heap %d %d/%d\r\n", iHeap, heap->used, heap->avail);

    u32 used  = heap->size;
    u32 avail = heap->dataSize;
    float scale = (float)(SCREEN_WIDTH-(x*2)) / ((float)avail / BYTE_SCALE);
    int w;
    Color4b color = {0, 0, 0, 128};
    begin2D(NULL);
    draw2Dbox(x, y, (SCREEN_WIDTH-(x*2)), GRAPH_HEIGHT, &color);

    u32 total = 0, totalBlocks = 0;
    for(int iBlock=0; iBlock >= 0; iBlock = heap->data[iBlock].next) {
        SfaHeapEntry *entry = &heap->data[iBlock];
        if(entry && entry->type != HEAP_ENTRY_TYPE_FREE) {
            u32 col = entry->col;
            if(col < NUM_ALLOC_TAGS) col = allocTagColorTbl[col];
            color.value = col;
            //if(iBlock >= heap->used) color.a /= 8;
            //else total += entry->size;
            if(entry->loc) {
                total += entry->size;
                totalBlocks++;
            }
            else color.a /= 8;
            w = ((float)entry->size / BYTE_SCALE) / scale;
            draw2Dbox(x, y, w, GRAPH_HEIGHT, &color);
        }
        x += w;
        if(x > SCREEN_WIDTH) break;
    }

    //this shows that the correct usage values seem to belong to the NEXT heap.
    //no idea why. smells like a bug since even the game's original debug prints
    //give wrong values.
    //debugPrintf("heap %d used %d (%d) / %d blocks", iHeap,
    //    heap->used, totalBlocks, heap->avail);
    //debugPrintf(" %d (%d) / %d KB\n",
    //    heap->size/1024, total/1024, heap->dataSize/1024);
}

void printHeaps() {
    //print heap table to stdout
    OSReport(";heap;block;loc;size;type;prev;next;stack;col;lr;id\n");
    for(int iHeap=0; iHeap<NUM_HEAPS; iHeap++) {
        SfaHeap *heap = &heaps[iHeap];
        for(u32 iBlock=0; iBlock<heap->used; iBlock++) {
            SfaHeapEntry *block = &heap->data[iBlock];
            OSReport(";%d;%d;0x%08X;%d;%d;%d;%d;%d;0x%08X;0x%08X;%d\n",
                iHeap, iBlock, block->loc, block->size, block->type,
                block->prev, block->next, block->stack, block->col,
                block->unk14, block->mmUniqueIdent);
        }
    }
}

void drawHeaps() {
    switch(heapDrawMode) {
        case HEAP_DRAW_BLOCK_GRAPH: {
            for(int i=0; i<NUM_HEAPS; i++) drawHeapBlocks(i);
            break;
        }

        case HEAP_DRAW_BYTE_GRAPH: {
            for(int i=0; i<NUM_HEAPS; i++) drawHeapBytes(i);
            break;
        }

        case HEAP_DRAW_TOTALS: drawCounts(false); break;
        case HEAP_DRAW_TABLE:  drawCounts(true); break;

        default: return;
    }
}
