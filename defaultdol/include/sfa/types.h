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
#define OBJECT_NAME_LEN 15 //11 for final

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

typedef struct ObjDef {
	s16   objType;       //0x00 ObjDefEnum
	byte  allocatedSize; //0x02
	byte  mapActs1;      //0x03
	u8    loadFlags;     //0x04 RomListObjLoadFlags
	byte  mapActs2;      //0x05
	byte  bound;         //0x06
	byte  unk7;          //0x07
	vec3f pos;           //0x08
	u32   id;            //0x14 ObjUniqueId
} ObjDef;
CASSERT(sizeof(ObjDef) == 0x18, sizeof_ObjDef);

//XXX
typedef u32 ObjSeqCmd;
typedef void AButtonInteraction;
typedef void ObjWeaponData;
typedef void AttachPoint;
typedef void ModLine;

//much of this is copied from SFA and probably wrong
typedef struct PACKED ObjData {
/*0x00*/ float               unk00; //copied to shadow field 0
/*0x04*/ float               scale;
/*0x08*/ u32*                pModelList; //-> list of model IDs
/*0x0c*/ pointer             textures;
/*0x10*/ pointer             unk10;
/*0x14*/ pointer             unk14;
/*0x18*/ ObjSeqCmd*          unk18;
/*0x1c*/ u16*                pSeq; //[OPTIONAL] -> seq IDs
/*0x20*/ undefined*          pEvent; //[OPTIONAL] -> event IDs, ends with -1
/*0x24*/ undefined*          pHits; //[OPTIONAL]
/*0x28*/ ObjWeaponData*      pWeaponDa; //[OPTIONAL]
/*0x2c*/ AttachPoint*        pAttachPoints;
/*0x30*/ ModLine*            pModLines; //ignored in file (zeroed on load)
/*0x34*/ pointer	            wObjList; //ignored in file (zeroed on load)
/*0x38*/ pointer	            nextIntersectPoint;
/*0x3c*/ pointer	            nextIntersectLine;
/*0x40*/ AButtonInteraction* aButtonInteraction; //[OPTIONAL] count in field 0x72 -> something 0x12 or 0x18 bytes per model
/*0x44*/ u32                 flags;
/*0x48*/ s16                 shadowType;
/*0x4a*/ s16                 shadowTexture;
/*0x4c*/ undefined           unk4c;
/*0x4d*/ undefined           unk4d;
/*0x4e*/ u16                 hitbox_flags60;
/*0x50*/ undefined           unk50;
/*0x51*/ undefined           unk51;
/*0x52*/ undefined           unk52;
/*0x53*/ undefined           unk53;
/*0x54*/ undefined           unk54;
/*0x55*/ undefined           unk55;
/*0x56*/ byte                numPlayerObjs; //if > 0, objAddObjectType(obj, 8)
/*0x57*/ u8                  unk57;
/*0x58*/ u16	                dll_id;
/*0x5a*/ u16	                objId;
/*0x5c*/ byte                modLinesSize; //ignored in file
/*0x5d*/ s8                  nModels;
/*0x5e*/ u8                  numSeqs;
/*0x5f*/ char                name[OBJECT_NAME_LEN+1]; //+1 for null
/*0x6f*/ undefined           unk6f;
/*0x70*/ byte                hitbox_fieldB4; //related to hitbox (height?)
/*0x71*/ u8                  nTextures;
/*0x72*/ byte                nVecs;
/*0x73*/ byte                stateVar73; //1=translucent; 3=invincible - not flags
/*0x74*/ bool                bDisableHits;
/*0x75*/ undefined           unk75;
/*0x76*/ s16                 modLineCount;
/*0x78*/ s16                 modLineNo;
/*0x7a*/ undefined           unk7a;
/*0x7b*/ undefined           unk7b;
/*0x7c*/ undefined           unk7c;
/*0x7d*/ undefined           unk7d;
/*0x7e*/ undefined           unk7e;
/*0x7f*/ undefined           unk7f;
/*0x80*/ undefined           unk80;
/*0x81*/ undefined           unk81;
/*0x82*/ undefined           unk82;
/*0x83*/ byte                cend;
/*0x84*/ byte                csize;
/*0x85*/ byte                ambient;
/*0x86*/ byte                intensity;
/*0x87*/ byte                lightFlags;
/*0x88*/ float               lagVar88; //causes lag at ~65536.0; GPU hang at much more; related to shadow; maybe causing excessive map loads?
/*0x8c*/ byte                nLights;
/*0x8d*/ byte                lightIdx;
/*0x8e*/ byte                unk8E; //related to textures; 1=dark, 2=default, 3+=corrupt, 77=crash, 0=normal
/*0x8f*/ byte                maybeNumHits; //related to hitbox
/*0x90*/ u8                  hitbox_flagsB6; // < 0xE = invincible
/*0x91*/ undefined           unk91;
/*0x92*/ undefined           unk92;
/*0x93*/ byte                flags93;
/*0x94*/ undefined           unk94;
/*0x95*/ undefined           unk95;
/*0x96*/ undefined           unk96;
/*0x97*/ undefined           unk97;
/*0x98*/ undefined           unk98;
/*0x99*/ undefined           unk99;
/*0x9a*/ undefined           unk9a;
/*0x9b*/ byte                field74_count;
/*0x9c*/ undefined           unk9c;
/*0x9d*/ undefined           unk9d;
/*0x9e*/ undefined           unk9e;
/*0x9f*/ undefined           unk9f;
/*0xa0*/ undefined           unka0;
/*0xa1*/ undefined           unka1;
} ObjData;
CASSERT(sizeof(ObjData) == 0xA2, sizeof_ObjData); //XXX correct?

typedef enum {
    ASSET_TYPE_FILE = 0,
    ASSET_TYPE_FILE_WITH_OFFSET,
    ASSET_TYPE_FILE_WITH_ID_SIZE,
    ASSET_TYPE_TEXTURE,
    ASSET_TYPE_CHARACTER,
    ASSET_TYPE_DLL,
    ASSET_TYPE_MODEL_INSTANCE, //unused in final
    ASSET_TYPE_ANIMATION,
    NUM_ASSET_TYPES
} AssetTypeEnum;

typedef struct {
    bool    unk00;
    u8      type; //AssetTypeEnum
    u8      unk02; //padding?
    u8      unk03;
    union {
        struct {
            u32    id;
            void **buf;
            u32    len;
            u32    offset;
        } file;

        struct {
            int    id;
            Texture **buf;
        } texture;

        struct {
            u32           unused04; //04
            ObjInstance **buf;      //08
            u32           unused0C; //0C
            u32           unused10; //10
            ObjInstance  *obj;      //14
            ObjDef       *objDef;   //18
            u32           flags;    //1C
            u32           unused20; //20
            int           mapId;    //24
            u32           unk;      //28 unused param?
        } character;

        struct {
            int    id;          //04
            void **buf;         //08 DLL*
            u32    exportCount; //0C
        } dll;

        struct {
            int    id;    //04
            void **buf;   //08 ModelInstance*
            u32    flags; //0C
        } modelInstance;

        struct {
            int    id;       //04
            void **buf;      //08 Animation*
            int    id2;      //0C
            int    unused10; //10
            int    unused14; //14
            int    unused18; //18
            int    unused1C; //1C
            void  *dest;     //20
            Model *model;    //24
        } animation;
    };
} AssetDef;
CASSERT(sizeof(AssetDef) == 0x2C, sizeof_AssetDef);
