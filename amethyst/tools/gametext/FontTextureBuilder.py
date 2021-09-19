from __future__ import annotations
import struct
import enum
import io
import os
from PIL import Image, ImageFont, ImageDraw
from .util import printf, readStruct, writeStruct, BAD_DIR_CHARS
from .LangEnum import LangEnum, LangCodes
from texconv.sfatexture import SfaTexture, TexFmt, ImageFormat
from texconv.texture import BITS_PER_PIXEL, BLOCK_WIDTHS, BLOCK_HEIGHTS, convert_color_to_rgb5a3

class FontEnum(enum.IntEnum):
    Japanese = 0
    Unused   = 1 # doesn't actually exist
    Buttons  = 2
    Flags    = 3
    English  = 4
    Faces    = 5

ICON_FONTS = (FontEnum.Buttons, FontEnum.Flags, FontEnum.Faces) # which font IDs are icons
LANG_FONTS = { # LangEnum => (font name, size)
    # can pretty much just put any font you want in here...
    # very large size might not render nicely.
    # you can put absolute paths here too...
    LangEnum.English:  ('EurostileBQ-Regular.otf', 20),
    #LangEnum.English:  ('Z003-MediumItalic.otf', 23), # lol
    LangEnum.French:   ('EurostileBQ-Regular.otf', 20),
    LangEnum.German:   ('EurostileBQ-Regular.otf', 20),
    LangEnum.Italian:  ('EurostileBQ-Regular.otf', 20),
    LangEnum.Japanese: ('FOT-RodinNTLG Pro B.otf', 20),
    LangEnum.Spanish:  ('EurostileBQ-Regular.otf', 20),
}

class FontTextureBuilder:
    """Packs characters into a font texture."""

    # max dimensions of font texture
    # the game never exceeds 512x512 but the console should be able
    # to do 1024x1024 and the game seems to accept that fine.
    # XXX test on console.
    MAX_WIDTH, MAX_HEIGHT= 1024, 1024

    def __init__(self, imgDir:str, fmt:ImageFormat=ImageFormat.RGB5A3,
    lang:LangEnum=LangEnum.English):
        self.imgDir   = imgDir
        self.images   = {}
        self.format   = fmt
        self.language = lang
        self.width, self.height = BLOCK_WIDTHS[fmt], BLOCK_HEIGHTS[fmt]
        self.lastX, self.lastY  = 0, 0
        self._imgCache = {} # (fontNo,char) => Image

        # load the font for this language
        fontName, fontSize = LANG_FONTS[lang]
        self.fontPath = os.path.join(os.path.dirname(__file__), 'fonts')
        self.font = ImageFont.truetype(os.path.join(self.fontPath, fontName),
            size=fontSize)

    def getChrImg(self, char:str, fontNo:int) -> Image:
        """Get Image for one character."""
        fontNo = int(fontNo)
        cacheKey = (fontNo,char)
        if cacheKey not in self._imgCache:
            if fontNo in ICON_FONTS:
                name = char
                if name in BAD_DIR_CHARS: name = '%04X' % ord(char)
                path = os.path.join(self.fontPath, FontEnum(fontNo).name, name + '.png')
                try: self._imgCache[cacheKey] = Image.open(path)
                except FileNotFoundError:
                    raise KeyError("No graphic for character '%s' in font '%s' (not found at: %s)" % (
                        char, FontEnum(fontNo).name, path))
            else:
                size = self.font.getsize(char, language=LangCodes[self.language])
                imgChr = Image.new('RGBA', size)
                draw = ImageDraw.Draw(imgChr)
                draw.text((0, 0), char, font=self.font)
                self._imgCache[cacheKey] = imgChr
        return self._imgCache[cacheKey]

    def measure(self, text:str, fontNo:FontEnum=FontEnum.English) -> tuple:
        """Measure string."""
        if fontNo in ICON_FONTS:
            w, h = 0, 0
            for c in text:
                cw, ch = self.getChrImg(c, fontNo).size
                w, h = max(w, cw), max(h, ch)
            return w, h
        else:
            return self.font.getsize(text, language=LangCodes[self.language])

    def add(self, char:str, fontNo:FontEnum=FontEnum.English) -> None:
        """Add character to font."""
        if type(char) is int: char = chr(char)
        if (fontNo,char) in self.images: return # already have this

        name = char
        if name in BAD_DIR_CHARS: name = '%04X' % ord(char)

        imgChr = self.getChrImg(char, fontNo)
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
            raise ValueError("No texture space left for character '%s' in font %s" % (
                char, str(fontNo)))
        if newW > self.width: self.width = newW
        chrDef = {'x1':x1, 'x2':x2, 'y1':y1, 'y2':y2, 'image':imgChr}
        #printf("Added chr '%s' font %d: %s\n", chr, fontNo, chrDef)
        self.images[(fontNo,char)] = chrDef
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
