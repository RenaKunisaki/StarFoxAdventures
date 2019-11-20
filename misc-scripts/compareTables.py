#!/usr/bin/env python3
"""Open two table files and report offsets in which one entry is zero
and the other is not.
"""
import sys
import struct

with open(sys.argv[1], 'rb') as file1:
    with open(sys.argv[2], 'rb') as file2:
        offs = 0
        diffCount = 0
        while True:
            d1, d2 = file1.read(4), file2.read(4)
            if len(d1) != len(d2):
                if len(d1) < 4: print("%06X file 1 ends" % offs)
                else: print("%06X file 2 ends" % offs)
            if len(d1) < 4 or len(d2) < 4: break
            d1 = struct.unpack('>I', d1)[0] # grumble
            d2 = struct.unpack('>I', d2)[0] # grumble
            v1 = d1 & 0xFFFFFF
            v2 = d2 & 0xFFFFFF
            if (v1 == 0 and v2 != 0) or (v1 != 0 and v2 == 0):
                print("%04X %06X %08X %08X" % (offs//4, offs, d1, d2))
                diffCount += 1
            offs += 4
print(diffCount, "differences")
