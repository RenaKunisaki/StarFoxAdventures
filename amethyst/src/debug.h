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
} DebugTextFlags;

typedef enum {
    DEBUGRENDER_WORLD_MAP  = (1 <<  0),
    DEBUGRENDER_DEBUG_OBJS = (1 <<  1),
} DebugRenderFlags;

typedef enum {
    HEAP_DRAW_NONE = 0,
    HEAP_DRAW_BLOCK_GRAPH,
    HEAP_DRAW_BYTE_GRAPH,
    HEAP_DRAW_TOTALS,
    HEAP_DRAW_TABLE,
    NUM_HEAP_DRAW_MODES
} HeapDrawMode;

//debuglog.s
void debugPrintfHook(const char *fmt, ...);

//debugobjs.c
void debugObjsInit();

//debugprint.c
extern u32 debugTextFlags;
extern u32 debugRenderFlags;
void mainLoopDebugPrint();

//drawheap.c
extern u8 heapDrawMode;
void printHeaps();
