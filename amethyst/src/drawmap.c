/** Draw global map grid on screen.
 */
#include "main.h"

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

void drawMapGrid() {
    GlobalMaEntry    *mapGrid = dataFileBuffers[FILE_GLOBALMA_BIN];
    MapsTabEntry     *mapsTab = dataFileBuffers[FILE_MAPS_TAB];
    MapsBinInfoEntry *mapsBin = dataFileBuffers[FILE_MAPS_BIN];
    if(!(mapGrid && mapsTab && mapsBin)) return;

    int layer = curMapLayer;
    int scale = 4;
    int offsX = SCREEN_WIDTH / 2, offsY = -(SCREEN_HEIGHT / 3);
    for(int i=0; mapGrid[i].mapId >= 0; i++) {
        GlobalMaEntry *map = &mapGrid[i];
        if(map->layer != layer) continue;
        MapsTabEntry *tab = &mapsTab[map->mapId];
        MapsBinInfoEntry *info = (MapsBinInfoEntry*)(tab->infoOffset + (u32)mapsBin);
        if(!PTR_VALID(info)) continue;

        Color4b color = _mapIdToColor(map->mapId);
        color.a = 128;
        int x1 =  map->x * scale;
        int y1 =  map->z * scale;
        int x2 = (map->x + info->width)  * scale;
        int y2 = (map->z + info->height) * scale;
        //OSReport("Map 0x%02X at %d, %d, %dx%d; %d, %d - %d, %d\n", map->mapId,
        //    map->x, map->z, info->width, info->height, x1, y1, x2, y2);

        //the previous maps I've shown have been flipped
        y1 = SCREEN_HEIGHT - y1;
        y2 = SCREEN_HEIGHT - y2;

        hudDrawRect(x1+offsX, y1+offsY, x2+offsX, y2+offsY, &color);
        //outline
        Color4b outline = {255, 255, 255, 128};
        hudDrawRect(x1+offsX, (y1-1)+offsY, x2+offsX, y1+offsY, &outline); //top
        hudDrawRect(x1+offsX, (y2+1)+offsY, x2+offsX, y2+offsY, &outline); //bot
        hudDrawRect((x1-1)+offsX, y1+offsY, x1+offsX, y2+offsY, &outline); //left
        hudDrawRect((x2-1)+offsX, y1+offsY, x2+offsX, y2+offsY, &outline); //right

        //XXX draw individual cells?
        //also for some reason this isn't quite accurate!?
    }

    //draw player position
    if(pPlayer) {
        static Color4b color = {0, 0, 0, 0};
        color.r += 16; //just let these overflow.
        color.g += 16;
        color.b += 16;
        color.a = 255; //hudDrawTriangle actually changes this!

        int   x    = (pPlayer->pos.pos.x * scale) / MAP_CELL_SCALE;
        int   y    = (pPlayer->pos.pos.z * scale) / MAP_CELL_SCALE;
        float rx   = (float)pCamera->pos.rotation.x * S16_TO_RADIANS;
        float sinR = sinf(rx);
        float cosR = cosf(rx);
        float x1   = -sinR * scale * 2;
        float y1   =  cosR * scale * 2;
        float x2   = -cosR * scale * 0.5;
        float y2   = -sinR * scale * 0.5;
        float x3   =  cosR * scale * 0.5;
        float y3   =  sinR * scale * 0.5;

        hudDrawTriangle(
            (x+x1) + offsX, (SCREEN_HEIGHT - (y+y1)) + offsY,
            (x+x2) + offsX, (SCREEN_HEIGHT - (y+y2)) + offsY,
            (x+x3) + offsX, (SCREEN_HEIGHT - (y+y3)) + offsY,
            &color);
    }

    //XXX draw respawn point?
}
