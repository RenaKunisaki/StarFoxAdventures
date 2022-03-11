typedef enum { //u16
    ARG_SPAWN_POS  = (1 <<  0), //loadPos/loadMapLayer are valid
    ARG_DEBUG_TEXT = (1 <<  1), //debugTextFlags is valid
} ArgFlags;

typedef struct {
    u32   version;  //arg struct version (1)
    u32   debugCheats; //cheats to enable
    u32   debugTextFlags;
    u32   debugRenderFlags;
    s16   flags;
    s8    loadSave; //save slot to load (-1=don't, or 0-2)
    u8    loadMapLayer; //map layer and rotation to spawn
    vec3f loadPos; //coords to spawn at
} ArgStruct;

extern u8 didTryLoadArgs;
extern vec3f overrideSaveCoords;
extern u8 overrideSaveMapLayer;

int parseArgs(ArgStruct *args);
