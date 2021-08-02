/** Restore Krystal as a playable character throughout the game.
 */
#include "main.h"
#include "revolution/os.h"


s8 krystalModelNo = 0;
s8 enableKrystal = 0; //delay loading due to memory stuff
s8 overridePlayerNo = PLAYER_ID_KRYSTAL; //OverridePlayerId
s8 backpackMode = BACKPACK_NORMAL; //BackpackMode
static void *krystalModel         = NULL;
static u32   krystalModelSize     = 0;
static u32   krystalModelOffset   = 0;
static void *krystalTexture       = NULL;
static u32   krystalTextureSize   = 0;
static u32   krystalTextureOffset = 0;

void krystal_loadAssets() {
    //do not load on title screen due to memory starvation
    if(!enableKrystal) return;

    //load model
    if(!krystalModel) {
        char pathMod[] = {'k', 'm', krystalModelNo+0x30, 0};
        DPRINT("Loading %s", pathMod);
        krystalModel = loadFileByPath(pathMod, &krystalModelSize);
        if(krystalModel) {
            DPRINT("Loaded %s @0x%08X size 0x%08X", pathMod, krystalModel,
                krystalModelSize);
        }
        else DPRINT("LOAD FAILED(%s)", pathMod);
    }

    //load texture
    if(!krystalTexture) {
        char pathTex[] = {'k', 't', krystalModelNo+0x30, 0};
        DPRINT("Loading %s", pathTex);
        krystalTexture = loadFileByPath(pathTex, &krystalTextureSize);
        if(krystalTexture) {
            DPRINT("Loaded %s @0x%08X size 0x%08X", pathTex, krystalTexture,
                krystalTextureSize);
        }
        else DPRINT("LOAD FAILED(%s)", pathTex);
    }
}

//replaces a call to allocTagged() when loading MODELS.BIN
void* krystalHook_modelsBin(uint size,AllocTag tag,const char *name) {
    //copy Krystal model into MODELS
    krystal_loadAssets();
    if(!krystalModel) return allocTagged(size, tag, name);

    //do the original allocation, with the extra model's size added.
    void *buf = allocTagged(size+krystalModelSize, tag, name);
    if(!buf) {
        DPRINT("ALLOC FAILED for Krystal model");
        return allocTagged(size, tag, name); //try again with original size
    }
    registerFreeablePtr((void**)&krystalModel);

    //copy the new model into the buffer.
    krystalModelOffset = size;
    memcpy(buf + size, krystalModel, krystalModelSize);
    return buf;
}

//injected into mergeTableFiles() for MODELS.TAB
void krystalHook_modelsTab() {
    krystal_loadAssets();
    if(!krystalModel) return;
    //update the table to reference the Krystal model
    void *pModelsBin = dataFileBuffers[FILE_MODELS_BIN];
    u32  *pModelsTab = dataFileBuffers[FILE_MODELS_TAB];
    if(pModelsBin && pModelsTab) {
        if((*(u32*)(pModelsBin + krystalModelOffset)) == 0xFACEFEED) {
            //there is a model here, so update the table to point to it
            pModelsTab[KRYSTAL_MODEL_ID] = krystalModelOffset | 0x80000000;
        }
    }

    //patch secondary slot
    pModelsBin = dataFileBuffers[FILE_MODELS_BIN2];
    pModelsTab = dataFileBuffers[FILE_MODELS_TAB2];
    if(pModelsBin && pModelsTab) {
        if((*(u32*)(pModelsBin + krystalModelOffset)) == 0xFACEFEED) {
            pModelsTab[KRYSTAL_MODEL_ID] = krystalModelOffset | 0x80000000;
        }
    }
}

//gcc doesn't support naked functions on PPC because Reasons,
//so we need to build a little trampoline.
//Also we can't use a macro for the stack frame size because lol.
void _krystalHook_modelsTab(void);
__asm__(
    "_krystalHook_modelsTab: \n"
    ASM_FUNC_START(0x100)
    "bl krystalHook_modelsTab\n"
    "endTabPatch:            \n"
    ASM_FUNC_END(0x100)
    "lis     11, 0x8004      \n"
    "ori     11, 11, 0x3df4  \n"
    "mtctr   11              \n"
    "bctr                    \n"
);

static const u32 textureData[] = {
    0x87000000, //0724 @ 000000
    0x810038F0, //0725 @ 0071E0
    0x8100C8E0, //0726 @ 0191C0
    0x01000000, //0727
    0x8100D230, //0728 @ 01A460
    0x8100D5C0, //0729 @ 01AB80
    0x8100D710, //072A @ 01AE20
    0x87000000, //0724
    0x810038F0, //0725
    0x81013DCE, //0726
    0x01000000, //0727
    0x81014D5C, //0728
    0x8101538A, //0729
    0x81015628, //072A
};

//replaces a call to allocTagged() when loading TEX1.BIN
void* krystalHook_tex1Bin(uint size,AllocTag tag,const char *name) {
    //copy textures into TEX1
    krystal_loadAssets();
    if(!krystalTexture) return allocTagged(size, tag, name);

    //do the original allocation, with the extra texture's size added.
    void *buf = allocTagged(size+krystalTextureSize, tag, name);
    if(!buf) {
        DPRINT("ALLOC FAILED for Krystal texture");
        return allocTagged(size, tag, name); //try again with original size
    }
    registerFreeablePtr((void**)&krystalTexture);

    //copy the new texture into the buffer.
    krystalTextureOffset = size;
    memcpy(buf + size, krystalTexture, krystalTextureSize);
    return buf;
}

void _doTexPatch(void *texBin, u32 *texTab) {
    //DPRINT("tex bin=%08X tab=%08X", texBin, texTab);
    if(!(texBin && texTab)) return;
    if((*(u32*)(texBin + krystalTextureOffset)) == 0x1C) {
        //our texture is here
        //DPRINT("Patching TEX1.tab: %08X", offs);
        for(int i=0; i<KRYSTAL_NUM_TEXTURES; i++) {
            u32 data = textureData[(krystalModelNo*KRYSTAL_NUM_TEXTURES)+i];
            if(data & 0x80000000) {
                texTab[KRYSTAL_TEXTURE_ID+i] = data + (krystalTextureOffset >> 1);
            }
        }
    }
    //else DPRINT("Texture not present");
}

//injected into mergeTableFiles() for TEX1.TAB
void krystalHook_tex1Tab() {
    krystal_loadAssets();
    if(!krystalTexture) return;
    //update the table to reference the Krystal textures
    //DPRINT("Install TEX1 patch 1");
    _doTexPatch(dataFileBuffers[FILE_TEX1_BIN],  dataFileBuffers[FILE_TEX1_TAB]);
    //DPRINT("Install TEX1 patch 2");
    _doTexPatch(dataFileBuffers[FILE_TEX1_BIN2], dataFileBuffers[FILE_TEX1_TAB2]);
}

void _krystalHook_tex1Tab(void);
__asm__(
    "_krystalHook_tex1Tab:  \n"
    ASM_FUNC_START(0x100)
    "bl krystalHook_tex1Tab \n"
    "b  endTabPatch         \n" //save a few bytes
);


//correct Krystal's eye movement so they look the right direction
//and don't move too far (they don't have the same range as Fox)
void _krystalHook_eyePatch(void);
__asm__(
    "_krystalHook_eyePatch:  \n"
    "fmuls    0,  0,  1      \n" // replaced
    "mflr     4              \n"
    "bl       .krystalEyePatch_getpc\n"
    ".krystalEyePatch_getpc: mflr 4\n"
    //"bl krystalHook_eyePatch \n"
    "lwz      3, (.krystalEyePatch_offs - .krystalEyePatch_getpc)(4)\n"
    "lbzx     3,  3,  4      \n"
    "cmpwi    3,  0          \n"
    "beq      .krystalEyePatch_done\n"
    "lfs      1,  (.krystalEyePatch_mult - .krystalEyePatch_getpc)(4)\n"
    "fmuls    0,  0,  1      \n"
    ".krystalEyePatch_done:  \n"
    "mtlr     4              \n"
    "lis      4, 0x8008      \n"
    "ori      4,  4, 0x7054  \n"
    "mtctr    4              \n"
    "bctr                    \n"
    ".krystalEyePatch_mult: .float -0.7\n"
    ".krystalEyePatch_offs: .int overridePlayerNo"
);

void krystalDoModelOverrides() {
    //patch some objects
    /* u32  *pObjsTab = dataFileBuffers[FILE_OBJECTS_TAB];
    void *pObjsBin = dataFileBuffers[FILE_OBJECTS_BIN];
    if(PTR_VALID(pObjsTab) && PTR_VALID(pObjsBin)) {
        //0x487 (AnimFox): for Drakor scene
        //0x488 (AnimFoxLink): for WarpStone scene
        for(int iObj=0x487; iObj <= 0x488; iObj++) {
            ObjectFileStruct *obj = (ObjectFileStruct*)(pObjsBin + pObjsTab[iObj]);
            //OSReport("Got ptr 0x%08X, offs 0x%08X for obj 0x%04X", obj, pObjsTab[iObj], iObj);
            if(PTR_VALID(obj)) {
                s16 *pModelList = obj->pModelList;
                if((u32)pModelList < 0x80000000) pModelList = (s16*)((u32)pModelList + (u32)obj);
                //OSReport("pModelList=%08X", pModelList);
                //OSReport("*pModelList=%08X", pModelList[0]);
                pModelList[0] = 0x4E8; //pPlayer->models[pPlayer->curModel]->header->modelId;
            }
        }
    } */

    if(!pPlayer) return;
    if(pPlayer->catId    != 1) return; //don't apply to title screen Fox, Arwing, etc
    if(pPlayer->curModel == 2) return; //don't override SharpClaw disguise


    int playerNo = overridePlayerNo;
    if(playerNo == PLAYER_ID_AUTO) {
        playerNo = (pPlayer->defNo == ObjDefEnum_Sabre) ?
            PLAYER_ID_FOX : PLAYER_ID_KRYSTAL;
    }

    //override player model ID
    pPlayer->curModel = playerNo; //set model
    *((u16*)(pPlayer->state + 0x81A)) = playerNo; //voice, backpack

    //set some animation variables.
    float v1, v2;
    if(playerNo == PLAYER_ID_KRYSTAL) {
        v1 = 28.8; //unknown
        v2 = 33.0; //ledge grab height
    }
    else {
        v1 = 25.3;
        v2 = 27.8;
    }
    *((float*)(pPlayer->state + 0x7DC)) = v1;
    *((float*)(pPlayer->state + 0x874)) = v2;

    if(pBackpack) {
        if(backpackMode == BACKPACK_ON
        || (backpackMode == BACKPACK_NORMAL && playerNo == 1)) {
            pBackpack->pos.flags &= ~ObjInstance_Flags06_Invisible;
        }
        else pBackpack->pos.flags |= ObjInstance_Flags06_Invisible;
    }
}


void krystalHook_loadSave() {
    objLoadPlayerFromSave(); //replaced
    if(!(krystalModel && krystalTexture)) return;

    //switch to Krystal immediately to ensure animations init correctly.
    //the main loop patch won't do it soon enough.
    krystalDoModelOverrides();

    //copy some animation values from Fox to Krystal so she uses his animations.
    //this works well enough without having to load her own animations which
    //would take up more memory and be a lot more complicated, since the
    //animation data (unlike model/texture data) has to be sorted.
    if(pPlayer->models[0] && pPlayer->models[1]) {
        memcpy(pPlayer->models[0]->header->animIdxs,
            pPlayer->models[1]->header->animIdxs, 0x14);
        pPlayer->models[0]->header->animIds = pPlayer->models[1]->header->animIds;
    }
}


void krystalInit() {
    //Install hooks/patches at startup.
    WRITE32(0x802B08D8, 0x38000001); //Let Krystal use staff
    WRITE32(0x80295BE0, 0x38600001); //Let Krystal use map, PDA, etc.
    WRITE_NOP(0x802B6414); //Disable normal backpack handling
    hookBranch(0x800453E0, krystalHook_modelsBin,   1);
    hookBranch(0x80043D7C, _krystalHook_modelsTab,  0);
    hookBranch(0x80046164, krystalHook_tex1Bin,     1);
    hookBranch(0x80043DB8, _krystalHook_tex1Tab,    0);
    hookBranch(0x80087050, _krystalHook_eyePatch,   0); //yarr.
    hookBranch(0x8002cb20, krystalHook_loadSave,    1);
}

void krystalMainLoop() {
    krystalDoModelOverrides();
}
