#!/usr/bin/env python3
import sys

with open(sys.argv[1], 'rb') as inFile:
    with open(sys.argv[2], 'wt') as outFile:
        while True:
            d = inFile.read(8)
            if len(d) == 0: break
            # if not full line, must pad it with a NOP.
            if len(d) == 4: d = b'\x60\0\0\0' + d
            outFile.write('%s %s\n' % (
                d[0:4].hex(), d[4:8].hex() ))
