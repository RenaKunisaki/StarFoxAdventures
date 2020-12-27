#!/usr/bin/env python3
import sys
import struct

with open(sys.argv[1], 'rb') as mapInfo:
    i = 0
    while True:
        line = mapInfo.read(32)
        if len(line) < 32: break
        entry = struct.unpack('>28s4B', line)
        name, unk1, unk2, unk3, unk4 = entry
        name = name.strip().decode('utf-8').replace('\0', '')
        print("%02X|%-28s|%02X|%02X|%02X|%02X" % (i, name, unk1, unk2, unk3, unk4))
        i += 1
    #print("Used:  ", ', '.join(used))
    #print("Unused:", ', '.join(blank))
