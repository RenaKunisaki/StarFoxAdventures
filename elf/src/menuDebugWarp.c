/** Debug Warp submenu.
 */
#include "main.h"
#define WARP_MENU_XPOS 32
#define WARP_MENU_YPOS 32
#define WARP_MENU_WIDTH  (SCREEN_WIDTH  - (WARP_MENU_XPOS * 2))
#define WARP_MENU_HEIGHT (SCREEN_HEIGHT - (WARP_MENU_YPOS * 2))
#define WARP_MENU_NUM_LINES ((WARP_MENU_HEIGHT / MENU_LINE_HEIGHT) - 2)

typedef struct {
    float x, y, z;
    s8 layer;
    const char *name;
} WarpMenuItem;

WarpMenuItem warps[] = {
    { -5774,   -765,  -2542,  0, "ThornTail Hollow - Outside"},
    { -5517,   -636,   -800,  0, "ThornTail Hollow - Egg Cave"},
    { -6664,  -1086,  -2253, -1, "ThornTail Hollow - Bottom"},
    { -6069,   -980,  -2752,  1, "ThornTail Store"},
    { 12264,    211,    451,  0, "Krazoa Palace - Starting Area"},
    { 11840,   -245,   5864,  0, "Krazoa Palace - WarpStone Pad"},
    { 13154,    -83,   2880,  0, "Krazoa Palace - Inside"},
    { 13201,   1278,   2239,  0, "Krazoa Palace - Final Boss Warp"},
    { 12188,   1379,   2461,  0, "Krazoa Palace - Top"},
    {  3584,   6397,   4374,  0, "Ice Mountain - Top"},
    { -4159,   -521,   4159,  0, "SnowHorn Wastes - Shrine Warp"},
    { -2634,   -130,   2167,  0, "SnowHorn Wastes - Water Spout"},
    { -5408,   -822,   1610,  0, "SnowHorn Wastes - Entrance"},
    {  -511,    -41,   2951,  0, "SnowHorn Wastes - Bottom"},
    { -7378,  -1557,   8935,  0, "DarkIce Mines - Landing Pad"},
    { -7957,  -1177,  14025,  0, "DarkIce Mines - SharpClaw Fort"},
    {-10036,   -775,  14822,  0, "DarkIce Mines - Bike Start"},
    { -9075,  -1713,  17999, -2, "DarkIce Mines - Interior"},
    {-10460,  -2025,  17882, -2, "DarkIce Mines - Ice Section"},
    { -7949,  -2627,  17001, -2, "DarkIce Mines - Bottom"}, //freezes XXX why?
    {-11923,   -268,   -331,  0, "Moon Mountain Pass - Hut"},
    {-11839,     52,  -4791,  0, "Moon Mountain Pass - Top"},
    {-13440,   -111,   -381,  0, "Moon Mountain Pass - Volcano Entrance"},
    { -8902,   -925,   -313,  0, "Moon Mountain Pass - Bottom"},
    { -1601,  -1216,   -960,  0, "LightFoot Village - Shrine Warp"},
    { -1299,   -814,  -1932,  0, "LightFoot Village - Front"},
    { -3150,   -895,  -1758,  0, "LightFoot Village - Back"},
    {  1845,  -1624,  -2330,  0, "Cape Claw"},
    {   829,   1302, -16477,  0, "CloudRunner Fortress - Dungeon"},
    {  4125,   1464, -18232,  0, "CloudRunner Fortress - Entrance"},
    {   233,   2196, -17591,  0, "CloudRunner Fortress - Queen Cage"},
    {  -173,   1284, -14400,  0, "CloudRunner Race Start"},
    {   463,   1845, -16899,  0, "CloudRunner Mine"},
    {  1534,  -2536,  -8574, -1, "Ocean Force Point - Bottom"},
    {  4220,  -2536,  -8576, -1, "Ocean Force Point - End"},
    {  3198,  -2542,  -7981, -1, "Ocean Force Point - SpellStone Platform"},
    {  3357,  -1600,  -7428,  0, "Ocean Force Point - Entrance"},
    {  2872,  -1533,  -9247,  0, "Ocean Force Point - Top Warp"},
    {  2877,  -2718,  -9277, -1, "Ocean Force Point - Bottom Warp"},
    {-16065,     57,   -320,  0, "Volcano Force Point - Entrance"},
    {-18710,    389,   -307,  0, "Volcano Force Point - Cave Entrance"},
    {-17919,    198,   -975,  0, "Volcano Force Point - SpellStone Platform"},
    {-18648,    -45,    499,  0, "Volcano Force Point - SpellStone Platform 2"},
    {-18564,    293,    647,  0, "Volcano Force Point - End Warp"},
    {-16322,   -777, -13271,  0, "Walled City - Front of Pyramid"},
    {-16322,  -1209, -13518,  0, "Walled City - Boss Door"},
    {-13753,  -1714,  11227,  0, "Dragon Rock - Landing Pad"},
    {-17799,  -1764,   9032,  0, "Dragon Rock - CloudRunner Cage"},
    { 14079,   -204, -17065,  0, "Test of Combat"},
    { 17919,   -204, -10669,  0, "Test of Strength"},
    { 17919,   -204, -14511,  0, "Test of Knowledge"},
    { 10240,   -203, -18354,  0, "Test of Fear"},
    { 13440,   -204, -13225,  0, "Test of Skill"},
    { -9555,  -2973,  20457, -2, "Boss: Galdon"},
    { -7954,  -2978,  20162, -2, "Boss: Galdon - Inside"},
    {  7694,   -199,  25943,  1, "Boss: Drakor"},
    {-10138,      0, -13381,  0, "Boss: T-Rex"},
    {  7039,     58, -13887,  0, "Boss: General Scales"},
    {-11850,    131, -15476,  2, "Boss: Andross"},
    {  9628,    168,  -9566,  0, "World Map"},
    {-17280,    168, -17280,  2, "Arwing: to Dinosaur Planet"},
    { -5120,    168, -17280,  2, "Arwing: to DarkIce Mines"},
    {  6400,    168, -17280,  2, "Arwing: to CloudRunner Fortress"},
    { 10240,    168, -17280,  2, "Arwing: to Walled City"},
    { 14080,    168, -17280,  2, "Arwing: to Dragon Rock"},
    {  7203,     86,  30980,  0, "Game Well Maze"},
    {  9600,    300, -15360,  0, "Magic Cave"},
    {  -961,      9,  24522,  0, "Cutscene: Great Fox"},
    {-18870,      9,  28360,  0, "Cutscene: Title Screen"},
    {     0,      0,  13440,  0, "Ship Battle"},
    { -9495,   -127, -19015,  0, "Unused: AnimTest"},
    { 13440,   -127,  -5120,  0, "Unused: WGShrine"},
//  { -7040,   -127,  -8320,  0, "Unused: NikTest"}, //crashes
    {  7680,   -127,  14080,  0, "Unused: Rolling Demo"},
    {-12800,   -127,  27520,  0, "Unused: Kamerian"},
    {-10240,   -127,  27520,  0, "Unused: Duster Cave"},
    { -7040,   -127,  29440,  0, "Unused: Diamond Bay"},
    {  -640,   -127,  28800,  0, "Unused: Discovery Falls"},
    {  3200,   -127,  28160,  0, "Unused: Willow Grove"},
    { 12800,    973,  30080,  0, "Unused: Old Krazoa Palace"},
//  {  3693,    -37,    238,  0, "Unused: Old Krazoa Palace"},
    {-19200,   -127,  33920,  0, "Unused: CloudRunner Race 2"},
    {     0,      0,      0,  0, NULL},
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
        y += MENU_LINE_HEIGHT;
        bool selected = warpIdx == self->selected;
        if(selected) {
            u8  r = menuAnimFrame * 8, g = 255 - r;
            gameTextSetColor(r, g, 255, 255);
        }
        else gameTextSetColor(255, 255, 255, 255);
        gameTextShowStr(warps[warpIdx].name, MENU_TEXTBOX_ID, x, y);
    }
    //gameTextDrawFunc = prevDrawFunc;
}

void debugDoWarp(float x, float y, float z, int layer) {
    //calling warpToMap doesn't initialize the map correctly.
    //it's meant to be used from a sequence.
    //instead, we'll change the player's "last saved" position and then
    //load that as if we were loading the save file.
    PlayerCharPos *chr = &saveData.curSaveGame.charPos[saveData.curSaveGame.character];
    chr->pos.x = x;
    chr->pos.y = y;
    chr->pos.z = z;
    chr->mapLayer = layer;
    loadMapForCurrentSaveGame();
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
        DPRINT("Warp to %f %f %f ly %d: %s", warp->x, warp->y, warp->z,
            warp->layer, warp->name);
        debugDoWarp(warp->x, warp->y, warp->z, warp->layer);
        //close all menus
        mainMenu_close(&menuMain);
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) {
            while(warps[sel].name) sel++;
        }
        curMenu->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(!warps[sel].name) sel = 0;
        curMenu->selected = sel;
    }
}

Menu menuDebugWarp = {
    "Warp", 0,
    warpMenu_run, warpMenu_draw, debugMapSubMenu_close,
    NULL,
};
