from __future__ import annotations
import io
import struct
from .util import printf, readStruct, writeStruct

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
        self.language, self.phrases = readStruct(file, '>HHBbbbI')
        return self

    def toBytes(self, strIdx) -> bytes:
        self.numPhrases = len(self.phrases)
        return struct.pack('>HHBbbbI', self.identifier, self.numPhrases,
            self.window, self.alignH, self.alignV, self.language, strIdx)
