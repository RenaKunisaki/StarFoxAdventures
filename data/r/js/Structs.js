import Struct from './Struct.js';

export const ObjDef = Struct({
    'type':      ['h', 0x00], //obj type
    'size':      ['B', 0x02], //allocatedSize
    'acts1':     ['B', 0x03], //whether to NOT load in acts 1-8
    'loadFlags': ['B', 0x04],
    'acts2':     ['B', 0x05], //whether to NOT load in acts 15-9 (low bit unused?)
    'bound':     ['B', 0x06], //load if in range (bound<<3) of player
    'unk7':      ['B', 0x07],
    'x':         ['f', 0x08],
    'y':         ['f', 0x0C],
    'z':         ['f', 0x10],
    'id':        ['I', 0x14], //unique ID
    //beyond is (size * 4) bytes of object-specific params
});

export const FaceFeedHeader = Struct({
    'magic':   ['I', 0x00], //0xFACEFEED
    'rawLen':  ['I', 0x04], //length of uncompressed data (bytes)
    'zlbOffs': ['I', 0x08], //offset of ZLB data (skips header)
    'compLen': ['I', 0x0C], //length of compressed data (bytes)
    'unk10':   ['I', 0x10],
    'unk14':   ['I', 0x14],
    'unk18':   ['I', 0x18],
    'unk1C':   ['I', 0x1C],
});

export const ZlbHeader = Struct({
    'magic':   ['I', 0x00], //0x5A4C4200, ie "ZLB\0"
    'version': ['I', 0x04], //0 or 0x44495200 ie "DIR\0"
    'rawLen':  ['I', 0x08], //length of uncompressed data (bytes)
    'compLen': ['I', 0x08], //length of compressed data (bytes)
});
