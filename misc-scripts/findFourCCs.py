#!/usr/bin/env python3
"""Scan a file, logging instances of interesting 4-character aligned sequences."""
import os
import sys
import struct

def isUpper(c):
    return c >= 0x41 and c <= 0x5A

def isLetter(c):
    return (c >= 0x41 and c <= 0x5A) or (c >= 0x61 and c <= 0x7A)

def isLetterOrDigit(c):
    return (c >= 0x41 and c <= 0x5A) or (c >= 0x61 and c <= 0x7A) or \
        (c >= 0x30 and c <= 0x39)

def main():
    with open(sys.argv[1], 'rb') as file:
        offs = 0
        while True:
            data = file.read(8)
            if len(data) < 8: break
            d0, d1, d2, d3, d4, d5, d6, d7 = data
            if (isLetterOrDigit(d0) and isLetterOrDigit(d3)
            and isUpper(d1) and isUpper(d2) and (isUpper(d0) or isUpper(d3))
            and d4 < 0x20):
                d8, d9 = struct.unpack('>2I', file.read(8))
                print("%08X %c%c%c%c %08X %08X %08X" % (
                    offs, chr(d0), chr(d1), chr(d2), chr(d3),
                    (d4<<24) | (d5<<16) | (d6<<8) | (d7),
                    d8, d9))
                offs += 8
            offs += 8

if __name__ == '__main__':
    main()
