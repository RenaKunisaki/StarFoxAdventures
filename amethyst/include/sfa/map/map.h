typedef struct PACKED {
    //most of this isn't used, but would have been used by a debug version
    //of the game.
    char name[28];
    u8   type;  //actually used
    u8   unk1D;
    u16  unk1E;
} MapInfoEntry;
CASSERT(sizeof(MapInfoEntry) == 0x20, sizeof_MapInfoEntry);

typedef struct {
    u8 unk[0x20C]; //XXX find a way to extract this from ghidra
    float timeOfDay; //0x20C
} SkyStruct;
extern SkyStruct *pSkyStruct;
