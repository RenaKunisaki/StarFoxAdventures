import numpy as np
import struct
from wand.image import Image
from .base import TextureFormat

def _makeDdsHeader(width, height, numBytes):
    data = (
        0x20534444, # header "DDS "
        124, #DWORD           dwSize;
        0x081007, #DWORD           dwFlags;
        height, #DWORD           dwHeight;
        width, #DWORD           dwWidth;
        numBytes, #DWORD           dwPitchOrLinearSize;
        0, #DWORD           dwDepth;
        1, #DWORD           dwMipMapCount;
        0,0,0,0,0,0,0,0,0,0,0, #DWORD           dwReserved1[11];
        #struct DDS_PIXELFORMAT {
        32,    #DWORD dwSize;
        0x04,    #DWORD dwFlags;
        0x31545844,    #DWORD dwFourCC; "DXT1"
        0,    #DWORD dwRGBBitCount;
        0,    #DWORD dwRBitMask;
        0,    #DWORD dwGBitMask;
        0,    #DWORD dwBBitMask;
        0x00000000,    #DWORD dwABitMask;
        #} ddspf;
        0x1000, #DWORD           dwCaps;
        0, #DWORD           dwCaps2;
        0, #DWORD           dwCaps3;
        0, #DWORD           dwCaps4;
        0, #DWORD           dwReserved2;
    )
    return b''.join(map(lambda d: struct.pack('<I', d), data))


class CMPR(TextureFormat):
    """Compressed CMPR/DDS/BC1/DXT1 format."""
    fmtId        = 0x0E
    name         = "CMPR"
    hasAlpha     = True
    bitsPerPixel = 4
    blockW       = 8
    blockH       = 8

    def __init__(self, width:int, height:int, pixels:bytes):
        # byteswap pixels
        data = np.frombuffer(pixels,dtype=np.uint16)
        data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8)

        #data = np.frombuffer(data,dtype=np.uint8)
        #data = data & 0xFF # stupid hack to make data mutable
        #for i in range(0, len(data), 8):
            #data[i+0], data[i+1] = data[i+1], data[i+0]
            #data[i+2], data[i+3] = data[i+3], data[i+2]
            #data[i+4], data[i+5] = data[i+5], data[i+4]
            #data[i+6], data[i+7] = data[i+7], data[i+6]
            #data[i+4], data[i+5], data[i+6], data[i+7] = \
            #data[i+7], data[i+6], data[i+5], data[i+4]

        # generate DDS header
        data = data.tobytes()
        data = _makeDdsHeader(width, height, len(data))+data
        with open('butt.dds', 'wb') as file: file.write(data)
        img  = Image(blob=data, format='dds')
        data = bytes(img.export_pixels())
        data = np.frombuffer(data,dtype=np.uint32)
        data = np.reshape(data, (height, width))

        #data = data & 0xFFFFFFFF
        #for y in range(height):
        #    for x in range(width):
        #        data[y,x] = 0xFF000000 | ((y&0xF)<<4) | ((x&0xF)<<12)

        super().__init__(width, height, data)
        self.descramble()

    def descramble(self):
        # descramble blocks
        # 0123 4567 89AB CDEF GHIJ KLMN OPQR STUV =>
        # 0123 GHIJ
        # 4567 KLMN
        # 89AB OPQR
        # CDEF STUV
        # x,y = x+(y*4),y/4
        pixelsOut = np.zeros_like(self.pixels)
        idx = 0
        bw, bh = 4, 4
        for y in range(0, self.height, bh):
            for x in range(0, self.width, bw):
                for by in range(bh):
                    for bx in range(bw):
                        sx = (idx %  self.width)
                        sy = (idx // self.width)
                        dx = x+bx
                        dy = y+by
                        #print(x, y, sx, sy, dx, dy)
                        #pixelsOut[dy,dx] = self.pixels[sy,sx]
                        try: pixelsOut[sy,sx] = self.pixels[dy,dx]
                        #pixelsOut[y+by,x+bx] = self.pixels[py,px]
                        except IndexError: pass
                        idx += 1
        self.pixels = pixelsOut


    def toTexture(self) -> bytes:
        """Return buffer of this texture in SFA format."""
        header = self._makeSfaHeader()

        # read pixels and convert to RGB565
        # XXX this can probably be done faster
        buf = np.array(dtype=np.uint16, ndmin=3)
        for y in range(self.height):
            for x in range(self.width):
                rgba = self.pixels[x,y]
                r =  rgba >> 24 # XXX verify order
                g = (rgba >> 16) & 0xFF
                b = (rgba >>  8) & 0xFF
                buf[x,y] = (b >> 3) | ((g >> 2) << 5) | ((r >> 3) << 11)

        # byteswap
        buf = ((buf & 0x00FF) << 8) | ((buf & 0xFF00) >> 8)
        return header + buf.tobytes()

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
                for by in range(0, self.blockH):
                    for bx in range(self.blockW):
                        # notice y,x indexing here.
                        try: pixelsOut[y+by, x+bx] = data[idx]
                        except IndexError: continue
                        idx += 1
        self.pixels = pixelsOut
        #self.descramble()
        return self
