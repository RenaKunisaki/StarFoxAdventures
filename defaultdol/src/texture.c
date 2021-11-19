#include "main.h"

static const u8 texBinFiles[] = {TEX0_BIN, TEX1_BIN, TEX0_BIN2, TEX1_BIN2};
static const u8 texTabFiles[] = {TEX0_TAB, TEX1_TAB, TEX0_TAB2, TEX1_TAB2};

int _textureLoadHook(int id) {
    //game does this before loading textures
    tex0count(TEX0_TAB);
    tex1count(TEX1_TAB);

    int realId = (id < 0) ? -id : pTexTableBin[id];
    int tblIdx = (realId & 0x8000) ? 1 : 0;
    realId &= 0x7FFF;
    OSReport("Loading texture 0x%X: TEX%d[0x%X]\n", id, tblIdx, realId);
    if(realId >= numTextures[tblIdx]) {
        OSReport("Texture ID out of range!\n");
        realId = 0;
    }
    u32 tEntry = textureTables[tblIdx][realId];
    u32 offs   = (tEntry & 0xFFFFFF) * 2;

    bool useAlt;
    if((offs & 0x80000000) == 0) {
        if((offs & 0x40000000) == 0) {
            if(dataFileBuffers[TEX0_TAB] == NULL) {
                if(dataFileBuffers[TEX0_TAB2] != NULL) useAlt = true;
            }
            else useAlt = false;
        }
        else useAlt = false;
    }
    else useAlt = true;

    int binFileId = texBinFiles[tblIdx + (useAlt ? 2 : 0)];
    u32 *file  = dataFileBuffers[binFileId];
    u32 *bin   = (u32*)((u32)file + offs);
    OSReport("Using TEX%d slot %d at 0x%08X\n", tblIdx, useAlt ? 1 : 0, file);
    OSReport("Table entry: 0x%08X, offs 0x%08X -> 0x%08X: 0x%08X\n",
        tEntry, offs, bin, *bin);

    // u32 size=0xAAAAAAAA, compSize=0xAAAAAAAA;
    //if(tblIdx == 0) tex0GetMipmap(offs, realId, &size, &compSize, 1, NULL, 0);
    //else tex1GetMipmap(offs, realId, &size, &compSize, 1, NULL, 0);
    //OSReport("mipmap size=0x%X comp=0x%X\n", size, compSize);

    if(*bin == CHARS_TO_U32('Z', 'L', 'B', 0)) return id;
    if(*bin == CHARS_TO_U32('L', 'Z', '0', 0)) return id;
    if(*bin == CHARS_TO_U32('D', 'I', 'R', 0)) return id;
    if(*bin == CHARS_TO_U32('D', 'I', 'R', 'n')) return id;
    //XXX why does this happen? TEX0.TAB entry
    OSReport("No valid texture here!\n");
    return 1;
}
void textureLoadHook(void);
__asm__(
    "textureLoadHook:    \n"
    ASM_RFUNC_START(0x100)
    "mr     3, 24         \n" //r3 = ID
    "bl     _textureLoadHook\n"
    ASM_RFUNC_END(0x100)
    "mr     24, 3         \n" //returned ID
    "or     18, 24, 24    \n" //replaced
    "lis    30, 0x800A    \n"
    "ori    30, 30, 0x49C8\n"
    "mtctr  30            \n"
    "bctr"
);

void _textureAllocHook(u32 size, u32 compSize, int nFrames) {
    OSReport("Texture alloc: size=0x%X (comp 0x%X) frames=%d\n",
        size, compSize, nFrames);
}
void textureAllocHook(void);
__asm__(
    "textureAllocHook:    \n"
    ASM_RFUNC_START(0x100)
    "lwz    3, 0x118(1)   \n" //r3 = size
    "lwz    4, 0x114(1)   \n" //r4 = compressed size
    "mr     5, 28         \n" //r5 = nFrames
    "bl     _textureAllocHook\n"
    ASM_RFUNC_END(0x100)
    "li     23, 0         \n" //replaced
    "lis    20, 0x800A    \n"
    "ori    20, 20, 0x4B78\n"
    "mtctr  20            \n"
    "bctr"
);

void _tex0mipHook(u32 *entry) {
    OSReport("TEX0 entry @ 0x%08X: %08X %08X %08X %08X\n", entry,
        entry[0], entry[1], entry[2], entry[3]);
}
void tex0mipHook1(void);
__asm__(
    "tex0mipHook1:    \n"
    ASM_RFUNC_START(0x100)
    "mr     3, 27         \n" //r3 = entry
    "bl     _tex0mipHook\n"
    ASM_RFUNC_END(0x100)
    "lis    20, 0x8009    \n"
    "ori    20, 20, 0x9B64\n"
    "mtctr  20            \n"
    "bctr"
);
void tex0mipHook2(void);
__asm__(
    "tex0mipHook2:    \n"
    ASM_RFUNC_START(0x100)
    "mr     3, 31         \n" //r3 = entry
    "bl     _tex0mipHook\n"
    ASM_RFUNC_END(0x100)
    "lwz    26, 0x4(31)   \n" //replaced
    "lis    20, 0x8009    \n"
    "ori    20, 20, 0x9B58\n"
    "mtctr  20            \n"
    "bctr"
);

void _texDecompHook(u32 pTex) {
    u16 width  = *(u16*)(pTex + 0xA);
    u16 height = *(u16*)(pTex + 0xC);
    OSReport("Decompressed texture @%08X size %dx%d\n", pTex, width, height);
}

void texDecompHook(void);
__asm__(
    "texDecompHook:    \n"
    ASM_RFUNC_START(0x100)
    "mr     3, 31         \n" //r3 = texture
    "bl     _texDecompHook\n"
    ASM_RFUNC_END(0x100)
    "li      0, 0         \n" //replaced
    "lis    23, 0x800A    \n"
    "ori    23, 23, 0x4CE4\n"
    "mtctr  23            \n"
    "bctr"
);

void initTextureHooks() {
    hookBranch(0x800a49c4, textureLoadHook, 0);
    hookBranch(0x800a4b74, textureAllocHook, 0);
    hookBranch(0x80099b00, tex0mipHook1, 0);
    hookBranch(0x80099b54, tex0mipHook2, 0);
    hookBranch(0x800a4ce0, texDecompHook, 0);
    //OSReport("Installed texture hooks\n");
}
