#include "main.h"

//set to 1 when we try to load the args file, whether it's
//successful or not. prevents trying to load it again on
//following frames.
//reset to 0 once in-game so we'll load the file again
//on reset.
u8 didTryLoadArgs = 0;

//coords to override save file from args
vec3f overrideSaveCoords;
u8 overrideSaveMapLayer = 0x7F; //7F=don't override

int parseArgs(ArgStruct *args) {
    DPRINT("Using args file\n");
    if(args->version != 1) {
        OSReport("Unsupported args file version: 0x%08X\n", args->version);
        return -1;
    }
    //DPRINT("arg cheat=%08X text=%08X render=%08X save=%d\n",
    //    args->debugCheats, args->debugTextFlags, args->debugRenderFlags,
    //    args->loadSave);
    //DPRINT("arg pos=%d, %f, %f, %f\n", args->loadMapLayer,
    //    args->loadPos.x, args->loadPos.y, args->loadPos.z);

    debugCheats          = args->debugCheats;
    debugRenderFlags     = args->debugRenderFlags;
    if(args->flags & ARG_DEBUG_TEXT) debugTextFlags = args->debugTextFlags;
    if(args->flags & ARG_SPAWN_POS) {
        overrideSaveCoords   = args->loadPos;
        overrideSaveMapLayer = args->loadMapLayer;
    }
    else {
        overrideSaveCoords.x = 0;
        overrideSaveCoords.y = 0;
        overrideSaveCoords.z = 0;
    }

    if(debugTextFlags) enableDebugText = 1;
    if(args->loadSave >= 0) {
        DPRINT("arg: loadSave(%d)\n", args->loadSave);
        Amethyst_loadSaveFile(args->loadSave);
    }
    //else DPRINT("arg: not loading save (%d)\n", args->loadSave);

    return 0;
}
