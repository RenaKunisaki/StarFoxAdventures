import numpy as np
from .base import TextureFormat

class IA8(TextureFormat):
    """16-bit IA8 format."""
    fmtId        = 0x03
    name         = "IA8"
    hasAlpha     = True
    bitsPerPixel = 16
    blockW       = 4
    blockH       = 4

    def __init__(self, width:int, height:int, pixels:bytes):
        # convert to RGBA
        data = np.frombuffer(pixels,dtype=np.uint16).astype(np.uint32)
        data = ( # low byte: alpha, high byte: color
            ((data & 0xFF00) >>  8) |
            ((data & 0xFF00)      ) |
            ((data & 0xFF00) <<  8) |
            ((data & 0x00FF) << 24))
        super().__init__(width, height, data)
