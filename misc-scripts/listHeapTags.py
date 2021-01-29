#!/usr/bin/env python3
import sys
import os.path
import math
import struct

NUM_HEAPS = 4
HEAP_TABLE = 0x80340698 & 0x1FFFFFFF

def readStruct(file, fmt, offset=None):
    size = struct.calcsize(fmt)
    if offset is not None: file.seek(offset)
    data = file.read(size)
    r = struct.unpack(fmt, data)
    if len(r) == 1: return r[0] # grumble
    return r


def main(dumpPath):
    tagCounts = {}
    with open(dumpPath, 'rb') as ram:
        for iHeap in range(NUM_HEAPS):
            totalSize, usedSize, totalBlocks, usedBlocks, data = \
                readStruct(ram, '>5I', HEAP_TABLE + (iHeap * 0x14))
            data = data & 0x1FFFFFFF
            for iBlock in range(usedBlocks):
                entry = readStruct(ram, '>IIHHHHIII', data + (iBlock * 0x1C))
                tag = entry[6]
                if tag not in tagCounts: tagCounts[tag] = 0
                tagCounts[tag] += 1
    for tag in sorted(tagCounts.keys(), key=lambda k: tagCounts[k]):
        print('%08X %d' % (tag, tagCounts[tag]))


if __name__ == '__main__':
    main("/home/rena/.local/share/dolphin-emu/Dump/mem1.raw")
