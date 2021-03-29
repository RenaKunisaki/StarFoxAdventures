#include "main.h"

static u8 colorTbl[] = {
    //0xFF, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20, 0x00,
    //0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xFF,
    //0xFF, 0xE0, 0xC0, 0xA0, 0x80, 0x60, 0x40, 0x20,
    //0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0, 0xFF,
    0xFF, 0xE0, 0xD0, 0xC0, 0xB0, 0xA0, 0x90, 0x80,
    0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xFF,
};

static inline void writeVtx(void *fifo, float x, float y, float z,
u8 r, u8 g, u8 b, u8 a, float s, float t) {
    volatile float *outF = (volatile float*)(fifo);
    volatile u8    *outB = (volatile u8*   )(fifo);
    *outF = x; *outF = y; *outF = z;
    *outB = r; *outB = g; *outB = b; *outB = a;
    *outF = s; *outF = t;
}

void _staffFxHook(void* data, void *fifo) {
    //simplified HSV-to-RGB conversion
    //h stores the hue (0 to 7)
    //S, V are fixed
    static u8 lastH = 0;
    u8 h = lastH++;
    h >>= 2; //slow down cycling

    u8 r = colorTbl[ h      & 0xF];
    u8 g = colorTbl[(h + 2) & 0xF];
    u8 b = colorTbl[(h + 8) & 0xF];

    //repeat most of the original code here...
    float px = playerMapOffsetX, pz = playerMapOffsetZ;
    struct {
        float x, y, z;
        int unk0C;
        short a;
        short unk12;
    } *V = data;
    //note order here 0 1 3 2, not 0 1 2 3
    writeVtx(fifo, V[0].x - px, V[0].y, V[0].z - pz, r, g, b, V[0].a, 0.5, 0.0);
    writeVtx(fifo, V[1].x - px, V[1].y, V[1].z - pz, r, g, b, V[1].a, 0.5, 1.0);
    writeVtx(fifo, V[3].x - px, V[3].y, V[3].z - pz, r, g, b, V[3].a, 0.5, 1.0);
    writeVtx(fifo, V[2].x - px, V[2].y, V[2].z - pz, r, g, b, V[2].a, 0.5, 0.0);
}

//this has to be referenced within the same file it's defined in.
void staffFxHook(void);
__asm__(
    "staffFxHook:        \n"
    ASM_FUNC_START(0x100)
    "mr    3, 28         \n"
    "addi  4, 29, -0x8000\n"
    "bl    _staffFxHook  \n"
    ASM_FUNC_END(0x100)
    "blr                 \n"
);

void staffFxInit(void) {
    hookBranch(0x8016de64, staffFxHook, 1);
    WRITE32(0x8016dd9c, 0x38a00005); //change blend mode
    WRITE32(0x803dbd50, 0x0C9F0C9F); //change texture IDs
    hookBranch(0x8016de68, (void*)0x8016df88, 0); //skip replaced code

    //make the grow/shrink animation run even if the staff was
    //"instantly" taken out/put away
    WRITE_NOP(0x8016e758);
    WRITE_NOP(0x8016e71c);

    //slow down that animation a bit so you can actually see it
    WRITEFLOAT(0x803e3320,  0.10);
    WRITEFLOAT(0x803e3324, -0.10);
}
