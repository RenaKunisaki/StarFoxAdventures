import numpy as np
import struct

class TextureFormat:
    """Base class for texture formats."""
    fmtId:int        = None # format ID in SFA texture header
    name:str         = None # format name
    hasAlpha:bool    = None # has an alpha channel?
    bitsPerPixel:int = None # bits per pixel in packed form
    blockW:int       = None # block size for swizzle
    blockH:int       = None

    def __init__(self, width:int, height:int, data:bytes):
        self.width  = width
        self.height = height

        # np takes dimensions in enas order (that's reverse of "sane")
        if type(data) is bytes:
            data = np.frombuffer(data, dtype=np.uint32)
        #print(data.shape, data.dtype)
        self.pixels = data.reshape((self.height, self.width))

    def toTexture(self) -> bytes:
        """Return buffer of this texture in SFA format."""
        raise NotImplementedError

    def _makeSfaHeader(self) -> bytearray:
        """Make the header for an SFA texture."""
        header = bytearray(0x60)
        struct.pack_into('>HH', header, 0x0A, self.width, self.height)
        header[0x0F] = 1 # unknown
        header[0x10] = 1 # unknown
        header[0x16] = self.fmtId # format
        header[0x17] = 1 # unknown
        header[0x18] = 1 # unknown
        header[0x19] = 1 # number of mipmaps including base image
        header[0x1A] = 1 # unknown
        header[0x1D] = 6 # unknown

    def swizzle(self):
        """Swizzle this texture, rearranging the pixels into the format
        used by the GameCube.

        This changes the texture; it does not return a copy.
        """
        pixelsOut = np.zeros_like(self.pixels)
        idx = 0
        for y in range(0, self.height, self.blockH):
            for x in range(0, self.width, self.blockW):
                for by in range(self.blockH):
                    for bx in range(self.blockW):
                        px = (idx% self.width)
                        py = (idx//self.width)
                        # non-power-of-two-size textures can have blocks
                        # that are partially out of bounds. just ignore
                        # those pixels.
                        # XXX should be y,x indexing here?
                        try: pixelsOut[px,py] = self.pixels[x+bx, y+by]
                        except IndexError: pass
                        idx += 1
        #we could return a copy, but then we have to deal with the fact
        #that the data is already RGBA and the constructor will want
        #to convert it again...
        #return type(self)(self.width, self.height, pixelsOut)
        self.pixels = pixelsOut
        return self

    def unswizzle(self):
        """Un-swizzle this texture from the GameCube pixel order.

        This changes the texture; it does not return a copy.
        """
        pixelsOut = np.zeros_like(self.pixels)
        data = self.pixels.ravel() # to 1D array (should be unravel?)
        #data = self.pixels
        idx = 0
        for y in range(0, self.height, self.blockH):
            for x in range(0, self.width, self.blockW):
                for by in range(self.blockH):
                    for bx in range(self.blockW):
                        # notice y,x indexing here.
                        try: pixelsOut[y+by, x+bx] = data[idx]
                        except IndexError: continue
                        idx += 1
                        # XXX this isn't quite correct. it fails on some
                        # of the last few pixels of a texture that isn't
                        # a power-of-two size.
        #return type(self)(self.width, self.height, pixelsOut)
        self.pixels = pixelsOut
        return self
