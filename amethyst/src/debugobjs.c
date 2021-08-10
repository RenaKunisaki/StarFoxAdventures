#include "main.h"

#if 1

/** These objects' render methods do nothing, but have more than just a blr instruction.
 *  Usually they look something like: if(shouldRender) {}
 *  The DLLs listed here will have the render method changed to an actual blr stub
 *  so that they appear when debug objects are enabled.
 */
static const u16 overrideDlls[] = {
    0x00E0, //SwarmBaddie
    0x00E1, //WispBaddie
    0x00F0, //WarpPoint
    0x012B, //FXEmit
    0x0130, //AreaFXEmit
    0x0181, //MMP_trenchFX
    0x018E, //MMSH_WaterSpike
    0x01BD, //PaymentKiosk
    0x01E6, //DIMbosscrackpar
    0x01E7, //DIMbossfire
    0x01F2, //SB_CageKyte
    0x0204, //WM_Torch
    0x0207, //WM_Worm
    0x020C, //WM_spiritplace
    0x0217, //VFP_ObjCreator
    0x0224, //?
    0x022A, //DFP_ObjCreator
    0x0236, //Laser
    0x023A, //Platform1
    0x0254, //KT_Fallingrocks
    0x02B2, //DustMoteSou
    0};

//the Magic Cave doesn't set a culling distance for some objects.
//normally it's no problem because they're invisble, but, when making them
//visible, they use this field, making it impossible to interact with them.
//rather than patch the romlist, I'll just give them an init method that
//sets a cull distance, since they don't normally have one.
void dll_MagicCaveBottom_init_hook(ObjInstance *obj) {
    obj->cullDistance = 2048.0f;
}

void dll_MCUpgradeMa_init_hook(ObjInstance *obj) {
    obj->cullDistance = 2048.0f;
    //this one does have an init method so we need to call it
    dll_MCUpgradeMa_init(obj);
}

//This hook detects when an object's render method is about to be used but is
//a stub, and instead uses the default render method.
//The effect is that more debug objects will be visible when displaying them
//is enabled (as well as potentially a few that weren't meant to be).
//This isn't perfect since there could still be some whose render method isn't
//a one-instruction stub but still does nothing.
//This hook has no effect if debug objects aren't enabled.
void _debugObjsHook(void);
__asm__(
    "_debugObjsHook:               \n"
    "lwz     9,  0(12)             \n" //get first instr of render method
    "lis     8,  0x4E80            \n"
    "ori     8,  8,  0x0020        \n"
    "cmpw    9,  8                 \n" //is it a blr instruction?
    "beq     .debugObjsHook_stub   \n"
    //not a stub, so call it like normal.
    "or      8,  31, 31            \n" //replaced
    "mtctr   12                    \n"
    "bctrl                         \n"

    ".debugObjsHook_done:          \n" //return to game code
    "lis     4,  0x8003            \n"
    "ori     4,  4,  0xBB00        \n"
    "mtctr   4                     \n"
    "bctr                          \n"

    ".debugObjsHook_stub:          \n" //render method is a stub
    "lis     9,  0x8003            \n"
    "ori     9,  9,  0xBAC8        \n"
    "mtctr   9                     \n"
    "bctr                          \n" //jump to default render method
);

static bool isInList_u16(u16 val, u16 *list) {
    for(int i=0; list[i] != 0xFFFF; i++) {
        if(list[i] == val) return true;
    }
    return false;
}

bool _updateOpacityHook(ObjInstance *obj) {
    static u16 debugModels[] = {
        0x0000, //Placeholder Cube
        0x0002, //yellow cylinder
        0x0010, //small gray sphere
        0x004D, //checkered sphere
        0x02D1, //N64 logo
        0x02D3, //Rare logo
        0x02D5, //small pyramid
        0x02D9, //small arrow
        0x031B, //red crystal (InvisibleHitSwitch)
        0x0321, //default dube (WaterFallSpray)
        0x044C, //red cylinder
        0x044E, //red/green disc
        0x0458, //CHECK plane
        0x045E, //camera
        0x045F, //Large gray arrow
        0x0465, //Curve
        0x0466, //Clapper Board
        0x0467, //LEV CTRL cube
        0x0468, //LEV NAME cube
        0x0469, //OBJ CTRL cube
        0x046A, //FX cube
        0x046B, //TEX SCRL cube
        0x046C, //RE START POINT cube
        0x046D, //WAVE ANIM cube
        0x046E, //ALPHA ANIM cube
        //XXX 0x046F, 0x0470?
        0x0471, //GRND ANIM cube
        0x0472, //TRIG AREA cube
        0x0473, //TRIG CURVE cube
        0x0474, //should be TRIG BITS cube?
        //XXX 0x0475?
        0x0476, //OVER RIDE cube
        //XXX 0x0477?
        0x0478, //HIT ANIM cube
        0x0479, //light bulb
        0x047E, //LGTDirectio
        0xFFFF  //end of list
    };
    //the models for these are also used for debug objects,
    //so don't hide those.
    static u16 excludeDefs[] = {
        0x00D9, //CFPowerCrys red collectible
        0x00DB, //CFPowerCrys red anim
        0x00DF, //CFPowerCrys green collectible
        0x00E0, //CFPowerCrys blue collectible
        0x00E1, //CFPowerCrys green anim
        0x00E2, //CFPowerCrys blue anim
        0x075A, //CFSmallPowe red
        0x075B, //CFSmallPowe green
        0x075C, //CFSmallPowe blue
        0xFFFF  //end of list
    };

    //if debug objects are disabled, hide any object that uses a debug model.
    bool visible = debugRenderFlags & DEBUGRENDER_DEBUG_OBJS;
    Model *model = PTR_VALID(obj->models) ? obj->models[obj->curModel] : NULL;
    u16 mid = (PTR_VALID(model) && PTR_VALID(model->header)) ?
        model->header->modelId : 0xFFFF;
    if(mid != 0xFFFF && isInList_u16(mid, debugModels)
    && !isInList_u16(obj->defNo, excludeDefs)) {
        if(visible) {
            //some fade away if you aren't right on top of them,
            //so prevent that. also, prevent them from being fully opaque
            //and being in the way.
            objUpdateOpacity(obj);
            if(obj->opacity > 192) obj->opacity = 192;
            if(obj->opacity <  64) obj->opacity =  64;
            return true;
        }
        else {
            //OSReport("Obj %s model ID %04X (%04X %08X)", obj->file->name,
            //    mid, model->cacheModNo, model);
            //setting to 0 makes them skipped entirely which breaks
            //magic caves again because lol.
            //setting to 1 makes them still effectively invisible,
            //while not being actually considered "invisible".
            obj->opacity = 1;
            return true; //object is "visible"
        }
    }

    return objUpdateOpacity(obj);
}

void debugObjsInit() {
    hookBranch(0x8003ba28, _debugObjsHook, 1);
    hookBranch(0x8005b6f8, _updateOpacityHook, 1);

    //force debug object models to load.
    WRITE32(0x8002d820, 0x5480003c);
    WRITE32(0x8002d824, 0x901e0044);
    WRITE32(0x8002d828, 0x60000000);
    WRITE32(0x8002d82c, 0x60000000);
    //patch some magic cave DLL tables to set cull distances.
    WRITE32(0x80321cb4, dll_MagicCaveBottom_init_hook);
    WRITE32(0x8032bf94, dll_MCUpgradeMa_init_hook);
    //XXX 0x8032BF40 DLL MCUpgrade (new spells) - verify this doesn't need patching

    //override some more complex stubs
    for(int i=0; overrideDlls[i]; i++) {
        DPRINT("Patch render of DLL %04X", overrideDlls[i]);
        WRITE_BLR(dlls[overrideDlls[i]]->functions[4]);
    }
}

#else

void debugObjsInit() {
    //do nothing
}

#endif
