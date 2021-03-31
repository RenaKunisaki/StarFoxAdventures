#!/usr/bin/env python3
import os
import os.path
import sys
import struct
import shutil

def pack(fmt, *args):
    return struct.pack('>'+fmt, *args)

def dolAddressToOffset(sections, addr):
    for i, sec in enumerate(sections):
        aStart = sec['address']
        aEnd   = aStart+sec['size']
        if aStart <= addr and aEnd > addr:
            offs = (addr - aStart) + sec['offset']
            print("Address 0x%08X is in section 0x%X offs 0x%06X" % (
                addr, i, offs))
            return offs
    print("Address 0x%08X isn't in DOL" % addr)
    return None


def applyDolPatch(pathPatch, pathInput, pathOutput):
    shutil.copy(pathInput, pathOutput)
    dolFile   = open(pathOutput, 'r+b')
    patchFile = open(pathPatch,  'rb')

    # read DOL header
    header = struct.unpack('>54I', dolFile.read(18*4*3))
    sections = []
    for i in range(18):
        sec = {
            'offset':  header[i],
            'address': header[i+18],
            'size':    header[i+36],
        }
        sections.append(sec)
        #print("Section 0x%02X: Addr=0x%08X Offs=0x%06X Size=0x%06X" % (
        #    i, sec['address'], sec['offset'], sec['size']))
    #bssAddr, bssSize = struct.unpack('>2I', dolFile.read(8))
    #sections.append({'address':bssAddr, 'size':bssSize, 'offset':None})

    # read patch header and content
    jumpAddr, patchAddr = struct.unpack('>2I', patchFile.read(8))
    patchData = patchFile.read()

    # calculate offsets
    jumpOffs  = dolAddressToOffset(sections, jumpAddr)
    patchOffs = dolAddressToOffset(sections, patchAddr)
    assert jumpOffs  is not None, "Invalid jump address"
    assert patchOffs is not None, "Invalid patch address"

    # apply patch
    print("Patch 0x%X bytes at 0x%08X: 0x%06X" % (
        len(patchData), patchAddr, patchOffs))
    dolFile.seek(patchOffs)
    dolFile.write(patchData)

    # make jump
    jumpRel = (patchAddr - jumpAddr) & 0x03FFFFFC
    branch  = jumpRel | 0x48000001 # make bl
    print("Write jump to 0x%08X at 0x%08X (0x%06X): 0x%08X" % (
        patchAddr, jumpAddr, jumpOffs, branch))
    dolFile.seek(jumpOffs)
    dolFile.write(struct.pack('>I', branch))

    dolFile.close()
    patchFile.close()


def main():
    pathPatch  = sys.argv[1]
    pathInput  = sys.argv[2]
    pathOutput = sys.argv[3]
    print("patch", pathPatch, pathInput, pathOutput)
    applyDolPatch(pathPatch, pathInput, pathOutput)

if __name__ == '__main__':
    main()
