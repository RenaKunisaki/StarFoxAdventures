import struct
from .binaryfile import BinaryFile
from .isofile import IsoFile

DVD_MAGIC = 0xC2339F3D
BOOT_BIN_SIZE = 0x440

class BootBin(IsoFile):
    """boot.bin file, ie header of ISO"""

    def __init__(self, offset:int=0, file:BinaryFile=None):
        super().__init__("boot.bin", isDir=False, offset=offset,
            size=BOOT_BIN_SIZE, file=file, fileOffs=offset)
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
        self.fstAddr           = 0
        self.fileOffset       = 0
        self.fileLength           = 0
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
        self.fstAddr          = file.readu32()
        self.fileOffset       = file.readu32() # user area
        self.fileLength       = file.readu32()

        if self.magic != DVD_MAGIC:
            raise ValueError("Incorrect/corrupt boot.bin file")

        return self


    def writeToFile(self, file:(str,BinaryFile), chunkSize:int=4096):
        """Write this file's content to disk."""
        if type(file) is str: file = BinaryFile(file, 'wb')
        file.writeString(self.gameCode, length=4)
        file.writeString(self.makerCode, length=2)
        file.writeu8(self.discNo)
        file.writeu8(self.version)
        file.writeu8(1 if self.audioStreaming else 0)
        file.writeu8(self.streamBufSize)
        file.padUntil(0x1C)
        file.writeu32(self.magic)
        file.writeString(self.gameName, length=0x3E0)
        file.padUntil(0x400)
        file.writeu32(self.debugMonitorOffs)
        file.writeu32(self.debugMonitorAddr)
        file.padUntil(0x420)
        file.writeu32(self.mainDolOffs)
        file.writeu32(self.fstOffs)
        file.writeu32(self.fstSize)
        file.writeu32(self.maxFstSize)
        file.writeu32(self.fstAddr)
        file.writeu32(self.fileOffset)
        file.writeu32(self.fileLength)
        file.padUntil(0x440)



    def dump(self):
        """Dump to console."""
        print("boot.bin @ 0x%08X:" % self.offset)
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
        print(" FST Address:          0x%08X" % self.fstAddr)
        print(" FST Size:             0x%08X" % self.fstSize)
        print(" FST Max Size:         0x%08X" % self.maxFstSize)
        print(" File Offset:          0x%08X" % self.fileOffset)
        print(" User Data Size:       0x%08X" % self.fileLength)


    def setGameName(self, name:str):
        """Set the game's name."""
        # XXX is this correct encoding?
        if len(bytes(name, 'shift-jis')) >= 0x3E0:
            raise ValueError("Name too long")
        self.gameName = name
