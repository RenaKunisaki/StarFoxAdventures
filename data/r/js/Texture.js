export const TexFmt = {
    I4:     0x00,
    I8:     0x01,
    IA4:    0x02,
    IA8:    0x03,
    RGB565: 0x04,
    RGB5A3: 0x05,
    RGBA32: 0x06,
    C4:     0x08,
    C8:     0x09,
    C14X2:  0x0A,
    BC1:    0x0E, //aka CMPR
};

/** A texture graphic read from the game.
 */
export class Texture {
    /** Calculate the size of a texture given its size and format.
     */
    static calcSize(width, height, format, useMipMaps, nMipMaps) {
        let bitsW=0, bitsH=0;
        //copied from game. not sure what most of these are.
        switch(format) {
            case TexFmt.I4: case TexFmt.C4: case TexFmt.BC1: case 0x20: case 0x30:
                bitsW = 3;
                bitsH = 3;
                break;
            case TexFmt.I8: case TexFmt.IA4: case TexFmt.C8: case 0x11: case 0x22:
            case 0x27: case 0x28: case 0x29: case 0x2a: case 0x39: case 0x3a:
                bitsW = 3;
                bitsH = 2;
                break;
            case TexFmt.IA8: case TexFmt.RGB565: case TexFmt.RGB5A3: case TexFmt.RGBA32:
            case TexFmt.C14X2: case 0x13: case 0x16: case 0x23: case 0x2b: case 0x2c: case 0x3c:
                bitsW = 2;
                bitsH = 2;
                break;
            default: break;
        }
        let iVar2, uVar1, w, h, result;
        if ((format == TexFmt.RGBA32) || (format == 0x16)) iVar2 = 0x40;
        else iVar2 = 0x20;
        if (useMipMaps) {
            result = 0;
            for (uVar1 = nMipMaps & 0xff; uVar1 != 0; uVar1 -= 1) {
                w = width & 0xffff;
                h = height & 0xffff;
                result += iVar2 * (w + (1 << bitsW) + -1 >> bitsW) * (h + (1 << bitsH) + -1 >> bitsH);
                if ((w == 1) && (h == 1)) return result;
                if ((width & 0xffff) < 2) width = 1;
                else width = w >> 1;
                if ((height & 0xffff) < 2) height = 1;
                else height = h >> 1;
            }
        }
        else {
            result = iVar2 * ((width & 0xffff) + (1 << bitsW) + -1 >> bitsW) *
                ((height & 0xffff) + (1 << bitsH) + -1 >> bitsH);
        }
        return result;
    }

    //hoo boy I don't want to think about decoding textures in JS
};
