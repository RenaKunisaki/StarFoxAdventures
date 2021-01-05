import Struct from '/r/js/Struct.js';

export const ShaderLayer = Struct({
    //this might be totally wrong...
    'texture':             ['I',  0x00], //index into model's texture list
    'tevMode':             ['B',  0x04],
    'enableTexChainStuff': ['B',  0x05],
    'scrollingTexMtx':     ['B',  0x06],
    'unk07':               ['B',  0x07],
});

export const Shader = Struct({
    'unk00':     ['I',  0x00],
    'unk04':     ['I',  0x04],
    'auxTex0':   ['I',  0x08],
    'alpha':     ['B',  0x0C],
    'unk0D':     ['3B', 0x0D],
    'unk10':     ['4B', 0x10],
    'auxTex1':   ['i',  0x14],
    'texture18': ['i',  0x18], //same as 0x24 but not sure where it's used
    'unk1C':     ['i',  0x1C],
    'unk20':     ['i',  0x20],
    'layer':     [ShaderLayer[2], 0x24],
    'auxTex2':   ['i',  0x34], //affects CP regs
    'furTexture':['i',  0x38], //which texture for fur effect
    'flags':     ['I',  0x3C], //affects lighting
    'attrFlags': ['B',  0x40], //affects CP/XF, can crash Dolphin
    'nLayers':   ['B',  0x41],
    '_pad42':    ['2B', 0x42],
});

/* export const Shader = Struct({
    'auxTex0':   ['I',  0x08],
    'auxTex1':   ['I',  0x14],
    'alpha':     ['B',  0x22],
    'unk24':     ['4I', 0x24],
    'auxTex2':   ['I',  0x34],
    'furTexture':['I',  0x38],
    'flags':     ['I',  0x3C],
    'flags_40':  ['B',  0x40],
    'nTextures': ['B',  0x41],
}); */

export const TextureHeader = Struct({
    'width':  ['H',  0x0A],
    'height': ['H',  0x0C],
    'unk0F':  ['B',  0x0F], //always 1?
    'unk10':  ['B',  0x10], //always 1?
    'format': ['B',  0x16], //format ID
    'unk17':  ['B',  0x17], //always 1?
    'unk18':  ['B',  0x18], //always 1?
    'nFrames':['B',  0x19],
    'unk1A':  ['B',  0x1A], //always 1?
    'unk1D':  ['B',  0x1D], //always 6?
    'unk5F':  ['B',  0x5F], //to set struct size
});

export const DisplayListPtr = Struct({
    'offset':         ['I', 0x00],
    'size':           ['H', 0x04],
    //seems like none of these fields are used, at least for character models.
    //I don't see any code that accesses them, and changing them does nothing.
    'bbox':           ['6h',0x06],
    'unk12':          ['B', 0x12],
    'shaderId':       ['B', 0x13], // maybe u16
    'specialBitAddr': ['H', 0x14], // related to shader
    'unk16':          ['H', 0x16],
    'unk18':          ['I', 0x18], // always 07 00 00 00?
});
