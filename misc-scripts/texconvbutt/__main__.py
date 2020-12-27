#!/usr/bin/env python3
"""Converts between texture data and standard image files."""
import sys
import math
import struct
import numpy as np
from wand.image import Image
from formats.rgb565 import RGB565
from formats.rgba8 import RGBA8
from formats.ia8 import IA8
from formats.i8 import I8
from formats.cmpr import CMPR
# XXX: I4, IA4, RGB5A3, C4, C8, C14X2

formats = {}
_fmts = (RGB565, RGBA8, I8, IA8, CMPR)
for fmt in _fmts:
    formats[fmt.fmtId] = fmt
    formats[fmt.name]  = fmt


def img2texture(inPath, outPath):
    image  = Image.open(inPath).convert('RGB')
    pixels = image.blob
    width, height = image.width, image.height

    # make the header
    header = bytearray(0x60)
    struct.pack_into('>HH', header, 0x0A, width, height)
    header[0x0F] = 1 # unknown
    header[0x10] = 1 # unknown
    header[0x16] = 4 # format (RGB565)
    header[0x17] = 1 # unknown
    header[0x18] = 1 # unknown
    header[0x19] = 1 # number of mipmaps including base image
    header[0x1A] = 1 # unknown
    header[0x1D] = 6 # unknown

    # read pixels and convert to RGB565
    # XXX this can probably be done faster
    buf = np.array(dtype=np.uint16, ndmin=3)
    for y in range(height):
        for x in range(width):
            r, g, b = pixels[x, y]
            px = (b >> 3) | ((g >> 2) << 5) | ((r >> 3) << 11)
            buf[x, y] = px


    # read the pixels, convert to RGB565, and swizzle
    pixelsOut = swizzle(buf, width, height)

    with open(outPath, 'wb') as outFile:
        outFile.write(bytes(header))
        for p in pixelsOut:
            outFile.write(struct.pack('>H', p))


def texture2img(inPath, outPath):
    with open(inPath, 'rb') as inFile:
        header = inFile.read(0x60)
        if len(header) < 0x60:
            raise EOFError("Invalid texture file (missing header)")
        width, height = struct.unpack_from('>HH', header, 0x0A)
        fmtId = struct.unpack_from('>B', header, 0x16)[0] # grumble
        numMipMaps = struct.unpack_from('>B', header, 0x19)[0] # grumble

        format = formats.get(fmtId, None)
        if format is not None: fmtName = format.name
        else: fmtName = 'unknown'
        print("Texture: %dx%d pixels, format 0x%02X (%s), %d mipmmaps"%(
            width, height, fmtId, fmtName, numMipMaps))

        if format is None:
            raise TypeError("Unsupported format 0x%02X" % fmtId)
        data = inFile.read((width * height * format.bitsPerPixel) // 8)

        tex = format(width, height, data)
        #data = tex.unswizzle().pixels
        data = tex.pixels

        img = Image(blob=data, format='RGBA', depth=8, width=width, height=height)
        with img.convert('png') as converted:
            f = 'PNG32' if format.hasAlpha else 'PNG24'
            converted.save(filename=f+':'+outPath)


def main():
    if len(sys.argv) < 4:
        print("Usage: %s [pack|unpack] inpath outpath" % sys.argv[0])
        return 1
    if sys.argv[1] == 'pack':
        img2texture(sys.argv[2], sys.argv[3])
    elif sys.argv[1] == 'unpack':
        texture2img(sys.argv[2], sys.argv[3])
    else:
        print("unknown operation:", sys.argv[1])
        return 1


if __name__ == '__main__':
    sys.exit(main())
