import struct
from .binaryfile import BinaryFile
from .isofile import IsoFile

GC_MEM_SIZE = 0x01800000
BI2_BIN_SIZE = 0x2000

class Bi2Bin(IsoFile):
    """bi2.bin file"""

    CountryCodes = {
        0x00: "Japan",
        0x01: "Americas", # USA, Canada, others?
        0x02: "Europe",
        0x03: "All", # not valid on retail consoles
    }

    def __init__(self, offset:int=0, file:BinaryFile=None):
        super().__init__("bi2.bin", isDir=False, offset=offset,
            size=BI2_BIN_SIZE, file=file, fileOffs=offset)
        self.debugMonitorSize = 0
        self.simMemSize       = GC_MEM_SIZE
        self.argOffs          = 0
        self.debugFlag        = 0
        self.trackLoc         = 0
        self.trackSize        = 0
        self.countryCode      = 0
        self.numDiscs         = 0
        self.supportLFN       = False
        self.padSpec          = 0
        self.dolSizeLimit     = 0
        self.isSystem         = True

        if file is not None:
            self.readFile(file, offset)


    def readFile(self, file:(str,BinaryFile)="bi2.bin", offset:int=0):
        """Read bi2.bin file."""
        if type(file) is str:
            file = BinaryFile(file, 'rb', offset=offset)
        else: file.seek(offset)
        self.debugMonitorSize = file.readu32()
        self.simMemSize       = file.readu32()
        self.argOffs          = file.readu32()
        self.debugFlag        = file.readu32()
        self.trackLoc         = file.readu32()
        self.trackSize        = file.readu32()
        self.countryCode      = file.readu32()
        self.numDiscs         = file.readu32()
        self.supportLFN       = file.readu32() # support long file names
        self.padSpec          = file.readu32()
        self.dolSizeLimit     = file.readu32()

        return self


    def dump(self):
        """Dump to console."""
        print("bi2.bin @ 0x%08X:" % self.offset)
        print(" Debug Monitor Size:   0x%08X" % self.debugMonitorSize)
        print(" Sim. Memory Size:     0x%08X (%s)" % (
            self.simMemSize,
            "Normal" if self.simMemSize == GC_MEM_SIZE else (
                "Large" if self.simMemSize > GC_MEM_SIZE else "Small"
            )))
        print(" Argument Offset:      0x%08X" % self.argOffs)
        print(" Debug Flag:           0x%08X" % self.debugFlag)
        print(" Track Location:       0x%08X" % self.trackLoc)
        print(" Track Size:           0x%08X" % self.trackSize)
        print(" Country Code:         0x%08X (%s)" % (self.countryCode,
            self.CountryCodes.get(self.countryCode, "Unknown")))
        print(" Disc Count:           %d" % self.numDiscs)
        print(" Long File Names:      %d (%s)" % (self.supportLFN,
            "Supported" if self.supportLFN else "Not supported"))
        print(" Pad Spec:             0x%08X" % self.padSpec)
        print(" DOL Size Limit:       0x%08X" % self.dolSizeLimit)
