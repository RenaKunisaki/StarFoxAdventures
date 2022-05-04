/** Debug Warp submenu.
 */
#include "main.h"
#define WARP_MENU_XPOS 32
#define WARP_MENU_YPOS 32
#define WARP_MENU_WIDTH  (SCREEN_WIDTH  - (WARP_MENU_XPOS * 2))
#define WARP_MENU_HEIGHT (SCREEN_HEIGHT - (WARP_MENU_YPOS * 2))
#define WARP_MENU_NUM_LINES ((WARP_MENU_HEIGHT / LINE_HEIGHT) - 2)

typedef struct {
    s16 x, y, z;
    s8 layer;
} WarpMenuItem;

//the names need to be separate or else the coords will get placed in the GOT
//and will get relocated which will corrupt them.
//Z coords are divided by 2 because one entry has a value > 32767.
WarpMenuItem warps[] = {
    { -5774,   -765,  -2542/2,  0}, //"ThornTail Hollow - Outside",
    { -5517,   -636,   -800/2,  0}, //"ThornTail Hollow - Egg Cave",
    { -6664,  -1086,  -2253/2, -1}, //"ThornTail Hollow - Bottom",
    { -6069,   -980,  -2752/2,  1}, //"ThornTail Store",
    { 12264,    211,    451/2,  0}, //"Krazoa Palace - Starting Area",
    { 11840,   -245,   5864/2,  0}, //"Krazoa Palace - WarpStone Pad",
    { 13154,    -83,   2880/2,  0}, //"Krazoa Palace - Inside",
    { 13201,   1278,   2239/2,  0}, //"Krazoa Palace - Final Boss Warp",
    { 12188,   1379,   2461/2,  0}, //"Krazoa Palace - Top",
    {  3584,   6397,   4374/2,  0}, //"Ice Mountain - Top",
    { 1068,     226,   3179/2,  0}, //"Ice Mountain - Bottom",
    { -4159,   -521,   4159/2,  0}, //"SnowHorn Wastes - Shrine Warp",
    { -2634,   -130,   2167/2,  0}, //"SnowHorn Wastes - Water Spout",
    { -5408,   -822,   1610/2,  0}, //"SnowHorn Wastes - Entrance",
    {  -511,    -41,   2951/2,  0}, //"SnowHorn Wastes - Bottom",
    { -7378,  -1557,   8935/2,  0}, //"DarkIce Mines - Landing Pad",
    { -7957,  -1177,  14025/2,  0}, //"DarkIce Mines - SharpClaw Fort",
    {-10036,   -775,  14822/2,  0}, //"DarkIce Mines - Bike Start",
    { -9075,  -1713,  17999/2, -2}, //"DarkIce Mines - Interior",
    {-10460,  -2025,  17882/2, -2}, //"DarkIce Mines - Ice Section",
    { -7949,  -2627,  17001/2, -2}, //"DarkIce Mines - Bottom",
    {-11923,   -268,   -331/2,  0}, //"Moon Mountain Pass - Hut",
    {-11839,     52,  -4791/2,  0}, //"Moon Mountain Pass - Top",
    {-13440,   -111,   -381/2,  0}, //"Moon Mountain Pass - Volcano Entrance",
    { -8902,   -925,   -313/2,  0}, //"Moon Mountain Pass - Bottom",
    { -1601,  -1216,   -960/2,  0}, //"LightFoot Village - Shrine Warp",
    { -1299,   -814,  -1932/2,  0}, //"LightFoot Village - Front",
    { -3150,   -895,  -1758/2,  0}, //"LightFoot Village - Back",
    {  1309,  -1201,  -4317/2,  0}, //"Cape Claw",
    {   829,   1302, -16477/2,  0}, //"CloudRunner Fortress - Dungeon",
    {  4125,   1464, -18232/2,  0}, //"CloudRunner Fortress - Entrance",
    {   233,   2196, -17591/2,  0}, //"CloudRunner Fortress - Queen Cage",
    {  -173,   1284, -14400/2,  0}, //"CloudRunner Race Start",
    {   463,   1845, -16899/2,  0}, //"CloudRunner Mine",
    {  1534,  -2536,  -8574/2, -1}, //"Ocean Force Point - Bottom",
    {  4220,  -2536,  -8576/2, -1}, //"Ocean Force Point - End",
    {  3198,  -2542,  -7981/2, -1}, //"Ocean Force Point - SpellStone Platform",
    {  3357,  -1600,  -7428/2,  0}, //"Ocean Force Point - Entrance",
    {  2872,  -1533,  -9247/2,  0}, //"Ocean Force Point - Top Warp",
    {  2877,  -2718,  -9277/2, -1}, //"Ocean Force Point - Bottom Warp",
    {-16065,     57,   -320/2,  0}, //"Volcano Force Point - Entrance",
    {-18710,    389,   -307/2,  0}, //"Volcano Force Point - Cave Entrance",
    {-17919,    198,   -975/2,  0}, //"Volcano Force Point - SpellStone Platform",
    {-18648,    -45,    499/2,  0}, //"Volcano Force Point - SpellStone Platform 2",
    {-18564,    293,    647/2,  0}, //"Volcano Force Point - End Warp",
    {-16322,   -777, -13271/2,  0}, //"Walled City - Front of Pyramid",
    {-16322,  -1209, -13518/2,  0}, //"Walled City - Boss Door",
    {-13753,  -1714,  11227/2,  0}, //"Dragon Rock - Landing Pad",
    {-17799,  -1764,   9032/2,  0}, //"Dragon Rock - CloudRunner Cage",
    { 14079,   -204, -17065/2,  0}, //"Test of Combat",
    { 17919,   -204, -10669/2,  0}, //"Test of Strength",
    { 17919,   -204, -14511/2,  0}, //"Test of Knowledge",
    { 10240,   -203, -18354/2,  0}, //"Test of Fear",
    { 13440,   -204, -13225/2,  0}, //"Test of Skill",
    { -9555,  -2973,  20457/2, -2}, //"Boss: Galdon",
    { -7954,  -2978,  20162/2, -2}, //"Boss: Galdon - Inside",
    {  7694,   -199,  25943/2,  1}, //"Boss: Drakor",
    {-10138,      0, -13381/2,  0}, //"Boss: T-Rex",
    {  7039,     58, -13887/2,  0}, //"Boss: General Scales",
    {-11850,    131, -15476/2,  2}, //"Boss: Andross",
    {  9628,    168,  -9566/2,  0}, //"World Map",
    {-17280,    168, -17280/2,  2}, //"Arwing: to Dinosaur Planet",
    { -5120,    168, -17280/2,  2}, //"Arwing: to DarkIce Mines",
    {  6400,    168, -17280/2,  2}, //"Arwing: to CloudRunner Fortress",
    { 10240,    168, -17280/2,  2}, //"Arwing: to Walled City",
    { 14080,    168, -17280/2,  2}, //"Arwing: to Dragon Rock",
    {  7203,     86,  30980/2,  0}, //"Game Well Maze",
    {  9600,    300, -15360/2,  0}, //"Magic Cave",
    {  -961,      9,  24522/2,  0}, //"Cutscene: Great Fox",
    {-18870,      9,  28360/2,  0}, //"Cutscene: Title Screen",
    {     0,      0,  13440/2,  0}, //"Ship Battle",
    { -9495,   -127, -19015/2,  0}, //"Unused: AnimTest",
//  { 13440,   -127,  -5120/2,  0}, //"Unused: WGShrine",
//  { -7040,   -127,  -8320/2,  0}, //"Unused: NikTest", //crashes
//  {  7680,   -127,  14080/2,  0}, //"Unused: Rolling Demo",
//  {-12800,   -127,  27520/2,  0}, //"Unused: Kamerian",
//  {-10240,   -127,  27520/2,  0}, //"Unused: Duster Cave",
//  { -7040,   -127,  29440/2,  0}, //"Unused: Diamond Bay",
//  {  -640,   -127,  28800/2,  0}, //"Unused: Discovery Falls",
//  {  3200,   -127,  28160/2,  0}, //"Unused: Willow Grove",
    { 19373,     23,  30079/2,  0}, //"Unused: Old Krazoa Palace",
//  {  3693,    -37,    238/2,  0}, //"Unused: Old Krazoa Palace", where did these coords come from?
//  {-19200,   -127,  33920/2,  0}, //"Unused: CloudRunner Race 2",
    {-12768,  -2364,  12376/2, -1} //"Unused: Dragon Rock Bottom"
};

//employ some simple dictionary compression
//XXX how do we translate these?
const char *warpDict[] = {
    "ThornTail Hollow - ",     //0x80
    "Krazoa Palace - ",        //0x81
    "SnowHorn Wastes - ",      //0x82
    "DarkIce Mines - ",        //0x83
    "Moon Mountain Pass - ",   //0x84
    "LightFoot Village - ",    //0x85
    "CloudRunner Fortress - ", //0x86
    "Ocean Force Point - ",    //0x87
    "Volcano Force Point - ",  //0x88
    "Walled City - ",          //0x89
    "Dragon Rock - ",          //0x8A
    "Test of ",                //0x8B
    "Boss: ",                  //0x8C
    "Arwing: to ",             //0x8D
    "Cutscene: ",              //0x8E
    "Unused: ",                //0x8F
    "Ice Mountain - ",         //0x90
};
const char *warpNames[] = {
    "\x80" "Outside",
    "\x80" "Egg Cave",
    "\x80" "Bottom",
    "ThornTail Store",
    "\x81" "Starting Area",
    "\x81" "WarpStone Pad",
    "\x81" "Inside",
    "\x81" "Final Boss Warp",
    "\x81" "Top",
    "\x90" "Top",
    "\x90" "Bottom",
    "\x82" "Shrine Warp",
    "\x82" "Water Spout",
    "\x82" "Entrance",
    "\x82" "Bottom",
    "\x83" "Landing Pad",
    "\x83" "SharpClaw Fort",
    "\x83" "Bike Start",
    "\x83" "Interior",
    "\x83" "Ice Section",
    "\x83" "Bottom",
    "\x84" "Hut",
    "\x84" "Top",
    "\x84" "Volcano Entrance",
    "\x84" "Bottom",
    "\x85" "Shrine Warp",
    "\x85" "Front",
    "\x85" "Back",
    "Cape Claw",
    "\x86" "Dungeon",
    "\x86" "Entrance",
    "\x86" "Queen Cage",
    "CloudRunner Race Start",
    "CloudRunner Mine",
    "\x87" "Bottom",
    "\x87" "End",
    "\x87" "SpellStone Platform",
    "\x87" "Entrance",
    "\x87" "Top Warp",
    "\x87" "Bottom Warp",
    "\x88" "Entrance",
    "\x88" "Cave Entrance",
    "\x88" "SpellStone Platform",
    "\x88" "SpellStone Platform 2",
    "\x88" "End Warp",
    "\x89" "Front of Pyramid",
    "\x89" "Boss Door",
    "\x8A" "Landing Pad",
    "\x8A" "CloudRunner Cage",
    "\x8B" "Combat",
    "\x8B" "Strength",
    "\x8B" "Knowledge",
    "\x8B" "Fear",
    "\x8B" "Skill",
    "\x8C" "Galdon",
    "\x8C" "Galdon - Inside",
    "\x8C" "Drakor",
    "\x8C" "T-Rex",
    "\x8C" "General Scales",
    "\x8C" "Andross",
    "World Map",
    "\x8D" "Dinosaur Planet",
    "\x8D" "DarkIce Mines",
    "\x8D" "CloudRunner Fortress",
    "\x8D" "Walled City",
    "\x8D" "Dragon Rock",
    "Game Well Maze",
    "Magic Cave",
    "\x8E" "Great Fox",
    "\x8E" "Title Screen",
    "Ship Battle",
    "\x8F" "AnimTest",
//    "\x8F" "WGShrine",
//    "\x8F" "NikTest",
//    "\x8F" "Rolling Demo",
//    "\x8F" "Kamerian",
//    "\x8F" "Duster Cave",
//    "\x8F" "Diamond Bay",
//    "\x8F" "Discovery Falls",
//    "\x8F" "Willow Grove",
    "\x8F" "Old Krazoa Palace",
//    "\x8F" "CloudRunner Race 2",
    "\x8F" "Dragon Rock Bottom",
    NULL
};

void warpMenu_draw(Menu *self) {
    //Draw function for warp menu
    menuAnimFrame++;

    drawMenuBox(WARP_MENU_XPOS, WARP_MENU_YPOS, WARP_MENU_WIDTH, WARP_MENU_HEIGHT);
    int x = WARP_MENU_XPOS + MENU_PADDING, y = WARP_MENU_YPOS + MENU_PADDING;

    //Draw title
    //box type 0 is (center, y+40), no background
    gameTextSetColor(255, 255, 255, 255);
    gameTextShowStr(self->title, 0, x, y-40);

    int start = MAX(0, self->selected - (WARP_MENU_NUM_LINES-1));

    for(int i=0; i < WARP_MENU_NUM_LINES; i++) {
        int warpIdx = i + start;
        if(!warpNames[warpIdx]) break;
        y += LINE_HEIGHT;
        bool selected = warpIdx == self->selected;
        if(selected) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else gameTextSetColor(255, 255, 255, 255);

        const char *name = warpNames[warpIdx];
        char str[256];
        char *dst = str;
        for(int ic=0; name[ic]; ic++) {
            char c = name[ic];
            if(c >= 0x80) {
                strcpy(dst, warpDict[c - 0x80]);
                dst += strlen(warpDict[c - 0x80]);
            }
            else *(dst++) = c;
        }
        *dst = 0;
        menuDrawText(str, x, y, selected);
    }
}
void debugDoWarp(float x, float y, float z, int layer) {
    //calling warpToMap doesn't initialize the map correctly.
    //it's meant to be used from a sequence.
    //previously we instead changed the player's "last saved" location and
    //loaded that as if loading the save file, but that has memory issues.
    //what we do now is create a restart point at the destination, respawn at
    //it, then restore the previous restart point (if any).
    int blocksPct=0, bytesPct=0;
    //getFreeMemory(NULL, NULL, NULL, NULL, &blocksPct, &bytesPct);
    //OSReport("Before unload: free = %d%%, %d%%", blocksPct, bytesPct);

    //objFreeAll(); //already done in unloadMap
    //not sure how much of this is necessary...
    objFreeMode = OBJ_FREE_IMMEDIATE; //actually free it now
    unloadMap();
    mapUnload(0, 0x80000000);
    expgfxRemoveAll();
    waitNextFrame(); //the game waits 3 frames between some loading methods,
    waitNextFrame(); //so we'll do the same. not sure it's necessary,
    waitNextFrame(); //but it doesn't hurt.
    objFreeMode = OBJ_FREE_DEFERRED; //back to normal

    //getFreeMemory(NULL, NULL, NULL, NULL, &blocksPct, &bytesPct);
    //OSReport("After unload: free = %d%%, %d%%", blocksPct, bytesPct);

    //back up the current restart point
    //OSReport("restart=0x%08X save=0x%08X", pRestartPoint, pLastSavedGame);
    SaveGame *save = NULL;
    if(pRestartPoint) {
        save = allocTagged(sizeof(SaveGame), ALLOC_TAG_GAME_COL, "debug:saveTemp");
        if(!save) OSReport("Warp buffer out of memory! "
            "Discarding restart point instead.");
        else memcpy(save, pRestartPoint, sizeof(SaveGame));
    }

    //replace the current restart point.
    vec3f pos; pos.x = x; pos.y = y; pos.z = z;
    gplayRestartPoint(&pos, 0, layer, false);
    mapLoadByCoords(x, y, z, layer);
    waitNextFrame(); //probably unnecessary
    gplayGotoRestartPoint();

    //don't immediately warp back if free move is on.
    freeMoveCoords.x = x;
    freeMoveCoords.y = y;
    freeMoveCoords.z = z;

    //not sure why this doesn't work, since it's basically the same thing
    //as what gplayGotoRestartPoint() does.
    /* PlayerCharPos *chr = &saveData.curSaveGame.charPos[saveData.curSaveGame.character];
    chr->pos.x = x;
    chr->pos.y = y;
    chr->pos.z = z;
    chr->mapLayer = layer;
    chr->mapId = mapCoordsToId(x / MAP_CELL_SCALE, z / MAP_CELL_SCALE, layer);
    loadMapForCurrentSaveGame(); */

    if(save) { //restore the previous restart point
        //OSReport("Restore restart point to 0x%08X", pRestartPoint);
        if(pRestartPoint) memcpy(pRestartPoint, save, sizeof(SaveGame));
        free(save);
    }
    else {
        //OSReport("No restart point");
        gplayClearRestartPoint(); //wasn't one before, so don't have one now.
    }

    //getFreeMemory(NULL, NULL, NULL, NULL, &blocksPct, &bytesPct);
    //OSReport("After load: free = %d%%, %d%%", blocksPct, bytesPct);
}

void warpMenu_run(Menu *self) {
    //Run function for warp menu
    int sel = curMenu->selected;
    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_A) {
        menuInputDelayTimer = MENU_INPUT_DELAY_SELECT;
        WarpMenuItem *warp = &warps[self->selected];
        DPRINT("Warp to %f %f %f ly %d (idx %d:%08X)",
            (float)warp->x, (float)warp->y, (float)warp->z, warp->layer,
            sel, warp);
        debugDoWarp(warp->x, warp->y, warp->z*2, warp->layer);
        //close all menus
        mainMenu_close(&menuMain);
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) {
            while(warpNames[sel]) sel++;
        }
        curMenu->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(!warpNames[sel]) sel = 0;
        curMenu->selected = sel;
    }
}

Menu menuDebugWarp = {
    "Warp", 0,
    warpMenu_run, warpMenu_draw, debugMapSubMenu_close,
    NULL,
};
