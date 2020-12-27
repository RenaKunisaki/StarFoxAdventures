#!/usr/bin/env python3
"""Converts between texture data and standard image files."""
import sys
import math
import struct
import numpy as np
from PIL import Image

formats = {
    # ID => (name, mode, bitsPerPixel, blockW, blockH)
    0x00: ('I4',     'RGB',   4, 8, 8),
    0x01: ('I8',     'RGB',   8, 8, 4),
    0x02: ('IA4',    'RGBA',  8, 8, 4),
    0x03: ('IA8',    'RGBA', 16, 4, 4),
    0x04: ('RGB565', 'RGB',  16, 4, 4),
    0x05: ('RGB5A3', 'RGBA', 16, 4, 4),
    0x06: ('RGBA8',  'RGBA', 32, 4, 4),
    0x08: ('C4',     'RGB',   4, 8, 8),
    0x09: ('C8',     'RGB',   8, 8, 4),
    0x0A: ('C14X2',  'RGB',  16, 4, 4),
    0x0E: ('CMPR',   'RGBA',  4, 8, 8),
}


def swizzle(pixels, width, height, blockW=4, blockH=4):
    """Swizzle pixels into format used by game.

    pixels: buffer of pixels, which should be an np.array of
        at least 2 dimensions.
    width, height: size of buffer.
    blockW, blockH: swizzle block size, which depends on pixel format.

    Returns a list (1-dimensional) of pixels in swizzled order.
    """
    pixelsOut = []
    for y in range(0, height, blockH):
        for x in range(0, width, blockW):
            for by in range(blockH):
                for bx in range(blockW):
                    #idx  = ((y+by)*width) + (x+bx)
                    try: p = pixels[x+bx, y+by]
                    #except IndexError: p = pixels[0, 0]
                    except IndexError: continue
                    pixelsOut.append(p)
    return pixelsOut

def unswizzle(pixels, width, height, blockW=4, blockH=4):
    """Un-swizzle pixels out of format used by game.

    pixels: buffer of pixels, which should be an np.array of
        at least 2 dimensions.
    width, height: size of buffer.
    blockW, blockH: swizzle block size, which depends on pixel format.

    Returns pixels in unswizzled order.
    """
    pixelsOut = np.array(pixels)
    idx = 0
    for y in range(0, height, blockH):
        for x in range(0, width, blockW):
            for by in range(blockH):
                for bx in range(blockW):
                    px = (idx%width)
                    py = (idx//width)
                    idx += 1
                    try: pixelsOut[x+bx, y+by] = pixels[px, py]
                    except IndexError: continue
    return pixelsOut


def img2texture(inPath, outPath):
    image  = Image.open(inPath).convert('RGB')
    pixels = image.load()
    width, height = image.size

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
        fmt = struct.unpack_from('>B', header, 0x16)[0] # grumble
        numMipMaps = struct.unpack_from('>B', header, 0x19)[0] # grumble

        print("Texture: %dx%d pixels, format %02X (%s), %d mipmmaps" % (
            width, height, fmt, formats[fmt][0], numMipMaps))

        if fmt not in formats:
            raise TypeError("Unsupported format 0x%02X" % fmt)
        fmtName, mode, bitsPerPixel, blockW, blockH = formats[fmt]
        data = inFile.read((width * height * bitsPerPixel) // 8)

        if fmtName == 'I8':
            data = np.frombuffer(data,dtype=np.uint8).astype(np.uint32)
            data = 0xFF000000 + data + (data << 8) + (data << 16)
        elif fmtName == 'IA8':
            data = np.frombuffer(data,dtype=np.uint16).astype(np.uint32)
            data = (
                 (data & 0x00FF) |
                ((data & 0x00FF) <<  8) |
                ((data & 0x00FF) << 16) |
                ((data & 0xFF00) << 16))
        elif fmtName == 'RGB565':
            data = np.frombuffer(data,dtype=np.uint16).astype(np.uint32)
            data = (0xFF000000 |
                ((data & 0xF800) >> 8) |
                ((data & 0x07E0) << 5) |
                ((data & 0x001F) << 19))
        elif fmtName == 'RGBA8':
            data = np.frombuffer(data,dtype=np.uint32)#.astype(np.uint32)
            #data = (0xFF000000 +
            #    ((data & 0xF800) >> 8) +
            #    ((data & 0x07E0) << 5) +
            #    ((data & 0x001F) << 19))
        else: raise NotImplementedError("Unsupported format: %s" % fmtName)
        data = np.reshape(data, (width, height))
        data = unswizzle(data, width, height, blockW, blockH)
        data = np.reshape(data, (width, height)) # this is dumb
        img = Image.frombuffer(mode, (width,height), data, 'raw', 'RGBA', 0, 1)
        img.save(outPath)



def convertDDS(inPath, outPath):
    """Convert DDS exported from Gimp to game's texture format.

    Requires that the image be pre-swizzled and exported as BC1.
    """
    with open(inPath, 'rb') as inFile:
        magic = inFile.read(4)
        if magic != b'DDS ':
            raise TypeError("Not a DDS file")
        header = inFile.read(124)
        # height and width are in a stupid order
        height, width = struct.unpack_from('<II', header, 0x08)
        numMipMaps = struct.unpack_from('<I', header, 0x18)[0] # grumble
        data = inFile.read()
        print("Size %dx%d, %d mipmaps" % (width, height, numMipMaps))

    # byteswap pixels
    pixels = []
    for i in range(0, len(data), 2):
        pixels.append(bytes([data[i+1], data[i]]))

    with open(outPath, 'wb') as outFile:
        outHeader = bytearray(0x60)
        struct.pack_into('>HH', outHeader, 0x0A, width, height)
        outHeader[0x0F] = 1 # unknown
        outHeader[0x10] = 1 # unknown
        outHeader[0x16] = 0xE # format (BC1)
        outHeader[0x17] = 1 # unknown
        outHeader[0x18] = 1 # unknown
        outHeader[0x19] = numMipMaps
        outHeader[0x1A] = 1 # unknown
        outHeader[0x1D] = 6 # unknown
        outFile.write(bytes(outHeader))
        outFile.write(b''.join(pixels))


def main():
    if len(sys.argv) < 4:
        print("Usage: %s [pack|unpack] inpath outpath" % sys.argv[0])
        return 1
    if sys.argv[1] == 'pack':
        img2texture(sys.argv[2], sys.argv[3])
    elif sys.argv[1] == 'unpack':
        texture2img(sys.argv[2], sys.argv[3])
    elif sys.argv[1] == 'convertdds':
        convertDDS(sys.argv[2], sys.argv[3])
    else:
        print("unknown operation:", sys.argv[1])
        return 1


if __name__ == '__main__':
    sys.exit(main())
