/** Utility function to draw a box on the screen.
 */
#include "main.h"

void drawBox(float x, float y, int w, int h, u8 opacity, bool fill) {
    //Draw a box on the screen.

    //debugPrintf("Box %f, %f, %d, %d, op %d\n", x, y, w, h, opacity);

    if(fill) {
        //XXX find a way to make this go *behind* the things drawn after it.
        Texture *tex = getLoadedTexture(0x307);
        //Texture *tex = getLoadedTexture(-0x8566);
        //Texture *tex = getLoadedTexture(-0x8663); //funky
        if(!tex) tex = hudTextures[HUD_TEXTURE_BOX_INTERIOR];
        if(tex) drawScaledTexture(x+BOX_BORDER_WIDTH, y+BOX_BORDER_HEIGHT,
            tex, opacity / 2,
            DRAW_SCALED_TEXTURE_SCALE_ONE,
            w-(BOX_BORDER_WIDTH*2), h-(BOX_BORDER_HEIGHT*2), 0);
    }

    //top
    drawScaledTexture(x+BOX_BORDER_WIDTH, y,
        hudTextures[HUD_TEXTURE_BOX_TOP], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        w-(BOX_BORDER_WIDTH*2), BOX_BORDER_HEIGHT, 0);

    //bottom
    drawScaledTexture(x+BOX_BORDER_WIDTH, y+h-BOX_BORDER_HEIGHT,
        hudTextures[HUD_TEXTURE_BOX_TOP], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        w-(BOX_BORDER_WIDTH*2), BOX_BORDER_HEIGHT, DRAW_SCALED_TEXTURE_FLIP_V);

    //left
    drawScaledTexture(x, y+BOX_BORDER_HEIGHT,
        hudTextures[HUD_TEXTURE_BOX_SIDE], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        BOX_BORDER_WIDTH, h-(BOX_BORDER_HEIGHT*2), 0);

    //right
    drawScaledTexture(x+w-BOX_BORDER_WIDTH, y+BOX_BORDER_HEIGHT,
        hudTextures[HUD_TEXTURE_BOX_SIDE], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        BOX_BORDER_WIDTH, h-(BOX_BORDER_HEIGHT*2), DRAW_SCALED_TEXTURE_FLIP_H);

    //top left
    drawTexture(x, y, hudTextures[HUD_TEXTURE_BOX_CORNER], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE);

    //top right
    drawScaledTexture(x+w-BOX_BORDER_WIDTH, y,
        hudTextures[HUD_TEXTURE_BOX_CORNER], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        BOX_BORDER_WIDTH, BOX_BORDER_HEIGHT, DRAW_SCALED_TEXTURE_FLIP_H);

    //bottom left
    drawScaledTexture(x, y+h-BOX_BORDER_HEIGHT,
        hudTextures[HUD_TEXTURE_BOX_CORNER], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        BOX_BORDER_WIDTH, BOX_BORDER_HEIGHT, DRAW_SCALED_TEXTURE_FLIP_V);

    //bottom right
    drawScaledTexture(x+w-BOX_BORDER_WIDTH, y+h-BOX_BORDER_HEIGHT,
        hudTextures[HUD_TEXTURE_BOX_CORNER], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        BOX_BORDER_WIDTH, BOX_BORDER_HEIGHT,
        DRAW_SCALED_TEXTURE_FLIP_H | DRAW_SCALED_TEXTURE_FLIP_V);
}
