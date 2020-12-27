import numpy as np
from .base import TextureFormat

class I8(TextureFormat):
    """8-bit I8 format."""
    fmtId        = 0x01
    name         = "IA8"
    hasAlpha     = False
    bitsPerPixel = 8
    blockW       = 8
    blockH       = 4

    def __init__(self, width:int, height:int, pixels:bytes):
        # convert to RGBA
        data = np.frombuffer(pixels,dtype=np.uint8).astype(np.uint32)
        data = 0xFF000000 + data + (data << 8) + (data << 16)
        super().__init__(width, height, data)
