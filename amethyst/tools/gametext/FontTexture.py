from __future__ import annotations
import struct
import enum
import io
from PIL import Image
from .util import printf, readStruct, writeStruct
from texconv.sfatexture import SfaTexture, TexFmt, ImageFormat
from texconv.texture import BITS_PER_PIXEL, BLOCK_WIDTHS, BLOCK_HEIGHTS, convert_color_to_rgb5a3

class FontTexture(SfaTexture):
    """Defines a font texture image."""

    def __init__(self, file:io.FileIO=None):
        super().__init__()
        if file: self.readFile(file)

    def readFile(self, file:io.FileIO) -> None:
        """Read from binary file."""
        self.format, self.bitsPerPx, self.width, self.height = readStruct(file, '>4H')
        if self.width == 0 and self.height == 0: raise StopIteration # end of texture list

        self.realTexFmt = self.format
        if self.format == 2: self.format = 0 #game does this for some reason
        elif self.format == 1: self.format = 5
        self.numMipMaps = 0
        #dataSize = self.width * self.height * (self.bitsPerPx // 8)
        #printf('READ TexFmt=0x%X %dBPP size=%dx%d (0x%X) @ 0x%X ~ 0x%X\n',
        #    self.format, self.bitsPerPx, self.width, self.height, dataSize,
        #    file.tell(), file.tell() + dataSize)
        self._readData(file, self.format)

    # XXX do we need this? it's basically the same as the SfaTexture method.
    # main difference is default format.
    @staticmethod
    def readImage(img:Image, fmt:ImageFormat=ImageFormat.RGB5A3,
    numMipMaps:int=1) -> FontTexture:
        """Instantiate texture from image."""
        self = FontTexture()
        self.width, self.height = img.size
        self.format = ImageFormat(fmt)
        self.bitsPerPx = BITS_PER_PIXEL.get(fmt, 0)
        self.numMipMaps = numMipMaps
        self.image = img
        return self

    def writeFile(self, file:io.FileIO, format:TexFmt=TexFmt.RGB5A3) -> None:
        """Write to binary file."""
        self.format = format
        writeStruct(file, '>4H', self.format.value, self.bitsPerPx, self.width, self.height)
        dataSize = self.width * self.height * (self.bitsPerPx // 8)
        startOffs = file.tell()
        printf('WRITE TexFmt=%s (0x%X) %dBPP size=%dx%d (0x%X) @ 0x%X ~ 0x%X\n',
            self.format, self.format.value, self.bitsPerPx, self.width, self.height, dataSize,
            startOffs, startOffs + dataSize)
        self._writeData(file)
        printf("actual size = 0x%X end = 0x%X\n", file.tell() - startOffs, file.tell())
