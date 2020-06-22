import struct
from binaryfile import BinaryFile
from isofile import IsoFile

class Appldr(IsoFile):
    """apploader.img file"""

    def __init__(self, offset:int=0, file:BinaryFile=None):
        super().__init__("apploader.img", isDir=False, offset=offset,
            size=0x2000, file=file, fileOffs=offset)
        self.buildDate   = "0000/00/00"
        self.entryPoint  = 0
        self.dataSize    = 0
        self.trailerSize = 0
        self.isSystem    = True

        if file is not None:
            self.readFile(file, offset)


    def readFile(self, file:(str,BinaryFile)="apploader.img", offset:int=0):
        """Read apploader.img file."""
        if type(file) is str:
            file = BinaryFile(file, 'rb', offset=offset)
        else: file.seek(offset)
        self.buildDate   = file.readString(maxLen=10)
        file.seek(offset+0x10)
        self.entryPoint  = file.readu32()
        self.dataSize    = file.readu32()
        self.trailerSize = file.readu32()
        self.size = self.dataSize + self.trailerSize + 0x20
        return self


    def dump(self):
        """Dump to console."""
        print("apploader.img @ 0x%08X:" % self.offset)
        print(" Build Date:           \"%s\"" % self.buildDate)
        print(" Entry Point:          0x%08X" % self.entryPoint)
        print(" Appldr Size:          0x%08X" % self.size)
        print(" Trailer Size:         0x%08X" % self.trailerSize)
