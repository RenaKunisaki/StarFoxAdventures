typedef enum { //names extracted from default.dol
    ALLOC_TAG_0,
    ALLOC_TAG_LISTS_COL,
    ALLOC_TAG_SCREEN_COL,
    ALLOC_TAG_CODE_COL,
    ALLOC_TAG_DLL_COL,
    ALLOC_TAG_TRACK_COL,
    ALLOC_TAG_TEX_COL,
    ALLOC_TAG_TRACKTEX_COL,
    ALLOC_TAG_SPRITETEX_COL,
    ALLOC_TAG_MODELS_COL,
    ALLOC_TAG_ANIMS_COL,
    ALLOC_TAG_AUDIO_COL,
    ALLOC_TAG_SEQ_COL,
    ALLOC_TAG_SFX_COL,
    ALLOC_TAG_OBJECTS_COL,
    ALLOC_TAG_CAM_COL,
    ALLOC_TAG_VOX_COL,
    ALLOC_TAG_ANIMSEQ_COL,
    ALLOC_TAG_LFX_COL,
    ALLOC_TAG_GFX_COL,
    ALLOC_TAG_EXPGFX_COL,
    ALLOC_TAG_MODGFX_COL,
    ALLOC_TAG_PROJGFX_COL,
    ALLOC_TAG_SKY_COL,
    ALLOC_TAG_SHAD_COL,
    ALLOC_TAG_GAME_COL,
    ALLOC_TAG_TEST_COL,
    ALLOC_TAG_BLACK,
    ALLOC_TAG_RED,
    ALLOC_TAG_GREEN,
    ALLOC_TAG_BLUE,
    ALLOC_TAG_CYAN,
    ALLOC_TAG_MAGENTA,
    ALLOC_TAG_YELLOW,
    ALLOC_TAG_WHITE,
    ALLOC_TAG_GREY,
    ALLOC_TAG_ORANGE,
    ALLOC_TAG_UNKNOWN, //added by me
    NUM_ALLOC_TAGS
} AllocTag;

//XXX some not in this table:
//FILE	0x7d7d7d7d
//COMPRESSED_FILE	0x7f7f7fff
//FACEFEED	0xfacefeed
//INTERSECT_POINT	0xffff00ff
//SAVEGAME	0xffffffff

//this table maps tags to colors.
//in some places the game specifies the color directly.
extern const u32 allocTagColorTbl[]; //drawheap.c

typedef enum {
    HEAP_ENTRY_TYPE_FREE = 0, //free block
    HEAP_ENTRY_TYPE_RAM  = 1, //RAM block
    //only types 1 and 4 can be freed.
    //the game seems to only use types 0 and 1.
    //presumably the others would have been for different
    //memory regions on the N64, such as cartridge ROM,
    //or for ARAM on the GC.
    HEAP_ENTRY_TYPE_RAM2 = 4, //unknown, but can be freed
} HeapEntryType;

typedef struct { //names extracted from debug messages
    void *loc;
    int   size;
    u16   type;  //HeapEntryType
    s16   prev;  //idx of previous block (-1 = none)
    s16   next;  //idx of next block (-1 = none)
    s16   stack; //idx of this block, but not always?
    u32   col;   //AllocTag
    int   unk14; //not unused
    int   mmUniqueIdent; //just incrementing unique ID
} SfaHeapEntry;
//usually stack == the index of the block, but sometimes it's not.

typedef struct {
    uint          dataSize; //total size
    u32           size;     //used size
    u32           avail;    //total blocks
    u32           used;     //used blocks (or just idx of last used block?)
    SfaHeapEntry *data;
} SfaHeap;

//these are not actually bool, they get set to -1 sometimes
extern int bOnlyUseHeaps1and2;
extern int bOnlyUseHeap3;


typedef enum {
    OBJ_FREE_IMMEDIATE = 0, //set by objFreeAll()
    OBJ_FREE_DEFERRED  = 2, //normal mode
} ObjFreeMode;
extern u32 objFreeMode; //ObjFreeMode
