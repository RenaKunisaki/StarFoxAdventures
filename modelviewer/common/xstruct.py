import logging; log = logging.getLogger(__name__)
import struct

class Struct:
    """Class wrapping struct unpacking."""

    _order = ''
    def __init__(self, data):
        self._raw = data
        for name, field in self._fields.items():
            fmt, offs = field
            if type(fmt) is str:
                r = struct.unpack_from(self._order + fmt, data, offs)
                if len(r) == 1: r = r[0] # grumble
                if type(r) is bytes:
                    r = r.decode('ascii')
                    lol = r.find("\x00")
                    if lol > 0: r = r[0:lol]
            else: # should be a Struct
                r = fmt(data[offs:])
            setattr(self, name, r)


    def __repr__(self):
        res = []
        fields = []
        for name, field in self._fields.items():
            fields.append((field[1], name))
        for field in sorted(fields, key=lambda f: f[0]):
            name = field[1]
            res.append('%s=%s' % (name, getattr(self, name, None)))
        return '%s(%s)' % (type(self).__name__, ', '.join(res))


    @classmethod
    def readFromFile(cls, file, offset=None, count=None):
        if offset is not None: file.seek(offset)
        if count is None:
            data = file.read(cls._size)
            return cls(data)
        else:
            #log.debug("Read %d x %s from 0x%X", count, cls.__name__, file.tell())
            data = file.read(cls._size * count)
            res  = []
            offs = 0
            for i in range(count):
                obj = cls(data[offs:])
                setattr(obj, '_idx', i)
                res.append(obj)
                offs += cls._size
            return res
