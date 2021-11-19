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

typedef enum {
    AMAP_BIN      = 0x32,
    AMAP_TAB      = 0x31,
    AMBIENT_BIN   = 0x05,
    AMBIENT_TAB   = 0x04,
    ANIM_BIN      = 0x30,
    ANIM_BIN2     = 0x4a,
    ANIM_TAB      = 0x2f,
    ANIM_TAB2     = 0x49,
    ANIMCURV_BIN  = 0x0d,
    ANIMCURV_TAB  = 0x0e,
    AUDIO_BIN     = 0x01,
    AUDIO_TAB     = 0x00,
    BITTABLE_BIN  = 0x33,
    BLOCKS_BIN    = 0x25,
    BLOCKS_BIN2   = 0x47,
    BLOCKS_TAB    = 0x26,
    BLOCKS_TAB2   = 0x48,
    CACHEFON_BIN  = 0x11,
    CACHEFON_BIN2 = 0x12,
    CAMACTIO_BIN  = 0x0b,
    DLLS_BIN      = 0x42,
    DLLS_TAB      = 0x43,
    DLLSIMPO_BIN  = 0x44,
    ENVFXACT_BIN  = 0x4f,
    FONTS_BIN     = 0x10,
    GAMETEXT_BIN  = 0x13,
    GAMETEXT_TAB  = 0x14,
    GLOBALMA_BIN  = 0x15,
    HITS_BIN      = 0x28,
    HITS_TAB      = 0x29,
    LACTIONS_BIN  = 0x0c,
    MAPINFO_BIN   = 0x1f,
    MAPS_BIN      = 0x1d,
    MAPS_TAB      = 0x1e,
    MODANIM_BIN   = 0x2e,
    MODANIM_TAB   = 0x2d,
    MODELIND_BIN  = 0x2c,
    MODELS_BIN    = 0x2b,
    MODELS_BIN2   = 0x46,
    MODELS_TAB    = 0x2a,
    MODELS_TAB2   = 0x45,
    MODLINES_BIN  = 0x37,
    MODLINES_TAB  = 0x38,
    MPEG_BIN      = 0x09,
    MPEG_TAB      = 0x08,
    MUSIC_BIN     = 0x07,
    MUSIC_TAB     = 0x06,
    MUSICACT_BIN  = 0x0a,
    NULL_FILE     = 0x50,
    NULL_FILE2    = 0x51,
    OBJECTS_BIN   = 0x3e,
    OBJECTS_TAB   = 0x3d,
    OBJEVENT_BIN  = 0x40,
    OBJHITS_BIN   = 0x41,
    OBJINDEX_BIN  = 0x3f,
    OBJSEQ_BIN    = 0x3b,
    OBJSEQ_TAB    = 0x3c,
    OBJSEQ2C_TAB  = 0x0f,
    SAVEGAME_BIN  = 0x39,
    SAVEGAME_TAB  = 0x3a,
    SCREENS_BIN   = 0x18,
    SCREENS_TAB   = 0x19,
    SFX_BIN       = 0x03,
    SFX_TAB       = 0x02,
    TABLES_BIN    = 0x16,
    TABLES_TAB    = 0x17,
    TEX0_BIN      = 0x23,
    TEX0_BIN2     = 0x4d,
    TEX0_TAB      = 0x24,
    TEX0_TAB2     = 0x4e,
    TEX1_BIN      = 0x20,
    TEX1_BIN2     = 0x4b,
    TEX1_TAB      = 0x21,
    TEX1_TAB2     = 0x4c,
    TEXTABLE_BIN  = 0x22,
    TRKBLK_TAB    = 0x27,
    VOXMAP_BIN    = 0x1b,
    VOXMAP_TAB    = 0x1a,
    VOXOBJ_BIN    = 0x36,
    VOXOBJ_TAB    = 0x35,
    WARPTAB_BIN   = 0x1c,
    WEAPONDA_BIN  = 0x34,
} DataFileEnum;
