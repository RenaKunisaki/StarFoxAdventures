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
        printf("  %5d x %5d @ %5d, %5d", nRows, nCols, originX, originY)

        mapsBin.seek(tab[1])
        d = mapsBin.read(nCols*nRows*4)
        d = struct.unpack('>%dI' % (nRows*nCols), d)
        for y in range(nRows):
            printf("\n  ")
            for x in range(nCols):
                bk  = d[(y*nCols)+x]
                mod = bk >> 23
                sub = (bk >> 17) & 0x3F
                unk = bk & 0x1FF
                if unk != 0x7F: unk = '%04X' % unk
                else: unk = '----'
                if mod == 0xFF: printf("---.-- %s│", unk)
                else: printf("%3d.%2d %s│", mod, sub, unk)
        printf("\n")
