#!/usr/bin/env python3
"""This script applies patches to some of the game files."""
import os
import os.path
import sys
import struct
import shutil
import subprocess


def pack(fmt, *args):
    return struct.pack('>'+fmt, *args)

patches = []
def patch(path, offset, data):
    def apply(basePath):
        with open(os.path.join(basePath, path), 'r+b') as file:
            file.seek(offset)
            if type(data) is int: file.write(b'\0' * data)
            else: file.write(data)
    patches.append(apply)

# replace some empty entries in warptab with more useful/interesting ones
# no longer necessary since we have new warp menu
#warptab = 'files/WARPTAB.bin'
#patch(warptab, 0x0010, pack('3fh', -9496,  -127, -19025, 0)) # animtest
#patch(warptab, 0x0040, pack('3fh', -7683,   -11,  14083, 0)) # rolling demo
#patch(warptab, 0x0070, pack('3fh',  3200,  -789,  28160, 0)) # willow grove
#patch(warptab, 0x0080, pack('3fh',  -640,   151,  28800, 0)) # discovery falls
#patch(warptab, 0x0090, pack('3fh',-12700, -1805,  28000, 0)) # boss Kamerian
#patch(warptab, 0x00A0, pack('3fh',-19000,  1280,  33920, 0)) # CloudRunner2Rac
#patch(warptab, 0x00B0, pack('3fh',  7200,   134,  31000, 0)) # MazeTest
#patch(warptab, 0x00D0, pack('3fh', 12800,   192,  30080, 0)) # old palace
#patch(warptab, 0x00E0, pack('3fh',     0,     0,  13440, 0)) # ship battle
#patch(warptab, 0x0110, pack('3fh', -7040,     0,  -8320, 0)) # Nik Test
#patch(warptab, 0x0140, pack('3fh', -6069,  -921,  -2738, 1)) # shop

def applyFilePatches(basePath):
    for patch in patches:
        patch(basePath)


def buildPatch(basePath, name, dest):
    cmd = ('./makedolpatch.sh', name)
    try: result = subprocess.run(cmd, capture_output=True)
    except KeyboardInterrupt: sys.exit(1)
    output = result.stderr.decode('utf-8', 'replace').strip()
    if output != '':
        print("Error building", name, output)
        sys.exit(1)
    shutil.move(name+'.bin', dest)

patchOrder = (
    'debugprint',
    'krystal',
    #'cmenu',
    'climb',
    'pausemenu',
    'pda',
    'freemove',
    'alloc',
    #'gamebit',
    'hitboxdebug',
    #'dll',
    'staff_fx',
    'autosave',
    #'debugobjs',
    'inventory',
    'pad3',
    'camera',
    'startmsg',
    #'ybutton',
    'rumble',
)
def buildPatches(basePath):
    print("Building debug.bin...")
    buildPatch(basePath, 'debugbin', os.path.join(basePath, 'files', 'debug.bin'))
    for i, name in enumerate(patchOrder):
        print("Building", name)
        buildPatch(basePath, name, os.path.join(basePath,
            'files', 'patches', '%04d' % i))




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


def applyDolPatch(basePath):
    dolFile = open(os.path.join(basePath, 'sys', 'main.dol'), 'r+b')
    patchFile = open('dolpatch.bin', 'rb')

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
    applyDolPatch(sys.argv[1])
    buildPatches(sys.argv[1])
    applyFilePatches(sys.argv[1])

if __name__ == '__main__':
    main()
