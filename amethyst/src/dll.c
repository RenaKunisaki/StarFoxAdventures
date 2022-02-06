#include "main.h"
#include "revolution/os.h"

void* dll_load_hook(int dll_id, u16 param) {
    //replaces the body of dll_load()
    OSReport("dll_load(0x%04X, 0x%04X)\n", dll_id, param);

    //HACK to fix dragrockbot
    if(dll_id == 0x27F) dll_id = 0x131;

    if(dll_id >= 0 && dll_id < NUM_DLLS) {
        DLL *dll = g_dlls[dll_id];
        if(g_dllRefCount[dll_id] == 0) {
            if(dll->initialise == NULL) { /* do nothing */ }
            else if(!PTR_VALID(dll->initialise)) {
                OSReport("ERR: Trying to load DLL 0x%04X with invalid constructor 0x%08X\n",
                    dll_id, dll->initialise);
            }
            else dll->initialise(dll, param);
        }
        g_dllRefCount[dll_id]++;
        g_dllsLoaded[dll_id] = &dll->functions;
        return &g_dllsLoaded[dll_id];
    }
    else {
        OSReport("ERR: Trying to load invalid DLL 0x%04X\n", dll_id);
        return NULL;
    }
}

void dllHooksInit() {
    hookBranch(0x80013ec8, dll_load_hook, 0);
}

