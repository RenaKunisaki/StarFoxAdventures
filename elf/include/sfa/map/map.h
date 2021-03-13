//most of this isn't used, but would have been used by a debug version
//of the game.
typedef struct PACKED {
    char name[28];
    u8   type;  //actually used
    u8   unk1D;
    u16  unk1E;
} MapInfoEntry;
CASSERT(sizeof(MapInfoEntry) == 0x20, sizeof_MapInfoEntry);
