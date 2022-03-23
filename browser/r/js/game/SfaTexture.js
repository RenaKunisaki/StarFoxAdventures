import { BITS_PER_PIXEL, ImageFormat } from "../lib/Texture/types.js";
import { decode_image } from "../lib/Texture/decode.js";
import { hex } from "../Util.js";
import { DataError } from "../app/errors.js";

//struct types
let Header;

export default class SfaTexture {
    /** A texture file in SFA's custom wrapper format. */
    constructor(game) {
        this.game       = game;
        this.width      = null;
        this.height     = null;
        this.format     = null;
        this.numMipMaps = null;
        this.image      = null;
        Header = game.app.types.getType('sfa.textures.Header');
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
                throw new DataError(`Unsupported texture format 0x${hex(format)}`);
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

    static fromData(game, data, canvas=null) {
        const self      = new SfaTexture(game);
        const header    = Header.fromBytes(data);
        self.width      = header.width;
        self.height     = header.height;
        self.numMipMaps = header.numMipMaps;
        self.format     = header.format;
        const bpp       = BITS_PER_PIXEL[self.format];
        if(self.width < 1 || self.height < 1 || bpp == undefined) return null;
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
