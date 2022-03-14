import { BITS_PER_PIXEL, ImageFormat } from "../lib/Texture/types.js";
import { decode_image } from "../lib/Texture/decode.js";
import { hex } from "../Util.js";
import Struct from "../lib/Struct.js";

const TextureStruct4 = Struct(
    /* 0x00 */ ['b', 'unk00'],
    /* 0x01 */ ['b', 'unk01'],
    /* 0x02 */ ['b', 'unk02'],
    /* 0x03 */ ['b', 'unk03'],
    /* 0x04 */ ['i', 'unk04'],
    /* 0x08 */ ['I', 'unk08'],
    /* 0x0c */ ['I', 'pTexture2C'],
    /* 0x10 */ ['I', 'pTexture30'],
    /* 0x14 */ ['I', 'format'],
    /* 0x18 */ ['I', 'pTexture18'],
    /* 0x1c */ ['H', 'blockSize'],
    /* 0x1e */ ['B', 'pixelFmt'], //0=CMPR 1=4bpp 2=8bpp 3=32bpp
    /* 0x1f */ ['B', 'flags_0x1f'],
);

const Header = Struct(
    /* 0x00 */ ['I',  'next'], //Texture* - next mipmap/frame
    /* 0x04 */ ['I',  'flags'],
    /* 0x08 */ ['h',  'xOffset'],
    /* 0x0a */ ['H',  'width'],
    /* 0x0c */ ['H',  'height'],
    /* 0x0e */ ['h',  'usafe'], //ref count
    /* 0x10 */ ['h',  'unk10'], //short
    /* 0x12 */ ['b',  'unk12'],
    /* 0x13 */ ['b',  'unk13'],
    /* 0x14 */ ['H',  'unk14'],
    /* 0x16 */ ['B',  'format'], //TextureFormat
    /* 0x17 */ ['b',  'unk17'],
    /* 0x18 */ ['b',  'unk18'],
    /* 0x19 */ ['b',  'numMipMaps'],
    /* 0x1A */ ['b',  'unk1A'],
    /* 0x1b */ ['b',  'unk1B'], //??
    /* 0x1c */ ['b',  'unk1C'], //db
    /* 0x1d */ ['b',  'unk1D'], //db
    /* 0x1e */ ['b',  'unk1E'], //??
    /* 0x1f */ ['b',  'unk1F'], //db
    /* 0x20 */ [TextureStruct4, 'unk20'],
    /* 0x40 */ ['I',  'unk40'], //undefined4
    /* 0x44 */ ['I',  'dataSize'],
    /* 0x48 */ ['b',  'unk48'], //db
    /* 0x49 */ ['b',  'unk49'], //db
    /* 0x4a */ ['b',  'unk4A'], //??
    /* 0x4b */ ['b',  'unk4B'], //??
    /* 0x4c */ ['I',  'size'], //uint	size
    /* 0x50 */ ['I',  'unk50'], //ddw
    /* 0x54 */ ['b',  'unk54'],
    /* 0x55 */ ['b',  'unk55'],
    /* 0x56 */ ['b',  'unk56'],
    /* 0x57 */ ['b',  'unk57'],
    /* 0x58 */ ['b',  'unk58'],
    /* 0x59 */ ['b',  'unk59'],
    /* 0x5a */ ['b',  'unk5A'],
    /* 0x5b */ ['b',  'unk5B'],
    /* 0x5c */ ['b',  'unk5C'],
    /* 0x5d */ ['b',  'unk5D'],
    /* 0x5e */ ['b',  'unk5E'],
    /* 0x5f */ ['b',  'unk5F'],
);

export default class SfaTexture {
    /** A texture file in SFA's custom wrapper format. */
    constructor() {
        this.width      = null;
        this.height     = null;
        this.format     = null;
        this.numMipMaps = null;
        this.image      = null;
    }

    static calcSize(width, height, format, useMipMaps, nMipMaps) {
        let bitsW=0, bitsH=0;
        let iVar2, result;

        //copied from game. not sure what most of these are.
        switch(format) {
            case ImageFormat.I4: case ImageFormat.C4: case ImageFormat.BC1:
            case 0x20: case 0x30:
                bitsW = 3; bitsH = 3; break;
            case TexFmt.I8: case TexFmt.IA4: case TexFmt.C8:
            case 0x11: case 0x22: case 0x27: case 0x28:
            case 0x29: case 0x2a: case 0x39: case 0x3a:
                bitsW = 3; bitsH = 2; break;
            case TexFmt.IA8: case TexFmt.RGB565: case TexFmt.RGB5A3:
            case TexFmt.RGBA32: case TexFmt.C14X2:
            case 0x13: case 0x16: case 0x23: case 0x2b: case 0x2c: case 0x3c:
                bitsW = 2; bitsH = 2; break;
            default:
                throw new Error(`Unsupported texture format 0x${hex(format)}`);
        }

        if(format == TexFmt.RGBA32 || format == 0x16) iVar2 = 0x40;
        else iVar2 = 0x20;

        if(useMipMaps) {
            result = 0;
            let uVar1 = nMipMaps & 0xff;
            while(uVar1 != 0) {
                let w = width  & 0xffff;
                let h = height & 0xffff;
                result += iVar2 * (w + (1 << bitsW) + -1 >> bitsW) * (h + (1 << bitsH) + -1 >> bitsH)
                if(w == 1 && h == 1) return result;
                if((width & 0xffff) < 2) width = 1;
                else width = w >> 1;
                if((height & 0xffff) < 2) height = 1;
                else height = h >> 1;
                uVar1 -= 1;
            }
        }
        else {
            result = (iVar2 * ((width & 0xffff) + (1 << bitsW) + -1 >> bitsW) *
                ((height & 0xffff) + (1 << bitsH) + -1 >> bitsH))
        }
        return result;
    }

    static fromData(data, canvas=null) {
        const self      = new SfaTexture();
        const header    = new Header(data);
        self.width      = header.width;
        self.height     = header.height;
        self.numMipMaps = header.numMipMaps;
        self.format     = header.format;
        const bpp       = BITS_PER_PIXEL[self.format];
        console.assert(self.width > 0 && self.height > 0);
        //const dataLen   = Math.trunc(self.width * self.height * bpp / 8);
        console.log(
            `Texture size ${self.width}x${self.height} `+
            `mips=${self.numMipMaps} fmt=0x${hex(self.format)}`,
            header, data);
        self.image      = decode_image(data,
            null, //palette_data
            self.format, //image_format
            null, //palette_format
            0, //num_colors (for palettes)
            self.width, self.height, 0x60);
        return self;
    }
}
