from __future__ import annotations
import struct
import enum
import io
import os
from PIL import Image
import xml.etree.ElementTree as ET
from .util import printf, readStruct, writeStruct
from .GameTextReader import GameTextReader
from .GameTextWriter import GameTextWriter
from .CharacterStruct import CharacterStruct
from .GameTextStruct import GameTextStruct
from .GameString import GameString
from .FontTexture import FontTexture

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
