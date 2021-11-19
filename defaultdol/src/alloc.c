#include "main.h"

//for zlib
void* malloc(size_t size) {
    //OSReport("malloc(%d)\n", size);
    return allocTagged(size, 0x75110420, "malloc");
}
void* calloc(size_t nitems, size_t size) {
    //we don't need to clear, it's already cleared for us
    //OSReport("calloc(%d, %d)\n", nitems, size);
    return allocTagged(size * nitems, 0x75110420, "calloc");
}
void* zalloc(void *stream, size_t nitems, size_t size) {
    //OSReport("zalloc(%08X, %d, %d) ", wtf, nitems, size);
    void *r = allocTagged(size * nitems, 0x75110420, "zalloc");
    //OSReport("-> %08X\n", r);
    return r;
}
void zfree(void *stream, void *ptr) {
    //OSReport("zfree(%08X, %08X)\n", wtf, ptr);
    free(ptr);
}

void* allocTaggedHook(u32 size, u32 tag, char *name) {
    //replaces the body of allocTagged.
    //pretty much the same logic, but flags invalid sizes.
    u32 lr = (u32)__builtin_return_address(0);
    //OSReport("alloc size=%8X tag=%08X lr=%08X name=\"%s\"",
    //    size, tag, lr, (char*)name);
    //tag = lr; //this is more useful

    if(size >= 0x01800000) {
        OSReport("Invalid alloc size (%08X)", size);
        HALT();
    }

    void *result;
    uint tag_;

    if (size == 0) {
        result = NULL;
    }
    else {
        tag_ = tag;
        if (tag < 0x1b) {
            tag_ = allocTagColorTbl[tag];
        }
        if (size < 0x3000) {
            if (size < 0x400) {
                result = heapAlloc(2,size,tag_,name);
                if (result == NULL) {
                    result = heapAlloc(0,size,tag_,name);
                }
            }
            else {
                result = heapAlloc(1,size,tag_,name);
                if (result == NULL) {
                    result = heapAlloc(2,size,tag_,name);
                }
            }
        }
        else {
            result = heapAlloc(0,size,tag_,name);
            if (result == NULL) {
                result = heapAlloc(1,size,tag_,name);
            }
        }
    }
    //OSReport(" -> %08X\n", result);

    //the game seems to expect newly allocated buffers to be cleared.
    //it doesn't do this itself, but...
    if(result) memset(result, 0x00, size);

    return result;
}
