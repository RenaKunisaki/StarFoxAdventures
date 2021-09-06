from __future__ import annotations
import struct
import enum
import io
from .util import printf, readStruct, writeStruct
from .CharacterStruct import CharacterStruct
from .GameTextStruct import GameTextStruct
from .GameString import GameString
from .FontTexture import FontTexture

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
