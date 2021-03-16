/** Change position/size/opacity of the minimap.
 */
#include "main.h"

u8 overrideMinimapSize  = MINIMAP_SIZE_NORMAL;
u8 overrideMinimapAlpha = 255;

void minimapMainLoopHook() {
    //override zoom range
    minimapMaxZoom = 2;
    minimapMinZoom = 0.05;

    //override the minimap size and position
    static u8 prevSize = 0;
    int xpos, ypos, size;
    switch(overrideMinimapSize) {
        case MINIMAP_SIZE_SMALL:  xpos = 50; ypos = 440; size =  64; break;
        case MINIMAP_SIZE_NORMAL: xpos = 50; ypos = 440; size = 128; break;
        case MINIMAP_SIZE_BIG:    xpos =  2; ypos = 472; size = 256; break;
    }
    if(minimapTexture) minimapWidth = size;
    else {
        //if no map, hide the "no map data" message
        if(minimapWidth > 2) {
            //every frame the game increases this by 1,
            //so this shrinking is one unit less than we write here.
            if(overrideMinimapSize == MINIMAP_SIZE_BIG) minimapWidth -= 10;
            else minimapWidth -= 5;
        }
        size = minimapWidth;
    }
    if(prevSize != size) {
        //override some hardcoded sizes
        //XXX make this not hardcoded addresses
        WRITE16   (0x801324CA, size); //height
        WRITE16   (0x801324D2, size); //height
        WRITE16   (0x801325FA, size); //width
        WRITE16   (0x8013266A, ypos);
        WRITE16   (0x8013267A, xpos); //box X
        WRITE16   (0x801326A6, xpos); //box X clip
        WRITE16   (0x80132A92, xpos); //texture stuff
        WRITE16   (0x80132A96, xpos); //texture stuff
        WRITEFLOAT(0x803E2210, (float)xpos); //box clip offset
        iCacheFlush((void*)0x801324CA, 0x80132A98 - 0x801324CA);
        prevSize = size;
    }
    minimapHeight = minimapWidth;

    //override opacity
    if(overrideMinimapAlpha != 255) {
        minimapAlpha = overrideMinimapAlpha;
    }
    //debugPrintf("Map size=%d pos=%d,%d tex=%08X\n",
    //    minimapWidth, xpos, ypos, minimapTexture);
}
