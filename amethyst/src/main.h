#include <sfa/sfa.h>

#define MOD_VERSION_MAJOR 2
#define MOD_VERSION_MINOR 1
#define MOD_VERSION_PATCH 0
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

typedef enum {
    HUD_LOW_HP_FLASH = (1 << 0), //flash hearts continuously when low HP
    HUD_LOW_HP_BEEP  = (1 << 1), //for players who hate themselves
} HudFlag;

typedef enum { //for savedata (field unused01)
    EXTRA_FEATURE_RUMBLE_BLUR   = (1 << 0),
    EXTRA_FEATURE_NO_PARTICLEFX = (1 << 1),
} ExtraFeatureFlag;

//see also CameraFlags in camera.h

//bsod.c
void bsodHook(void);

//bugfixes.c
void initBugFixes();

//drawarrow.c
void drawArrow(vec3f pos, vec3s rot, float scale, Color4b color);

//drawbox.c
void drawBox(float x, float y, int w, int h, u8 opacity, bool fill);

//drawheap.c
void drawHeaps();

//drawmap.c
void drawMapGrid();

//drawsphere.c
extern const float pi;
extern const float two_pi;
void drawSolidVtx(vec3f pos, Color4b *color);
void drawSphere(vec3f pos, float radius, Color4b color);

//freemove.c
extern bool bFreeMove;
extern vec3f freeMoveCoords;
void doFreeMove();

//gameBitLog.c
void gameBitHookInit();
void gameBitHookUpdate();

//hitbox.c
void hitboxHooksInit();

//hook.c
uint32_t hookBranch(uint32_t addr, void *target, int isBl);

//hud.c
extern uint8_t hudFlags;
void doHudHacks();

//loghits.c
void printHits();
void logHitsInit();

//main.c
extern u32 debugCheats;
extern s16 overrideColorScale;
extern u8 overrideFov;
extern u8 furFxMode;
extern u16 dayOfYear, curYear;
extern bool bRumbleBlur;
extern bool bDisableParticleFx;

//map.c
void initMapHacks();

//menu.c
void runMenu();

//menuDebugGameBits.c
extern s8 *bitNames;
BitTableEntry* getBitTableEntry(int bit);
const char* getBitName(int bit);

//minimap.c
extern u8 overrideMinimapSize;
extern u8 overrideMinimapAlpha;
void minimapMainLoopHook();

//pdahook.c
void pdaHookInit();

//perf.c
extern u64 tLoopStart;   //start time of main loop
extern u64 tLoopEnd;     //end   time of main loop
extern u64 tLogicStart;  //start time of game logic section
extern u64 tLogicEnd;    //end   time of game logic section
extern u64 tRenderStart; //start time of render section
extern u64 tRenderEnd;   //end   time of render section
extern u64 tLoop, tLogic, tRender; //durations
extern float pctLogic;
extern float pctRender;
extern float pctTotal;
void renderPerfMeters();
void perfMonInit();

//player.c
extern PlayerStateFunc origClimbWallFn;
void initPlayerStatesHook(void);
PlayerStateEnum playerStateClimbWallHook(double dT, ObjInstance *player, void *state);
void playerMainLoopHook();
void firstPersonHook(void *param1, void *param2);

//race.c
void raceTimerToggle(bool start);
void raceTimerUpdate();

//random.c
extern u32 rngCalls;
u32 rngHook();
void drawRNG();
void printRNG();

//save.c
extern bool bAutoSave;
void saveLoadHook();
void updateSaveData();
void saveUpdateHook();
void* saveMapLoadHook(MapDirIdx32 map, DataFileEnum file);
void saveShowMsgHook(int param);

//sort.c
typedef int(*CompareFunc)(const void *itemA, const void *itemB);
//void quicksortStrings(const char **items, int iStart, int iEnd);
void quicksort(const void **items, int iStart, int iEnd, CompareFunc compare);

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

//title.c
void titleHooksInit();

//ui.c
bool motionBlurHook();
void hudDrawHook(int p1, int p2, int p3);

//util.c
char* bin2str(char *str, u32 val, int len);
int roundTo(int num, int target);
void getObjName(char *dest, ObjInstance *obj);
void getObjFileName(char *dest, ObjectFileStruct *file);
int strcmpi(const char *sa, const char *sb);
int compareObjsByType(const void *objA, const void *objB);
int compareObjsById(const void *objA, const void *objB);
int compareObjsByName(const void *objA, const void *objB);
int compareObjsByDistance(const void *objA, const void *objB);
int compareObjsByAddr(const void *objA, const void *objB);
double u64toDouble(u64 val);
double ticksToSecs(u64 ticks);
Color4b hsv2rgb(u8 h, u8 s, u8 v, u8 a);

//worldmap.c
void worldMapHook();
