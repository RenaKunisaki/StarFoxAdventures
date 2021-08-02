#!/usr/bin/env python3
import sys
import struct

def printf(fmt, *args, **kw):
    print(fmt % args, end='', **kw)

def readStruct(file, fmt, offset=None):
    if offset is not None: file.seek(offset, 0)
    size = struct.calcsize(fmt)
    data = struct.unpack(fmt, file.read(size))
    if len(data) == 1: return data[0] # grumble
    return data

def main():
    printf("Input:  %s\n", sys.argv[1])
    printf("Output: %s\n", sys.argv[2])
    with open(sys.argv[1], 'rb') as inFile:
        inFile.seek(0, 2)
        size = inFile.tell()
        inFile.seek(0)
        printf("Patch size: 0x%X\n", size)
        assert size > 0, "Patch file is empty"

        with open(sys.argv[2], 'r+b') as dolFile:
            # read header
            textOffsets = readStruct(dolFile, '>7I')
            dataOffsets = readStruct(dolFile, '>11I')
            textAddrs   = readStruct(dolFile, '>7I')
            dataAddrs   = readStruct(dolFile, '>11I')
            textSizes   = readStruct(dolFile, '>7I')
            dataSizes   = readStruct(dolFile, '>11I')
            bssAddr, bssSize, entryPoint = readStruct(dolFile, '>3I')

            # print header
            printf("Section Offset  MemAddr   Size EndOfs  EndAddr\n")
            for i in range(7):
                printf("text%d   %06X %08X %06X %06X %08X\n", i,
                    textOffsets[i], textAddrs[i], textSizes[i],
                    textOffsets[i] + textSizes[i],
                    textAddrs[i]   + textSizes[i])
            for i in range(11):
                printf("data%02d  %06X %08X %06X %06X %08X\n", i,
                    dataOffsets[i], dataAddrs[i], dataSizes[i],
                    dataOffsets[i] + dataSizes[i],
                    dataAddrs[i]   + dataSizes[i])
            printf("BSS     ------ %08X %06X ------ %08X\n", bssAddr, bssSize, bssAddr + bssSize)
            printf("Entry   ------ %08X ------ --------\n", entryPoint)

            # find an unused text section
            textIdx = None
            for i in range(7):
                if textSizes[i] == 0:
                    textIdx = i
                    break
            assert textIdx is not None, "Can't find a free text section"
            printf("Adding patch as section %d\n", textIdx)

            # read the jump address and load address from the patch file
            jumpAddr = readStruct(inFile, '>I')
            loadAddr = readStruct(inFile, '>I')
            printf("jumpAddr=0x%08X loadAddr=0x%08X\n", jumpAddr, loadAddr)

            # figure out what section the jump address is in
            patchSection = None
            for i in range(7):
                if textAddrs[i] <= jumpAddr and (textAddrs[i]+textSizes[i] > jumpAddr):
                    patchSection = i
                    break
            assert patchSection is not None, "Jump address not in text section"

            jumpOffs = textOffsets[patchSection] + (jumpAddr - textAddrs[patchSection])
            printf("Jump is at offset 0x%06X, section %d\n", jumpOffs, patchSection)

            # read the rest of the patch file and append to the dol file
            dolFile.seek(0, 2)
            dolSize = dolFile.tell()
            patchSize = 0

            while True:
                data = inFile.read(8192)
                if len(data) == 0: break
                dolFile.write(data)
                patchSize += len(data)

            # add this to the section list
            dolFile.seek(textIdx * 4)
            dolFile.write(struct.pack('>I', dolSize)) # offset
            dolFile.seek((textIdx * 4) + (18*4))
            dolFile.write(struct.pack('>I', loadAddr)) # address
            dolFile.seek((textIdx * 4) + (18*4*2))
            dolFile.write(struct.pack('>I', patchSize)) # size

            # build the branch instruction
            branchRel = (loadAddr - jumpAddr)
            if branchRel < 0: branchRel += 0x1_0000_0000
            branchOp  = (branchRel & 0x03FFFFFC) | 0x48000001
            printf("Patch opcode = 0x%08X, offset = 0x%X\n", branchOp, branchRel)
            #assert branchOp == 0x4BFE2FD5
            dolFile.seek(jumpOffs)
            dolFile.write(struct.pack('>I', branchOp))

            print("Patch OK")

if __name__ == "__main__":
    main()
