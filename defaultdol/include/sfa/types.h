//predeclare some types
typedef struct Model Model;
typedef struct ObjInstance ObjInstance;
typedef struct Texture Texture;
typedef struct TextureHeader TextureHeader;

#define NUM_DLLS 0x2C2
#define NUM_FILES 80
#define NUM_HEAPS 4
#define NUM_MAPS 120
#define NUM_MAP_DIRS 80
#define OBJ_SEQ_MAX 0x54
#define PLAYER_POS_LOG_SIZE 60

typedef s16 DataFileEnum16;
typedef s32 DataFileEnum32;
typedef s16 DLL_ID16;
typedef s32 DLL_ID32;
typedef s32 DLL_ID;
typedef s16 GameBit16;
typedef s32 GameBit32;
typedef u16 GamepadButton;
typedef u32 GamepadButton32;
typedef s16 GameTextId;
typedef s16 GameTextId16;
typedef s32 GameTextId32;
typedef s8  GameTextWindow8;
typedef s16 GameTextWindow16;
typedef s32 GameTextWindow32;
typedef s8  MapDirIdx8;
typedef s16 MapDirIdx16;
typedef s32 MapDirIdx32;
typedef s8  MapId8;
typedef s16 MapId16;
typedef s32 MapId32;
typedef s32 ObjUniqueId;
typedef s32 PlayerStateEnum32;

#include "math.h"
#include "gx/gx.h"
#include "gx/dlist.h"
#include "gx/opcodes.h"
