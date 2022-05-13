//for debugging the hacks, this can be toggled
#define DPRINT OSReport

typedef enum {
    DEBUGTEXT_TRICKY            = (1 <<  0),
    DEBUGTEXT_PLAYER_COORDS     = (1 <<  1),
    DEBUGTEXT_CAMERA_COORDS     = (1 <<  2),
    DEBUGTEXT_RESTART_POINT     = (1 <<  3),
    DEBUGTEXT_MEMORY_INFO       = (1 <<  4),
    DEBUGTEXT_INTERACT_OBJ_INFO = (1 <<  5),
    DEBUGTEXT_HEAP_STATE        = (1 <<  6),
    DEBUGTEXT_GAMEBIT_LOG       = (1 <<  7),
    DEBUGTEXT_PLAYER_STATE      = (1 <<  8),
    DEBUGTEXT_PERFORMANCE       = (1 <<  9),
    DEBUGTEXT_FPS               = (1 << 10),
    DEBUGTEXT_RNG               = (1 << 11),
    DEBUGTEXT_AUDIO_STREAMS     = (1 << 12),
    DEBUGTEXT_AUDIO_SFX         = (1 << 13),
    DEBUGTEXT_ENVIRONMENT       = (1 << 14),
    DEBUGTEXT_OBJSEQ            = (1 << 15),
} DebugTextFlags;

typedef enum {
    DEBUGRENDER_WORLD_MAP      = (1 <<  0),
    DEBUGRENDER_DEBUG_OBJS     = (1 <<  1),
    DEBUGRENDER_PERF_METERS    = (1 <<  2),
    DEBUGRENDER_RNG            = (1 <<  3),
    DEBUGRENDER_HITBOXES       = (1 <<  4),
    DEBUGRENDER_ATTACH_POINTS  = (1 <<  5),
    DEBUGRENDER_FOCUS_POINTS   = (1 <<  6),
    DEBUGRENDER_UNK_POINTS     = (1 <<  7),
    DEBUGRENDER_PLAYER_VEL     = (1 <<  8),
    DEBUGRENDER_HIDE_DISABLED  = (1 <<  9),
    DEBUGRENDER_DEBUG_MAP_GEOM = (1 << 10),
} DebugRenderFlags;

typedef enum {
    HEAP_DRAW_NONE = 0,
    HEAP_DRAW_BLOCK_GRAPH,
    HEAP_DRAW_BYTE_GRAPH,
    HEAP_DRAW_TOTALS,
    HEAP_DRAW_TABLE,
    NUM_HEAP_DRAW_MODES
} HeapDrawMode;

enum RngMode {
    RNG_MODE_NORMAL = 0, //normal RNG
    RNG_MODE_ZERO, //always return 0
    RNG_MODE_ONE, //always return 1
    RNG_MODE_MAX, //always return RAND_MAX
    RNG_MODE_INC, //return incrementing values
    RNG_MODE_ANALOG, //return controller 4 R trigger
    NUM_RNG_MODES
};

//debuglog.s
void debugPrintfHook(const char *fmt, ...);

//debugobjs.c
void debugObjsInit();

//debugprint.c
extern u32 debugTextFlags;
extern u32 debugRenderFlags;
//debugPrintSetColor is already in the game
void debugPrintSetPos(s16 x, s16 y);
void debugPrintSetBgColor(u8 r, u8 g, u8 b, u8 a);
void mainLoopDebugPrint();

//drawheap.c
extern u8 heapDrawMode;
void printHeaps();
