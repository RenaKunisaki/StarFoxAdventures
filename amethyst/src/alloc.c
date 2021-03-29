#include "main.h"

u32 allocFailCount = 0;
u32 emergFreeCount = 0;
u8 allocFailLogIdx = 0;
AllocFailLogItem allocFailLog[ALLOC_FAIL_LOG_SIZE];

//object category IDs
//ordered roughly from least to most important as well as least to
//most noticeable
static const u8 objEmergFreeCats[] = {
    0x7F, //Decorative
    0x69, //CurveFish
    0x43, //LevelName
    0x83, //Lightning
    0x35, //CCriverflow
    0x5C, //sfxPlayer
    0x08, //InfoPoint
    0x79, //Light
    0x7B, //ProjectedLight
    0x73, //WaterFallSpray
    0x3C, //SidekickBall
    0x1D, //Torch
    0x32, //CampFire
    0x3D, //CFPerch
    0x45, //DIMLavaBall
    0x46, //DIMSnowBall
    0x24, //Fireball
    0x28, //Falling Rock
    0x2A, //Enemy Mushroom
    0x1C, //Baddie
    0x38, //some doors
    0x53, //other doors
    0x59, //WM_Column
    0x6A, //Tree
    0x29, //Edible Mushroom
    0x65, //MagicPlant
    0x06, //Collectible
    0x63, //Exploded, Explodable
    0x7C, //ArwingLevelObj
    0x44, //SH_thorntail
    0x26, //Mammoth
    0x40, //CloudRunner
    0x80, //DeathGas
    0xFF, //end of list
};

void doEmergencyFree() {
    emergFreeCount++;
    int nObj = numLoadedObjs;
    for(int i=0; i<nObj; i++) {
        ObjInstance *obj = loadedObjects[i];
        if(obj) {
            for(int cat=0; objEmergFreeCats[cat] != 0xFF; cat++) {
                if(obj->catId == objEmergFreeCats[cat]) {
                    OSReport("Emergency free object \"%s\"", obj->file->name);
                    objFree(obj);
                    return;
                }
            }
        }
    }
    OSReport("Emergency free: didn't free any object");
    expgfxRemoveAll(); //maybe this will help
    //XXX try more things
}

void* allocTaggedHook(u32 size, AllocTag tag, const char *name) {
    //replaces the body of allocTagged.
    //implements the same logic, but with tweaks:
    //1) if out of memory, free some objects and try again
    //2) replace tag with return address
    //3) flag invalid sizes
    //4) log failed allocations for display on BSOD
    //5) optionally log all allocations to console

    //name is almost never set and isn't stored
    u32 lr = (u32)__builtin_return_address(0);
    //DPRINT("alloc size=%8X tag=%08X lr=%08X", size, tag, lr);
    tag = lr; //this is more useful

    //mostly copied game code
    if (size == 0) return NULL;

    int count = 0;
    void *buf = NULL;
    while((buf == NULL) && (count < 100)) {
        if(bOnlyUseHeaps1and2 == 1) {
            buf = heapAlloc(1,size,tag,name);
            if(buf == NULL) buf = heapAlloc(2,size,tag,name);
        }
        else if(bOnlyUseHeap3 != 0) {
            buf = heapAlloc(3,size,tag,name);
        }
        else if(size < 0x3000) {
            if(size < 0x400) {
                buf = heapAlloc(2,size,tag,name);
                if(buf == NULL) buf = heapAlloc(1,size,tag,name);
                if(buf == NULL) buf = heapAlloc(0,size,tag,name);
            }
            else {
                buf = heapAlloc(1,size,tag,name);
                if(buf == NULL) buf = heapAlloc(2,size,tag,name);
                if(buf == NULL) buf = heapAlloc(0,size,tag,name);
            }
        }
        else {
            buf = heapAlloc(0,size,tag,name);
            if(buf == NULL) buf = heapAlloc(1,size,tag,name);
        }
        if(buf) {
            //DPRINT("alloc success, %08X", buf);
            return buf;
        }
        else {
            //alloc failed. try freeing something.
            doEmergencyFree();
        }
        count += 1;
    }
    OSReport("ALLOC FAIL size=0x%X tag=0x%X", size, tag);
    allocFailLog[allocFailLogIdx].size = size;
    allocFailLog[allocFailLogIdx].tag  = tag;
    allocFailLogIdx++;
    if(allocFailLogIdx >= ALLOC_FAIL_LOG_SIZE) allocFailLogIdx = 0;
    return NULL;
}
