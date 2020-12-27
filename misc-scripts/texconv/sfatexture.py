# this line allows methods in SfaTexture to be annotated with
# a return type of SfaTexture.
from __future__ import annotations
import io
import sys
import math
import struct
import numpy as np
from texture import ImageFormat, BITS_PER_PIXEL, decode_image, encode_image
from PIL import Image

class SfaTexture:
    """Texture file in SFA."""

    def __init__(self):
        self.width      = None
        self.height     = None
        self.format     = None
        self.numMipMaps = None
        self.image      = None


    @staticmethod
    def fromFile(file:io.RawIOBase) -> SfaTexture:
        """Instantiate texture from file."""
        self = SfaTexture()

        header = file.read(0x60)
        self.width, self.height = struct.unpack_from('>HH', header, 0x0A)
        self.numMipMaps = struct.unpack_from('>B', header, 0x19)[0] # grumble
        fmtId = struct.unpack_from('>B', header, 0x16)[0] # grumble
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
        return self


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
