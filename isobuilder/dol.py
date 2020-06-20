import struct
from binaryfile import BinaryFile
from isofile import IsoFile

class DOL(IsoFile):
    """DOL file"""

    def __init__(self, path:str, offset:int=0, size:int=0, file:BinaryFile=None, isSystem=False):
        super().__init__(path, isDir=False, offset=offset, size=size, file=file, isSystem=isSystem)
        self.textSections = []
        self.dataSections = []
        self.bssAddr      = 0
        self.bssSize      = 0
        self.entryPoint   = 0

        if file is not None:
            self.readFile(file, offset)


    def readFile(self, file:(str,BinaryFile), offset:int=0):
        """Read DOL file."""
        if type(file) is str:
            file = BinaryFile(file, 'rb', offset=offset)
        else: file.seek(offset)

        self.textSections = []
        self.dataSections = []

        for i in range( 7): # text section offsets
            self.textSections.append({'offset': file.readu32()})
        for i in range(11): # data section offsets
            self.dataSections.append({'offset': file.readu32()})
        for i in range( 7): # text section addrs
            self.textSections[i]['addr'] = file.readu32()
        for i in range(11): # data section addrs
            self.dataSections[i]['addr'] = file.readu32()
        for i in range( 7): # text section sizes
            self.textSections[i]['size'] = file.readu32()
        for i in range(11): # data section sizes
            self.dataSections[i]['size'] = file.readu32()
        self.bssAddr, self.bssSize, self.entryPoint = file.readu32(3)

        self.size = max(map(
            lambda s: s['offset']+s['size'],
            self.textSections + self.dataSections))

        return self


    def dump(self):
        """Dump to console."""
        print(self.path)
        print("  Section | Offset | MemAddr  | Size")
        for i, sec in enumerate(self.textSections):
            print("  text%-2d  | %06X | %08X | %06X" % (i,
                sec['offset'], sec['addr'], sec['size'] ))
        for i, sec in enumerate(self.dataSections):
            print("  data%-2d  | %06X | %08X | %06X" % (i,
                sec['offset'], sec['addr'], sec['size'] ))
        print("  bss     | ------ | %08X | %06X" % (
            self.bssAddr, self.bssSize))
        print("  entry point: 0x%08X" % self.entryPoint)
