//predeclare some types
typedef struct Model Model;
typedef struct ObjInstance ObjInstance;
typedef struct Texture Texture;
typedef struct TextureHeader TextureHeader;

#define NUM_DLLS 0x2C2
#define NUM_FILES 0x5A
#define NUM_HEAPS 4
#define NUM_MAPS 120
#define NUM_MAP_DIRS 80
#define OBJ_SEQ_MAX 0x54

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
#include "debug/debug.h"
#include "dll/dll.h"
#include "data/fileids.h"
#include "data/zlb.h"
#include "gx/gx.h"
#include "gx/dlist.h"
#include "gx/opcodes.h"
#include "gametext/gametext.h"
#include "objects/objcats.h"
#include "objects/objseq.h"
#include "objects/objdata.h"
#include "objects/objhits.h"
#include "assets/texture.h"
#include "assets/model.h"
#include "objects/objdef.h"
#include "objects/objinstance.h"
#include "objects/camera.h"
#include "assets/assets.h"
#include "memory/alloc.h"
#include "audio/audio.h"
#include "map/map.h"
#include "game/game.h"
#include "game/gamebits.h"
#include "game/cheats.h"
#include "game/player/playerstate.h"
#include "game/save/savegame.h"
#include "ui/pda.h"
