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
        'curSeq':   (">h",  0xB4),
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
            if name in ("pNext", "parent", "pC8", "pCC") and val != 0:
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
        "avail":    ('>I', 0x00),
        "used":     ('>I', 0x04),
        "data":     ('>I', 0x08),
        "size":     ('>I', 0x0C),
        "usedSize": ('>I', 0x10),
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


class ModelFileHeader(Struct):
    SIZE = 0xFC
    _fields = {
        "refCount":      ('>B',  0x00),
        "unk01":         ('>B',  0x01),
        "flags":         ('>H',  0x02),
        "ID":            ('>H',  0x04),
        "unk06":         ('>H',  0x06),
        "unk08":         ('>I',  0x08),
        "fileSize":      ('>I',  0x0C),
        "unk10":         ('>2I', 0x10),
        "unk18":         ('>I',  0x18), # animation related
        "extraAmapSize": ('>I',  0x1C),
        "pTextures":     ('>I',  0x20),
        "flags24":       ('>4B', 0x24),
        "pVtxs":         ('>I',  0x28), # vec3s*
        "pNormals":      ('>I',  0x2C), # vec3s*
        "pColors":       ('>I',  0x30),
        "pTexCoords":    ('>I',  0x34),
        "pMaterials":    ('>I',  0x38),
        "pBones":        ('>I',  0x3C),
        "pBoneQuats":    ('>I',  0x40),
        "unk44":         ('>4I', 0x44),
        "pVtxGroups":    ('>I',  0x54),
        "pHitBoxes":     ('>I',  0x58),
        "unk5C":         ('>2I', 0x5C),
        "pAltIndBuf":    ('>I',  0x64),
        "pAnimBuf":      ('>I',  0x68),
        "pModelIndBuf":  ('>I',  0x6C),
        "animIdxs":      ('>8h', 0x70),
        "amapTabEntry":  ('>I',  0x80),
        "unk84":         ('>I',  0x84),
        "unk88":         ('>4I', 0x88), # astruct_54
        "unk98":         ('>3I', 0x98),
        "unkA4":         ('>2I', 0xA4), # anim related
        "unkAC":         ('>2H', 0xAC),
        "unkB0":         ('>6I', 0xB0),
        "unkC8":         ('>I',  0xC8), # if nonzero, copy normals on load
        "unkCC":         ('>I',  0xCC),
        "pDlists":       ('>I',  0xD0),
        "pRenderInstrs": ('>I',  0xD4),
        "nRenderInstrs": ('>H',  0xD8),
        "unkDA":         ('>H',  0xDA),
        "unkDC":         ('>I',  0xDC),
        "unkE0":         ('>H',  0xE0),
        "flagsE2":       ('>H',  0xE2),
        "nVtxs":         ('>H',  0xE4),
        "nNormals":      ('>H',  0xE6),
        "nColors":       ('>H',  0xE8),
        "nTexCoords":    ('>H',  0xEA),
        "nAnimations":   ('>H',  0xEC),
        "unkEE":         ('>2H', 0xEE),
        "nTextures":     ('>B',  0xF2),
        "nBones":        ('>B',  0xF3),
        "nVtxGroups":    ('>B',  0xF4),
        "nDlists":       ('>B',  0xF5),
        "unkF6":         ('>B',  0xF6),
        "nHitBoxes":     ('>B',  0xF7),
        "nMaterials":    ('>B',  0xF8),
        "nUnkDC":        ('>B',  0xF9),
        "nTexMtxs":      ('>B',  0xFA),
        "unkFB":         ('>B',  0xFB),
    }


class Model(Struct):
    SIZE = 0x64
    _fields = {
        "pHeader":      ('>I',  0x00), # ModelFileHeader*
        "unk04":        ('>2I', 0x04),
        "pMtxs":        ('>2I', 0x0C),
        "unk14":        ('>I',  0x14),
        "flags18":      ('>H',  0x18),
        "unk1A":        ('>H',  0x1A),
        "pVtxs":        ('>2I', 0x1C), # vec3s*
        "pNormals":     ('>I',  0x24),
        "pAnim":        ('>3I', 0x28),
        "pTextures":    ('>I',  0x34),
        "pFunc38":      ('>I',  0x38),
        "unk3C":        ('>I',  0x3C),
        "pAnimBuf":     ('>I',  0x40),
        "unk44":        ('>I',  0x44),
        "pBuffers":     ('>3I', 0x48),
        "unk54":        ('>2I', 0x54),
        "pMtxBufs":     ('>I',  0x5C),
        "bTransformed": ('>B',  0x60),
        "unk61":        ('>3B', 0x61),
    }

    def __init__(self, client, addr):
        super().__init__(client, addr)
        if self.pHeader == 0:
            self.header = None
            self.ID = "<unknown>"
        else:
            self.header = ModelFileHeader(self.client, self.pHeader)
            self.ID = hex(self.header.ID)

    def printSelf(self):
        printf("Model ID %s\n", self.ID)
        printf("  Mtxs:    %4d @ 0x%08X,0x%08X, bufs: 0x%08X\n",
            self.header.nBones + self.header.nVtxGroups,
            self.pMtxs[0], self.pMtxs[1],
            self.pMtxBufs) # pMtxBufs is same as pMtxs[0] ?
        printf("  Vtxs:    %4d @ 0x%08X,0x%08X\n", self.header.nVtxs, *self.pVtxs)
        printf("  Normals: %4d @ 0x%08X\n", self.header.nNormals, self.pNormals)
        printf("  Func 38: 0x%08X\n", self.pFunc38)
        printf("  Anims:   0x%08X 0x%08X 0x%08X, buf 0x%08X\n",
            self.pAnim[0], self.pAnim[1], self.pAnim[2],
            self.pAnimBuf)
        printf("  Flags:   0x%04X  Transformed: 0x%02X %02X %02X %02X\n",
            self.flags18,
            self.bTransformed, *self.unk61)
        printf("  Unk:     04=%08X %08X 14=%08X 1A=%04X 3C=%04X 44=%08X 54=%08X %08X\n",
            self.unk04[0], self.unk04[1],
            self.unk14, self.unk1A, self.unk3C, self.unk44,
            self.unk54[0], self.unk54[1])
        printf("  Header @ 0x%08X:\n", self.pHeader)
        printf("    refCount=%2d fileSize=0x%08X\n",
            self.header.refCount, self.header.fileSize)
        printf("    flags=0x%04X 24=%02X %02X %02X %02X E2=%04X\n",
            self.header.flags,
            self.header.flags24[0], self.header.flags24[1],
            self.header.flags24[2], self.header.flags24[3],
            self.header.flagsE2)
        printf("    pAltIndBuf=0x%08X pAnimBuf=0x%08X\n",
            self.header.pAltIndBuf, self.header.pAnimBuf)
        printf("    animIdxs=%s\n", self.header.animIdxs)
        printf("    amapTabEntry=0x%08X extraSize=0x%08X\n",
            self.header.amapTabEntry, self.header.extraAmapSize)
        printf("    nVtxs=%4d nNormals=%4d nColors=%4d nTexCoords=%4d\n",
            self.header.nVtxs, self.header.nNormals,
            self.header.nColors, self.header.nTexCoords)
        printf("    nAnimations=%4d nTextures=%3d nBones=%3d nVtxGroups=%3d\n",
            self.header.nAnimations, self.header.nTextures,
            self.header.nBones, self.header.nVtxGroups)
        printf("    nDlists=%3d nHitBoxes=%3d nMaterials=%3d\n",
            self.header.nDlists, self.header.nHitBoxes,
            self.header.nMaterials)
        printf("    nUnkDC=%3d nTexMtxs=%3d\n",
            self.header.nUnkDC, self.header.nTexMtxs)
        printf("    Unk: 01=%02X 06=%04X 08=%08X 10=%08X %08X\n",
            self.header.unk01, self.header.unk06,
            self.header.unk08, self.header.unk10[0], self.header.unk10[1])
        printf("      18=%08X 84=%08X 88=%08X %08X %08X %08X\n",
            self.header.unk18, self.header.unk84, *self.header.unk88)
        printf("      98=%08X %08X %08X\n", *self.header.unk98)
        printf("      A4=%08X %08X AC=%02X %02X\n",
            self.header.unkA4[0], self.header.unkA4[1],
            self.header.unkAC[0], self.header.unkAC[1])
        printf("      B0=%08X %08X %08X %08X %08X %08X\n",
            *self.header.unkB0)
        printf("      C8=%08X CC=%08X DA=%04X E0=%04X EE=%04X %04X\n",
            self.header.unkC8, self.header.unkCC, self.header.unkDA,
            self.header.unkE0, self.header.unkEE[0], self.header.unkEE[1])
        printf("      F6=%02X FB=%02X\n",
            self.header.unkF6, self.header.unkFB)
