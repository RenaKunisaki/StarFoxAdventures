#include <sfa/sfa.h>

#define MOD_VERSION_MAJOR 2
#define MOD_VERSION_MINOR 0
#define MOD_VERSION_PATCH 2
#define __STR(x) #x
#define _STR(x) __STR(x)
#define MOD_VERSION_STR _STR(MOD_VERSION_MAJOR) "." _STR(MOD_VERSION_MINOR) "." _STR(MOD_VERSION_PATCH)

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })
#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })
#define ABS(n) \
    ({ __typeof__ (n) _n = (n); \
        _n < 0 ? -_n : _n; })

#include "alloc.h"
#include "camera.h"
#include "debug.h"
#include "krystal.h"
#include "menu.h"

typedef enum {
    MINIMAP_SIZE_SMALL = 0,
    MINIMAP_SIZE_NORMAL,
    MINIMAP_SIZE_BIG,
    NUM_MINIMAP_SIZES,
} OverrideMinimapSize;

typedef enum {
    //order same as backpack modes for consistency
    FURFX_NORMAL = 0,
    FURFX_NEVER,
    FURFX_ALWAYS,
    NUM_FURFX_MODES,
} FurFxMode;

typedef enum {
    DBGCHT_INF_HP       = (1 << 0),
    DBGCHT_INF_MP       = (1 << 1),
    DBGCHT_INF_MONEY    = (1 << 2),
    DBGCHT_INF_LIVES    = (1 << 3),
    DBGCHT_ENEMY_FROZEN = (1 << 4),
    DBGCHT_INF_TRICKY   = (1 << 5),
} DebugCheat;

//bsod.c
void bsodHook(void);

//drawbox.c
void drawBox(float x, float y, int w, int h, u8 opacity, bool fill);

//drawheap.c
void drawHeaps();

//freemove.c
extern bool bFreeMove;
void doFreeMove();

//gameBitLog.c
void gameBitHookInit();
void gameBitHookUpdate();

//hook.c
uint32_t hookBranch(uint32_t addr, void *target, int isBl);

//main.c
extern u32 debugCheats;
extern s16 overrideColorScale;
extern u8 overrideFov;
extern u8 furFxMode;
extern u16 dayOfYear, curYear;
extern bool bRumbleBlur;
extern bool bDisableParticleFx;

//menu.c
void runMenu();

//minimap.c
extern u8 overrideMinimapSize;
extern u8 overrideMinimapAlpha;
void minimapMainLoopHook();

//pdahook.c
void pdaHookInit();

//player.c
extern PlayerStateFunc origClimbWallFn;
void initPlayerStatesHook(void);
PlayerStateEnum playerStateClimbWallHook(double dT, ObjInstance *player, void *state);
void playerMainLoopHook();
void firstPersonHook(void *param1, void *param2);

//save.c
extern bool bAutoSave;
void saveLoadHook();
void saveUpdateHook();
void* saveMapLoadHook(MapDirIdx32 map, DataFileEnum file);
void saveShowMsgHook(int param);

//stafffx.c
void staffFxInit(void);

//startmsg.c
extern void (*runLoadingScreens_replaced)();
extern void (*startMsg_initDoneHook_replaced)();
void runLoadingScreens_hook();
void startMsg_initDoneHook();

//text.s
extern u8 textForceFixedWidth;
void textSizeHook();
void textDrawHook();

//texthook.c
void textHookInit();

//ui.c
bool motionBlurHook();
void hudDrawHook(int p1, int p2, int p3);

//util.c
char* bin2str(char *str, u32 val, int len);
int roundTo(int num, int target);

//worldmap.c
void worldMapHook();
