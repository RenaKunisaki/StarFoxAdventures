# this line allows methods in SfaTexture to be annotated with
# a return type of SfaTexture.
from __future__ import annotations
import io
import sys
import math
import enum
import struct
import numpy as np
from .texture import ImageFormat, BITS_PER_PIXEL, decode_image, encode_image
from PIL import Image

# XXX move this
class TexFmt(enum.Enum):
    I4 =     0x00
    I8 =     0x01
    IA4 =    0x02
    IA8 =    0x03
    RGB565 = 0x04
    RGB5A3 = 0x05
    RGBA32 = 0x06
    C4 =     0x08
    C8 =     0x09
    C14X2 =  0x0A
    BC1 =    0x0E # aka CMPR

class SfaTexture:
    """Texture file in SFA."""

    def __init__(self):
        self.width      = None
        self.height     = None
        self.format     = None
        self.numMipMaps = None
        self.image      = None

    @staticmethod
    def calcSize(width:int, height:int, format:int, useMipMaps:bool, nMipMaps:int):
        bitsW, bitsH = 0, 0

        # copied from game. not sure what most of these are.
        if format in (TexFmt.I4, TexFmt.C4, TexFmt.BC1, 0x20, 0x30):
            bitsW = 3
            bitsH = 3
        elif format in (TexFmt.I8, TexFmt.IA4, TexFmt.C8, 0x11, 0x22,
            0x27, 0x28, 0x29, 0x2a, 0x39, 0x3a):
            bitsW = 3
            bitsH = 2
        elif format in (TexFmt.IA8, TexFmt.RGB565, TexFmt.RGB5A3, TexFmt.RGBA32,
            TexFmt.C14X2, 0x13, 0x16, 0x23, 0x2b, 0x2c, 0x3c):
            bitsW = 2
            bitsH = 2

        if format in (TexFmt.RGBA32, 0x16): iVar2 = 0x40
        else: iVar2 = 0x20

        if (useMipMaps):
            result = 0
            uVar1 = nMipMaps & 0xff
            while uVar1 != 0:
                w = width & 0xffff
                h = height & 0xffff
                result += iVar2 * (w + (1 << bitsW) + -1 >> bitsW) * (h + (1 << bitsH) + -1 >> bitsH)
                if w == 1 and h == 1: return result
                if (width & 0xffff) < 2: width = 1
                else: width = w >> 1
                if (height & 0xffff) < 2: height = 1
                else: height = h >> 1
                uVar1 -= 1
        else:
            result = (iVar2 * ((width & 0xffff) + (1 << bitsW) + -1 >> bitsW) *
                ((height & 0xffff) + (1 << bitsH) + -1 >> bitsH))
        return result


    @staticmethod
    def fromFile(file:io.RawIOBase) -> SfaTexture:
        """Instantiate texture from file."""
        self = SfaTexture()
        self._readFromFile(file)
        return self

    def _readFromFile(self, file:io.RawIOBase):
        header = file.read(0x60)
        self.width, self.height = struct.unpack_from('>HH', header, 0x0A)
        self.numMipMaps = struct.unpack_from('>B', header, 0x19)[0] # grumble
        fmtId = struct.unpack_from('>B', header, 0x16)[0] # grumble
        self._readData(file, fmtId)

    def _readData(self, file:io.RawIOBase, fmtId:int):
        self.format = ImageFormat(fmtId)
        bpp = BITS_PER_PIXEL[self.format]
        dataLen = self.width * self.height * bpp // 8
        data = file.read(dataLen)
        self.image = decode_image(data,
            None, # palette_data
            self.format, # image_format
            None, # palette_format
            0, # num_colors (for palettes)
            self.width, self.height)


    @staticmethod
    def fromImage(img:Image,
    fmt:ImageFormat=ImageFormat.RGBA32,
    numMipMaps:int=1) -> SfaTexture:
        """Instantiate texture from image."""
        assert numMipMaps > 0, "numMipMaps must be greater than zero"
        self = SfaTexture()
        self.width, self.height = img.size
        self.format = ImageFormat(fmt)
        self.numMipMaps = numMipMaps
        self.image = img
        return self


    def _makeHeader(self) -> bytearray:
        """Build the SFA texture file header for this texture."""
        header = bytearray(0x60)
        struct.pack_into('>HH', header, 0x0A, self.width, self.height)
        header[0x0F] = 1 # unknown
        header[0x10] = 1 # unknown
        header[0x16] = int(self.format)
        header[0x17] = 1 # unknown
        header[0x18] = 1 # unknown
        header[0x19] = self.numMipMaps
        header[0x1A] = 1 # unknown
        header[0x1D] = 6 # unknown
        return header


    def writeToFile(self, file:io.RawIOBase) -> None:
        """Write this texture to SFA-format file."""
        header = self._makeHeader()
        imageData, paletteData, colors = encode_image(
            self.image, self.format, None, mipmap_count=self.numMipMaps)
        file.write(header)
        file.write(imageData.getbuffer())
