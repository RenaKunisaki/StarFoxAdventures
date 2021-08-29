#!/usr/bin/env python3
"""Parse/build gametext files."""
import os
import io
import sys
sys.path.append(".") # ugh
import struct
import inspect
import xml.etree.ElementTree as ET
from texconv.sfatexture import SfaTexture

def printf(fmt, *args):
    print(fmt % args, end='')

def readStruct(file, fmt:str, offset:int=None):
    """Read a struct from a file."""
    if offset is not None: file.seek(offset, 0)
    size = struct.calcsize(fmt)
    res  = struct.unpack(fmt, file.read(size))
    if len(res) == 1: res = res[0] # grumble
    return res

controlCharLength = {
    0xF8F2:2,
    0xF8F3:0,
    0xF8F4:1, # set scale
    0xF8F5:1,
    0xF8F6:1,
    0xF8F7:1, # select font
    0xF8F8:0, # left justify
    0xF8F9:0, # right justify
    0xF8FA:0, # center justify
    0xF8FB:0, # full justify
    0xF8FC:0,
    0xF8FD:0,
    0xF8FE:0,
    0xF8FF:4,
    0xE000:1, # sequence ID (no idea what this is actually used for)
    0xE018:3, # sequence timing (params: time, ?, ?)
    0xE020:1,
}

def readStr(file:io.IOBase, offset:int=None):
    if offset is not None: file.seek(offset, 0)
    offset = file.tell()
    res = ''
    while True:
        b = file.read(1)[0]
        if b == 0: break
        if b >= 0x80: # UTF-8 more bytes
            if   b >= 0xF0: n, b = 3, b & 0x07
            elif b >= 0xE0: n, b = 2, b & 0x0F
            else: n, b = 1, 0x1F
            for _ in range(n):
                b = (b << 6) | (file.read(1)[0] & 0x3F)

        params = []
        if b in controlCharLength: # XXX what's this table used for?
            for _ in range(controlCharLength[b]):
                params.append(readStruct(file, '>H'))

        if   b == 0xE000: res += '\\seq{%d}' % params[0]
        elif b == 0xE018: res += '\\time{%d,%d,%d}' % (params[0], params[1], params[2])
        elif b == 0xF8F4: res += '\\scale{%d}' % params[0]
        elif b == 0xF8F7: res += '\\font{%d}' % params[0]
        elif b == 0xF8F8: res += '\\ljust'
        elif b == 0xF8F9: res += '\\rjust'
        elif b == 0xF8FA: res += '\\cjust'
        elif b == 0xF8FB: res += '\\fjust'
        elif b == 0xF8FF: res += '\\color{%04X%04X,%04X%04X}' % (params[0], params[1], params[2], params[3])
        else:
            #res += chr(b)
            #for p in params: res += chr(p)
            res += '\\u%04X' % b
            for p in params: res += '\\u%04X' % p
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


class App:
    def __init__(self):
        pass

    def help(self):
        """Display help text."""
        methods = [
            (func, getattr(self, func))
            for func in dir(self)
            if callable(getattr(self, func))
            and not func.startswith('_')
        ]
        for name, method in sorted(methods, key = lambda it: it[0]):
            doc = method.__doc__.split('\n')
            printf("%s: %s\n", name, doc[0])
            for line in doc[1:]:
                line = line.strip()
                if line != '': print('    '+line)

    def extract(self, inPath:str, outPath:str):
        """Extract GameText bin file to directory."""
        file = GameTextBin(inPath)
        root = ET.Element('gametext')

        eChars = ET.SubElement(root, 'chars')
        for char in file.chars:
            eChar = ET.SubElement(eChars, 'char', {
                'character': chr(char.character),
                #'character': str(char.character),
                'xpos':      str(char.xpos),
                'ypos':      str(char.ypos),
                'left':      str(char.left),
                'right':     str(char.right),
                'top':       str(char.top),
                'bottom':    str(char.bottom),
                'width':     str(char.width),
                'height':    str(char.height),
                'fontNo':    str(char.fontNo),
                'textureNo': str(char.textureNo),
            })

        eTexts = ET.SubElement(root, 'texts')
        for text in file.texts:
            eText = ET.SubElement(eTexts, 'text', {
                'identifier': str(text.identifier),
                'numPhrases': str(text.numPhrases),
                'window':     str(text.window),
                'alignH':     str(text.alignH),
                'alignV':     str(text.alignV),
                'language':   str(text.language),
            })
            for phrase in text.phrases:
                ePhrase = ET.SubElement(eText, 'phrase')
                ePhrase.text = phrase

        ET.ElementTree(root).write(os.path.join(outPath, 'gametext.xml'),
            encoding='utf-8', xml_declaration=True)

        for b in file.unkData:
            if b != 0xEE:
                with open(os.path.join(outPath, 'unknown.bin'), 'wb') as outFile:
                    outFile.write(file.unkData)
                break

        for i, tex in enumerate(file.textures):
            tex.image.save(os.path.join(outPath, '%d.png' % i))


def main(*args):
    app = App()
    if len(args) < 1:
        app.help()
        return 0

    while len(args) > 0:
        method = args[0]
        if method.startswith('--'): method = method[2:]

        func = getattr(app, method, None)
        if func is None or not callable(func):
            print("Unknown operation:",method)
            return 1

        sig   = inspect.signature(func)
        nArg  = len(sig.parameters)
        fArgs = args[1:nArg+1]
        args  = args[nArg+1:]

        if len(fArgs) < nArg:
            msg = [method]
            for name in sig.parameters:
                msg.append(name)
            print("Usage:", ' '.join(msg))
            return 1

        func(*fArgs)
    return 0

if __name__ == '__main__':
    sys.exit(main(*sys.argv[1:]))
