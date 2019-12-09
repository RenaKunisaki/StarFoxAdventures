#!/usr/bin/env python3
# read gametext files and produce C header for Ghidra
import os
import os.path
import re
import sys
import json
import struct

texts = {}

def readStrs(file):
    numStrs = struct.unpack('>I', file.read(4))[0] # grumble
    offsets = []
    for i in range(numStrs):
        offsets.append(struct.unpack('>I', file.read(4))[0]) # grumble

    strs = []
    base = file.tell()
    for i in range(numStrs):
        file.seek(base + offsets[i])
        s = []
        while True:
            b = file.read(1)[0]
            if b == 0xEF: # control code
                c = file.read(1)[0]
                if c == 0xA3:
                    c = file.read(1)[0]
                    if c == 0xB4:
                        # set styles or something
                        # used for area names, "no controller" msg
                        c = file.read(2)
                        #b = "<STYLE %02X%02X>" % (c[0], c[1])
                        b = ''
                    elif c == 0xB7:
                        # switch font texture
                        # used to display buttons and faces
                        # 2: buttons, 4: default, 5: faces
                        c = file.read(2)
                        #b = "<FONT %02X%02X>" % (c[0], c[1])
                        b = ''
                    else:
                        c = file.read(2)
                        #b = '<UNK %02X%02X>' % (c[0], c[1])
                        b = ''
                #else: b = '\\xEF\\x%02X' % c
                else: b = ''
            elif b == 0: break

            if type(b) is not str:
                if b > 127 or b < 32: b = '' #b = "\\x%02X" % b
                else: b = chr(b)
            s.append(b)
        strs.append(''.join(s))
    return strs


def readFile(path):
    global texts
    with open(path, 'rb') as file:
        numCharStructs = struct.unpack('>I', file.read(4))[0] # grumble
        file.seek(4 + (numCharStructs * 16)) # skip those

        numTexts, unknown = struct.unpack('>HH', file.read(4))
        for i in range(numTexts):
            id, nPhrases, window, alignH, alignV, lang, offset = struct.unpack(
                '>HHBBBBI', file.read(12))
            if id not in texts:
                texts[id] = {
                    'nPhrases': nPhrases,
                    'window':   window,
                    'alignH':   alignH,
                    'alignV':   alignV,
                    'language': lang,
                    'phrases':  offset,
                }

        strs = readStrs(file)
        for id, text in texts.items():
            #print("%04X W=%02X H=%02X V=%02X L=%02X P=%04X" % (
            #    id, text['window'], text['alignH'], text['alignV'],
            #    text['language'], text['nPhrases']
            #))
            if type(text['phrases']) is not int: continue
            phrases = []
            for i in range(text['nPhrases']):
                #print("", '"' + strs[i+text['phrases']] + '"')
                phrases.append(strs[i+text['phrases']])
            text['phrases'] = phrases


def listDir(path, _depth=0):
    assert _depth < 5
    for name in os.listdir(path):
        if name != "." and name != "..":
            fullPath = os.path.join(path, name)
            if os.path.isdir(fullPath):
                if _depth < 1:
                    listDir(fullPath, _depth+1)
            elif os.path.isfile(fullPath):
                if name.lower() == 'english.bin':
                    readFile(fullPath)

listDir('../discroot/gametext')

rx = re.compile(r'[^0-9a-zA-Z_]+')
with open('gametext.h', 'wt') as file:
    file.write("enum GameTextId {\n")
    for id, text in texts.items():
        name = '_'.join(text['phrases'])
        name = name.replace("'", '')
        name = name.replace('"', '')
        name = rx.sub('_', name)[:32]
        # Ghidra won't accept enum starting with a number,
        # even though it will if manually entered...
        file.write('T%04X_%s = 0x%04X,\n' % (id, name, id))
    file.write("};\n")
