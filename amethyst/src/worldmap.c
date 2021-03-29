#include "main.h"

void worldMapHook() {
    //replaces a call to a function that just does this:
    pauseMenuFrameCounter = 60;

    //pad with spaces to center
    //we can't actually set the X offset with this window type
    gameTextShowStr("Z: Skip   ", 2, 0, 352);

    if(!(buttonsJustPressed == PAD_TRIGGER_Z)) return;

    //unlock all level buckets, so that destination map loads correctly.
    unlockLevel(0, 0, true);

    //map setup logic copied from game (8022c680)
    //XXX get enums for these values
    switch(worldMapSelected) {
        case 0x38: //Planet
            if(mainGetBit(0x0C85)) { //HaveKrazoaSpirit6
                //go to Krazoa Palace
                mainSetBits(0x0405, 0); //WM_ObjGroups
                gplaySetAct(0x0B, 5);
                gplaySetObjGroupStatus(0x0B, 0x0A, 1);
                gplaySetObjGroupStatus(0x0B, 0x0B, 1);
                warpToMap(0x22, false);
            }
            else { //go to planet
                warpToMap(0x6C, false);
            }
            break;

        case 0x39: //DarkIce
            gplaySetObjGroupStatus(0x13, 0x00, 1);
            gplaySetObjGroupStatus(0x13, 0x16, 1);
            warpToMap(0x77, false);
            break;

        case 0x3A: //CloudRunner
            mainSetBits(0x0458, 0); //CF_ObjGroups
            mainSetBits(0x047C, 0); //CD_ObjGroups
            mainSetBits(0x04A3, 0); //CR_ObjGroups
            warpToMap(0x63, false);
            break;

        case 0x3B: //WallCity
            mainSetBits(0x036A, 0); //WC_ObjGroups
            gplaySetObjGroupStatus(0x0D, 0x00, 1);
            gplaySetObjGroupStatus(0x0D, 0x01, 1);
            gplaySetObjGroupStatus(0x0D, 0x05, 1);
            gplaySetObjGroupStatus(0x0D, 0x0A, 1);
            gplaySetObjGroupStatus(0x0D, 0x0B, 1);
            mainSetBits(0x0E05, 0);
            warpToMap(0x78, false);
            break;

        default: //DragRock
            mainSetBits(0x05DB, 0); //DR_ObjGroups
            gplaySetObjGroupStatus(0x02, 0x0F, 1);
            gplaySetObjGroupStatus(0x02, 0x10, 1);
            mainSetBits(0x0E7B, 0);
            mainSetBits(0x09E9, 0);
            warpToMap(0x79, false);
    }
}
