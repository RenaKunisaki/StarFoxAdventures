import struct

class BinaryFile:
    def __init__(self, path, mode, byteOrder='>', offset=0):
        self.path = path
        self.mode = mode
        self.file = open(path, mode)
        self.byteOrder = byteOrder
        self.offset = offset
        self.file.seek(0, 2)
        self.size = self.file.tell() - offset
        self.file.seek(offset)

    def seek(self, offset, origin=0):
        self.file.seek(offset+self.offset, origin)

    def tell(self):
        return self.offset + self.file.tell()

    def read(self, size):
        return self.file.read(size)

    def _readFmt(self, fmt, offset=None):
        if offset is not None: self.seek(offset)
        length = struct.calcsize(fmt)
        r = struct.unpack(self.byteOrder + fmt, self.file.read(length))
        if len(r) == 1: return r[0] # grumble
        return r

    def reads8(self, count=1, offset=None):
        return self._readFmt('%db' % count, offset)

    def readu8(self, count=1, offset=None):
        return self._readFmt('%dB' % count, offset)

    def readChar(self, count=1, offset=None):
        return self._readFmt('%dc' % count, offset)

    def reads16(self, count=1, offset=None):
        return self._readFmt('%dh' % count, offset)

    def readu16(self, count=1, offset=None):
        return self._readFmt('%dH' % count, offset)

    def reads32(self, count=1, offset=None):
        return self._readFmt('%di' % count, offset)

    def readu32(self, count=1, offset=None):
        return self._readFmt('%dI' % count, offset)

    def reads64(self, count=1, offset=None):
        return self._readFmt('%dq' % count, offset)

    def readu64(self, count=1, offset=None):
        return self._readFmt('%dQ' % count, offset)

    def readFloat(self, count=1, offset=None):
        return self._readFmt('%df' % count, offset)

    def readDouble(self, count=1, offset=None):
        return self._readFmt('%dd' % count, offset)

    def readStruct(self, fmt, offset=None):
        r = self._readFmt(fmt, offset)
        if len(r) == 1: return r[0] # grumble
        return r

    def readString(self, offset=None, maxLen=1000000):
        r = []
        if offset is not None: self.seek(offset)
        while len(r) < maxLen:
            b = self.file.read(1)
            if b == b'' or b[0] == 0: break
            r.append(chr(b[0]))
        return ''.join(r)

    def readBytes(self, count=None, offset=None):
        if offset is not None: self.seek(offset)
        return self.file.read(count)


    def write(self, buffer):
        return self.file.write(buffer)

    def _writeFmt(self, fmt, *vals, offset=None):
        if offset is not None: self.seek(offset)
        buf = struct.pack(self.byteOrder+fmt, *vals)
        return self.write(buf)

    def writes8(self, *vals, offset=None):
        return self._writeFmt('%db' % len(vals), *vals, offset=offset)

    def writeu8(self, *vals, offset=None):
        return self._writeFmt('%dB' % len(vals), *vals, offset=offset)

    def writeChar(self, *vals, offset=None):
        return self._writeFmt('%dc' % len(vals), *vals, offset=offset)

    def writes16(self, *vals, offset=None):
        return self._writeFmt('%dh' % len(vals), *vals, offset=offset)

    def writeu16(self, *vals, offset=None):
        return self._writeFmt('%dH' % len(vals), *vals, offset=offset)

    def writes32(self, *vals, offset=None):
        return self._writeFmt('%di' % len(vals), *vals, offset=offset)

    def writeu32(self, *vals, offset=None):
        return self._writeFmt('%dI' % len(vals), *vals, offset=offset)

    def writes64(self, *vals, offset=None):
        return self._writeFmt('%dq' % len(vals), *vals, offset=offset)

    def writeu64(self, *vals, offset=None):
        return self._writeFmt('%dQ' % len(vals), *vals, offset=offset)

    def writeFloat(self, *vals, offset=None):
        return self._writeFmt('%df' % len(vals), *vals, offset=offset)

    def writeDouble(self, *vals, offset=None):
        return self._writeFmt('%dd' % len(vals), *vals, offset=offset)

    def writeStruct(self, fmt, *vals, offset=None):
        return self._writeFmt(fmt, *vals, offset=offset)

    def writeString(self, data, offset=None, encoding='utf-8', length=None, padding=b'\0'):
        r = []
        if offset is not None: self.seek(offset)
        if type(data) is str: raw = bytes(data, encoding)
        else: raw = data
        if length is not None:
            if len(raw) > length: raise ValueError("String too long for field (%d bytes, max is %d): %s" % (
                len(raw), length, data))
            elif len(raw) < length:
                assert len(padding) > 0, "padding is empty"
                pLen = length - len(raw)
                raw = raw + (padding * (pLen // len(padding)))
                # XXX this may fail if padding length is not a multiple
                # of the actual amount of padding needed. do we care?
        return self.writeBytes(raw)

    def writeBytes(self, data, offset=None):
        if offset is not None: self.seek(offset)
        return self.file.write(data)

    def writePadding(self, length, offset=None, padding=b'\0'):
        if offset is not None: self.seek(offset)
        assert len(padding) > 0, "padding is empty"
        val = padding * (length // len(padding))
        # XXX this may fail if padding length is not a multiple
        # of the actual amount of padding needed. do we care?
        return self.file.write(val)

    def padUntil(self, offset, padding=b'\0'):
        cur = self.tell()
        if cur >= offset: return
        return self.writePadding(offset - cur, padding=padding)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        pass # XXX is this correct?

    def __str__(self):
        return "<BinaryFile(%s) @ 0x%x>" % (self.path, id(self))
