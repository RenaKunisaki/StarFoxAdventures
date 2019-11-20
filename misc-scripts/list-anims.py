#!/usr/bin/env python3
# Read out the ANIM.BIN offsets for a model
import os
import os.path
import sys
import struct
import zlib


def main():
    discRoot = sys.argv[1]
    dirName  = sys.argv[2]
    modelId  = int(sys.argv[3], 0)

    # get the model's offset from MODELS.TAB
    with open('%s/%s/MODELS.tab' % (discRoot, dirName), 'rb') as modelsTab:
        modelsTab.seek(modelId << 2)
        modelsBinOffs = struct.unpack('>I', modelsTab.read(4))[0] # grumble
    print("MODELS.BIN offset 0x%06X flag 0x%02X" % (
        modelsBinOffs & 0xFFFFFF, modelsBinOffs >> 24))

    # get the model from MODELS.BIN
    with open('%s/%s/MODELS.bin' % (discRoot, dirName), 'rb') as modelsBin:
        modelsBin.seek(modelsBinOffs & 0xFFFFFF)
        header = modelsBin.read(4)
        assert len(header) == 4

        if header == b'\xFA\xCE\xFE\xED':
            header      = struct.unpack('>8I', modelsBin.read(8*4))
            decLen      = header[0]
            zlbDataOffs = header[1]
            compLen     = header[2] # minus 0x10
            # other fields unknown

            # XXX the game doesn't actually do these checks.
            # it's entirely possible for there to not be a ZLB header here.
            h2 = struct.unpack('>3sb3I', modelsBin.read(4*4))
            assert h2[0] == b'ZLB' and h2[1] == 0, \
                "Corrupt ZLB header following FACEFEED"
            assert h2[2] == 1, "Unsupported ZLB version"
            assert h2[3] == decLen, "ZLB/FACEFEED headers disagree on decLen"
            #assert h2[4] == compLen+0x10, \
            #    "ZLB/FACEFEED headers disagree on compLen"

            print("FACEFEED compLen=0x%X decLen=0x%X" % (compLen, decLen))
            compData  = modelsBin.read(compLen+0x10)
            modelData = zlib.decompress(compData)
            assert len(modelData) == decLen

            #for i in range(0, 256, 16):
            #    l = ['%04X' % i]
            #    for j in range(16):
            #        l.append('%02X' % modelData[i+j])
            #    print(' '.join(l))

        elif header == b'ZLB\0':
            header    = modelsBin.read(12)
            decLen    = struct.unpack_from('>I', header, 0x08)
            compLen   = struct.unpack_from('>I', header, 0x0C)
            print("ZLB compLen=0x%X decLen=0x%X" % (compLen, decLen))
            compData  = modelsBin.read(compLen)
            modelData = zlib.decompress(compData)
            assert len(modelData) == decLen

        else:
            modelData = header + modelsBin.read(0xF8)

        nAnimations = struct.unpack_from('>H', modelData, 0xEC)[0] # grumble
        print("Model has %d animations" % nAnimations)
        nAnimations = 0x359 # HACK
        #assert nAnimations > 0, "Model has no animations"

    # get the model's offset from MODANIM.TAB
    with open(discRoot+'/MODANIM.TAB', 'rb') as modAnimTab:
        modAnimTab.seek(modelId << 1)
        modAnimOffs = struct.unpack('>H', modAnimTab.read(2))[0] # grumble

    # get the animation IDs from MODANIM.BIN
    with open(discRoot+'/MODANIM.BIN', 'rb') as modAnimBin:
        modAnimBin.seek(modAnimOffs)
        animData = modAnimBin.read(2 * nAnimations)
        assert len(animData) == 2 * nAnimations

    # get the animation offsets from ANIM.TAB
    with open('%s/%s/ANIM.TAB' % (discRoot, dirName), 'rb') as animTab:
        minOffs, maxOffs = 999999999, -1
        result = []
        for i in range(nAnimations):
            animId = struct.unpack_from('>h', animData, i*2)[0] # grumble
            if animId < 0: offs = 0xFFFFFFFF
            else:
                animTab.seek(animId * 4)
                offs, nextOffs = struct.unpack('>II', animTab.read(8))
                offs &= 0xFFFFFF
                nextOffs &= 0xFFFFFF
                minOffs = min(minOffs, offs)
                maxOffs = max(maxOffs, nextOffs)
            result.append((offs, nextOffs))
            print("%04X %04X %08X - %08X" % (i, animId, offs, nextOffs))
        print("Range is 0x%08X - 0x%08X" % (minOffs, maxOffs))



if __name__ == '__main__':
    main()
