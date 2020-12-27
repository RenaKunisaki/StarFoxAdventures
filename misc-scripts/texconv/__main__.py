#!/usr/bin/env python3
"""Converts between texture data and standard image files."""
# XXX paletted formats don't work for packing.
import sys
import math
import struct
import numpy as np
from sfatexture import SfaTexture
from texture import ImageFormat
from PIL import Image


def texture2img(inPath, outPath):
    with open(inPath, 'rb') as file:
        tex = SfaTexture.fromFile(file)
        tex.image.save(outPath)

def img2texture(format, mipmaps, inPath, outPath):
    mipmaps = int(mipmaps)
    assert mipmaps > 0, "mipmaps must be at least 1"
    format = ImageFormat[format]
    img = Image.open(inPath)
    tex = SfaTexture.fromImage(img, fmt=format, numMipMaps=mipmaps)
    with open(outPath, 'wb') as file:
        tex.writeToFile(file)


def main():
    if len(sys.argv) < 4:
        print("Usage: %s unpack inpath outpath" % sys.argv[0])
        print("       %s pack format mipmaps inpath outpath" % sys.argv[0])
        return 1
    if sys.argv[1] == 'pack':
        img2texture(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
    elif sys.argv[1] == 'unpack':
        texture2img(sys.argv[2], sys.argv[3])
    else:
        print("unknown operation:", sys.argv[1])
        return 1


if __name__ == '__main__':
    sys.exit(main())
