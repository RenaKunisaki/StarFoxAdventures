import struct
from binaryfile import BinaryFile
from isofile import IsoFile

DVD_MAGIC = 0xC2339F3D
BOOT_BIN_SIZE = 0x440

class BootBin(IsoFile):
    """boot.bin file, ie header of ISO"""

    def __init__(self, offset:int=0, file:BinaryFile=None):
        super().__init__("boot.bin", isDir=False, offset=offset,
            size=BOOT_BIN_SIZE, file=file)
        self.gameName         = "No Name"
        self.gameCode         = "XXXX"
        self.makerCode        = "01"
        self.discNo           = 0
        self.version          = 0
        self.audioStreaming   = False
        self.streamBufSize    = 0
        self.debugMonitorOffs = 0
        self.debugMonitorAddr = 0
        self.mainDolOffs      = 0
        self.fstOffs          = 0
        self.fstSize          = 0
        self.maxFstSize       = 0 # for multi-disc games
        self.unk430           = 0
        self.unk434           = 0
        self.unk438           = 0
        self.magic            = DVD_MAGIC
        self.isSystem         = True

        if file is not None:
            self.readFile(file, offset)


    def readFile(self, file:(str,BinaryFile)="boot.bin", offset:int=0):
        """Read boot.bin file."""
        if type(file) is str:
            file = BinaryFile(file, 'rb', offset=offset)
        else: file.seek(offset)
        self.gameCode         = file.readString(maxLen=4)
        self.makerCode        = file.readString(maxLen=2)
        self.discNo           = file.readu8()
        self.version          = file.readu8()
        self.audioStreaming   = file.readu8() != 0
        self.streamBufSize    = file.readu8()
        file.seek(0x1C)
        self.magic            = file.readu32()
        self.gameName         = file.readString(maxLen=0x3E0)
        file.seek(0x400)
        self.debugMonitorOffs = file.readu32()
        self.debugMonitorAddr = file.readu32()
        file.seek(0x420)
        self.mainDolOffs      = file.readu32()
        self.fstOffs          = file.readu32()
        self.fstSize          = file.readu32()
        self.maxFstSize       = file.readu32()
        self.unk430           = file.readu32()
        self.unk434           = file.readu32()
        self.unk438           = file.readu32()

        if self.magic != DVD_MAGIC:
            raise ValueError("Incorrect/corrupt boot.bin file")

        return self


    def dump(self):
        """Dump to console."""
        print("boot.bin:")
        print(" Game Code:            \"%s\"" % self.gameCode)
        print(" Maker Code:           \"%s\"" % self.makerCode)
        print(" Disc #:               %d"     % self.discNo)
        print(" Version:              %d"     % self.version)
        print(" Audio Streaming:      %s"     % self.audioStreaming)
        print(" Stream Buf Size:      0x%X"   % self.streamBufSize)
        print(" DVD Magic:            0x%08X (%s)" % (self.magic,
            "OK" if self.magic == DVD_MAGIC else "FAIL"))
        print(" Game Name:            \"%s\"" % self.gameName)
        print(" Debug Monitor Offset: 0x%08X" % self.debugMonitorOffs)
        print(" Debug Monitor Addr:   0x%08X" % self.debugMonitorAddr)
        print(" main.dol Offset:      0x%08X" % self.mainDolOffs)
        print(" FST Offset:           0x%08X" % self.fstOffs)
        print(" FST Size:             0x%08X" % self.fstSize)
        print(" FST Max Size:         0x%08X" % self.maxFstSize)
        print(" Unknown 0x430:        0x%08X 0x%08X 0x%08X" % (
            self.unk430, self.unk434, self.unk438))
