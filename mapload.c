//decompiled by Ghidra and tidied up.

    // DISPLAY WARNING: Type casts are NOT being printed

bool checkRect(MapRectStruct *rect, int idx) {
    int width = (rect.xMax - rect.xMin);
    int dist = (x - rect.xMin) + (y - rect.yMin);
    int flagIdx = dist * (width+1);
    int mask = 1 << (flagIdx & 7);
    int flag = piVar5[(flagIdx / 8) + (idx*0x40)];
    return (mask & flag) != 0;
}

int getWhichMapObjectsToLoad(int x,int y,int idx) {
    uint uVar2;

    int resIdx = 0;
    int count = 0x40;
    s16 *psVar3 = PTR_mapFlags_80382244;
    MapRectStruct *rects4 = PTR_mapFlags_8038223c;
        //idx 0 = start X, 1 = end X, 2 = start Y, 3 = end Y
        //5 = start X, 6 = end X, 7 = start Y, 8 = end Y
    int *piVar5 = PTR_mapVars_80382248;
    int load = mapNoToLoad_[idx];
    while (
        //psVar3[0] isn't what we want
        mapFlag_803dcde1 + load != psVar3[0]
        //or x is outside rects4[0]
        || x < rects4[0].xMin || rects4[0].xMax < x
        //or y is outside
        || y < rects4[0].yMin || rects4[0].yMax < y
        || !checkRect(rects4[0], 0) //or this isn't set
    ) {

        if (
            //this is the one we want
            mapFlag_803dcde1 + load == psVar3[1]
            //and x is in range
            && rects4[1].xMin <= x && x <= rects4[1].xMax
            //and y is in range
            && rects4[1].yMin <= y && y <= rects4[1].yMax
            && checkRect(rects4[1], 1) //and this is set
        ) {
            return resIdx + 1;
        }
        rects4 = rects4 + 2;
        piVar5 = piVar5 + 0x20;
        psVar3 = psVar3 + 1;
        resIdx += 2;
        count += -1;
        if (count == 0) {
            return -1;
        }
    } //while
    return resIdx;
}
