#!/usr/bin/env python3
import sys
import struct

with open(sys.argv[1], 'rb') as warpTab:
    i = 0
    blank, used = [], []
    while True:
        line = warpTab.read(16)
        if len(line) < 16: break
        entry = struct.unpack('>3f2h', line)
        x, y, z, idx, rot = entry
        rot = (rot / 255) * 360
        print("%02X %+10.2f %+10.2f %+10.2f %3d %3d" % (
            i, x, y, z, idx, rot))
        if x == 0 and y == 0 and z == 0:
            blank.append('0x%02X' % i)
        else: used.append('0x%02X' % i)
        i += 1
    #print("Used:  ", ', '.join(used))
    #print("Unused:", ', '.join(blank))
