#!/usr/bin/env python3
import os
import struct

def printf(fmt, *args):
    print(fmt % args)

sfa_object_fmt = (
    '>f',      # 00
    'f',       # 04
    'I:start', # 08 start of object data
    'I:end',   # 0C end of object data
    'i',       # 10 some offset into object data
    'i',       # 14 always 0?
    'i',       # 18 offset or 0
    'i',       # 1C offset or 0
    'i',       # 20 offset or 0
    'i',       # 24 offset or 0
    'i',       # 28 offset or 0
    'i',       # 2C offset or 0
    'i',       # 30 offset or 0
    'i',       # 34 offset or 0
    'i',       # 38 offset or 0
    'i',       # 3C offset or 0
    'i',       # 40 offset or 0
    'h',       # 44 0, 4, 32, 64 - flags?
    'h',       # 46
    'h',       # 48
    'h',       # 4A
    'h',       # 4C
    'h',       # 4E
    'h',       # 50
    'h',       # 52
    'h',       # 54
    'h',       # 56
    'h',       # 58
    'h',       # 5A
    'h',       # 5C usually 255
    'h',       # 5E
    'h',       # 60
    'h',       # 62
    'h',       # 64
    'h',       # 66
    'h',       # 68
    'h',       # 6A
    'h',       # 6C
    'h',       # 6E
    'h',       # 70 usually 0 or ~32000, sometimes ~300, ~-300
    'h',       # 72
    'h',       # 74
    'h',       # 76
    'h',       # 78
    'h',       # 7A
    'h',       # 7C
    'h',       # 7E
    'h',       # 80
    'h',       # 82
    'h',       # 84
    'h',       # 86
    'f',       # 88 usually 0 or 1, sometimes 3, 4, 10
    'h',       # 8C usually 512, sometimes 513, 1024
    'h',       # 8E
    'B:type',  # 90
    '11s:name',# 91
    'i',       # 9C - first part of obj data.
)

def makeReader(structDef):
    resFmt = ''
    names  = []
    sizes  = []
    offset = 0
    totalSize = 0
    for field in structDef:
        fields = field.split(':')
        typ = fields[0]
        try: name = fields[1]
        except IndexError: name = "field%02X" % offset
        #printf("offs %02X (%s): %s", offset, typ, name)
        names.append(name)
        size = struct.calcsize(typ)
        sizes.append(size)
        totalSize += size
        offset += size
        resFmt += typ

    def read(file):
        result = {}
        try:
            data = struct.unpack(resFmt, file.read(totalSize))
        except struct.error:
            return None
        for i, name in enumerate(names):
            result[name] = data[i]
        return result

    return read


def words(file):
    while True:
        w = file.read(4)
        if w is None or len(w) == 0: break
        if len(w) < 4:
            printf("Leftover bytes: %s", w)
            break
        yield struct.unpack('>I', w)[0]

def main(argv):
    objsbin = open('discroot/OBJECTS.bin', 'rb')
    with open('discroot/OBJECTS.tab', 'rb') as tab:
        reader  = makeReader(sfa_object_fmt)
        offsets = list(words(tab))
        for i, offs in enumerate(offsets):
            objsbin.seek(offs)
            obj = reader(objsbin)
            if obj is None:
                printf("%06X Failed to read object", offs)
            else:
                try: size = offsets[i+1] - offs
                except IndexError: size = 0
                name = obj['name'].partition(b'\0')[0].decode('utf-8')
                data = [
                    '%06X' % offs,
                    '%04X' % size,
                ]
                for k in sorted(obj.keys()):
                    v = obj[k]
                    #if type(v) is bytes:
                    #    v = v.partition(b'\0')[0].decode('utf-8')
                    #data.append(str(v))
                    data.append(k)
                print(' '.join(data))

                #printf("%06X %02X %04X %-11s %04X %6.1f %6.1f %04X %04X %6.1f", offs, obj['type'], size,
                #    name, obj['field9C'],
                #    obj['field00'], obj['field04'],
                #    obj['start'], obj['end'],
                #    obj['field88'])
    objsbin.close()


if __name__ == "__main__":
    import sys
    sys.exit(main(sys.argv))
