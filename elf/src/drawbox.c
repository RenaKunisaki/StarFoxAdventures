#include "main.h"

void drawBox(float x, float y, int w, int h, u8 opacity) {
    //Draw a box on the screen.

    //debugPrintf("Box %f, %f, %d, %d, op %d\n", x, y, w, h, opacity);

    //interior
    drawScaledTexture(x+BOX_BORDER_WIDTH, y+BOX_BORDER_HEIGHT,
        hudTextures[HUD_TEXTURE_BOX_INTERIOR], opacity,
        DRAW_SCALED_TEXTURE_SCALE_ONE,
        w-(BOX_BORDER_WIDTH*2), h-(BOX_BORDER_HEIGHT*2), 0);

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
