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
    DEBUGTEXT_HEAP_GRAPH        = (1 <<  9), //not really text but oh well
    DEBUGTEXT_WORLD_MAP         = (1 << 10),
} DebugTextFlags;

//debuglog.s
void debugPrintfHook(const char *fmt, ...);

//debugprint.c
extern u32 debugTextFlags;
void mainLoopDebugPrint();
