#include "main.h"
#define MAP_STATES_MENU_XPOS 24
#define MAP_STATES_MENU_YPOS 64
#define MAP_STATES_MENU_WIDTH  (SCREEN_WIDTH  - (MAP_STATES_MENU_XPOS * 2))
#define MAP_STATES_MENU_HEIGHT (SCREEN_HEIGHT - (MAP_STATES_MENU_YPOS * 2))
#define MAP_STATES_MENU_NUM_LINES ((MAP_STATES_MENU_HEIGHT / MENU_LINE_HEIGHT) - 2)

static u8 listOrder[] = {
    0x1d, //"Cape Claw",                    capeclaw
    0x10, //"CloudRunner Dungeon",          clouddungeon
    0x0c, //"CloudRunner Fortress",         fortress
    0x2b, //"CloudRunner Race",             cloudrace
    0x1b, //"DarkIce Mines - Bottom",       snowmines2
    0x13, //"DarkIce Mines - Top",          snowmines
    0x1c, //"DarkIce Mines Boss",           snowmines3
    0x02, //"Dragon Rock",                  dragrock
    0x2c, //"Drakor",                       finalboss
    0x09, //"Game Well Maze",               mazecave
    0x41, //"Great Fox",                    greatfox
    0x17, //"Ice Mountain",                 newicemount
    0x0b, //"Krazoa Palace",                warlock
    0x0e, //"LightFoot Village",            swapcircle
    0x42, //"LinkA - Warpstone to Others",  linka
    0x38, //"LinkB - Ice2Wastes",           linkb
    0x43, //"LinkC - Wastes to Hollow",     linkc
    0x44, //"LinkD - Darkmines top 2 bot",  linkd
    0x46, //"LinkF - moonpass to volcano",  linkf
    0x47, //"LinkG - hollow to lightfoot",  linkg
    0x48, //"LinkH - lightfoot to capecl",  linkh
    0x4a, //"LinkI - CloudRunner2Race",     linki
    0x36, //"Magic Cave",                   magicave
    0x12, //"Moon Mountain Pass",           moonpass
    0x15, //"Ocean Force Point - Bottom",   kraztest
    //0x32, //"Ocean Force Point - Top",      dfptop
    0x00, //"Ship Battle",                  frontend
    0x0a, //"SnowHorn Wastes",              wastes
    0x1f, //"Test Of Combat",               dfshrine
    0x27, //"Test Of Strength",             dbshrine
    //0x08, //"ThornTail Hollow Underground", hollow2
    0x07, //"ThornTail Hollow",             hollow
    0x33, //"ThornTail Shop",               swapstore
    0x30, //"TRex Boss",                    trexboss
    0x04, //"Volcano Force Point",          temple
    0x0d, //"Walled City",                  wallcity
    0x1a, //"Animtest",                     animtest
    0x11, //"(unused) CloudTrap",           cloudtrap
    0x0f, //"(unused) CloudTreasure",       cloudtreasure
    //0x2e, //"(unused) DarkIce Mines 4",     snowmines4
    //0x2f, //"(unused) DarkIce Mines 5",     snowmines5
    0x23, //"(unused) Diamond Bay",         diamondbay
    0x06, //"(unused) Discovery Falls",     discovery
    0x34, //"(unused) Dragon Rock Bottom",  dragbot
    0x01, //"(unused) Frontend 2",          frontend2
    0x50, //"galleonship",                  galleonship
    0x05, //"(unused) HighTop",             hightop
    //0x18, //"(unused) Ice Mountain 2",      newicemount2
    //0x19, //"(unused) Ice Mountain 3",      newicemount3
    0x1e, //"(unused) Inside Galleon",      insidegal
    0x03, //"(unused) Old Krazoa Palace",   krazoapalace
    0xFF, //end
};

static const char *displayNames[] = {
    "Ship Battle",        //00 frontend
    "(unused) Frontend2", //01 frontend2
    "Dragon Rock",        //02 dragrock
    "(unused) Old KP",    //03 krazoapalace
    "Volcano ForcePoint", //04 temple
    "(unused) HighTop",   //05 hightop        "Rolling Demo - Just In Case"
    "(unused) DiscFalls", //06 discovery
    "ThornTail Hollow",   //07 hollow
    NULL, //"ThornTail Undrgrnd", //08 hollow2
    "Game Well Maze",     //09 mazecave
    "SnowHorn Wastes",    //0a wastes
    "Krazoa Palace",      //0b warlock
    "CloudRun Fortress",  //0c fortress
    "Walled City",        //0d wallcity
    "LightFoot Village",  //0e swapcircle
    "(unused) CloudTrea", //0f cloudtreasure
    "CloudRun Dungeon",   //10 clouddungeon
    "(unused) CloudTrap", //11 cloudtrap
    "Moon Mountain Pass", //12 moonpass
    "DarkIceMines Top",   //13 snowmines
    NULL, //"(unused) Krazoa Shrine",       //14 krashrin2
    "Ocean Force Point", //15 kraztest
    NULL, //"krazchamber",                  //16 krazchamber
    "Ice Mountain",                 //17 newicemount
    NULL, //"(unused) Ice Mt2",   //18 newicemount2
    NULL, //"(unused) Ice Mt3",   //19 newicemount3
    "(unused) Animtest",           //1a animtest
    "DarkIceMines Botm",  //1b snowmines2
    "DarkIceMines Boss",  //1c snowmines3
    "Cape Claw",          //1d capeclaw
    "(unused) InsideGal", //1e insidegal
    "Test Of Combat",     //1f dfshrine
    NULL, //"Test Of Fear",                 //20 mmshrine
    NULL, //"Test Of Skill",                //21 ecshrine
    NULL, //"Test Of Knowledge",            //22 gpshrine
    "(unused) DiamndBay", //23 diamondbay
    NULL, //"(unused) EarthWalker Temple",  //24 earthwalker
    NULL, //"(unused) Willow Grove",        //25 willow
    NULL, //"Andross",                      //26 arwing
    "Test Of Strength",   //27 dbshrine
    NULL, //"Scales Boss",                  //28 nwshrine
    NULL, //"World Map",                    //29 ccshrine
    NULL, //"(unused) WGShrine",            //2a wgshrine
    "CloudRunner Race",   //2b cloudrace
    "Drakor",             //2c finalboss
    NULL, //"(unused) WMinsert",            //2d wminsert
    NULL, //"(unused) DrkIceMn4", //2e snowmines4
    NULL, //"(unused) DrkIceMn5", //2f snowmines5
    "TRex Boss",          //30 trexboss
    NULL, //"(unused) MikesLava",           //31 mikelava
    NULL, //"Ocean Force Pt Top", //32 dfptop
    "ThornTail Shop",     //33 swapstore
    "(unused) DrgRckBot", //34 dragbot
    NULL, //"(unused) Boss KamerianD",      //35 kamdrag
    "Magic Cave",         //36 magicave
    NULL, //"(unused) Duster Cave",         //37 duster
    "LinkB Ice2Wastes",   //38 linkb
    NULL, //"(unused) CloudRunner2Rac",     //39 cloudjoin
    NULL, //"Arwing to Planet",             //3a arwingtoplanet
    NULL, //"Arwing Darkice",               //3b arwingdarkice
    NULL, //"Arwing Cloud",                 //3c arwingcloud
    NULL, //"Arwing City",                  //3d arwingcity
    NULL, //"Arwing Dragon",                //3e arwingdragon
    NULL, //"Title Screen",                 //3f gamefront
    NULL, //"(unused) LinkK - Nik Test",    //40 linklevel
    "Great Fox",          //41 greatfox
    "LinkA Warpstone",    //42 linka
    "LinkC Wastes2TTH",   //43 linkc
    "LinkD DIM top2bot",  //44 linkd
    NULL, //"LinkE - hollow to moon pass",  //45 linke
    "LinkF moonpass2VFP", //46 linkf
    "LinkG TTH 2 LV",     //47 linkg
    "LinkH LV 2 Cape",    //48 linkh
    NULL, //"LinkJ - capeclaw 2 ocean fo",  //49 linkj
    "LinkI CRF 2 Race",   //4a linki
    NULL, //"dfpodium",                     //4b dfpodium
    NULL, //"dfcradle",                     //4c dfcradle
    NULL, //"dfcavehatch1",                 //4d dfcavehatch1
    NULL, //"dfcavehatch2",                 //4e dfcavehatch2
    NULL, //"scstatue",                     //4f scstatue
    "(unused) galeonshp",                  //50 galleonship
    /* "cfgalleon",                    //51 cfgalleon
    "cfgangplank",                  //52 cfgangplank
    "nwtreebridge",                 //53 nwtreebridge
    "cfdungeonblock",               //54 cfdungeonblock
    "cloudrunnermap",               //55 cloudrunnermap
    "ccBridge",                     //56 ccBridge
    "cfcolumn",                     //57 cfcolumn
    "nwboulder",                    //58 nwboulder
    "cfprisondoor",                 //59 cfprisondoor
    "cfprisoncage",                 //5a cfprisoncage
    "nwtreebridge2",                //5b nwtreebridge2
    "dim2 ice block1",              //5c dim2 ice block1
    "dimpushblock",                 //5d dimpushblock
    "dim2 ice block2",              //5e dim2 ice block2
    "dimhornplinth",                //5f dimhornplinth
    "nwshcolpush",                  //60 nwshcolpush
    "dim2lift",                     //61 dim2lift
    "dim2icefloe",                  //62 dim2icefloe
    "dim2icefloe1",                 //63 dim2icefloe1
    "dim2icefloe2",                 //64 dim2icefloe2
    "cfliftplat",                   //65 cfliftplat
    "imspacecraft",                 //66 imspacecraft
    "dimbossgut",                   //67 dimbossgut
    "wmcolrise",                    //68 wmcolrise
    "vfpslide1",                    //69 vfpslide1
    "vfpslide2",                    //6a vfpslide2
    "drpushcart",                   //6b drpushcart
    "drliftplat",                   //6c drliftplat
    "dim2stonepillar",              //6d dim2stonepillar
    "bossdrakorrock",               //6e bossdrakorrock
    "wcbouncycrate",                //6f wcbouncycrate
    "wcpushblock",                  //70 wcpushblock
    "wctemplelift",                 //71 wctemplelift
    "kameriancolumn",               //72 kameriancolumn
    "dbstepstone",                  //73 dbstepstone
    "vfppushblock",                 //74 vfppushblock */
    NULL
};

static u8 cursorX = 0;
void mapStatesMenu_draw(Menu *self) {
    //Draw function for map states menu
    menuAnimFrame++;
    char str[256];

    drawMenuBox(MAP_STATES_MENU_XPOS, MAP_STATES_MENU_YPOS,
        MAP_STATES_MENU_WIDTH, MAP_STATES_MENU_HEIGHT);

    int x = MAP_STATES_MENU_XPOS + MENU_PADDING;
    int y = MAP_STATES_MENU_YPOS + MENU_PADDING;
    drawSimpleText(T("Map"),       x,     y);
    drawSimpleText(T("Act"),       x+180, y);
    drawSimpleText(T("ObjGroups"), x+220, y);

    if(cursorX > 0) {
        sprintf(str, "%2d", cursorX - 1);
        drawSimpleText(str, x+(MAP_STATES_MENU_WIDTH-35), y);
    }

    int start = MAX(0, self->selected - (MAP_STATES_MENU_NUM_LINES-1));

    for(int i=0; i < MAP_STATES_MENU_NUM_LINES; i++) {
        int iMap = listOrder[i + start];
        if(iMap == 0xFF) break;
        //if(!PTR_VALID(mapNames[iMap])) break;

        y += MENU_LINE_HEIGHT;
        bool selected = (i+start) == self->selected;
        if(selected) {
            debugPrintf("ActBit=%04X ObjsBit=%04X\n",
                mapActBitIdx[iMap], mapObjGroupBit[iMap]);
        }

        if(displayNames[iMap]) {
            menuDrawText(displayNames[iMap], x, y, selected);
        }

        if(mapActBitIdx[iMap]) {
            sprintf(str, "%X", mainGetBit(mapActBitIdx[iMap]));
            menuDrawText(str, x + 200, y, selected);
        }
        if(mapObjGroupBit[iMap]) {
            u32 groups = mainGetBit(mapObjGroupBit[iMap]);
            for(int iBit=0; iBit<32; iBit++) {
                str[iBit] = ((groups >> (31 - iBit)) & 1) + 0x30;
                str[iBit+1] = 0;
            }
            menuDrawText(str, x + 220, y, selected);
        }
    }

    //draw cursor
    if(cursorX == 0) x = 203;
    else x = (223 - MENU_FIXED_WIDTH) + (cursorX * MENU_FIXED_WIDTH);
    x += MAP_STATES_MENU_XPOS;
    y = MAP_STATES_MENU_YPOS + 5 + (MENU_LINE_HEIGHT * (self->selected - start+1));
    drawBox(x, y, 20, 24, 255, false);
}


void mapStatesMenu_run(Menu *self) {
    //Run function for map states menu
    int sel  = curMenu->selected;
    int iMap = listOrder[sel];

    if(buttonsJustPressed == PAD_BUTTON_B) {
        menuInputDelayTimer = MENU_INPUT_DELAY_CLOSE;
        curMenu->close(curMenu);
    }
    else if(buttonsJustPressed == PAD_BUTTON_X
    ||      buttonsJustPressed == PAD_BUTTON_Y
    ||      buttonsJustPressed == PAD_BUTTON_A) {
        menuInputDelayTimer = MENU_INPUT_DELAY_ADJUST;
        if(cursorX == 0) { //change act
            if(mapActBitIdx[iMap]) {
                int adj = (buttonsJustPressed == PAD_BUTTON_Y) ? -1 : 1;
                int act = mainGetBit(mapActBitIdx[iMap]);
                gplaySetAct(iMap, act + adj);
            }
            else audioPlaySound(NULL, MENU_FAIL_SOUND);
        }
        else { //change obj group
            if(mapObjGroupBit[iMap]) {
                int iBit = 31 - (cursorX - 1);
                u32 groups = mainGetBit(mapObjGroupBit[iMap]);
                groups = (groups >> iBit) & 1;
                gplaySetObjGroupStatus(iMap, iBit, groups ^ 1);
            }
            else audioPlaySound(NULL, MENU_FAIL_SOUND);
        }
    }
    else if(controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY > MENU_CSTICK_THRESHOLD) { //up
        menuInputDelayTimer =
            (controllerStates[0].stickY > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(sel == 0) {
            while(listOrder[sel] != 0xFF) sel++;
        }
        curMenu->selected = sel - 1;
    }
    else if(controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickY < -MENU_CSTICK_THRESHOLD) { //down
        menuInputDelayTimer = (controllerStates[0].stickY < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        sel++;
        if(listOrder[sel] == 0xFF) sel = 0;
        curMenu->selected = sel;
    }
    else if(controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX < -MENU_CSTICK_THRESHOLD) { //left
        menuInputDelayTimer = (controllerStates[0].stickX < -MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        if(cursorX > 0) cursorX--;
        else cursorX = 32;
    }
    else if(controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD
    ||      controllerStates[0].substickX > MENU_CSTICK_THRESHOLD) { //right
        menuInputDelayTimer = (controllerStates[0].stickX > MENU_ANALOG_STICK_THRESHOLD)
            ? MENU_INPUT_DELAY_MOVE : MENU_INPUT_DELAY_MOVE_FAST;
        cursorX++;
        if(cursorX >= 33) cursorX = 0;
    }
}


Menu menuDebugMapStates = {
    "Map States", 0,
    mapStatesMenu_run, mapStatesMenu_draw, debugMapSubMenu_close,
    //"Time: %02d:%02d:%02d", menuDebugMapEnvTime_draw, menuDebugMapEnvTime_select,
    NULL,
};
