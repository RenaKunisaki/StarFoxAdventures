#include <sfa/sfa.h>

#define MOD_VERSION_MAJOR 2
#define MOD_VERSION_MINOR 0
#define MOD_VERSION_PATCH 0
#define __STR(x) #x
#define _STR(x) __STR(x)
#define MOD_VERSION_STR _STR(MOD_VERSION_MAJOR) "." _STR(MOD_VERSION_MINOR) "." _STR(MOD_VERSION_PATCH)

//debuglog.s
void debugPrintfHook(const char *fmt, ...);

//hook.c
uint32_t hookBranch(uint32_t addr, void *target, int isBl);

//startmsg.c
extern void (*runLoadingScreens_replaced)();
extern void (*startMsg_initDoneHook_replaced)();
void runLoadingScreens_hook();
void startMsg_initDoneHook();
