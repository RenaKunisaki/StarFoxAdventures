try: import lzo
except ModuleNotFoundError: lzo = None
import struct
from binaryfile import BinaryFile

class DPLZO:
    """Handle LZO files used in `default.dol`."""
    def __init__(self, file:BinaryFile):
        self.file = file

    def decompress(self):
        assert lzo is not None, "Python lzo module not installed"
        sig = self.file.readStruct('4s')
        if sig == b"LZO\0":
            version = self.file.readu32()
            decLen  = self.file.readu32()
            compLen = self.file.readu32()
        else:
            raise TypeError("Not a LZO file (header: " + str(sig) + ")")
        #result = bytearray(decLen)
        compData = self.file.readBytes(compLen)
        result = lzo.decompress(compData)
        assert len(result) == decLen
        return result

    def compress(self, rawData=None):
        assert lzo is not None, "Python lzo module not installed"
        if rawData is None:
            rawData = self.file.readBytes()
        data = lzo.compress(rawData, 1)
        #dOut = []
        #for i in range(0, len(data), 4):
        #    b = data[i:i+4]
        #    while len(b) < 4: b += b'\0'
        #    dOut.append(b[::-1])
        #data = b''.join(dOut)

        return (
            b'LZO\0' + b'\0\0\0\x01' + # magic, version
            struct.pack('>II', len(rawData), len(data)) +
            data)
