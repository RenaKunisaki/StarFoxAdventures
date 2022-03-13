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
export const BLOCK_WIDTHS = { //ImageFormat
    I4    : 8,
    I8    : 8,
    IA4   : 8,
    IA8   : 4,
    RGB565: 4,
    RGB5A3: 4,
    RGBA32: 4,
    C4    : 8,
    C8    : 8,
    C14X2 : 4,
    CMPR  : 8,
};
export const BLOCK_HEIGHTS = { //ImageFormat
    I4    : 8,
    I8    : 4,
    IA4   : 4,
    IA8   : 4,
    RGB565: 4,
    RGB5A3: 4,
    RGBA32: 4,
    C4    : 8,
    C8    : 4,
    C14X2 : 4,
    CMPR  : 8,
};
export const BLOCK_DATA_SIZES = { //ImageFormat
    I4    : 32,
    I8    : 32,
    IA4   : 32,
    IA8   : 32,
    RGB565: 32,
    RGB5A3: 32,
    RGBA32: 64,
    C4    : 32,
    C8    : 32,
    C14X2 : 32,
    CMPR  : 32,
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
export const MAX_COLORS_FOR_IMAGE_FORMAT = { //ImageFormat
    C4   : 1<<4,
    C8   : 1<<8,
    C14X2: 1<<14,
};
export const BITS_PER_PIXEL = { //ImageFormat
    I4:      4,
    I8:      8,
    IA4:     8,
    IA8:    16,
    RGB565: 16,
    RGB5A3: 16,
    RGBA32: 32,
    C4:      4,
    C8:      8,
    C14X2:  16,
    CMPR:    4,
};
