#!/usr/bin/env python3
import sys
import inspect
from binaryfile import BinaryFile
from tabfile import TabFile
from zlb import ZlbDecoder

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


    def decompress(self, inPath, outPath, inOffset=0):
        """Decompress a ZLB file."""
        if type(inOffset) is str: inOffset = int(inOffset, 0)
        file = BinaryFile(inPath, offset=inOffset)
        with open(outPath, 'wb') as outFile:
            outFile.write(ZlbDecoder(file).decompress())


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
                if compressed: data = ZlbDecoder(file).decompress()
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
