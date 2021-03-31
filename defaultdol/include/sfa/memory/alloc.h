typedef enum {
    ALLOC_TAG_GENERIC               =        0x0,
    ALLOC_TAG_SCREENS_BIN           =        0x2,
    ALLOC_TAG_MAP_BLOCKS            =        0x5,
    ALLOC_TAG_TEXTURE               =        0x6,
    ALLOC_TAG_MODEL_DATA            =        0x9,
    ALLOC_TAG_MODELS                =        0xA,
    ALLOC_TAG_AUDIO                 =        0xB,
    ALLOC_TAG_OBJECTS               =        0xE,
    ALLOC_TAG_CAMERA_PARAMS         =        0xF,
    ALLOC_TAG_VOX                   =       0x10,
    ALLOC_TAG_STACK                 =       0x11, //data structure, not thread stack
    ALLOC_TAG_WATERFX               =       0x13,
    ALLOC_TAG_VEC3F_ARRAY           =       0x18,
    ALLOC_TAG_MODELSTRUCT           =       0x1A,
    ALLOC_TAG_UNKNOWN_32BYTE_BUFFER =       0xFF,
    ALLOC_TAG_FILE                  = 0x7D7D7D7D,
    ALLOC_TAG_COMPRESSED_FILE       = 0x7F7F7FFF,
    ALLOC_TAG_FACEFEED              = 0xFACEFEED,
    ALLOC_TAG_INTERSECT_POINT       = 0xFFFF00FF,
    ALLOC_TAG_SAVEGAME              = 0xFFFFFFFF,
} AllocTag;

typedef struct { //names extracted from debug messages
    void *loc;
    int   size;
    u16   unk08;
    short idx;
    short stack2;
    short stack;
    u32   col; //AllocTag
    int   unk14;
    int   mmUniqueIdent;
} SfaHeapEntry;

typedef struct {
    uint          dataSize; //total size
    u32           size;     //used size
    u32           avail;    //total blocks
    u32           used;     //used blocks
    SfaHeapEntry *data;
} SfaHeap;

//these are not actually bool, they get set to -1 sometimes
extern int bOnlyUseHeaps1and2;
extern int bOnlyUseHeap3;
