from ctypes import (BigEndianStructure,
    c_byte   as s8,
    c_ubyte  as u8,
    c_int16  as s16,
    c_uint16 as u16,
    c_int32  as s32,
    c_uint32 as u32,
    c_float  as f32, # name "float" is already used
    c_double as f64)

class GXTexObj(BigEndianStructure):
    _fields_ = [
        ('mode0',    u32), # 00
        ('mode1',    u32), # 04
        ('image0',   u32), # 08
        ('image3',   u32), # 0C
        ('userData', u32), # 10 void*
        ('format',   u32), # 14 GXTexFmt
        ('tlutName', u32), # 18
        ('loadCnt',  u16), # 1C
        ('loadFmt',  u8),  # 1E (0=CMPR 1=4bpp 2=8bpp 3=32bpp)
        ('flags',    u8),  # 1F (1:mipmap; 2:isRGB)
    ]

class Header(BigEndianStructure):
    _fields_ = [
        ('next',                 u32), # 00 Texture*
        ('flags',                u32), # 04
        ('xOffset',              s16), # 08
        ('width',                u16), # 0A
        ('height',               u16), # 0C
        ('usage',                s16), # 0E ref count
        ('nFrames',              s16), # 10
        ('unk12',                s16), # 12
        ('framesPerTick',        u16), # 14 how many frames to advance each tick
        ('format',               u8),  # 16 GXTexFmt
        ('wrapS',                u8),  # 17
        ('wrapT',                u8),  # 18
        ('minFilter',            u8),  # 19
        ('magFilter',            u8),  # 1A
        ('unk1B',                u8),  # 1B
        ('mipmapVar1C',          u8),  # 1C
        ('mipmapVar1D',          u8),  # 1D
        ('unk1E',                u8),  # 1E
        ('unk1F',                u8),  # 1F
        ('texObj',               GXTexObj), # 20
        ('texRegion',            u32), # 40 GXTexRegion*
        ('bufSize',              s32), # 44 raw image data size
        ('bNoTexRegionCallback', s8),  # 48 bool use gxSetTexImage0 instead of gxCallTexRegionCallback
        ('bDoNotFree',           s8),  # 49 bool (maybe memory region?)
        ('unk4A',                s8),  # 4A
        ('unk4B',                s8),  # 4B
        ('size',                 u32), # 4C
        ('tevVal50',             u32), # 50 0:use 1 TEV stage, not 2 (maybe bHasTwoTevStages?)
        ('unk54',                u32), # 54
        ('unk58',                u32), # 58
        ('unk5C',                u32), # 5C
    ]
