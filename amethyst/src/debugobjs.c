#include "main.h"

#if 1 //currently no way to toggle in-game and it causes bugs, so disabled for now

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
//XXX we still need a way to toggle them.
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

bool _updateOpacityHook(ObjInstance *obj) {
    static u16 debugModels[] = {
        0x0000, //Placeholder Cube
        0x0002, //yellow cylinder
        0x0010, //small gray sphere
        0x004D, //checkered sphere or tumbleweed
        0x02D1, //N64 logo
        0x02D3, //Rare logo
        0x02D5, //small pyramid
        0x02D9, //small arrow
        0x031B, //red crystal (InvisibleHitSwitch)
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
        0x0471, //GRND ANIM cube
        0x0472, //TRIG AREA cube
        0x0473, //TRIG CURVE cube
        0x0476, //OVER RIDE cube
        0x0478, //HIT ANIM cube
        0x0479, //light bulb
        0xFFFF
    };

    //if debug objects are disabled, hide any object that uses a debug model.
    if(!(debugRenderFlags & DEBUGRENDER_DEBUG_OBJS)) {
        Model *model = PTR_VALID(obj->models) ? obj->models[obj->curModel] : NULL;
        u16 mid = (PTR_VALID(model) && PTR_VALID(model->header)) ?
            model->header->modelId : 0xFFFF;
        if(mid != 0xFFFF) {
            for(int i=0; debugModels[i] != 0xFFFF; i++) {
                if(mid == debugModels[i]) {
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
}

#else

void debugObjsInit() {
    //do nothing
}

#endif
