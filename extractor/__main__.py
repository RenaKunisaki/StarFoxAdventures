#!/usr/bin/env python3
import sys
import inspect
import struct
from binaryfile import BinaryFile
from tabfile import TabFile
from zlb import Zlb

def printf(fmt, *args):
    print(fmt % args, end='')

class App:
    def __init__(self):
        pass


    def help(self):
        """Display help text."""
        methods = [
            (func, getattr(self, func))
            for func in dir(self)
            if callable(getattr(self, func))
            and not func.startswith('_')
        ]
        for name, method in sorted(methods, key = lambda it: it[0]):
            printf("%s: %s\n", name, method.__doc__)


    def compress(self, inPath, outPath, inOffset=0):
        """Compress a ZLB file."""
        if type(inOffset) is str: inOffset = int(inOffset, 0)
        file = BinaryFile(inPath, offset=inOffset)
        with open(outPath, 'wb') as outFile:
            outFile.write(Zlb(file).compress())


    def decompress(self, inPath, outPath, inOffset=0):
        """Decompress a ZLB file."""
        if type(inOffset) is str: inOffset = int(inOffset, 0)
        file = BinaryFile(inPath, offset=inOffset)
        with open(outPath, 'wb') as outFile:
            outFile.write(Zlb(file).decompress())


    def listTable(self, inPath):
        """List entries in a TAB file."""
        file = BinaryFile(inPath)
        tbl  = TabFile(file)
        print("Item C Flg Offset  (C=compressed?)")
        for i, entry in enumerate(tbl.getEntries()):
            printf("%5d: %s  %02X %06X\n", i,
                'Y' if entry['compressed'] else '-',
                entry['flags'], entry['offset'])


    def dumpTable(self, tblPath, dataPath, outPath):
        """Dump objects in file `dataPath` listed in table `tblPath` to dir `outPath`."""
        file = BinaryFile(dataPath)
        tbl  = TabFile(tblPath)
        dumped = {0xFFFFFF: True}
        entries = tbl.getEntries()
        for i, entry in enumerate(entries):
            offset = entry['offset']
            compressed = entry['compressed']
            if not dumped.get(offset, False):
                printf("Dumping entry 0x%06X (%02X, %s)... ", offset,
                    entry['flags'],
                    "compressed" if compressed else "raw")
                file.seek(offset)
                if compressed: data = Zlb(file).decompress()
                else:
                    # we don't know the actual size, just guess...
                    try: sz = entries[i+1]['offset'] - offset
                    except IndexError: sz = 0x8000
                    if sz > 0: data = file.readBytes(sz)
                    else: data = None
                if data is not None and len(data) > 0:
                    with open("%s/%06X.bin" % (outPath, offset), 'wb') as outFile:
                        outFile.write(data)
                    printf("OK (%d bytes)\n", len(data))
                else:
                    printf("empty\n")
                dumped[offset] = True


    def listAnimations(self, discRoot, modelId):
        """List the animation IDs used by given model."""
        modelId = int(modelId, 0)
        modAnimTab = BinaryFile(discRoot+'/MODANIM.TAB')
        modAnimOffs = modAnimTab.readu16(modelId << 1)
        nextOffs = modAnimTab.readu16((modelId+1) << 1)
        nAnims = (nextOffs - modAnimOffs) >> 1
        modAnimBin = BinaryFile(discRoot+'/MODANIM.BIN')
        animIds = modAnimBin.readBytes(nAnims*2, modAnimOffs)
        animIds = struct.unpack('>%dh' % nAnims, animIds)
        printf("%4d animations; MODANIM.BIN 0x%06X - 0x%06X, min 0x%04X max 0x%04X\n",
            nAnims, modAnimOffs, modAnimOffs+(nAnims*2),
            max(0, min(animIds)), max(animIds))
        for i in range(0, nAnims, 8):
            printf('%04X: %s\n', i,
            ' '.join(map(lambda v: '%04X' % (v&0xFFFF), animIds[i:i+8])))


    def readRomList(self, path, discRoot):
        """Read romlist.zlb file."""
        file = BinaryFile(path, offset=0)
        data = Zlb(file).decompress()

        # read OBJINDEX.bin
        objIndex = []
        with open(discRoot+'/OBJINDEX.bin', 'rb') as objIdxFile:
            entries = objIdxFile.read()
            for i in range(0, len(entries), 2):
                it = struct.unpack_from('>H', entries, i)[0] # grumble
                objIndex.append(it)

        # read OBJECTS.tab
        objsTab = []
        with open(discRoot+'/OBJECTS.tab', 'rb') as objTabFile:
            entries = objTabFile.read()
            for i in range(0, len(entries), 4):
                it = struct.unpack_from('>I', entries, i)[0] # grumble
                objsTab.append(it)

        # read OBJECTS.bin to get names
        objNames = []
        with open(discRoot+'/OBJECTS.bin', 'rb') as objBinFile:
            for offs in objsTab:
                objBinFile.seek(offs + 0x91)
                name = objBinFile.read(11).decode('utf-8').replace('\0', '')
                objNames.append(name)


        offs = 0
        idx  = 0
        printf("Idx  Offs Type ID   ObjName     Sz 03 04 05 06 07      X        Y        Z   14\n")
        while offs < len(data):
            entry = struct.unpack_from('>hBBBBBBfffI', data, offs)
            length = entry[1]
            typ = entry[0]
            if typ >= 0:
                if typ < len(objIndex): realTyp = objIndex[typ]
                else: realTyp = "????"
            else: realTyp = -typ
            if type(realTyp) != str:
                try: name = objNames[realTyp]
                except: name = "?"
                realTyp = '%04X' % realTyp
            else: name = "?"

            # idx offs typ realTyp name sz 03 ID 05 06 07 X Y Z 14
            printf("%04X %04X %04X %s %-11s %02X %02X %02X %02X %02X %02X %+8.2f %+8.2f %+8.2f %08X ",
                idx, offs, typ, realTyp, name, *(entry[1:]))
            for i in range(0x18, length*4, 4):
                printf("%02X%02X%02X%02X ", *data[offs+i: offs+i+4])
            printf("\n")

            if length == 0: break
            offs += length * 4
            idx  += 1



def main(*args):
    app = App()

    if len(args) < 1:
        app.help()
        return 0

    while len(args) > 0:
        method = args[0]
        if method.startswith('--'): method = method[2:]

        func = getattr(app, method, None)
        if func is None or not callable(func):
            print("Unknown operation:",method)
            return 1

        sig   = inspect.signature(func)
        nArg  = len(sig.parameters)
        fArgs = args[1:nArg+1]
        args  = args[nArg+1:]

        if len(fArgs) < nArg:
            msg = [method]
            for name in sig.parameters:
                msg.append(name)
            print("Usage:", ' '.join(msg))
            return 1

        func(*fArgs)
    return 0


if __name__ == '__main__':
    sys.exit(main(*sys.argv[1:]))
