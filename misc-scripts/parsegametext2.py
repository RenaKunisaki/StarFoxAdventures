#!/usr/bin/env python3
"""Better "parse gametext" script."""
import sys
import struct

def readStruct(file, fmt, offset=None):
    size = struct.calcsize(fmt)
    if offset is not None: file.seek(offset)
    data = file.read(size)
    r = struct.unpack(fmt, data)
    if len(r) == 1: return r[0] # grumble
    return r

def readStr(file):
    r = []
    while True:
        b = file.read(1)
        if len(b) == 0 or b[0] == 0: break
        r.append(chr(b[0]))
    return ''.join(r)

class GametextFile:
    def __init__(self, path):
        self.file = open(path, 'rb')


    def readCharStruct(self):
        fields = readStruct(self.file, '>IHHbbbbBBBB')
        return {
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
        }


    def readCharStructs(self):
        numCharStructs = readStruct(self.file, '>I')
        self.charStructs = []
        for i in range(numCharStructs):
            self.charStructs.append(self.readCharStruct())


    def readText(self):
        fields = readStruct(self.file, '>HHBBBBI')
        return {
            'id':       fields[0],
            'nPhrases': fields[1],
            'window':   fields[2],
            'alignH':   fields[3],
            'alignV':   fields[4],
            'language': fields[5],
            'phrases':  fields[6],
        }


    def readTexts(self):
        numTexts, self.strDataLen = readStruct(self.file, '>2H')
        self.texts = []
        for i in range(numTexts):
            self.texts.append(self.readText())


    def readString(self):
        res = b''
        while True:
            c = self.file.read(1)
            if len(c) == 0: break

            if c == b'\xEE':
                c = self.file.read(2)
                if len(c) < 2: break
                if c == b'\x80\x80':
                    c = readStruct(self.file, '>H')
                    res += b'<SEQ 0x%04X>' % c

                elif c == b'\x80\x98':
                    a, b, c = readStruct(self.file, '>3H')
                    res += b'<TIME %d %d %d>' % (a, b, c)

                elif c == b'\x80\x98':
                    a, b, c = readStruct(self.file, '>3H')
                    res += b'<TIME %d %d %d>' % (a, b, c)

                elif c == b'\x80\xA0':
                    c = readStruct(self.file, '>H')
                    res += b'<UNKA0 0x%04X>' % c

                else: res += b'\\xEE\\x%02X\\x%02X' % (c[0], c[1])

            elif c == b'\xEF':
                c = self.file.read(1)
                if len(c) == 0: break
                if c == b'\xA3':
                    c = self.file.read(1)
                    if len(c) == 0: break

                    if c == b'\xB4':
                        res += b'<SCALE 0x%04X>' % readStruct(self.file, '>H')

                    elif c == b'\xB7':
                        res += b'<FONT 0x%04X>' % readStruct(self.file, '>H')

                    elif c == b'\xB8': res += b'<JUSTIFY LEFT>'
                    elif c == b'\xB9': res += b'<JUSTIFY RIGHT>'
                    elif c == b'\xBA': res += b'<JUSTIFY CENTER>'
                    elif c == b'\xBB': res += b'<JUSTIFY FULL>'

                    elif c == b'\xBF':
                        col = readStruct(self.file, '>8B')
                        res += b'<COLOR %02X%02X%02X%02X BG %02X%02X%02X%02X>' % col

                    else: res += b'\\xEF\\xA3\\x%02X' % c

                else: res += b'\\xEF\\x%02X' % c[0]

            elif c == b'\0': break
            else: res += c
        return res.decode('utf-8')


    def readStrings(self):
        numStrs = readStruct(self.file, '>I')
        self.strings = []
        for i in range(numStrs):
            offs = readStruct(self.file, '>I')
            self.strings.append({'offset':offs})
        base = self.file.tell()
        for i in range(numStrs):
            self.file.seek(self.strings[i]['offset'] + base)
            self.strings[i]['str'] = self.readString()
        self.file.seek(base + self.strDataLen)


    def readUnknown(self):
        numBytes = readStruct(self.file, '>I')
        self.unkData = self.file.read(numBytes)


    def readCharTextures(self):
        self.charTextures = []
        self.textureOffset = self.file.tell()
        while True:
            start = self.file.tell()
            fields = readStruct(self.file, '>HHHH')
            tex = {
                'offset': start,
                'texFmt': fields[0],
                'pixFmt': fields[1],
                'width':  fields[2],
                'height': fields[3],
            }
            if tex['width'] == 0 and tex['height'] == 0: break
            size = tex['width'] * tex['height']
            if tex['pixFmt'] == 4: size >>= 1
            tex['data'] = self.file.read(size)

            dLen = ((tex['width'] * tex['height'] * tex['pixFmt']) >> 4) * 2
            tex['length'] = dLen
            self.file.seek(start+dLen+4)
            self.charTextures.append(tex)


    def read(self):
        self.readCharStructs()
        self.readTexts()
        self.readStrings()
        self.readUnknown()
        self.readCharTextures()


def main():
    file = GametextFile(sys.argv[1])
    file.read()

    print("#  │C │Xpos │Ypos │Lft│Rgt│Top│Bot│Wdh│Hgt│Fnt│Tex  Character Structs")
    for i, cs in enumerate(file.charStructs):
        c = cs['character']
        if c >= 0x20 and c < 0x110000: c = chr(c)
        else: c = '%02X' % c
        print("%3d│%2s│%5d│%5d│%3d│%3d│%3d│%3d│%3d│%3d│%3d│%3d" % (i,
            c,           cs['xpos'],  cs['ypos'],
            cs['left'],  cs['right'], cs['top'], cs['bottom'],
            cs['width'], cs['height'],
            cs['font'],  cs['texture'],
        ))

    print("\n#  │Ph│Win│Ah│Av│Ln│Offs  Texts")
    for i, text in enumerate(file.texts):
        print("%3d│%2d│%3d│%2d│%2d│%2d│%06X" % (i,
            text['nPhrases'], text['window'],
            text['alignH'],   text['alignV'],
            text['language'], text['phrases'],
        ))

    print("\nStrings:")
    for i, string in enumerate(file.strings):
        print("#%3d @%06X: \"%s\"" % (i, string['offset'], string['str']))

    print("\nUnknown data:")
    for i in range(0, len(file.unkData), 16):
        line = []
        for j in range(16):
            try: line.append('%02X ' % file.unkData[i+j])
            except IndexError: line.append('.. ')
            if (j & 3) == 3: line.append(' ')
        for j in range(16):
            try: c = file.unkData[i+j]
            except IndexError: c = 0x20
            if c >= 0x20 and c <= 0x7E: c = chr(c)
            else: c = '.'
            line.append(c)
            if (j & 3) == 3: line.append(' ')
        print("%04X  %s" % (i, ''.join(line)))

    print("\n#  │TF│PF│Wdh│Hgt|Offset|Length  Char Textures")
    for i, tex in enumerate(file.charTextures):
        print("%3d│%02X│%02X│%3d│%3d|%06X|%06X" % (i,
            tex['texFmt'], tex['pixFmt'], tex['width'], tex['height'],
            tex['offset'], tex['length'],
        ))

    print("Next unknown offset: 0x%X" % file.file.tell())


if __name__ == '__main__':
    main()
