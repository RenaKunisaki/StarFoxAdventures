export const ImageFormat = {
    I4:       0,
    I8:       1,
    IA4:      2,
    IA8:      3,
    RGB565:   4,
    RGB5A3:   5,
    RGBA32:   6,
    C4:       8,
    C8:       9,
    C14X2:  0xA,
    CMPR:   0xE,
};
export const PaletteFormat = {
    IA8:    0,
    RGB565: 1,
    RGB5A3: 2,
};
export const BLOCK_WIDTHS = {
    [ImageFormat.I4]    : 8,
    [ImageFormat.I8]    : 8,
    [ImageFormat.IA4]   : 8,
    [ImageFormat.IA8]   : 4,
    [ImageFormat.RGB565]: 4,
    [ImageFormat.RGB5A3]: 4,
    [ImageFormat.RGBA32]: 4,
    [ImageFormat.C4]    : 8,
    [ImageFormat.C8]    : 8,
    [ImageFormat.C14X2] : 4,
    [ImageFormat.CMPR]  : 8,
};
export const BLOCK_HEIGHTS = {
    [ImageFormat.I4]    : 8,
    [ImageFormat.I8]    : 4,
    [ImageFormat.IA4]   : 4,
    [ImageFormat.IA8]   : 4,
    [ImageFormat.RGB565]: 4,
    [ImageFormat.RGB5A3]: 4,
    [ImageFormat.RGBA32]: 4,
    [ImageFormat.C4]    : 8,
    [ImageFormat.C8]    : 4,
    [ImageFormat.C14X2] : 4,
    [ImageFormat.CMPR]  : 8,
};
export const BLOCK_DATA_SIZES = {
    [ImageFormat.I4]    : 32,
    [ImageFormat.I8]    : 32,
    [ImageFormat.IA4]   : 32,
    [ImageFormat.IA8]   : 32,
    [ImageFormat.RGB565]: 32,
    [ImageFormat.RGB5A3]: 32,
    [ImageFormat.RGBA32]: 64,
    [ImageFormat.C4]    : 32,
    [ImageFormat.C8]    : 32,
    [ImageFormat.C14X2] : 32,
    [ImageFormat.CMPR]  : 32,
};
export const IMAGE_FORMATS_THAT_USE_PALETTES = [
    ImageFormat.C4,
    ImageFormat.C8,
    ImageFormat.C14X2,
];
export const GREYSCALE_IMAGE_FORMATS = [
    ImageFormat.I4,
    ImageFormat.I8,
    ImageFormat.IA4,
    ImageFormat.IA8,
];
export const GREYSCALE_PALETTE_FORMATS = [
    PaletteFormat.IA8,
];
export const MAX_COLORS_FOR_IMAGE_FORMAT = {
    [ImageFormat.C4]   : 1<<4,
    [ImageFormat.C8]   : 1<<8,
    [ImageFormat.C14X2]: 1<<14,
};
export const BITS_PER_PIXEL = {
    [ImageFormat.I4]:      4,
    [ImageFormat.I8]:      8,
    [ImageFormat.IA4]:     8,
    [ImageFormat.IA8]:    16,
    [ImageFormat.RGB565]: 16,
    [ImageFormat.RGB5A3]: 16,
    [ImageFormat.RGBA32]: 32,
    [ImageFormat.C4]:      4,
    [ImageFormat.C8]:      8,
    [ImageFormat.C14X2]:  16,
    [ImageFormat.CMPR]:    4,
};
