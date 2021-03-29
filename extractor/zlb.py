import zlib
import struct
from binaryfile import BinaryFile

# XXX version can be "DIR\0"

class Zlb:
    def __init__(self, file:BinaryFile):
        self.file = file

    def decompress(self):
        sig  = self.file.readStruct('4s')
        offs = self.file.tell()
        if sig == b"\xFA\xCE\xFE\xED": # wrapper used by game
            header = self.file.readStruct('8I')
            decLen, zlbDataOffs, compLen = header[0:3]
            # other fields unknown
            compLen += 0x10
            zlbDataOffs = (zlbDataOffs*4) + offs
            self.file.seek(zlbDataOffs)
            d = self.file.read(4)
            self.file.seek(zlbDataOffs)
            #print("FACEFEED offs=%X dec=%X len=%X -> %X %s" % (
            #    zlbDataOffs, decLen, compLen, self.file.tell(), d.hex()))
        elif sig == b"ZLB\0":
            version = self.file.readu32()
            decLen  = self.file.readu32()
            compLen = self.file.readu32()
        elif sig[0:3] == b'DIR': # not compressed
            version = self.file.readu32()
            decLen  = self.file.readu32()
            compLen = self.file.readu32()
            unk10   = self.file.readu32()
            unk14   = self.file.readu32()
            unk18   = self.file.readu32()
            unk1C   = self.file.readu32()
            #print("DIR%s v=%08X len=%08X %08X unk=%08X %08X %08X %08X" % (
            #    chr(sig[3]), version, decLen, compLen,
            #    unk10, unk14, unk18, unk1C))
            return self.file.readBytes(decLen)
        else:
            raise TypeError("Not a ZLB file (header: %s, offset: 0x%08X)" % (
                sig.hex(), offs - 4))
        #print("ZLB version", version, "decLen", decLen, "compLen", compLen)
        # this can happen when a very small file is compressed
        #if decLen < compLen:
        #    raise RuntimeError("ZLB: Output size < compressed size")

        #result = bytearray(decLen)
        compData = self.file.readBytes(compLen)
        result = zlib.decompress(compData)
        assert len(result) == decLen
        return result

    def compress(self, rawData=None):
        if rawData is None:
            rawData = self.file.readBytes()
        comp = zlib.compressobj(level=9,wbits=13)
        data = comp.compress(rawData) + comp.flush()
        return (
            b'ZLB\0' + b'\0\0\0\x01' + # magic, version
            struct.pack('>II', len(rawData), len(data)) +
            data)
