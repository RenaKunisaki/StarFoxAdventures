#!/usr/bin/env python3
"""Parse/build gametext files."""
# this line allows methods in a class to be annotated with
# a return type of that class.
from __future__ import annotations
import os
import io
import sys
import enum
import struct
import inspect
import xml.etree.ElementTree as ET
from PIL import Image
import hashlib
sys.path.append(".") # ugh
from texconv.sfatexture import SfaTexture, TexFmt, ImageFormat
from texconv.texture import BITS_PER_PIXEL, convert_color_to_rgb5a3

def printf(fmt, *args):
    print(fmt % args, end='')

def readStruct(file, fmt:str, offset:int=None):
    """Read a struct from a file."""
    if offset is not None: file.seek(offset, 0)
    size = struct.calcsize(fmt)
    res  = struct.unpack(fmt, file.read(size))
    if len(res) == 1: res = res[0] # grumble
    return res

def writeStruct(file, fmt:str, *items, offset:int=None) -> None:
    """Write a struct to a file."""
    if offset is not None: file.seek(offset, 0)
    file.write(struct.pack(fmt, *items))

class GameString:
    """A string from the gametext files."""

    class ControlCode(enum.IntEnum):
        SeqId      = 0xE000 # sequence ID (param: ID) (no idea what this is actually used for)
        SeqTime    = 0xE018 # sequence timing (params: time, ?, ?)
        Scale      = 0xF8F4 # set scale (param: scale; 256 = 100%)
        Font       = 0xF8F7 # select font (param: font ID)
        JustLeft   = 0xF8F8 # left justify
        JustRight  = 0xF8F9 # right justify
        JustCenter = 0xF8FA # center justify
        JustFull   = 0xF8FB # full justify
        Color      = 0xF8FF # set color (params: text rg, text ba, shadow rg, shadow ba)

    ControlCodeChars = {
        'seq':   ControlCode.SeqId,
        'time':  ControlCode.SeqTime,
        'scale': ControlCode.SeqTime,
        'font':  ControlCode.Font,
        'ljust': ControlCode.JustLeft,
        'rjust': ControlCode.JustRight,
        'cjust': ControlCode.JustCenter,
        'fjust': ControlCode.JustFull,
        'color': ControlCode.Color,
    }

    controlCharLength = { # Unicode code point => number of u16 params to follow
        # some of these are unused? at least, I don't see any code handling them.
        # these are all Private Use characters (E000-F8FF).
        # the order is the same as in the game code.
        0xF8F2:                 2,
        0xF8F3:                 0,
        ControlCode.Scale:      1,
        0xF8F5:                 1,
        0xF8F6:                 1,
        ControlCode.Font:       1,
        ControlCode.JustLeft:   0,
        ControlCode.JustRight:  0,
        ControlCode.JustCenter: 0,
        ControlCode.JustFull:   0,
        0xF8FC:                 0,
        0xF8FD:                 0,
        0xF8FE:                 0,
        ControlCode.Color:      4,
        ControlCode.SeqId:      1,
        ControlCode.SeqTime:    3,
        0xE020:                 1,
    }

    escapeChars = {
        '\\': '\\',
        't':  '\t',
        'n':  '\n',
        'r':  '\r',
        '{':  '{',
    }

    def __init__(self, value:str = None):
        self.str = value

    def __str__(self):
        if self.str is None: return ""
        return self.str

    def __repr__(self):
        if self.str is None: return 'GameString()'
        return 'GameString("%s")' % self.str

    @staticmethod
    def read(file:io.IOBase, offset:int=None, maxLen:int=50000) -> GameString:
        """Read string from binary file.

        file: File to read from.
        offset: Offset to seek to before reading.

        Reads a UTF-8 string, including the game's control codes which can
        include embedded nulls. (These are converted to a markup format I've made up.)
        Carriage return, line feed, tab, and unrecognized control codes
        are converted to escapes.
        """
        if offset is not None: file.seek(offset, 0)
        res = ''
        C = GameString.ControlCode

        while len(res) < maxLen:
            b = file.read(1)[0]
            if b == 0: break
            if b >= 0x80: # UTF-8 more bytes
                if   b >= 0xF0: n, b = 3, b & 0x07
                elif b >= 0xE0: n, b = 2, b & 0x0F
                else: n, b = 1, 0x1F
                for _ in range(n):
                    b = (b << 6) | (file.read(1)[0] & 0x3F)

            params = []
            try:
                code = GameString.ControlCode(b)
                if code in GameString.controlCharLength:
                    cl = GameString.controlCharLength[code]
                    if cl == 1: params.append(readStruct(file, '>H'))
                    else: params += readStruct(file, '>%dH' % cl)
            except ValueError: code = 0

            if   b == 0x0009:       res += '\\t'
            elif b == 0x000A:       res += '\\n'
            elif b == 0x000D:       res += '\\r'
            elif b == 0x005C:       res += '\\\\'
            elif b == 0x007B:       res += '\\{'
            elif b == C.SeqId:      res += '{seq %d}' % params[0]
            elif b == C.SeqTime:    res += '{time %d %d %d}' % (params[0], params[1], params[2])
            elif b == C.Scale:      res += '{scale %d}' % params[0]
            elif b == C.Font:       res += '{font %d}' % params[0]
            elif b == C.JustLeft:   res += '{ljust}'
            elif b == C.JustRight:  res += '{rjust}'
            elif b == C.JustCenter: res += '{cjust}'
            elif b == C.JustFull:   res += '{fjust}'
            elif b == C.Color:
                c = (params[0], params[1], params[2], params[3])
                if c[0] > 0xFF or c[1] > 0xFF or c[2] > 0xFF or c[3] > 0xFF:
                    print("Color:", c)
                res += '{color %02X%02X%02X%02X}' % c
            elif (b >= 0xE000 and b <= 0xF8FF) or b < 0x20:
                res += '\\u%04X' % b
                for p in params: res += '\\u%04X' % p
            else: res += chr(b)
        return GameString(res)

    def _parseControlCode(self, string:str) -> bytes:
        C = GameString.ControlCode
        res = b''
        end = string.find('}')
        params = string[0:end].split(' ')
        cmd = params.pop(0)
        try: cmdId = GameString.ControlCodeChars[cmd]
        except KeyError: raise ValueError("Unrecognized control code: "+cmd)

        paramData = []
        if cmd == 'color': # hex params
            # color is 4 params (r, g, b, a) with high byte ignored.
            # XXX but in some case it's also rrgg bbaa (text) rrgg bbaa (shadow)?
            # is that actually used somewhere?
            paramData.append(int(params[0][0:2], 16))
            paramData.append(int(params[0][2:4], 16))
            paramData.append(int(params[0][4:6], 16))
            paramData.append(int(params[0][6:8], 16))
        else:
            for p in params:
                paramData.append(int(p, 0))
        res += bytes(chr(cmdId), 'utf-8') + struct.pack('>%dH' % len(paramData), *paramData)
        return res, end

    def _parseEscape(self, string:str) -> (str,int):
        # expects string to be the substring following the backslash
        c = string[0]
        if c in GameString.escapeChars: return GameString.escapeChars[c], 1
        elif c == 'x': return chr(int(string[1:3], 16)), 3
        elif c == 'u': return chr(int(string[1:5], 16)), 5
        else: return '\\' + c, 1

    def toBytes(self) -> bytes:
        """Encode to binary string."""
        res, i = b'', 0
        while i < len(self.str):
            c = self.str[i]
            if c == '\\':
                s, l = self._parseEscape(self.str[i+1:])
                res += bytes(s, 'utf-8')
                i += l
            elif c == '{': # control code
                i += 1
                rs, ln = self._parseControlCode(self.str[i:])
                res, i = res+rs, i+ln
            else: res += bytes(c, 'utf-8')
            i += 1
        return res

    def getUsedChars(self) -> set:
        """Return set of characters used in this string.

        The entries are (font, character).
        """
        font = 0
        res, i = set(), 0
        while i < len(self.str):
            c = self.str[i]
            if c == '\\':
                s, l = self._parseEscape(self.str[i+1:])
                for ch in s: res.add((font, ch))
                i += l
            elif c == '{': # control code
                i += 1
                s = self.str[i:]
                end = s.find('}')
                code = s[0:end].split(' ')
                if code[0] == 'font': font = int(code[1], 0)
                i += end+1
            else: res.add((font,c))
            i += 1
        return res


class CharacterStruct:
    """Identifies the location of one character in the font textures."""
    def __init__(self,
    character :(int,str) = None, #unicode number
    xpos      :int = None, #coordinate of leftmost texel in font bitmap
    ypos      :int = None, #coordinate of topmost texel in font bitmap
    left      :int = None, #spacing
    right     :int = None, #spacing
    top       :int = None, #spacing
    bottom    :int = None, #spacing
    width     :int = None, #size
    height    :int = None, #size
    fontNo    :int = None, #Index into GameTextFonts[] in GameTextData.h
    textureNo :int = None): #Index into Textures[] in the binary file
        if type(character) is str: character = ord(character[0])
        self.character = character
        self.xpos      = xpos
        self.ypos      = ypos
        self.left      = left
        self.right     = right
        self.top       = top
        self.bottom    = bottom
        self.width     = width
        self.height    = height
        self.fontNo    = fontNo
        self.textureNo = textureNo

    @staticmethod
    def fromFile(file:io.FileIO) -> CharacterStruct:
        self = CharacterStruct()
        self.character, self.xpos, self.ypos, self.left, self.right, \
        self.top, self.bottom, self.width, self.height, self.fontNo, \
        self.textureNo = readStruct(file, '>IHHbbbbBBBB')
        return self

    def toBytes(self) -> bytes:
        return struct.pack('>IHHbbbbBBBB', self.character, self.xpos, self.ypos,
            self.left, self.right, self.top, self.bottom, self.width,
            self.height, self.fontNo, self.textureNo)

class GameTextStruct:
    """Defines a dialogue message."""
    def __init__(self,
    identifier :int = None, #which game text is this
    numPhrases :int = None, #how many phrases this text has
    window     :int = None, #preferred window
    alignH     :int = None, #horizontal alignment
    alignV     :int = None, #vertical alignment
    language   :int = None, #LANGUAGE_xxx from GameTextData.h
    phrases    :int = None): #pointer to <numPhrases> pointers to localised strings
        self.identifier = identifier
        self.numPhrases = numPhrases
        self.window     = window
        self.alignH     = alignH
        self.alignV     = alignV
        self.language   = language
        self.phrases    = phrases

    @staticmethod
    def fromFile(file:io.FileIO) -> GameTextStruct:
        self = GameTextStruct()
        self.identifier, self.numPhrases, self.window, self.alignH, self.alignV, \
        self.language, self.phrases = readStruct(file, '>HHbbbbI')
        return self

    def toBytes(self, strIdx) -> bytes:
        self.numPhrases = len(self.phrases)
        return struct.pack('>HHbbbbI', self.identifier, self.numPhrases,
            self.window, self.alignH, self.alignV, self.language, strIdx)

class FontTexture(SfaTexture):
    """Defines a font texture image."""

    def __init__(self, file:io.FileIO=None):
        super().__init__()
        if file: self.readFile(file)

    def readFile(self, file:io.FileIO) -> None:
        """Read from binary file."""
        self.format, self.bitsPerPx, self.width, self.height = readStruct(file, '>4H')
        if self.width == 0 and self.height == 0: raise StopIteration # end of texture list

        self.realTexFmt = self.format
        if self.format == 2: self.format = 0 #game does this for some reason
        elif self.format == 1: self.format = 5
        self.numMipMaps = 0
        #print('TexFmt=0x%X PixFmt=0x%X size=%dx%d' % (
        #    self.format, self.bitsPerPx, self.width, self.height))
        self._readData(file, self.format)

    # XXX do we need this? it's basically the same as the SfaTexture method.
    # main difference is default format.
    @staticmethod
    def readImage(img:Image, fmt:ImageFormat=ImageFormat.RGB5A3,
    numMipMaps:int=1) -> FontTexture:
        """Instantiate texture from image."""
        self = FontTexture()
        self.width, self.height = img.size
        self.format = ImageFormat(fmt)
        self.bitsPerPx = BITS_PER_PIXEL.get(fmt, 0)
        self.numMipMaps = numMipMaps
        self.image = img
        return self

    def writeFile(self, file:io.FileIO, format:TexFmt=TexFmt.RGB5A3) -> None:
        """Write to binary file."""
        self.format = format
        #print("write tex fmt", self.format, hex(self.bitsPerPx),
        #    "size", self.width, self.height, "at", hex(file.tell()))
        writeStruct(file, '>4H', self.format.value, self.bitsPerPx, self.width, self.height)
        self._writeData(file)

class GameTextReader:
    """Represents the contents of a .bin file under gametext directory."""
    def __init__(self, file:(str,io.FileIO)=None):
        self.chars    = [] # CharacterStruct
        self.texts    = [] # GameTextStruct
        self.strings  = {} # offset:int => string:str
        self.textures = [] # FontTexture
        self.unkData  = b'' # bytes
        if file is not None: self.read(file)

    def _readCharStructs(self, file:io.FileIO) -> None:
        """Read CharacterStructs from file."""
        self.numCharStructs = readStruct(file, '>I')
        #print("numCharStructs =", self.numCharStructs, "at", hex(file.tell()-4))
        for _ in range(self.numCharStructs):
            self.chars.append(CharacterStruct.fromFile(file))

    def _readTexts(self, file:io.FileIO) -> None:
        """Read GameTextStructs from file."""
        self.numTexts, self.strDataLen = readStruct(file, '>2H')
        #print("numTexts=", self.numTexts, "strDataLen=", hex(self.strDataLen),
        #    "at", hex(file.tell()-4))
        for _ in range(self.numTexts):
            self.texts.append(GameTextStruct.fromFile(file))

    def _readStringOffsets(self, file:io.FileIO) -> None:
        """Read string offset table from file."""
        self.strOffsets = []
        self.numStrings = readStruct(file, '>I')
        #print("numStrings =", self.numStrings, "at", hex(file.tell()-4))
        for _ in range(self.numStrings):
            self.strOffsets.append(readStruct(file, '>I'))

    def _readStringData(self, file:io.FileIO) -> None:
        """Read string data from file."""
        #print("String data offset=", hex(file.tell()))
        strOffs = file.tell()
        for i in range(self.numStrings):
            startOffs = strOffs + self.strOffsets[i]
            sOffs = startOffs - strOffs
            self.strings[sOffs] = GameString.read(file, startOffs)
        file.seek(strOffs + self.strDataLen, 0) # skip padding, etc

    def _readUnkData(self, file:io.FileIO) -> None:
        """Read unknown data from file."""
        self.unkDataLen = readStruct(file, '>I')
        offset = file.tell()
        #print('unkDataLen=0x%X, offset=0x%X' % (self.unkDataLen, offset))
        self.unkData = file.read(self.unkDataLen)
        for i, b in enumerate(self.unkData):
            if b != 0xEE:
                print("non-EE byte in unkdata at 0x%X" % (i + offset))
                break

    def _readTextures(self, file:io.FileIO) -> None:
        """Read texture images from file."""
        while len(self.textures) < 32: # failsafe
            try:
                tex = FontTexture(file)
                self.textures.append(tex)
                #print("Texture %d at 0x%X: size %dx%d fmt 0x%X 0x%X" % (
                #    len(self.textures)-1, file.tell(), tex.width, tex.height,
                #    tex.format, tex.bitsPerPx))
            except StopIteration:
                break
        #print("EOF at", hex(file.tell()))

    def _setPhraseData(self) -> None:
        """Replace `self.phrases` with the array of strings it points to."""
        for text in self.texts:
            idx = text.phrases
            text.phrases = []
            for i in range(text.numPhrases):
                offs = self.strOffsets[idx+i]
                text.phrases.append(self.strings[offs])

    def read(self, file:(str,io.FileIO)) -> None:
        """Read from file."""
        if type(file) is str: file = open(file, 'rb')
        self._readCharStructs(file)
        self._readTexts(file)
        self._readStringOffsets(file)
        self._readStringData(file)
        self._readUnkData(file)
        self._readTextures(file)
        self._setPhraseData()


class GameTextWriter:
    """Creates a .bin file under gametext directory."""
    def __init__(self):
        self.chars      = [] # CharacterStruct
        self.texts      = [] # GameTextStruct
        self.strings    = {} # offset:int => string:str
        self.textures   = [] # FontTexture
        self.unkDataLen = 0

    def addFont(self, font:(Image,io.FileIO,str)) -> None:
        """Add a font texture."""
        if type(font) is not Image:
            font = Image.open(font)
        font = FontTexture.readImage(font)
        self.textures.append(font)

    def write(self, file:(str,io.FileIO)) -> None:
        """Create new file."""
        if type(file) is str: file = open(file, 'wb')

        # file structure:
        # u32 numCharStructs; characterStruct[numCharStructs];
        # u16 numTexts, strDataLen; gametextStruct[numTexts] texts;
        # u32 numStrings; u32 strOffset[numStrings]; char[] strings[strDataLen];
        # u32 unknownLen; u8 unknownData[unknownLen];
        # FontTexture fontTextures[];

        # iterate all texts; build set of needed characters, string table,
        # and string offset table.
        usedChars  = set() # (font,character)
        strings    = []
        strOffsets = [] # strIdx => offset relative to start of str table
        nextOffs   = 0
        for text in self.texts:
            for phrase in text.phrases:
                usedChars = usedChars.union(phrase.getUsedChars())
                string = phrase.toBytes()
                strOffsets.append(nextOffs)
                data = string + b'\0'
                nextOffs += len(data)
                strings.append(data)

        strDataLen = nextOffs

        # pad (XXX how much is needed?)
        padding = b''
        pad = strDataLen & 3
        if pad: padding += (b'\0' * (4 - pad))
        strDataLen += len(padding)

        # character structs define the position of each character in the font texture.
        # XXX generate these from the usedChars set.
        #print("write numChars =", len(self.chars), "at", hex(file.tell()))
        writeStruct(file, '>I', len(self.chars))
        for char in self.chars:
            file.write(char.toBytes())

        # write text structs
        #print("write numTexts =", len(self.texts), "strDataLen =", hex(strDataLen),
        #    "at", hex(file.tell()))
        writeStruct(file, '>HH', len(self.texts), strDataLen)
        strIdx = 0
        for text in self.texts:
            data = text.toBytes(strIdx)
            file.write(data)
            strIdx += len(text.phrases)

        # write string offsets
        #print("write numStrs =", len(strOffsets), "at", hex(file.tell()))
        writeStruct(file, '>I', len(strOffsets))
        for offs in strOffsets:
            writeStruct(file, '>I', offs)

        # write strings
        for s in strings: file.write(s)
        file.write(padding)

        # write unknown data
        #self.unkDataLen = 0 # the game doesn't seem to actually need this...
        #print("write unkLen =", self.unkDataLen, "at", hex(file.tell()))
        writeStruct(file, '>I', self.unkDataLen)
        file.write(b'\xEE' * self.unkDataLen)

        # write texture graphics
        for tex in self.textures:
            tex.writeFile(file)

        # write end-of-file marker
        file.write(b'\0' * 8)


class GameTextXml:
    """Helper class for reading/writing XML files for GameText."""

    def _charFromXml(self, eChar:ET.Element) -> CharacterStruct:
        """Create CharacterStruct from XML element."""
        return CharacterStruct(
            character = ord(eChar.attrib.get('character')),
            xpos      = int(eChar.attrib.get('xpos'),      0),
            ypos      = int(eChar.attrib.get('ypos'),      0),
            left      = int(eChar.attrib.get('left'),      0),
            right     = int(eChar.attrib.get('right'),     0),
            top       = int(eChar.attrib.get('top'),       0),
            bottom    = int(eChar.attrib.get('bottom'),    0),
            width     = int(eChar.attrib.get('width'),     0),
            height    = int(eChar.attrib.get('height'),    0),
            fontNo    = int(eChar.attrib.get('fontNo'),    0),
            textureNo = int(eChar.attrib.get('textureNo'), 0),
        )

    def _textFromXml(self, eText:ET.Element) -> GameTextStruct:
        """Create GameTextStruct from XML element."""
        text = GameTextStruct(
            identifier = int(eText.attrib.get('identifier'), 0),
            window     = int(eText.attrib.get('window'),     0),
            alignH     = int(eText.attrib.get('alignH'),     0),
            alignV     = int(eText.attrib.get('alignV'),     0),
            language   = int(eText.attrib.get('language'),   0),
            phrases    = [],
        )
        for ePhrase in eText.iter('phrase'):
            phrase = ePhrase.text
            if phrase is None: phrase = ""
            text.phrases.append(GameString(phrase))
        return text

    def _writeXmlChars(self, file:GameTextReader, root:ET.Element) -> None:
        """Create 'char' elements in XML for CharacterStructs."""
        eChars = ET.SubElement(root, 'chars')
        for char in file.chars:
            eChar = ET.SubElement(eChars, 'char', {
                'character': chr(char.character),
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

    def _writeXmlTexts(self, file:GameTextReader, root:ET.Element) -> None:
        """Create 'text' elements in XML for GameTextStructs."""
        eTexts = ET.SubElement(root, 'texts')
        for text in file.texts:
            eText = ET.SubElement(eTexts, 'text', {
                'identifier': str(text.identifier),
                'window':     str(text.window),
                'alignH':     str(text.alignH),
                'alignV':     str(text.alignV),
                'language':   str(text.language),
            })
            for phrase in text.phrases:
                ePhrase = ET.SubElement(eText, 'phrase')
                ePhrase.text = str(phrase)

    def read(self, inPath:str, file:GameTextWriter) -> None:
        """Read definitions from gametext.xml file."""
        root = ET.parse(os.path.join(inPath, 'gametext.xml')).getroot()
        if root.tag != 'gametext': raise TypeError("Incorrect XML file")
        file.unkDataLen = int(root.attrib['unklen'], 0)

        # read character structs (XXX build from text)
        eChars = root.find('chars')
        for eChar in eChars.iter('char'):
            file.chars.append(self._charFromXml(eChar))

        # read texts
        eTexts = root.find('texts')
        for eText in eTexts.iter('text'):
            file.texts.append(self._textFromXml(eText))

    def write(self, file:GameTextReader) -> ET.ElementTree:
        """Create elements for gametext.xml file."""
        root = ET.Element('gametext', {
            'unklen': str(len(file.unkData)),
        })

        # write XML
        self._writeXmlChars(file, root)
        self._writeXmlTexts(file, root)
        return ET.ElementTree(root)

class App:
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
        #print("Extract", inPath)
        file = GameTextReader(inPath)
        xml = GameTextXml().write(file)
        xml.write(os.path.join(outPath, 'gametext.xml'),
            encoding='utf-8', xml_declaration=True)

        # this is always all 0xEE
        #for b in file.unkData:
        #    if b != 0xEE:
        #        with open(os.path.join(outPath, 'unknown.bin'), 'wb') as outFile:
        #            outFile.write(file.unkData)
        #        break

        # write texture images
        for i, tex in enumerate(file.textures):
            tex.image.save(os.path.join(outPath, '%d.png' % i))

    def build(self, inPath:str, outPath:str):
        """Create GameTexr bin file from directory."""
        file = GameTextWriter()
        i = 0
        while True:
            try:
                file.addFont(os.path.join(inPath, '%d.png' % i))
                #print("added font", i)
                i += 1
            except FileNotFoundError: break
        GameTextXml().read(inPath, file)
        file.write(outPath)

    def extractAllChars(self, inPath:str, outPath:str):
        """Extract all font character graphics."""
        xml = ET.Element('chars')
        charHashes = {} # char => [hash, hash...]
        nChars = 0 # for reporting status
        def _extractDir(path, out, _depth=0):
            nonlocal nChars
            assert _depth < 10, 'Maximum depth exceeded'
            for name in os.listdir(path):
                p = os.path.join(path, name)
                if os.path.isdir(p): _extractDir(p, out, _depth+1)
                elif name.endswith('.bin'):
                    pShort = '/'.join((p.split('/')[-2:]))
                    print("\rChars: %6d File: %s   " % (nChars, pShort), end='')
                    try:
                        file = GameTextReader(p)
                        for char in file.chars:
                            c = char.character
                            if c not in charHashes: charHashes[c] = []

                            # extract this character graphic
                            img = file.textures[char.textureNo].image
                            x1, y1 = char.xpos, char.ypos
                            x2, y2 = x1+char.width+1, y1+char.height+1
                            #print(c, x1, y1, x2, y2)
                            cImg = img.crop((x1, y1, x2, y2))

                            # hash it
                            md5 = hashlib.md5()
                            md5.update(cImg.tobytes())
                            hash = md5.digest()

                            if hash not in charHashes[c]: # not seen this one before
                                # so extract it
                                name = chr(c)
                                if name in ' !#$%&*()/\\?:\'"`~<>.':
                                    name = '%04X' % c
                                name = os.path.join(str(char.fontNo), '%s.png' % name)
                                os.makedirs(os.path.join(out, str(char.fontNo)), exist_ok=True)
                                dest = os.path.join(out, name)
                                idx = 0
                                while os.path.exists(dest):
                                    idx += 1
                                    name = os.path.join(str(char.fontNo), '%s.%d.png' % (idx, name))
                                    dest = os.path.join(out, name)
                                #print("save:", name)
                                cImg.save(dest)
                                charHashes[c].append(hash)

                                # and add it to the XML
                                ET.SubElement(xml, 'char', {
                                    'character': chr(c),
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

                                nChars += 1
                    except Exception as ex:
                        print("Error extracting", p)
                        raise

        _extractDir(inPath, outPath)
        ET.ElementTree(xml).write(os.path.join(outPath, 'chars.xml'),
            encoding='utf-8', xml_declaration=True)


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
