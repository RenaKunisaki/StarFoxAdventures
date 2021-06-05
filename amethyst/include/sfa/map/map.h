#define MAP_CELL_SCALE 640.0

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
    s32	infoOffset; //Offset of map info (MapsBinInfoEntry*)
    s32	blockTable; //Offset of map block list
    s32	rects[4];   //rects that somehow define visible regions
    s32	listSize;   //a FACEFEED header which gives the size to allocate for the romlist
} MapsTabEntry;
CASSERT(sizeof(MapsTabEntry) == 0x1C, sizeof_MapsTabEntry);

typedef struct {
    u16	width;   //Number of columns
    u16	height;  //Number of rows
    u16	originX; //Which column is the origin
    u16	originY; //Which row is the origin
    //XXX more here?
} MapsBinInfoEntry;
CASSERT(sizeof(MapsBinInfoEntry) == 8, sizeof_MapsBinInfoEntry);

typedef struct {
    s16 x; //global X coord
    s16 z; //global Z coord
    s16 layer; //map layer
    s16 mapId; //map ID (<0 = end of list)
    s16 link[2]; //linked map IDs
} GlobalMaEntry;
CASSERT(sizeof(GlobalMaEntry) == 0xC, sizeof_GlobalMaEntry);

typedef struct {
    u8 unk[0x20C]; //XXX find a way to extract this from ghidra
    float timeOfDay; //0x20C
} SkyStruct;
extern SkyStruct *pSkyStruct;
