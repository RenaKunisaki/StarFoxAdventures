#!/usr/bin/env python3
"""Parse/build gametext files."""
import os
import io
import sys
sys.path.append(".") # ugh
import struct
from texconv.sfatexture import SfaTexture

def readStruct(file, fmt:str, offset:int=None):
    """Read a struct from a file."""
    if offset is not None: file.seek(offset, 0)
    size = struct.calcsize(fmt)
    res  = struct.unpack(fmt, file.read(size))
    if len(res) == 1: res = res[0] # grumble
    return res

controlCharLength = {
    0xF8F2:2, 0xF8F3:0, 0xF8F4:1, 0xF8F5:1, 0xF8F6:1, 0xF8F7:1, 0xF8F8:0, 0xF8F9:0,
    0xF8FA:0, 0xF8FB:0, 0xF8FC:0, 0xF8FD:0, 0xF8FE:0, 0xF8FF:4, 0xE000:1, 0xE018:3,
    0xE020:1,
}

def readStr(file:io.IOBase, offset:int=None):
    if offset is not None: file.seek(offset, 0)
    offset = file.tell()
    res = ''
    while True:
        b = file.read(1)[0]
        if b == 0: break
        if b >= 0x80: b = (b << 8) | file.read(1)[0]

        if b in controlCharLength: # XXX what's this table used for?
            res = '<%04X' % b
            for _ in range(controlCharLength[b]):
                b  = (file.read(1)[0] << 8)
                b |=  file.read(1)[0]
                res += ' %04X' % b
            res += '>'

        elif b >= 0x80:
            if b == 0xEE80: #sequence commands
                b2 = file.read(1)[0]
                if b2 == 0x80: #sequence ID
                    res += '<SEQ #0x%04X>' % readStruct(file, '>H')
                elif b2 == 0x98: #sequence timing
                    b3, b4, b5 = readStruct(file, '>3H')
                    res += '<SEQ TIME 0x%04X 0x%04X 0x%04X>' % (b3, b4, b5)
                else:
                    res += '<SEQ UNK%02X>' % b2

            elif b == 0xEFA3: #render commands
                b2 = file.read(1)[0]
                if b2 == 0xB4: # set scale
                    res += '<SCALE %1.2f%%>' % (readStruct(file, '>H') / 2.56)

                elif b2 == 0xB7: # set font
                    res += '<FONT %d>' % readStruct(file, '>H')

                elif b2 == 0xB8: res += '<JUSTIFY LEFT>'
                elif b2 == 0xB9: res += '<JUSTIFY RIGHT>'
                elif b2 == 0xBA: res += '<JUSTIFY CENTER>'
                elif b2 == 0xBB: res += '<JUSTIFY FULL>'
                elif b2 == 0xBF: # set color
                    res += '<COLOR #%02X%02X%02X%02X, #%02X%02X%02X%02X>' % readStruct(file, '>8B')
                else: res += '<UNK EF A3 %02X>' % b2

            else: res += chr(b)
        else: res += chr(b)
    return res, file.tell() - offset

class CharacterStruct:
    """Identifies the location of one character in the font textures."""
    def __init__(self, file:io.FileIO):
        self.character = readStruct(file, '>I') #unicode number
        self.xpos      = readStruct(file, '>H') #coordinate of leftmost texel in font bitmap
        self.ypos      = readStruct(file, '>H') #coordinate of topmost texel in font bitmap
        self.left      = readStruct(file, '>b') #spacing
        self.right     = readStruct(file, '>b') #spacing
        self.top       = readStruct(file, '>b') #spacing
        self.bottom    = readStruct(file, '>b') #spacing
        self.width     = readStruct(file, '>B') #size
        self.height    = readStruct(file, '>B') #size
        self.fontNo    = readStruct(file, '>B') #Index into GameTextFonts[] in GameTextData.h
        self.textureNo = readStruct(file, '>B') #Index into Textures[] in the binary file


class GameTextStruct:
    """Defines a dialogue message."""
    def __init__(self, file:io.FileIO):
        self.identifier = readStruct(file, '>H') #which game text is this
        self.numPhrases = readStruct(file, '>H') #how many phrases this text has
        self.window     = readStruct(file, '>b') #preferred window
        self.alignH     = readStruct(file, '>b') #horizontal alignment
        self.alignV     = readStruct(file, '>b') #vertical alignment
        self.language   = readStruct(file, '>b') #LANGUAGE_xxx from GameTextData.h
        self.phrases    = readStruct(file, '>I') #pointer to <numPhrases> pointers to localised strings


class FontTexture(SfaTexture):
    """Defines a font texture image."""
    def __init__(self, file:io.FileIO):
        super().__init__()

        self.texFmt, self.pixFmt, self.width, self.height = readStruct(file, '>4H')
        if self.width == 0 and self.height == 0: raise StopIteration # end of texture list

        self.realTexFmt = self.texFmt
        if self.texFmt == 2: self.texFmt = 0 #game does this for some reason
        elif self.texFmt == 1: self.texFmt = 5
        self.numMipMaps = 0
        print('TexFmt=0x%X PixFmt=0x%X size=%dx%d' % (
            self.texFmt, self.pixFmt, self.width, self.height))
        self._readData(file, self.texFmt)


class GameTextBin:
    """Represents the contents of a .bin file under gametext directory."""
    def __init__(self, file:(str,io.FileIO)=None):
        self.chars    = [] # CharacterStruct
        self.texts    = [] # GameTextStruct
        self.strings  = {} # offset:int => string:str
        self.textures = [] # FontTexture
        self.unkData  = b'' # bytes
        if file is not None: self.read(file)


    def read(self, file:(str,io.FileIO)):
        """Read from file."""
        if type(file) is str: file = open(file, 'rb')

        # read character data
        numCharStructs = readStruct(file, '>I')
        for _ in range(numCharStructs):
            self.chars.append(CharacterStruct(file))

        # read texts
        numTexts, strDataLen = readStruct(file, '>2H')
        print("numTexts=", numTexts, "strDataLen=", hex(strDataLen))
        for _ in range(numTexts):
            self.texts.append(GameTextStruct(file))

        # read string offsets
        strOffsets = []
        numStrings = readStruct(file, '>I')
        for _ in range(numStrings):
            strOffsets.append(readStruct(file, '>I'))

        # read string data
        print("String data offset=", hex(file.tell()))
        strOffs = file.tell()
        for i in range(numStrings):
            startOffs = strOffs + strOffsets[i]
            sOffs = startOffs - strOffs
            string, _ = readStr(file, startOffs)
            self.strings[sOffs] = string
        file.seek(strOffs + strDataLen, 0) # skip padding, etc

        # read unknown data (seems to be all 0xEE)
        unkDataLen = readStruct(file, '>I')
        offset = file.tell()
        print('unkDataLen=0x%X, offset=0x%X' % (unkDataLen, offset))
        self.unkData = file.read(unkDataLen)
        # XXX any simpler way?
        for i, b in enumerate(self.unkData):
            if b != 0xEE:
                print("non-EE byte in unkdata at 0x%X" % (i + offset))
                break

        # read texture images
        while len(self.textures) < 32: # failsafe
            try:
                print("Texture %d at 0x%X" % (len(self.textures), file.tell()))
                tex = FontTexture(file)
                self.textures.append(tex)
            except StopIteration:
                break
        print("EOF at", hex(file.tell()))

        # set phrase data
        for text in self.texts:
            idx = text.phrases
            text.phrases = []
            for i in range(text.numPhrases):
                offs = strOffsets[idx+i]
                text.phrases.append(self.strings[offs])


def main(*args):
    g = GameTextBin(args[0])
    for text in g.texts:
        print(text.phrases)


if __name__ == '__main__':
    main(*sys.argv[1:])
