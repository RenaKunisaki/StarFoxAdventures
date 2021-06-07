/** Draw global map grid on screen.
 */
#include "main.h"

static int offsX = SCREEN_WIDTH / 2, offsY = -(SCREEN_HEIGHT / 3);

Color4b _mapIdToColor(int id) {
    Color4b result;
    int n = id + 1;
    id = 0;
    for(int i=0; i<8; i++) id |= ((n >> i) & 1) << i;
    float r = ((id >> 1) & 3) / 3.0f;
    float g = ((id >> 3) & 7) / 7.0f;
    float b = ((id >> 6) & 3) / 3.0f;
    result.r = (int)(r*255.0f);
    result.g = (int)(g*255.0f);
    result.b = (int)(b*255.0f);
    return result;
}

static void drawMap(GlobalMaEntry *map, MapsTabEntry *tab,
MapsBinInfoEntry *mapsBin, float scale) {
    MapsBinInfoEntry *info = (MapsBinInfoEntry*)(tab->infoOffset + (u32)mapsBin);
    if(!PTR_VALID(info)) return;

    Color4b color = _mapIdToColor(map->mapId);
    int mx =  map->x - info->originX;
    int mz =  map->z - info->originZ;
    int x1 =  mx * scale;
    int y1 =  mz * scale;
    //add 1 because we need to cover the last row/col too
    int x2 = (mx + info->width  + 0) * scale;
    int y2 = (mz + info->height + 0) * scale;
    //OSReport("Map 0x%02X at %d, %d, %dx%d; %d, %d - %d, %d\n", map->mapId,
    //    map->x, map->z, info->width, info->height, x1, y1, x2, y2);

    //the previous maps I've shown have been flipped, oops
    y1 = SCREEN_HEIGHT - (y1+1);
    y2 = SCREEN_HEIGHT - y2;

    //draw the map's region
    color.a = 64;
    hudDrawRect(x1+offsX, y1+offsY, x2+offsX, y2+offsY, &color);
    //outline
    Color4b outline = {255, 255, 255, 128};
    hudDrawRect(x1+offsX, (y1-1)+offsY, x2+offsX, y1+offsY, &outline); //top
    hudDrawRect(x1+offsX, (y2+1)+offsY, x2+offsX, y2+offsY, &outline); //bot
    hudDrawRect((x1-1)+offsX, y1+offsY, x1+offsX, y2+offsY, &outline); //left
    hudDrawRect((x2-1)+offsX, y1+offsY, x2+offsX, y2+offsY, &outline); //right

    //draw the cells
    u32 *blocks = (u32*)(tab->blockTable + (u32)mapsBin);
    if(!PTR_VALID(blocks)) return;

    color.a = 192;
    for(int by=0; by<info->height; by++) {
        for(int bx=0; bx<info->width; bx++) {
            u32 block = blocks[(by * info->width) + bx];
            u32 mod   = (block >> 23) & 0xFF;
            //u32 sub   = (block >> 17) & 0x3F;
            if(mod != 0xFF) {
                int ry = info->height - (by+1);
                int rx1 = x1 + ( bx    * scale);
                int ry1 = y2 + ( ry    * scale);
                int rx2 = x1 + ((bx+1) * scale) - 1;
                int ry2 = y2 + ((ry+1) * scale) - 1;
                hudDrawRect(
                    rx1 + offsX, ry1 + offsY,
                    rx2 + offsX, ry2 + offsY,
                    &color);
            }
        }
    }
}

static void drawPoint(float wx, float wz, Color4b *color, float scale, int size) {
    int x = (wx * scale) / MAP_CELL_SCALE;
    int y = (wz * scale) / MAP_CELL_SCALE;
    y = SCREEN_HEIGHT - y;
    //horizontal
    hudDrawRect((x-size)+offsX, (y-1)+offsY, (x+size)+offsX, y+offsY, color);
    //vertical
    hudDrawRect((x-1)+offsX, (y-size)+offsY, x+offsX, (y+size)+offsY, color);
    //XXX find/make a "draw line" method and draw diagonals instead so they
    //don't blend into the map borders.
}

static void drawObjPos(ObjInstance *obj, Color4b *color, float scale) {
    float x1, y1, x2, y2, x3, y3;
    int   x    = (obj->pos.pos.x * scale) / MAP_CELL_SCALE;
    int   y    = (obj->pos.pos.z * scale) / MAP_CELL_SCALE;
    //no idea why this subtraction is necessary.
    //the game does similar in various places.
    float rx   = (float)(0x8000 - (int)obj->pos.rotation.x) * S16_TO_RADIANS;
    float sinR = sinf(rx);
    float cosR = cosf(rx);

    //draw the outline
    /* x1 = -sinR * scale * 2.1;
    y1 =  cosR * scale * 2.1;
    x2 = -cosR * scale * 0.6;
    y2 = -sinR * scale * 0.6;
    x3 =  cosR * scale * 0.6;
    y3 =  sinR * scale * 0.6;
    Color4b outline = {255, 0, 0, 0};
    hudDrawTriangle(
        (x+x1) + offsX, (SCREEN_HEIGHT - (y+y1)) + offsY,
        (x+x2) + offsX, (SCREEN_HEIGHT - (y+y2)) + offsY,
        (x+x3) + offsX, (SCREEN_HEIGHT - (y+y3)) + offsY,
        &outline); */

    //draw the actual triangle
    x1 = -sinR * scale * 2;
    y1 =  cosR * scale * 2;
    x2 = -cosR * scale * 0.5;
    y2 = -sinR * scale * 0.5;
    x3 =  cosR * scale * 0.5;
    y3 =  sinR * scale * 0.5;
    hudDrawTriangle(
        (x+x1) + offsX, (SCREEN_HEIGHT - (y+y1)) + offsY,
        (x+x2) + offsX, (SCREEN_HEIGHT - (y+y2)) + offsY,
        (x+x3) + offsX, (SCREEN_HEIGHT - (y+y3)) + offsY,
        color);
}

void drawMapGrid() {
    GlobalMaEntry    *mapGrid = dataFileBuffers[FILE_GLOBALMA_BIN];
    MapsTabEntry     *mapsTab = dataFileBuffers[FILE_MAPS_TAB];
    MapsBinInfoEntry *mapsBin = dataFileBuffers[FILE_MAPS_BIN];
    if(!(mapGrid && mapsTab && mapsBin)) return;

    int layer = curMapLayer;
    float scale = 4.0f;
    for(int i=0; mapGrid[i].mapId >= 0; i++) {
        GlobalMaEntry *map = &mapGrid[i];
        if(map->layer != layer) continue;
        MapsTabEntry *tab = &mapsTab[map->mapId];
        drawMap(map, tab, mapsBin, scale);
    }

    //draw characters' saved positions (not very useful)
    /* if(pLastSavedGame) {
        static Color4b colors[2] = {{255, 255, 255, 255}, {255, 255, 255, 255}};
        colors[0].r += 16;
        colors[1].b += 16;

        for(int i=0; i<2; i++) {
            PlayerCharPos *chrPos = &pLastSavedGame->charPos[i];
            if(chrPos->mapLayer == curMapLayer) {
                colors[i].a = 255;
                drawPoint(chrPos->pos.x, chrPos->pos.z, &colors[i], scale, 4);
            }
        }
    } */

    //draw respawn point (pink cross)
    SaveGame *restart = pRestartPoint;
    if(!restart) restart = pLastSavedGame;
    if(restart) {
        PlayerCharPos *chrPos = &restart->charPos[restart->character];
        if(chrPos->mapLayer == curMapLayer) {
            static Color4b cRespawn = {255, 255, 255, 255};
            cRespawn.g += 16;
            cRespawn.a = 255;
            drawPoint(chrPos->pos.x, chrPos->pos.z, &cRespawn, scale, 8);
        }
    }

    //draw Tricky position (red arrow)
    if(pTricky && pTricky->catId == ObjCatId_Tricky) {
        static Color4b cTricky = {0, 0, 0, 0};
        cTricky.r += 16;
        cTricky.g =   0;
        cTricky.b =   0;
        cTricky.a = 255; //hudDrawTriangle actually changes this!
        drawObjPos(pTricky, &cTricky, scale);
    }

    if(pPlayer) {
        if(pPlayer->catId == ObjCatId_Player) {
            void *pState = pPlayer ? pPlayer->state : NULL;
            if(pState) {
                //draw riding object (blue/green arrow)
                ObjInstance *ride = *(ObjInstance**)(pState + 0x7F0);
                if(ride) {
                    static Color4b cRide = {0, 0, 0, 0};
                    cRide.r =   0;
                    cRide.g = 255;
                    cRide.b += 16;
                    cRide.a = 255; //hudDrawTriangle actually changes this!
                    drawObjPos(ride, &cRide, scale);
                }
            }
        }

        //draw player position last, so it's above the others (white arrow)
        static Color4b color = {0, 0, 0, 0};
        color.r += 16; //just let these overflow.
        color.g += 16;
        color.b += 16;
        color.a = 255; //hudDrawTriangle actually changes this!
        drawObjPos(pPlayer, &color, scale);
    }
}
