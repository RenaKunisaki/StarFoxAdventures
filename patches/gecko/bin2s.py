#!/usr/bin/env python3
import sys

with open(sys.argv[1], 'rb') as inFile:
    with open(sys.argv[2], 'wt') as outFile:
        while True:
            d = inFile.read(16)
            if len(d) == 0: break
            r = ', '.join(list(map(lambda b: '0x%02X' % b, d)))
            outFile.write('.byte %s\n' % r)
