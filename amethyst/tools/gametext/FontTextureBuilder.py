from __future__ import annotations
import struct
import enum
import io
import os
from PIL import Image
from .util import printf, readStruct, writeStruct, BAD_DIR_CHARS
from texconv.sfatexture import SfaTexture, TexFmt, ImageFormat
from texconv.texture import BITS_PER_PIXEL, BLOCK_WIDTHS, BLOCK_HEIGHTS, convert_color_to_rgb5a3

class FontTextureBuilder:
    """Packs characters into a font texture."""

    # max dimensions of font texture
    MAX_WIDTH  = 512
    MAX_HEIGHT = 512

    def __init__(self, imgDir:str, fmt:ImageFormat=ImageFormat.RGB5A3):
        self.imgDir = imgDir
        self.images = {}
        self.format = fmt
        self.width, self.height = BLOCK_WIDTHS[fmt], BLOCK_HEIGHTS[fmt]
        self.lastX, self.lastY  = 0, 0

    def mapFontId(self, chr:str, fontNo:int):
        # map fontNo to texture
        if fontNo == 2:
            if chr in 'ABCJLRSXYZ': fontNo = 2
            elif chr in 'DFIJST': fontNo = 3 # game bug: J used in both 2 and 3
            else: fontNo = 5
        elif fontNo == 4: fontNo = 0
        return fontNo

    def add(self, chr:str, fontNo:int=4, lang:str="English"):
        """Add character to font."""

        #fontId = self.mapFontId(chr, fontNo)
        fontId = fontNo
        name = chr
        if name in BAD_DIR_CHARS: name = '%04X' % ord(chr)
        try: imgChr = Image.open(os.path.join(self.imgDir, lang, str(fontId), name+'.png'))
        except FileNotFoundError:
            raise KeyError("No graphic for character '%s' in font %d (%d)" % (
                chr, fontId, fontNo))

        cw, ch = imgChr.size
        x1, y1 = self.lastX, self.lastY
        x2, y2 = x1+cw, y1+ch
        newW, newH = x2+1, y2+1 # ensure a space to avoid texture glitching
        if newH >= self.height:
            self.height = newH

        if newW >= self.MAX_WIDTH:
            self.width = self.MAX_WIDTH # ensure power of two
            x1, x2 = 0, cw
            y1 = self.height
            self.height += ch
            y2 = self.height
            newW, newH = x2+1, y2+1

        if self.height >= self.MAX_HEIGHT:
            self.height = self.MAX_HEIGHT # can handle exception and try again
            raise ValueError("No texture space left for character '%s' in font %d" % (
                chr, fontNo))
        if newW > self.width: self.width = newW
        chrDef = {'x1':x1, 'x2':x2, 'y1':y1, 'y2':y2, 'image':imgChr}
        #printf("Added chr '%s' font %d: %s\n", chr, fontNo, chrDef)
        self.images[(fontNo,chr)] = chrDef
        self.lastX, self.lastY = newW, y1 # top left of next character

    def build(self) -> Image:
        """Generate the texture image."""
        # round up to block size
        fmt = self.format
        padX, padY = self.width % BLOCK_WIDTHS[fmt], self.height % BLOCK_HEIGHTS[fmt]
        w, h = self.width, self.height
        if padX: self.width  += BLOCK_WIDTHS [fmt] - padX
        if padY: self.height += BLOCK_HEIGHTS[fmt] - padY
        printf("tex size %dx%d pad %dx%d to %dx%d\n", w, h, padX, padY, self.width, self.height)
        assert self.width <= self.MAX_WIDTH, "Texture max width exceeded"
        assert self.height <= self.MAX_HEIGHT, "Texture max height exceeded"

        img = Image.new('RGBA', (self.width, self.height))
        for k, chrDef in self.images.items():
            #fontNo, chr = k
            img.paste(chrDef['image'], (chrDef['x1'], chrDef['y1']))
        return img
