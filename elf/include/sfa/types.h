#define PACKED __attribute__((packed))

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
