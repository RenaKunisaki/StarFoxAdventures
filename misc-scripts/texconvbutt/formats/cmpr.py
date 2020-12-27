import numpy as np
import struct
from wand.image import Image
from .base import TextureFormat

class CMPR(TextureFormat):
    """Compressed CMPR/DDS/BC1/DXT1 format."""
    # code stolen from: https://github.com/LagoLunatic/wwrando/blob/ef02b4690ac41ca0230b542017f60d0f45c8536f/wwlib/texture_utils.py
    # because fuuuuuuuuuuuuuuuuuuuuuuuuck trying to do this myself.
    # XXX clean this up
    fmtId        = 0x0E
    name         = "CMPR"
    hasAlpha     = True
    bitsPerPixel = 4
    blockW       = 8
    blockH       = 8

    def __init__(self, width:int, height:int, pixels:bytes):
        data = pixels
        blockDataSize = 32
        blockW, blockH = 8, 8

        _offs = 0
        def readu16():
            nonlocal _offs
            r = (data[_offs] << 8) | data[_offs+1]
            _offs += 2
            return r
        def readu32():
            nonlocal _offs
            r = (data[_offs  ] << 24) | (data[_offs+1] << 16) | \
                (data[_offs+2] <<  8) |  data[_offs+3]
            _offs += 4
            return r

        def getColors(c1, c2): # c1, c2 are RGB565
            r1, r2 = (c1 & 0xF800) >> 8, (c2 & 0xF800) >> 8
            g1, g2 = (c1 & 0x07E0) >> 3, (c2 & 0x07E0) >> 3
            b1, b2 = (c1 & 0x001F) << 3, (c2 & 0x001F) << 3
            a1, a2 = 255, 255

            if c1 > c2: # 4 colors
                # c3 = 1/3 between c1, c2
                # c4 = 2/3 between c1, c2
                r3, r4 = ((r1*2) + r2) // 3, (r1 + (r2*2)) // 3
                g3, g4 = ((g1*2) + g2) // 3, (g1 + (g2*2)) // 3
                b3, b4 = ((b1*2) + b2) // 3, (b1 + (b2*2)) // 3
                a3, a4 = 255, 255
            else: # 3 colors + transparent
                # c3 = 1/2 between c1, c2
                r3, g3, b3 = (r1+r2)//2, (g1+g2)//2, (b1+b2)//2
                r4, g4, b4 = 0, 0, 0
                a3, a4 = 255, 0
            return (
                (r1, g1, b1, a1),
                (r2, g2, b2, a2),
                (r3, g3, b3, a3),
                (r4, g4, b4, a4),
            )

        def decodeBlock(data, offset):
            output = [None]*64
            subOffs = offset
            for subIdx in range(4):
                subX = (subIdx %  2) * 4
                subY = (subIdx // 2) * 4
                c1   = readu16()
                c2   = readu16()
                palette = getColors(c1, c2)
                colIdxs = readu32()
                for i in range(16):
                    colIdx = ((colIdxs >> ((15-i)*2)) & 3)
                    col    = palette[colIdx]
                    xInSub = i %  4
                    yInSub = i // 4
                    pIdxInSub = subX + (subY*8) + (yInSub*8) + xInSub
                    output[pIdxInSub] = col
                subOffs += 8
            return output

        offs = 0
        blockX, blockY = 0, 0
        pxOut = bytearray(width*height*4)
        while blockY < height:
            pixel = decodeBlock(data, offs)
            for i, px in enumerate(pixel):
                xInBlock = i %  blockW
                yInBlock = i // blockW
                x = blockX + xInBlock
                y = blockY + yInBlock
                if x >= width or y >= height: continue
                if px is None: px = b'\xFF\x00\xFF\xFF'
                idx = ((y*width)+x) * 4
                pxOut[idx:idx+4] = px
            offs += blockDataSize
            blockX += blockW
            if blockX >= width:
                blockX = 0
                blockY += blockH

        data = np.frombuffer(pxOut, dtype=np.uint32)
        super().__init__(width, height, data)
