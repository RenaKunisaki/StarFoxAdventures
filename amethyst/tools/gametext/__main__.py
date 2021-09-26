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
import string

if not __package__:
    from pathlib import Path
    DIR = Path(__file__).resolve().parent
    sys.path.insert(0, str(DIR.parent))
    __package__ = DIR.name

from .CharacterStruct import CharacterStruct
from .FontTexture import FontTexture
from .FontTextureBuilder import FontTextureBuilder, FontEnum, ICON_FONTS
from .GameString import GameString
from .GameTextReader import GameTextReader
from .GameTextStruct import GameTextStruct
from .GameTextWriter import GameTextWriter
from .GameTextXml import GameTextXml
from .LangEnum import LangEnum
from .util import printf, readStruct, writeStruct, makeCharFilename

CLEAR_LINE = '\r\x1B[2K' # ANSI control code

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

    def extractAll(self, inPath:str, outPath:str):
        """Recursively extract GameText bin files to directory."""
        # this produces an XML file which has too much text to convert
        # back into a bin file... max is 64K of text, file is about twice that
        # for English, probably similar for others.
        nTexts, nChars = 0, 0 # for reporting progress

        languages = {}
        for l in LangEnum:
            languages[l] = {
                'chars': {}, # (fontId, character) => {
                    # 'char': CharacterStruct,
                    # 'img': Image,
                #}
                'texts': {}, # identifier => GameTextStruct
            }

        def _extractDir(path, _depth=0):
            nonlocal languages, nTexts, nChars
            for name in os.listdir(path):
                p = os.path.join(path, name)
                if os.path.isdir(p): _extractDir(p, _depth+1)
                elif name.endswith('.bin'):
                    try:
                        lang = name.split('.')[0]
                        if '_' in lang: lang = lang.split('_')[1]
                        langId = LangEnum[lang]

                        # print status
                        pShort = '/'.join((p.split('/')[-2:]))
                        print(CLEAR_LINE + "Chars: %6d Texts: %6d File: %s   " % (
                            nChars, nTexts, pShort), end='')

                        # read file
                        file = GameTextReader(p)
                        nChars += len(file.chars)
                        nTexts += len(file.texts)

                        # grab texts
                        texts = languages[langId]['texts']
                        for t in file.texts:
                            texts[t.identifier] = t

                        # grab icon characters
                        chars = languages[langId]['chars']
                        for c in file.chars:
                            chars[(c.fontNo, c.character)] = {
                                'char': c,
                                'img': file.getChrImg(c),
                            }
                    except:
                        print("Error extracting", p)
                        raise

        _extractDir(inPath)

        # output results to an XML file and image files
        for lang, it in languages.items():
            lPath = os.path.join(outPath, lang.name)
            os.makedirs(lPath, exist_ok=True)

            xml = GameTextXml()
            for text in sorted(it['texts'].values(), key=lambda t: t.identifier):
                xml.addText(text)

            # write texture images and add char info
            for k, v in it['chars'].items():
                fontId, char = k
                xml.addChar(v['char'])
                if fontId in ICON_FONTS:
                    img = v['img']
                    cPath = os.path.join(lPath, FontEnum(fontId).name)
                    os.makedirs(cPath, exist_ok=True)

                    name = '%s.png' % makeCharFilename(char)
                    img.save(os.path.join(cPath, name))

            xml.build().write(os.path.join(lPath, 'gametext.xml'),
                encoding='utf-8', xml_declaration=True)

    def injectAllChars(self, inPath:str, outPath:str):
        """Modify a GameText bin file, adding all font characters from a set."""
        reader = GameTextReader(inPath)
        writer = GameTextWriter()
        for f in reader.textures: writer.addFont(f)
        for c in reader.chars: writer.addChar(c)
        for t in reader.texts: writer.addText(t)

        # read the character set file
        lang = os.path.basename(inPath).split('.')[0]
        if '_' in lang: lang = lang.split('_')[1]

        # XXX use whatever the method was to reference these by the import system
        # instead of assuming a specific working directory.
        with open('tools/gametext/fonts/Charsets/%s.txt' % lang, 'rt') as chrFile:
            chars = GameString(chrFile.read().strip())

        # add these characters to the file if not already present
        text  = GameTextStruct(0xFFFF, 1, 0, 0, 0, LangEnum[lang], (chars,))
        writer.addText(text)
        writer.write(outPath)

    def injectAllCharsRecursive(self, inPath:str, outPath:str):
        """Modify GameText bin files, adding all font characters from a set."""
        def _recurse(inPath:str, outPath:str, _depth:int=0):
            assert _depth < 10, 'Maximum depth exceeded'
            for name in os.listdir(inPath):
                p = os.path.join(inPath, name)
                if os.path.isdir(p):
                    _recurse(p, os.path.join(outPath, name), _depth+1)
                elif name.endswith('.bin'):
                    pOut = os.path.join(outPath, name)
                    print("Patching", pOut)
                    os.makedirs(outPath, exist_ok=True)
                    self.injectAllChars(p, pOut)
        _recurse(inPath, outPath)

    def build(self, inPath:str, outPath:str):
        """Create GameText bin file from directory."""
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

    def _extractChar(self, char, outDir, lang, cImg):
        c = char.character
        cName = makeCharFilename(c)
        dName = os.path.join(outDir, lang, str(char.fontNo))
        os.makedirs(dName, exist_ok=True)
        fName = os.path.join(dName, cName + '.png')
        idx = 0
        while os.path.exists(fName):
            idx += 1
            fName = os.path.join(dName, '%s.%d.png' % (cName, idx))
        #print("save:", fName, lang, hash.hex())
        cImg.save(fName)

    def extractAllChars(self, inPath:str, outPath:str):
        """Extract all font character graphics."""
        # XXX running this multiple times will create multiple copies of each...
        # also there's a lot of repetition here.
        # do we even still need this?
        xml = {
            FontEnum.Japanese: ET.Element('chars'),
            FontEnum.Buttons:  ET.Element('chars'),
            FontEnum.Flags:    ET.Element('chars'),
            FontEnum.English:  ET.Element('chars'),
            FontEnum.Faces:    ET.Element('chars'),
        }
        charHashes = {} # lang => {font => {char => [hash, hash...]}}
        xmlHashes  = set()
        nChars     = 0 # for reporting status
        usedChars  = {} # (fontNo,langId) => set(char)

        def _extractDir(path, out, _depth=0):
            nonlocal nChars
            assert _depth < 10, 'Maximum depth exceeded'
            for name in os.listdir(path):
                p = os.path.join(path, name)
                if os.path.isdir(p): _extractDir(p, out, _depth+1)
                elif name.endswith('.bin'):
                    lang = name.split('.')[0]
                    if '_' in lang: lang = lang.split('_')[1]

                    pShort = '/'.join((p.split('/')[-2:]))
                    print(CLEAR_LINE + "Chars: %6d File: %s   " % (nChars, pShort), end='')
                    try:
                        file = GameTextReader(p)
                        for char in file.chars:
                            c = char.character
                            fontNo = int(char.fontNo)
                            if lang not in charHashes: charHashes[lang] = {}
                            if fontNo not in charHashes[lang]: charHashes[lang][fontNo] = {}
                            if c not in charHashes[lang][fontNo]: charHashes[lang][fontNo][c] = []

                            cImg = file.getChrImg(char) # extract this character graphic
                            md5 = hashlib.md5() # hash it
                            md5.update(cImg.tobytes())
                            hash = md5.digest()

                            if hash not in charHashes[lang][fontNo][c]: # not seen this one before
                                # so extract it
                                self._extractChar(char, out, lang, cImg)
                                charHashes[lang][fontNo][c].append(hash)

                                # add it to the used char set
                                # we want to merge all Latin characters into one image,
                                # and all icons into their own images.
                                if fontNo in ICON_FONTS: langId = LangEnum.English
                                elif lang == 'Japanese': langId = LangEnum.Japanese
                                else: LangEnum.English
                                bk = (fontNo, langId)
                                if bk not in usedChars: usedChars[bk] = set()
                                usedChars[bk].add(chr(c))

                                # and add it to the XML
                                if hash not in xmlHashes:
                                    xmlHashes.add(hash)
                                    ET.SubElement(xml[fontNo], char.toXml())

                                nChars += 1
                    except:
                        print("Error extracting", p)
                        raise

        # iterate input
        _extractDir(inPath, outPath)

        # write XML
        for font, tree in xml.items():
            ET.ElementTree(tree).write(os.path.join(outPath, '%s.xml' % font.name),
                encoding='utf-8', xml_declaration=True)

        # write master images
        for (fontNo, langId), chars in usedChars.items():
            builder = FontTextureBuilder(outPath, lang=langId)
            for c in sorted(chars):
                builder.add(c, fontNo)
            builder.build().save(os.path.join(outPath, '%s%d.png' % (
                LangEnum(langId).name, int(fontNo))))


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
