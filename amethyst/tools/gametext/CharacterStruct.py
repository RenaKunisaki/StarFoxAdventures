from __future__ import annotations
from .util import printf, readStruct, writeStruct
import struct
import io
import xml.etree.ElementTree as ET

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

    def toXml(self) -> ET.Element:
        return ET.Element('char', {
            'character': chr(self.character),
            'xpos':      str(self.xpos),
            'ypos':      str(self.ypos),
            'left':      str(self.left),
            'right':     str(self.right),
            'top':       str(self.top),
            'bottom':    str(self.bottom),
            'width':     str(self.width),
            'height':    str(self.height),
            'fontNo':    str(self.fontNo),
            'textureNo': str(self.textureNo),
        })
