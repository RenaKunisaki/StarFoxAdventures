import numpy as np
from .base import TextureFormat

class RGB565(TextureFormat):
    """16-bit RGB565 format."""
    fmtId        = 0x04
    name         = "RGB565"
    hasAlpha     = False
    bitsPerPixel = 16
    blockW       = 4
    blockH       = 4

    def __init__(self, width:int, height:int, pixels:bytes):
        # byteswap pixels
        data = np.frombuffer(pixels, dtype=np.uint16)
        data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8)

        # convert to RGBA
        data = data.astype(np.uint32)
        data = (0xFF000000 |
            ((data & 0xF800) >> 8) |
            ((data & 0x07E0) << 5) |
            ((data & 0x001F) << 19))

        super().__init__(width, height, data)

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
