import struct
from util import printf

class Struct:
    def __init__(self, client, addr):
        self.client = client
        data = client.read(addr, self.SIZE)
        self._raw = data
        for name, field in self._fields.items():
            fmt, offs = field
            if type(fmt) is str:
                r = struct.unpack_from(fmt, data, offs)
                if len(r) == 1: r = r[0] # grumble
                if type(r) is bytes:
                    r = r.decode('ascii')
                    lol = r.find("\x00")
                    if lol > 0: r = r[0:lol]
            else: # should be a Struct
                p = struct.unpack_from('>I', data, offs)
                r = fmt(client, p[0])
            setattr(self, name, r)

    def printSelf(self):
        for name in self._fields.keys():
            val = getattr(self, name)
            if type(val) is int: val = hex(val)
            printf("%-10s: %s\n", name, val)


class ObjectFileStruct(Struct):
    SIZE = 0xA0
    _fields = {
        "unk00":   ('>2f', 0x00),
        "flags44": ('>I',  0x44),
        "nModels": ('B',   0x55),
        "nVecs":   ('B',   0x5A),
        "flags5F": ('B',   0x5F),
        "vecs":    ('>9f', 0x6C),
        "name":    ('11s', 0x91),
    }

    def __init__(self, client, addr):
        super().__init__(client, addr)
        vecs = []
        for i in range(self.nVecs):
            vecs.append(self.vecs[i*3: (i*3)+4])
        self.vecs = vecs


class GameObject(Struct):
    SIZE = 0x130
    _fields = { # name => (fmt, offset)
        'rotation': (">3H", 0x00),
        'flags06':  (">B",  0x06),
        'scale':    (">f",  0x08),
        'pos':      (">3f", 0x0C),
        'prevPos':  (">3f", 0x18),
        'vel':      (">3f", 0x24),
        'pMatrix':  (">I",  0x30),
        'pNext':    (">I",  0x38),
        'camVar':   (">2f", 0x3C),
        'objId':    (">h",  0x44),
        'defNo':    (">h",  0x46),
        'seq':      (">I",  0x4C),
        'pFile':    (">I",  0x50),
        'p54':      (">I",  0x54),
        'p58':      (">I",  0x58),
        'p5C':      (">I",  0x5C),
        'p60':      (">I",  0x60),
        'shadow':   (">I",  0x64),
        'fPtrs':    (">I",  0x68),
        'p6C':      (">I",  0x6C),
        'p70':      (">I",  0x70),
        'p74':      (">I",  0x74),
        'p78':      (">I",  0x78),
        'models':   (">I",  0x7C),
        'v80':      (">3f", 0x80),
        'v8C':      (">3f", 0x8C),
        'fA8':      (">f",  0xA8),
        'bAC':      (">B",  0xAC),
        'curModel': (">B",  0xAD),
        'slot':     (">B",  0xAE),
        'flagsAF':  (">3B", 0xAF),
        'parent':   (">I",  0xC4),
        'pC8':      (">I",  0xC8),
        'pCC':      (">I",  0xCC),
        'nChildren':(">B",  0xEB),
    }

    def __init__(self, client, addr):
        super().__init__(client, addr)
        if self.pFile == 0:
            self.file = None
            self.name = "<unknown>"
        else:
            self.file = ObjectFileStruct(self.client, self.pFile)
            self.name = self.file.name

    def printSelf(self):
        for name in self._fields.keys():
            val = getattr(self, name)
            if name in ("pNext", "parent", "pC8", "pCC"):
                ptr = val
                val = GameObject(self.client, val)
                if val is None: val = "<none>"
                else: val = val.name
                val = "%08X: %s" % (ptr, val)
            if type(val) is int: val = hex(val)
            printf("%-10s: %s\n", name, val)


class HeapStruct(Struct):
    SIZE = 0x14
    _fields = {
        "size":     ('>I', 0x00),
        "avail":    ('>I', 0x04),
        "used":     ('>I', 0x08),
        "data":     ('>I', 0x0C),
        "dataSize": ('>I', 0x10),
    }

class HeapEntry(Struct):
    SIZE = 0x1C
    _fields = {
        "loc":   ('>I',  0x00),
        "size":  ('>I',  0x04),
        "unk08": ('>H',  0x08),
        "idx":   ('>H',  0x0A),
        "stack": ('>2H', 0x0C),
        "tag":   ('>I',  0x10),
        "unk14": ('>I',  0x14),
        "unk18": ('>I',  0x18),
    }

    _tags = {
        0x00000005: "Map Blocks",
        0x00000006: "Texture",
        0x00000009: "Model Data",
        0x0000000A: "Models",
        0x0000000B: "MaybeAudio",
        0x0000000E: "Objects",
        0x00000010: "VOX",
        0x00000011: "Stack",
        0x00000017: "Texture Ptrs",
        0x00000018: "Vec3f Array",
        0x0000001A: "ModelStruct",
        0x000000FF: "Unknown 32-byte buffer",
        0x7D7D7D7D: "Data File",
        0x7F7F7FFF: "Compressed File",
        0xFFFF00FF: "Unknown, Map-related",
        0xFFFFFFFF: "Savegame",
    }


class Game:
    """Connection to SFA."""
    def __init__(self, client):
        self.client = client

    def showObject(self, addr):
        obj = GameObject(self.client, addr)
        obj.printSelf()

    def listLoadedObjects(self):
        # read ptr to object list
        cnt, ptr = self.client.read(0x803dcb84, ">II")
        if ptr == 0:
            print("objPtr is NULL")
            return
        printf("objPtr = %08X nObjs = %d\n", ptr, cnt)
        printf("\x1B[1mObj│Address │Name       │ID  │Def │ModelPtr│XPos     │YPos     │ZPos     │Ch\x1B[0m\n")
        for i in range(cnt):
            pObj = self.client.read(ptr + (i*4), ">I")
            if pObj == 0:
                printf("NULL at object %d\n", i)
                break

            obj = GameObject(self.client, pObj)
            printf("\x1B[48;5;%dm%3d│%08X│%-11s│%04X│%04X│%08X│%+9.2f│%+9.2f│%+9.2f│%2d\x1B[0m\n",
                19 if (i & 1 == 0) else 26,
                i, pObj, obj.name,
                obj.objId, obj.defNo, obj.models, obj.pos[0], obj.pos[1], obj.pos[2],
                obj.nChildren)

    def showHeap(self):
        printf("\x1B[1m#│Size    │Avail   │Used    │Data    │DataSize\x1B[0m\n")
        heaps = []
        for i in range(5):
            hs = HeapStruct(self.client, 0x8034069C + (i*HeapStruct.SIZE))
            heaps.append(hs)
            printf("\x1B[48;5;%dm%d│%08X│%08X│%08X│%08X│%08X\x1B[0m\n",
                19 if (i & 1 == 0) else 26, i,
                hs.size, hs.avail, hs.used, hs.data, hs.dataSize)

        for i, heap in enumerate(heaps):
            if heap.data != 0:
                printf("\n\x1B[1mHeap %d:\x1B[0m\n", i)
                printf("\x1B[1mIdx │Address │Size    │Unk8│Idx │Stack   │Unk14   │Unk18   │Tag\x1B[0m\n")
                for j in range(heap.used):
                    entry = HeapEntry(self.client, heap.data + (j * HeapEntry.SIZE))
                    printf("\x1B[48;5;%dm%4d│%08X│%08X│%04X│%04X│%04X%04X│%08X│%08X│%08X %s\x1B[0m\n",
                    19 if (j & 1 == 0) else 26, j,
                    entry.loc, entry.size, entry.unk08, entry.idx,
                    entry.stack[0], entry.stack[1], entry.unk14, entry.unk18,
                    entry.tag, HeapEntry._tags.get(entry.tag, "<unknown>"))

            else:
                printf("\x1B[1mHeap %d: \x1B[0mnull\n", i)
