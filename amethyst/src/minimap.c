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
    static s16 prevX = 50, prevY = 440;
    int xpos = 50, ypos = 440, width = 128, height = 128;

    switch(minimapMode) {
        case MINIMAP_MAP: {
            if(minimapTexture) switch(overrideMinimapSize) {
                case MINIMAP_SIZE_SMALL:  xpos = 50; ypos = 440; width =  64; height = 64; break;
                //case MINIMAP_SIZE_NORMAL: xpos = 50; ypos = 440; size = 128; break;
                case MINIMAP_SIZE_BIG:    xpos =  2; ypos = 472; width = 256; height = 256; break;
                default: break;
            }
            else { xpos = 50; ypos = 440; width = 0; height = 0; }
            break;
        }

        case MINIMAP_INFO: {
            if(minimapInfoGameText != -1) { width = 200; height = 100; }
            else { width = 0; height = 0; }
            break;
        }

        default: break;
    }

    //every frame the game increases this by 1,
    //so this is changing one unit less than we write here.
    if     (minimapWidth  < width)  minimapWidth  = MIN(minimapWidth  + 8, width);
    else if(minimapWidth  > width)  minimapWidth  = MAX(minimapWidth  - 8, width);
    if     (minimapHeight < height) minimapHeight = MIN(minimapHeight + 8, height);
    else if(minimapHeight > height) minimapHeight = MAX(minimapHeight - 8, height);
    if(prevX < xpos) prevX = MIN(prevX + 8, xpos);
    if(prevX > xpos) prevX = MAX(prevX - 8, xpos);
    if(prevY < ypos) prevY = MIN(prevY + 8, ypos);
    if(prevY > ypos) prevY = MAX(prevY - 8, ypos);

    //override some hardcoded sizes
    //use prevX/Y to animate
    //XXX make this not hardcoded addresses
    WRITE16   (0x801324CA, minimapHeight); //height
    WRITE16   (0x801324D2, minimapHeight); //height
    WRITE16   (0x801325FA, minimapWidth); //widths
    WRITE16   (0x8013266A, prevY);
    WRITE16   (0x8013267A, prevX); //box X
    WRITE16   (0x801326A6, prevX); //box X clip
    WRITE16   (0x80132A92, prevX); //texture stuff
    WRITE16   (0x80132A96, prevX); //texture stuff
    WRITEFLOAT(0x803E2210, (float)xpos); //box clip offset
    iCacheFlush((void*)0x801324CA, 0x80132A98 - 0x801324CA);

    //override opacity
    if(overrideMinimapAlpha != 255) {
        minimapAlpha = overrideMinimapAlpha;
    }
    //debugPrintf("Map size=%dx%d pos=%d,%d tex=%08X\n",
    //    minimapWidth, minimapHeight, xpos, ypos, minimapTexture);
}
