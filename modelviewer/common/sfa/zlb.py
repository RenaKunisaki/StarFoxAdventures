import zlib
import struct
from common.BinaryFile import BinaryFile

# XXX version can be "DIR\0"

class Zlb:
    def __init__(self, file:BinaryFile):
        self.file = file

    def decompress(self):
        sig = self.file.readStruct('4s')
        if sig == b"\xFA\xCE\xFE\xED": # wrapper used by game
            offs   = self.file.tell()
            header = self.file.readStruct('8I')
            decLen, zlbDataOffs, compLen = header[0:3]
            # other fields unknown
            compLen += 0x10
            self.file.seek(zlbDataOffs)
            print("FACEFEED offs=%X dec=%X len=%X -> %X" % (
                zlbDataOffs, decLen, compLen, self.file.tell()))
        elif sig == b"ZLB\0":
            version = self.file.readu32()
            decLen  = self.file.readu32()
            compLen = self.file.readu32()
        else:
            raise TypeError("Not a ZLB file (header: " + str(sig) + ")")
        #print("ZLB version", version, "decLen", decLen, "compLen", compLen)
        # this can happen when a very small file is compressed
        #if decLen < compLen:
        #    raise RuntimeError("ZLB: Output size < compressed size")

        #result = bytearray(decLen)
        compData = self.file.readBytes(compLen)
        result = zlib.decompress(compData)
        assert len(result) == decLen
        return result

    def compress(self):
        rawData = self.file.readBytes()
        comp = zlib.compressobj(level=9,wbits=13)
        data = comp.compress(rawData) + comp.flush()
        return (
            b'ZLB\0' + b'\0\0\0\x01' + # magic, version
            struct.pack('>II', len(rawData), len(data)) +
            data)
