import struct

class BinaryFile:
    def __init__(self, path, byteOrder='>', offset=0):
        self.path = path
        self.file = open(path, 'rb')
        self.byteOrder = byteOrder
        self.offset = offset
        self.file.seek(0, 2)
        self.size = self.file.tell() - offset
        self.file.seek(offset)

    def seek(self, offset, origin=0):
        self.file.seek(offset+self.offset, origin)

    def _readFmt(self, fmt, length, offset=None):
        if offset is not None: self.seek(offset)
        r = struct.unpack(self.byteOrder + fmt, self.file.read(length))
        if len(r) == 1: return r[0] # grumble
        return r

    def reads8(self, offset=None):
        return self._readFmt('b', 1, offset)

    def readu8(self, offset=None):
        return self._readFmt('B', 1, offset)

    def readChar(self, offset=None):
        return self._readFmt('c', 1, offset)

    def reads16(self, offset=None):
        return self._readFmt('h', 2, offset)

    def readu16(self, offset=None):
        return self._readFmt('H', 2, offset)

    def reads32(self, offset=None):
        return self._readFmt('i', 4, offset)

    def readu32(self, offset=None):
        return self._readFmt('I', 4, offset)

    def reads64(self, offset=None):
        return self._readFmt('q', 8, offset)

    def readu64(self, offset=None):
        return self._readFmt('Q', 8, offset)

    def readFloat(self, offset=None):
        return self._readFmt('f', 4, offset)

    def readDouble(self, offset=None):
        return self._readFmt('d', 8, offset)

    def readStruct(self, fmt, offset=None):
        r = self._readFmt(fmt, struct.calcsize(fmt), offset)
        if len(r) == 1: return r[0] # grumble
        return r

    def readString(self, offset=None, maxLen=1000000):
        r = []
        if offset is not None: self.seek(offset)
        while True:
            b = self.reads8()
            if b == 0: break
            r.append(chr(b))
        return ''.join(r)

    def readBytes(self, count=None, offset=None):
        if offset is not None: self.seek(offset)
        return self.file.read(count)
