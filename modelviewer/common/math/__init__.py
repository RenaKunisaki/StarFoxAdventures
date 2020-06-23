from common.xstruct import Struct

class Vec2s(Struct):
    _size   = 0x04
    _order  = '>'
    _fields = {
        'x': ('h', 0x00),
        'y': ('h', 0x02),
    }

class Vec3s(Struct):
    _size   = 0x06
    _order  = '>'
    _fields = {
        'x': ('h', 0x00),
        'y': ('h', 0x02),
        'z': ('h', 0x04),
    }

class Vec3f(Struct):
    _size   = 0x0C
    _order  = '>'
    _fields = {
        'x': ('f', 0x00),
        'y': ('f', 0x04),
        'z': ('f', 0x08),
    }

class Quaternion(Struct):
    _size   = 0x10
    _order  = '>'
    _fields = {
        'x': ('f', 0x00),
        'y': ('f', 0x04),
        'z': ('f', 0x08),
        'w': ('f', 0x0C),
    }

class Mtx43(Struct):
    _size   = 0x30
    _order  = '>'
    _fields = {
        'm11': ('f', 0x00),
        'm12': ('f', 0x04),
        'm13': ('f', 0x08),
        'm14': ('f', 0x0C),
        'm21': ('f', 0x10),
        'm22': ('f', 0x14),
        'm23': ('f', 0x18),
        'm24': ('f', 0x1C),
        'm31': ('f', 0x20),
        'm32': ('f', 0x24),
        'm33': ('f', 0x28),
        'm34': ('f', 0x2C),
    }

class Mtx44(Struct):
    _size   = 0x40
    _order  = '>'
    _fields = {
        'm11': ('f', 0x00),
        'm12': ('f', 0x04),
        'm13': ('f', 0x08),
        'm14': ('f', 0x0C),
        'm21': ('f', 0x10),
        'm22': ('f', 0x14),
        'm23': ('f', 0x18),
        'm24': ('f', 0x1C),
        'm31': ('f', 0x20),
        'm32': ('f', 0x24),
        'm33': ('f', 0x28),
        'm34': ('f', 0x2C),
        'm41': ('f', 0x30),
        'm42': ('f', 0x34),
        'm43': ('f', 0x38),
        'm44': ('f', 0x3C),
    }
