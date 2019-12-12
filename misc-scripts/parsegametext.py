#!/usr/bin/env python3
import sys
import json
import struct

with open(sys.argv[1], 'rb') as file:
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

    strs = []
    base = file.tell()
    for i in range(numStrs):
        offs = base + offsets[i]
        offsEnd = base + offsets[i+1]
        file.seek(offs)
        s = []
        while file.tell() < offsEnd-1:
            b = file.read(1)
            if len(b) == 0: break
            b = b[0]
            #if b == 0xEE:
            #    c = file.read(4)
            #    b = '<EE %02X%02X%02X%02X>' % (c[0], c[1], c[2], c[3])
            if b == 0xEF: # control code
                c = file.read(1)[0]
                if c == 0xA3:
                    c = file.read(1)[0]
                    if c == 0xB4:
                        # set styles or something
                        # used for area names, "no controller" msg
                        c = file.read(2)
                        b = "<STYLE %02X%02X>" % (c[0], c[1])
                    elif c == 0xB7:
                        # switch font texture
                        # used to display buttons and faces
                        # 2: buttons, 4: default, 5: faces
                        c = file.read(2)
                        b = "<FONT %02X%02X>" % (c[0], c[1])
                    else:
                        c = file.read(2)
                        b = '<UNK %02X%02X>' % (c[0], c[1])
                else: b = '\\xEF\\x%02X' % c
            #elif b == 0: break

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

    print(json.dumps({
        'unknown': unknown,
        'charStructs': charStructs,
        'texts': texts,
    }, sort_keys=True, indent=4))
