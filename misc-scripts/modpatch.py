#!/usr/bin/env python3
import sys
import struct

def pack(fmt, *args):
    return struct.pack('>'+fmt, *args)


DLISTS = 0x0028B4
INSTRS = 0x01A920
patches = ( # offset, length
    # material patches
    (0x012B + INSTRS, pack('H', 0x8039)), # list 29 material 5 -> 0
    #(0x0166 + INSTRS, pack('H', 0xFC17)), # list 30 material 5 -> 0
    # bugged?

    # dlist patches
    (0xC193 + DLISTS, 0x0030), # list 25 poly 42
    (0xC32E + DLISTS, 0x00A5), # list 25 poly 47-50
    (0xDD23 + DLISTS, 0x006F), # list 26 poly 2
    (0xDE13 + DLISTS, 0x0078), # list 26 poly 4
    (0xDF09 + DLISTS, 0x0108), # list 26 poly 8-11
    (0xE16D + DLISTS, 0x0060), # list 26 poly 20-21
    (0xE608 + DLISTS, 0x0105), # list 27 poly 1-3
    #(0xE77F + DLISTS, 0x0042), # list 27 poly 6
    (0xE8CC + DLISTS, 0x012C), # list 28 poly 0-3
    (0xEA58 + DLISTS, 0x0060), # list 28 poly 6-7
    (0xECF5 + DLISTS, 0x00A2), # list 28 poly 19-21
    (0xEDC7 + DLISTS, 0x0025), # list 28 poly 23

    # vertex patches
    (0x00EBB5, 0x002D), # list 25 poly 45
    (0x0108EC, 0x0030), # list 26 poly 7
    (0x01091C, 0x0057), # list 26 poly 8
    (0x0109CA, 0x0027), # list 26 poly 10
    (0x010BB6, 0x013E), # list 26 poly 21-27
    (0x010D1E, 0x006C), # list 26 poly 29 vtxs 0-11
    (0x010DF6, 0x0024), # list 26 poly 29 vtxs 24-26
    (0x011033, 0x0042), # list 27 poly 3
    (0x0110C3, 0x0054), # list 27 poly 5
    (0x011150, 0x001B), # list 27 poly 6 vtxs 6-8
    (0x01145C, 0x0042), # list 28 poly 7
    (0x0114C5, 0x0039), # list 28 poly 9
    (0x011E23, 0x0066), # list 30
    (0x011E8C, 0x0053), # list 30
    #(0x002BFC, pack('I', 0x11E89)),

    # mesh adjustments - bottom
    #(0x01DD24, pack('H', 0x1040)), # vtx 0x07FB, Y pos 0x105A
    (0x01AEF8, pack('H', 0xFCEB)), # vtx 0x0049, Y pos 0xFCAB
    (0x01AD4E, pack('H', 0xFCF2)), # vtx 0x0002, Y pos 0xFCC2
    (0x01AD84, pack('H', 0xFD82)), # vtx 0x000B, Y pos 0xFD42
)

with open(sys.argv[1], 'r+b') as file:
    for offset, length in patches:
        file.seek(offset)
        if type(length) is bytes: file.write(length)
        else: file.write(b'\0' * length)
