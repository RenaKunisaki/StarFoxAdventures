#include "main.h"
#include "revolution/os.h"

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

void getFreeMemory(u32 *outTotalBlocks, u32 *outTotalBytes,
u32 *outUsedBlocks, u32 *outUsedBytes, int *outBlocksPct, int *outBytesPct) {
    u32 totalBlocks=0, totalBytes=0, usedBlocks=0, usedBytes=0;
    for(int i=0; i<NUM_HEAPS; i++) {
        totalBytes  += heaps[i].dataSize;
        totalBlocks += heaps[i].avail;
        usedBytes   += heaps[i].size;
        usedBlocks  += heaps[i].used;
    }
    if(outTotalBlocks) *outTotalBlocks = totalBlocks;
    if(outTotalBytes)  *outTotalBytes  = totalBytes;
    if(outUsedBlocks)  *outUsedBlocks  = usedBlocks;
    if(outUsedBytes)   *outUsedBytes   = usedBytes;

    if(outBlocksPct) *outBlocksPct = (usedBlocks * 100) / totalBlocks;
    if(outBytesPct)  *outBytesPct  = (usedBytes  * 100) / totalBytes;
}

bool doEmergencyFree(int attempt) {
    emergFreeCount++;

    //I have no idea why these exist.
    //Our code is now big enough to cause memory outage at the title screen.
    //This seems to fix it...
    if(attempt == 0) {
        OSReport("Emergency free: retry with other heaps");
        bOnlyUseHeap3 = 0;
        bOnlyUseHeaps1and2 = -1;
        return true;
    }

    if(attempt == 1) {
        OSReport("Emergency free: dumping expgfx");
        expgfxRemoveAll();
        return true;
    }

    int nObj = numLoadedObjs;
    for(int i=0; i<nObj; i++) {
        ObjInstance *obj = loadedObjects[i];
        if(obj) {
            for(int cat=0; objEmergFreeCats[cat] != 0xFF; cat++) {
                if(obj->catId == objEmergFreeCats[cat]) {
                    OSReport("Emergency free object 0x%08X \"%s\"",
                        obj, obj->file->name);
                    objFreeMode = OBJ_FREE_IMMEDIATE; //actually free it now
                    objFree(obj);
                    objFreeMode = OBJ_FREE_DEFERRED; //back to normal
                    return true;
                }
            }
        }
    }

    u32 totalBlocks, totalBytes, usedBlocks, usedBytes;
    getFreeMemory(&totalBlocks, &totalBytes, &usedBlocks, &usedBytes, NULL, NULL);
    OSReport("Emergency free: didn't free any object. Used blocks %d/%d, bytes %d/%d K",
        usedBlocks, totalBlocks, usedBytes/1024, totalBytes/1024);
    //XXX try more things
    return false;
}

void* _doAlloc(int iRegion, u32 size, AllocTag tag, const char *name,
SfaHeapEntry **outEntry) {
    void *res = heapAlloc(iRegion, size, tag, name);
    if(!res) {
        if(outEntry) *outEntry = NULL;
        return NULL;
    }
    if(!outEntry) return res;

    //the only reliable way to get the entry seems to be this.
    //we do it in reverse order since the last block is most likely
    //to be the one we want.
    *outEntry = NULL;
    SfaHeap *heap = &heaps[iRegion];
    for(int i=heap->avail - 1; i >= 0; i--) {
        if(heap->data[i].loc == res) {
            *outEntry = &heap->data[i];
            break;
        }
    }
    return res;
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

    //mostly copied game code
    if (size == 0) return NULL;

    int count = 0;
    void *buf = NULL;
    SfaHeapEntry *entry = NULL;
    while((buf == NULL) && (count < 1000)) {
        if(bOnlyUseHeaps1and2 == 1) {
            buf = _doAlloc(1, size, tag, name, &entry);
            if(buf == NULL) buf = _doAlloc(2, size, tag, name, &entry);
        }
        else if(bOnlyUseHeap3 != 0) buf = _doAlloc(3, size, tag, name, &entry);
        else if(size < 0x3000) {
            if(size < 0x400) {
                buf = _doAlloc(2, size, tag, name, &entry);
                if(buf == NULL) buf = _doAlloc(1, size, tag, name, &entry);
                if(buf == NULL) buf = _doAlloc(0, size, tag, name, &entry);
            }
            else {
                buf = _doAlloc(1, size, tag, name, &entry);
                if(buf == NULL) buf = _doAlloc(2, size, tag, name, &entry);
                if(buf == NULL) buf = _doAlloc(0, size, tag, name, &entry);
            }
        }
        else {
            buf = _doAlloc(0, size, tag, name, &entry);
            if(buf == NULL) buf = _doAlloc(1, size, tag, name, &entry);
        }
        if(buf) {
            //DPRINT("alloc success, %08X, entry %08X (unk %04X %08X ID %08X stack %04X %04X)", buf, entry,
            //    entry->unk08, entry->unk14, entry->mmUniqueIdent,
            //    entry->stack, entry->stack2);
            if((!PTR_VALID(entry)) || (entry->type > 1)) {
                //sanity check, in case our patch to return the heap entry
                //doesn't work. XXX why does this happen?
                OSReport("Alloc %08X got bad entry %08X (%d) ptr %08X sz %d",
                    lr, entry, PTR_VALID(entry) ? entry->type : -1,
                    buf, size);
            }
            else {
                //this seems to be unused (or not?)
                //it's set, but not read.
                entry->unk14 = lr;
            }
            return buf;
        }
        else {
            //alloc failed. try freeing something.
            if(!count) {
                u32 totalBlocks, totalBytes, usedBlocks, usedBytes;
                getFreeMemory(&totalBlocks, &totalBytes, &usedBlocks, &usedBytes,
                    NULL, NULL);
                OSReport("Out of memory! size=0x%X tag=0x%X Used blocks %d/%d, bytes %d/%d K - initiating emergency free",
                    size, tag, usedBlocks, totalBlocks, usedBytes/1024,
                    totalBytes/1024);
                OSReport("Trace: %08X < %08X < %08X < %08X < %08X\n",
                    __builtin_return_address(0),
                    __builtin_return_address(1),
                    __builtin_return_address(2),
                    __builtin_return_address(3),
                    __builtin_return_address(4));
                OSReport("force heaps 1/2=%d 3=%d\n", bOnlyUseHeaps1and2, bOnlyUseHeap3);
                for(int i=0; i<NUM_HEAPS; i++) {
                    OSReport("Heap %d used Blocks %5d/%5d KBytes %5d/%5d\n", i,
                        heaps[i].used, heaps[i].avail,
                        heaps[i].size / 1024, heaps[i].dataSize / 1024);
                }
            }
            if(!doEmergencyFree(count)) break;
            //OSReport("Emergency free attempt %d OK", count);
        }
        count += 1;
    }

    allocFailCount++;
    u32 totalBlocks, totalBytes, usedBlocks, usedBytes;
    getFreeMemory(&totalBlocks, &totalBytes, &usedBlocks, &usedBytes, NULL, NULL);
    OSReport("ALLOC FAIL size=0x%X tag=0x%X Used blocks %d/%d, bytes %d/%d K",
        size, tag, usedBlocks, totalBlocks, usedBytes/1024, totalBytes/1024);
    allocFailLog[allocFailLogIdx].size = size;
    allocFailLog[allocFailLogIdx].tag  = tag;
    allocFailLog[allocFailLogIdx].lr   = lr;
    allocFailLogIdx++;
    if(allocFailLogIdx >= ALLOC_FAIL_LOG_SIZE) allocFailLogIdx = 0;
    return NULL;
}

void allocInit() {
    hookBranch((u32)allocTagged, allocTaggedHook, 0);
}
