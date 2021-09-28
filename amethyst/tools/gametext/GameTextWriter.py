from __future__ import annotations
import struct
import enum
import os
import io
from PIL import Image
from .util import printf, readStruct, writeStruct
from texconv.sfatexture import SfaTexture, TexFmt, ImageFormat
from texconv.texture import BITS_PER_PIXEL, BLOCK_WIDTHS, BLOCK_HEIGHTS, convert_color_to_rgb5a3
from .CharacterStruct import CharacterStruct
from .GameTextStruct import GameTextStruct
from .FontTexture import FontTexture
from .FontTextureBuilder import FontTextureBuilder, FontEnum
from .LangEnum import LangEnum, LangCodes

# file structure:
# u32 numCharStructs; characterStruct[numCharStructs];
# u16 numTexts, strDataLen; gametextStruct[numTexts] texts;
# u32 numStrings; u32 strOffset[numStrings]; char[] strings[strDataLen];
# u32 unknownLen; u8 unknownData[unknownLen];
# FontTexture fontTextures[];

class GameTextWriter:
    """Creates a .bin file under gametext directory."""

    MAX_TEXTURES = 2 # max textures allowed in a bin file
    FONT_MAP = { # font ID => texture ID
        FontEnum.Japanese: 1,
        FontEnum.Unused:   0,
        FontEnum.Buttons:  0,
        FontEnum.Flags:    0,
        FontEnum.English:  1,
        FontEnum.Faces:    0, # XXX requires some kind of special handling
    }

    def __init__(self, lang:LangEnum=LangEnum.English):
        self.language   = lang
        self.chars      = [] # CharacterStruct
        self.texts      = [] # GameTextStruct
        self.strings    = {} # offset:int => string:str
        self.textures   = [] # FontTexture
        self.unkDataLen = 0
        self.charDir    = 'tmp/chars/' # XXX
        self._haveChars = set() # (fontNo, character)
        self._haveTexts = set() # phrase texts concated

    def addFont(self, font:(Image, io.FileIO, str, FontTexture)) -> None:
        """Add a font texture."""
        if type(font) is not FontTexture:
            if type(font) is not Image:
                font = Image.open(font)
            font = FontTexture.readImage(font)
        self.textures.append(font)

    def addChar(self, char:CharacterStruct) -> None:
        """Add a character."""
        k = (char.fontNo, char.character)
        if k not in self._haveChars:
            self._haveChars.add(k)
            self.chars.append(char)

    def addText(self, text:GameTextStruct) -> None:
        """Add a text."""
        k = '\x1E'.join(map(str, text.phrases)) # ASCII record separator
        if k not in self._haveTexts:
            self._haveTexts.add(k)
            self.texts.append(text)

    def _buildStringTable(self):
        """Iterate all texts and build the set of used characters,
        the string data, and the string offset table.
        """
        self.usedChars  = set() # (font,character)
        self.strData    = []
        self.strOffsets = [] # strIdx => offset relative to start of str table
        nextOffs = 0
        font = FontEnum.Japanese if self.language == LangEnum.Japanese else FontEnum.English
        for text in self.texts:
            for phrase in text.phrases:
                #print(phrase, phrase.getUsedChars(font))
                self.usedChars = self.usedChars.union(phrase.getUsedChars(font))
                string = phrase.toBytes()
                self.strOffsets.append(nextOffs)
                data = string + b'\0'
                nextOffs += len(data)
                self.strData.append(data)
        self.usedChars = sorted(self.usedChars, key=lambda it: ord(it[1]) + (it[0] * 10000))

        # pad (XXX how much is needed?)
        self.padding = b''
        pad = nextOffs & 3
        if pad: self.padding += (b'\0' * (4 - pad))
        nextOffs += len(self.padding)

        return nextOffs

    def _generateTextures(self):
        """Generate the font textures containing the necessary characters."""
        # each file can only contain two textures.
        # typically one is used for letters (and is monochrome)
        # and the other is used for icons/flags/faces.
        # we'll use texture 0 for icons, 1 for text, like the game does.
        self.iconFont = FontTextureBuilder(self.charDir, lang=self.language, fmt=ImageFormat.RGB5A3)
        self.monoFont = FontTextureBuilder(self.charDir, lang=self.language, fmt=ImageFormat.I4)
        self.texBuilder = [self.iconFont, self.monoFont]

        # add needed characters to each texture
        self.usedFonts = set(map(lambda it: it[0], self.usedChars))
        for (fontNo, chr) in self.usedChars:
            texNo = self.FONT_MAP[fontNo]
            self.texBuilder[texNo].add(chr, fontNo)

        # generate the texture images
        self.textures = []
        for i, builder in enumerate(self.texBuilder):
            self.textures.append(FontTexture.readImage(builder.build()))
            self.textures[-1].image.save('tmp/built%d.png' % i) # debug

    def _generateCharStructs(self):
        """Generate the CharacterStruct table."""
        # character structs define the position of each character in the font texture.
        # read chars.xml to get the spacing and padding of each character.
        #eChars = ET.parse(os.path.join(self.charDir, 'chars.xml')).getroot()

        self.chars = []
        for (fontNo, chr) in self.usedChars:
            texNo = self.FONT_MAP[fontNo]

            # find char element
            #eChar = None
            #for e in eChars:
            #    #print(e.attrib['character'], e.attrib['fontNo'])
            #    if e.attrib['character'] == chr and int(e.attrib['fontNo'], 0) == fontNo:
            #        eChar = e
            #        break
            #if eChar is None:
            #    raise KeyError("No definition found for character '%s' (0x%X) in font %d (%d)" % (
            #        chr, ord(chr), texNo, fontNo))

            # the original game packs the font graphics into the textures without
            # accounting for baseline, then uses these spacing values to place them
            # correcftly. our script packs them so that every character's baseline is
            # at the same height of the texture, so we don't need spacing.
            # (maybe this is less efficient use of space? not sure)

            #chr = eChar.attrib['character']
            chrDef = self.texBuilder[texNo].images[(fontNo, chr)]
            cs = CharacterStruct(
                character = chr,
                xpos      = chrDef['x1'],
                ypos      = chrDef['y1'],
                width     = chrDef['x2'] - chrDef['x1'],
                height    = chrDef['y2'] - chrDef['y1'],
                left      = 0, # int(eChar.attrib['left']),
                right     = 0, # int(eChar.attrib['right']),
                top       = 0, # int(eChar.attrib['top']),
                bottom    = 0, # int(eChar.attrib['bottom']),
                fontNo    = fontNo,
                textureNo = texNo,
            )
            self.chars.append(cs)

    def _writeCharStructs(self, file):
        """Write the CharacterStruct table to a file."""
        # write the char structs
        writeStruct(file, '>I', len(self.chars))
        for char in self.chars:
            file.write(char.toBytes())

    def _writeTextStructs(self, file):
        """Write the GameTextStruct table to a file."""
        #print("write numTexts =", len(self.texts), "strDataLen =", hex(self.strDataLen),
        #    "at", hex(file.tell()))
        if len(self.texts) > 65535:
            raise ValueError("Too many texts (%d, max 65535)" % len(self.texts))
        if self.strDataLen > 65535:
            raise ValueError("Too much text data (%d bytes, max 65535)" % self.strDataLen)

        writeStruct(file, '>HH', len(self.texts), self.strDataLen)
        strIdx = 0
        for text in self.texts:
            data = text.toBytes(strIdx)
            file.write(data)
            strIdx += len(text.phrases)

    def _writeStringTable(self, file):
        """Write the string offset table and string data to a file."""
        # write string offsets
        #print("write numStrs =", len(strOffsets), "at", hex(file.tell()))
        writeStruct(file, '>I', len(self.strOffsets))
        for offs in self.strOffsets:
            writeStruct(file, '>I', offs)

        # write strings
        for s in self.strData: file.write(s)
        file.write(self.padding)

    def _writeUnknownData(self, file):
        """Write the unknown data to a file."""
        self.unkDataLen = 0 # the game doesn't seem to actually need this...
        #print("write unkLen =", self.unkDataLen, "at", hex(file.tell()))
        writeStruct(file, '>I', self.unkDataLen)
        file.write(b'\xEE' * self.unkDataLen)

    def _writeTextures(self, file):
        """Write the texture graphics to a file."""
        for i, tex in enumerate(self.textures):
            #printf("Write texture %d at 0x%X\n", i, file.tell())
            tex.writeFile(file)

    def _writeEOF(self, file):
        """Write the end-of-file marker."""
        #printf("Write EOF at 0x%X\n", file.tell())
        file.write(b'\0' * 8)

    def write(self, file:(str,io.FileIO)) -> None:
        """Create new file."""
        if type(file) is str: file = open(file, 'wb')
        self.strDataLen = self._buildStringTable()
        self._generateTextures()
        self._generateCharStructs()
        self._writeCharStructs(file)
        self._writeTextStructs(file)
        self._writeStringTable(file)
        self._writeUnknownData(file)
        self._writeTextures(file)
        self._writeEOF(file)
