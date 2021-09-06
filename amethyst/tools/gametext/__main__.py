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

if not __package__:
    from pathlib import Path
    DIR = Path(__file__).resolve().parent
    sys.path.insert(0, str(DIR.parent))
    __package__ = DIR.name

from .CharacterStruct import CharacterStruct
from .FontTexture import FontTexture
from .FontTextureBuilder import FontTextureBuilder
from .GameString import GameString
from .GameTextReader import GameTextReader
from .GameTextStruct import GameTextStruct
from .GameTextWriter import GameTextWriter
from .GameTextXml import GameTextXml
from .LangEnum import LangEnum
from .util import printf, readStruct, writeStruct, BAD_DIR_CHARS

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
        charHashes = {} # lang => {font => {char => [hash, hash...]}}
        nChars = 0 # for reporting status
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
                    print("\rChars: %6d File: %s   " % (nChars, pShort), end='')
                    try:
                        file = GameTextReader(p)
                        for char in file.chars:
                            c = char.character
                            fontNo = str(char.fontNo)
                            if lang not in charHashes: charHashes[lang] = {}
                            if fontNo not in charHashes[lang]: charHashes[lang][fontNo] = {}
                            if c not in charHashes[lang][fontNo]: charHashes[lang][fontNo][c] = []

                            # extract this character graphic
                            img = file.textures[char.textureNo].image
                            x1, y1 = char.xpos, char.ypos
                            x2, y2 = x1+char.width, y1+char.height
                            if x1 == x2: x2 += 1
                            if y1 == y2: y2 += 1
                            #print(c, x1, y1, x2, y2)
                            cImg = img.crop((x1, y1, x2, y2))

                            # hash it
                            md5 = hashlib.md5()
                            md5.update(cImg.tobytes())
                            hash = md5.digest()

                            if hash not in charHashes[lang][fontNo][c]: # not seen this one before
                                # so extract it
                                cName = chr(c)
                                if cName in BAD_DIR_CHARS:
                                    cName = '%04X' % c
                                dName = os.path.join(out, lang, str(char.fontNo))
                                os.makedirs(dName, exist_ok=True)
                                fName = os.path.join(dName, cName + '.png')
                                idx = 0
                                while os.path.exists(fName):
                                    idx += 1
                                    fName = os.path.join(dName, '%s.%d.png' % (cName, idx))
                                #print("save:", fName, lang, hash.hex())
                                cImg.save(fName)
                                charHashes[lang][fontNo][c].append(hash)

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
                    except Exception:
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
