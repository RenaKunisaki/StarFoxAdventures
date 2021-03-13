//XXX move these
#define PACKED __attribute__((packed))
#define WRITE8( addr, val) (*(u8*) addr) = (val)
#define WRITE16(addr, val) (*(u16*)addr) = (val)
#define WRITE32(addr, val) (*(u32*)addr) = (val)
#define WRITE64(addr, val) (*(u64*)addr) = (val)
#define WRITEFLOAT(addr, val) (*(float*)addr) = (val)
#define WRITEDOUBLE(addr, val) (*(double*)addr) = (val)
#define READ8( addr) (*(u8* )addr)
#define READ16(addr) (*(u16*)addr)
#define READ32(addr) (*(u32*)addr)
#define READ64(addr) (*(u64*)addr)
#define READFLOAT(addr) (*(float*)addr)
#define READDOUBLE(addr) (*(double*)addr)

//predeclare some types
typedef struct Model Model;
typedef struct ObjInstance ObjInstance;
typedef struct Texture Texture;
typedef struct TextureHeader TextureHeader;

#include "math.h"
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
#include "alloc.h"
#include "audio.h"
#include "game/gamebits.h"
#include "game/cheats.h"
#include "game/player/playerstate.h"
