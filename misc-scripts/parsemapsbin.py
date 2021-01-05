#!/usr/bin/env python3
import sys
import struct

def printf(fmt, *args, **kwargs):
    print(fmt % args, end='', **kwargs)

mapsTab = []
with open(sys.argv[1]+'/MAPS.tab', 'rb') as tab:
    while True:
        d = tab.read(4 * 7)
        if len(d) < 4 * 7: break
        d = struct.unpack('>7i', d)
        if d[0] < 0: break
        mapsTab.append(d)

with open(sys.argv[1]+'/MAPS.bin', 'rb') as mapsBin:
    for i, tab in enumerate(mapsTab):
        printf("Entry 0x%02X: info@%08X blk@%08X unk@%08X %08X %08X %08X %08X\n",
            i, *tab)

        mapsBin.seek(tab[0])
        d = mapsBin.read(8)
        if d.startswith(b'\xFA\xCE\xFE\xED'):
            print("  FACEFEED")
            continue
        nCols, nRows, originX, originY = struct.unpack('>4h', d)
        printf("  %5d x %5d @ %5d, %5d\n", nRows, nCols, originX, originY)

        mapsBin.seek(tab[6])
        d = mapsBin.read(32)
        d = struct.unpack('>8I', d)
        printf("%08X %08X %08X %08X\n", d[0], d[1], d[2], d[3])
        printf("%08X %08X %08X %08X\n", d[4], d[5], d[6], d[7])
