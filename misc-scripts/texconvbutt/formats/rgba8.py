import numpy as np
from .base import TextureFormat

class RGBA8(TextureFormat):
    """16-bit RGBA8888 format."""
    fmtId        = 0x06
    name         = "RGBA8"
    hasAlpha     = True
    bitsPerPixel = 32
    blockW       = 4
    blockH       = 4

    def __init__(self, width:int, height:int, pixels:bytes):
        data = np.frombuffer(pixels, dtype=np.uint32)
        # no need to convert to RGBA. XXX byteswap?
        super().__init__(width, height, data)

    def toTexture(self) -> bytes:
        """Return buffer of this texture in SFA format."""
        header = self._makeSfaHeader()

        buf = self.pixels.copy() # no need to convert. XXX byteswap?
        #buf = ((buf & 0x00FF) << 8) | ((buf & 0xFF00) >> 8)
        return header + buf.tobytes()
