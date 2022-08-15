# this line allows methods in SfaTexture to be annotated with
# a return type of SfaTexture.
from __future__ import annotations
import io
import sys
import math
import struct
import numpy as np
from .texture import ImageFormat, BITS_PER_PIXEL, decode_image, encode_image
from .Header import Header
from PIL import Image

class SfaTexture:
    """Texture file in SFA."""

    def __init__(self):
        self.width       = None
        self.height      = None
        self.format      = None
        self.nFrames     = None
        self.image       = None
        self.frameImages = None


    def _fromData(self, header:bytes, data:bytes):
        self.image = decode_image(data,
            None, # palette_data
            self.format, # image_format
            None, # palette_format
            0, # num_colors (for palettes)
            self.width, self.height)
        # XXX this isn't correct, but does work most of the time...
        if header[0x1D] & 1:
            self.image = self.image.transpose(Image.FLIP_TOP_BOTTOM)
        return self


    @staticmethod
    def fromFile(file:io.RawIOBase) -> SfaTexture:
        """Instantiate texture from file."""
        self = SfaTexture()

        header = Header.from_buffer_copy(file.read(0x60))
        self.width, self.height = header.width, header.height
        self.nFrames = header.nFrames
        self.format  = ImageFormat(header.format)

        bpp = BITS_PER_PIXEL[self.format]
        dataLen = self.width * self.height * bpp // 8
        return self._fromData(header, file.read(dataLen))


    @staticmethod
    def fromData(data:bytes) -> SfaTexture:
        """Instantiate texture from raw data."""
        self = SfaTexture()

        header = Header.from_buffer_copy(data[0:0x60])
        self.width, self.height = header.width, header.height
        self.nFrames = header.nFrames
        self.format  = ImageFormat(header.format)

        return self._fromData(data[0:0x60], data[0x60:])


    @staticmethod
    def fromImage(img:Image,
    fmt:ImageFormat=ImageFormat.RGBA8,
    nFrames:int=1) -> SfaTexture:
        """Instantiate texture from image."""
        assert nFrames > 0, "nFrames must be greater than zero"
        self = SfaTexture()
        self.width, self.height = img.size
        self.format  = ImageFormat(fmt)
        self.nFrames = nFrames
        self.image   = img
        return self


    @staticmethod
    def fromImageSet(img:list,
    fmt:ImageFormat=ImageFormat.RGBA8) -> SfaTexture:
        """Instantiate texture from multiple images.
        Each image is one frame.
        """
        self = SfaTexture()
        self.width, self.height = img[0].size
        self.format      = ImageFormat(fmt)
        self.nFrames     = len(img)
        self.image       = img[0]
        self.frameImages = img[1:]
        return self


    def _makeHeader(self) -> bytearray:
        """Build the SFA texture file header for this texture."""
        header = Header()
        header.width  = self.width
        header.height = self.height
        header.format = int(self.format)
        header.usage  = 1 # XXX check these fields
        header.magFilter = 1
        header.mipmapVar1D = 6
        return bytearray(header)


    def writeToFile(self, file:io.RawIOBase) -> None:
        """Write this texture to SFA-format file."""
        header = self._makeHeader()
        imageData, paletteData, colors = encode_image(
            self.image, self.format, None,
            mipmap_count=self.nFrames-1)
        file.write(header)
        file.write(imageData.getbuffer())


    def toData(self) -> bytes:
        """Encode this texture in SFA format and return data."""
        header = self._makeHeader()
        imageData, paletteData, colors = encode_image(
            self.image, self.format, None,
            mipmap_count=self.nFrames-1,
            mipmap_images=self.frameImages)
        return header + imageData.getbuffer()
