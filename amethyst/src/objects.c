#include "main.h"

bool isObjectEnabled(ObjInstance *obj) {
    if(!PTR_VALID(obj)) return false;
    switch(obj->file->dll_id) {
        case 0x174: //CCriverflow
            return *((u8*)obj->state);

        //XXX more
        default: return true;
    }
}
