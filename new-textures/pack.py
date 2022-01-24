#!/usr/bin/env python3
import os
import sys
import struct

texIds = (0x724, 0x725, 0x726, 0x728, 0x729, 0x72A) # skip 0x727
nMips  = {
    0x724: 7,
    0x725: 1,
    0x726: 1,
    0x728: 1,
    0x729: 1,
    0x72A: 1,
}

dirPath = sys.argv[1]
with open(dirPath+'/out.bin', 'wb') as outFile:
    for i, tid in enumerate(texIds):
        nMip = nMips[tid]
        offs = outFile.tell()
        print(".int 0x%08X # %04X @ %06X" % (
            (offs >> 1) | 0x80000000 | (nMip << 24), tid, offs))
        if tid == 0x726:
            print(".int 0x01000000 # 0727")

        # read mipmap data
        mipOffsets = []
        mipData = []
        mipOffs = 4 * nMip # skip past offset list
        for m in range(nMip):
            mipOffsets.append(mipOffs)
            with open(dirPath+'/%04X.%02X.bin' % (tid, m), 'rb') as inFile:
                data = inFile.read()

                # pad to 32 bytes
                pad = (mipOffs + len(data)) & 0x1F
                if pad: data += (b'\0' * (32 - pad))

                #print("mip offs  %08X len %08X" % (mipOffs, len(data)))
                mipOffs += len(data)
                mipData.append(data)

        # write mipmap offsets if more than one
        if nMip > 1:
            for o in mipOffsets:
                outFile.write(struct.pack('>I', o))
                #offs += 4

        # write data
        for d in mipData:
            #print("write mip %08X len %08X" % (outFile.tell(), len(d)))
            outFile.write(d)
            #offs += len(d)

        # pad to 32 bytes
        pad = (mipOffs + outFile.tell()) & 0x1F
        if pad: outFile.write((b'\0' * (32 - pad)))
