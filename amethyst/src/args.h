typedef struct {
    u32 version;  //arg struct version (1)
    u32 debugCheats; //cheats to enable
    u32 debugTextFlags;
    u32 debugRenderFlags;
    s8  loadSave; //save slot to load (-1=don't, or 0-2)
    s8  loadMapLayer; //map layer to spawn at (0x7F=use save file)
    s8  unused[2]; //padding
    vec3f loadPos; //coords to spawn at
} ArgStruct;

extern u8 didTryLoadArgs;
extern vec3f overrideSaveCoords;
extern s8 overrideSaveMapLayer;

int parseArgs(ArgStruct *args);
