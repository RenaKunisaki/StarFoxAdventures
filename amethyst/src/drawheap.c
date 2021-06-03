/** Draw heap usage on screen.
 */
#include "main.h"

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
};

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
    for(int i=0; i<avail; i++) {
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
    for(int i=0; i<NUM_HEAPS; i++) drawHeap(i);
}
