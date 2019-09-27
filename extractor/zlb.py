import zlib
from binaryfile import BinaryFile

# XXX version can be "DIR\0"

class ZlbDecoder:
    def __init__(self, file:BinaryFile):
        self.file = file

    def decompress(self):
        sig = self.file.readStruct('4s')
        if sig != b"ZLB\0":
            raise TypeError("Not a ZLB file (header: " + str(sig) + ")")
        version = self.file.readu32()
        decLen  = self.file.readu32()
        compLen = self.file.readu32()
        print("ZLB version", version, "decLen", decLen, "compLen", compLen)
        if decLen < compLen:
            raise RuntimeError("ZLB: Output size < compressed size")

        #result = bytearray(decLen)
        compData = self.file.readBytes(compLen)
        result = zlib.decompress(compData)
        assert len(result) == decLen
        return result
