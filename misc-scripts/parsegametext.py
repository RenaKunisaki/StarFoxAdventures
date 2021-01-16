#!/usr/bin/env python3
import sys
import struct
import os
import os.path
import re
import xml.etree.ElementTree as ET

def readCharStructs(file):
    numCharStructs = struct.unpack('>I', file.read(4))[0] # grumble
    charStructs = []
    for i in range(numCharStructs):
        fields = struct.unpack('>IHHbbbbBBBB', file.read(16))
        charStructs.append({
            'character': fields[ 0],
            'xpos':      fields[ 1],
            'ypos':      fields[ 2],
            'left':      fields[ 3],
            'right':     fields[ 4],
            'top':       fields[ 5],
            'bottom':    fields[ 6],
            'width':     fields[ 7],
            'height':    fields[ 8],
            'font':      fields[ 9],
            'texture':   fields[10],
        })
    return charStructs


def readTextList(file):
    numTexts, unknown = struct.unpack('>HH', file.read(4))
    #print("numTexts=", numTexts, "unknown=", unknown)

    texts = {}
    for i in range(numTexts):
        id, nPhrases, window, alignH, alignV, lang, offset = struct.unpack(
            '>HHBBBBI', file.read(12))
        texts[id] = {
            'nPhrases': nPhrases,
            'window':   window,
            'alignH':   alignH,
            'alignV':   alignV,
            'language': lang,
            'phrases':  offset,
        }

    numStrs = struct.unpack('>I', file.read(4))[0] # grumble
    #print("numStrs=", numStrs, "at 0x%X" % (file.tell() - 2))
    offsets = []
    for i in range(numStrs):
        offsets.append(struct.unpack('>I', file.read(4))[0]) # grumble
    p = file.tell()
    file.seek(0, 2)
    offsets.append(file.tell())
    file.seek(p)
    return texts, offsets, numStrs, unknown


def readTexts(file):
    texts, offsets, numStrs, unknown = readTextList(file)

    strs = []
    base = file.tell()
    for i in range(numStrs):
        offs = base + offsets[i]
        offsEnd = base + offsets[i+1]
        file.seek(offs)
        # seems like the seq files have a different format,
        # which includes the text length in bytes just before this offset.
        s = []
        while file.tell() < offsEnd-1:
            b = file.read(1)
            if len(b) == 0: break
            b = b[0]
            if b == 0xEE:
                c = file.read(4)
                if c[0] == 0x80 and c[1] == 0x98:
                    c2 = file.read(4)
                    b = '<EE %02X%02X%02X%02X %02X%02X%02X%02X>' % (
                        c[0], c[1], c[2], c[3], c2[0], c2[1], c2[2], c2[3])
                elif c[0] == 0x80 and c[1] == 0x80:
                    b = '<SEQ %02X%02X>' % (c[2], c[3])
                else: b = '<EE %02X%02X%02X%02X>' % (c[0], c[1], c[2], c[3])
            if b == 0xEF: # control code
                c = file.read(1)[0]
                if c == 0xA3:
                    c = file.read(1)[0]
                    if c == 0xB4:
                        c = file.read(2)
                        c = (c[0] * 256) + c[1]
                        b = "<SCALE 0x%X>" % c
                    elif c == 0xB7:
                        # switch font texture
                        # used to display buttons and faces
                        # 2: buttons, 4: default, 5: faces
                        c = file.read(2)
                        b = "<FONT %02X%02X>" % (c[0], c[1])
                    elif c == 0xB8: b = '<JUSTIFY LEFT>'
                    elif c == 0xB9: b = '<JUSTIFY RIGHT>'
                    elif c == 0xBA: b = '<JUSTIFY CENTER>'
                    elif c == 0xBB: b = '<JUSTIFY FULL>'
                    elif c == 0xBF:
                        c = file.read(8)
                        b = "<COLOR %02X%02X%02X%02X %02X%02X%02X%02X>" % (
                            c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7])
                    else:
                        #c = file.read(2)
                        #b = '<UNK %02X%02X>' % (c[0], c[1])
                        b = '\\x%02X' % c
                else: b = '\\xEF\\x%02X' % c
            elif b == 0: break

            if type(b) is not str:
                if b > 127 or b < 32: b = "\\x%02X" % b
                else: b = chr(b)
            s.append(b)
        strs.append(''.join(s))

    for id, text in texts.items():
        #print("%04X W=%02X H=%02X V=%02X L=%02X P=%04X" % (
        #    id, text['window'], text['alignH'], text['alignV'],
        #    text['language'], text['nPhrases']
        #))
        phrases = []
        for i in range(text['nPhrases']):
            #print("", '"' + strs[i+text['phrases']] + '"')
            phrases.append(strs[i+text['phrases']])
        text['phrases'] = phrases

    return texts, unknown


def readGameTextFile(path):
    with open(path, 'rb') as file:
        charStructs = readCharStructs(file)
        texts, unknown = readTexts(file)
        return texts, unknown, charStructs


def printJson(allTexts):
    print(json.dumps(allTexts))

def printHeader(allTexts):
    print("typedef enum {")
    for id, text in allTexts.items():
        s = ' '.join(text['phrases'])
        s = re.sub(r'\<[^\>]*\>', '', s)
        s = re.sub(r'[^0-9A-Za-z]+', '_', s)
        s = re.sub(r'__+', '_', s)
        print("T%04X_%s = 0x%X," % (id, s, id))
    print("} GameTextId32;")

def printXml(allTexts):
    eTexts = ET.Element('gametext')
    for id, text in allTexts.items():
        eText = ET.SubElement(eTexts, 'text', {
            'id':       '0x%04X' % id,
            #'nPhrases': str(text['nPhrases']),
            'window':   str(text['window']),
            'alignH':   str(text['alignH']),
            'alignV':   str(text['alignV']),
            'language': str(text['language']),
        })
        for dir in text.get('dirs', []):
            ET.SubElement(eText, 'dir', {'name':dir})
        for phrase in text['phrases']:
            ET.SubElement(eText, 'phrase').text = phrase
    print(ET.tostring(eTexts, encoding='unicode'))


def mergeTexts(allTexts, texts, dir):
    # this is ugly and assumes the text/params don't vary between dirs...
    for id, text in texts.items():
        if id not in allTexts:
            if 'dirs' not in text: text['dirs'] = []
            text['dirs'].append(dir)
            allTexts[id] = text
        else:
            if 'dirs' not in allTexts[id]: allTexts[id]['dirs'] = []
            allTexts[id]['dirs'].append(dir)


def readAllTexts(path, lang="English"):
    allTexts = {}
    textDir  = os.path.join(path, 'gametext')
    for dir in os.listdir(textDir):
        try:
            texts, unk, chars = readGameTextFile(
                os.path.join(textDir, dir, lang+'.bin'))
        except FileNotFoundError:
            continue
        mergeTexts(allTexts, texts, dir)

    seqDir = os.path.join(path, 'gametext', 'Sequences')
    for file in os.listdir(seqDir):
        if(file.endswith('_%s.bin' % lang)):
            texts, unk, chars = readGameTextFile(os.path.join(seqDir, file))
            idx, name = file.split('_', maxsplit=2)
            mergeTexts(allTexts, texts, 'Seq%04X' % int(idx))


    #printJson(allTexts)
    #printHeader(allTexts)
    printXml(allTexts)



def main():
    #texts, unknown, charStructs = readGameTextFile(sys.argv[1])
    #print(json.dumps({
    #    'unknown': unknown,
    #    'charStructs': charStructs,
    #    'texts': texts,
    #}, sort_keys=True, indent=4))
    readAllTexts(sys.argv[1])


if __name__ == '__main__':
    main()
