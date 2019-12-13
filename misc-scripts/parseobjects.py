#!/usr/bin/env python3
"""Read OBJECTS.bin and OBJECTS.tab and make a list of ID => name."""
import struct

with open('../discroot/OBJECTS.tab', 'rb') as tabFile:
    with open('../discroot/OBJECTS.bin', 'rb') as binFile:
        idx = 0
        while True:
            d = tabFile.read(4)
            if len(d) < 4: break
            offs = struct.unpack('>I', d)[0] # grumble
            if offs == 0xFFFFFFFF: break
            binFile.seek(offs + 0x91)
            name = binFile.read(11).replace(b'\0', b' ')
            name = name.decode('ascii')
            d = binFile.read(4)
            if len(d) < 4: break
            modelId = struct.unpack('>I', d)[0] # grumble
            print('%04X|%11s|%04X|' % (idx, name, modelId))
            idx += 1
