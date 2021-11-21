#include <sfa/sfa.h>

#define MOD_VERSION_MAJOR 2
#define MOD_VERSION_MINOR 0
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

#define CHARS_TO_U32(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
#define HALT() do { } while(1)

typedef uint32_t size_t;

//bsod.c
void initBsodHooks();

//compression.c
int decompress(byte *src, int compLen, byte *dest, int *outLen);
void initCompressionHooks();

//hook.c
uint32_t hookBranch(uint32_t addr, void *target, int isBl);

//logging.c
void initLoggingHooks();

//models.c
void initModelHooks();

//texture.c
void initTextureHooks();

//zlb.c
int zlbDecompress(void *data, uint compLen, void *out, uint outLen);
