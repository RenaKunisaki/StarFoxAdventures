from .xstruct import Struct

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

class Bone(Struct):
    _size   = 0x1C
    _order  = '>'
    _fields = {
        'parent': ('B',   0x00), # high bit is a flag?
        'unk01':  ('B',   0x01), # idx to write to?
        'unk02':  ('B',   0x02), # idx -> something 0x40 bytes (Mtx44?)
        'unk03':  ('B',   0x03), # idx -> something 0x20 bytes
        'head':   (Vec3f, 0x04),
        'tail':   (Vec3f, 0x10),
    }
    # unk01, 02, 03 are all the same except 01 sometimes has high bit set
    # they seem to always be the bone's index
    # related to pAnimBuf; see modelAnimFn_80024524
    # 02 and 03 get updated every frame
    # 01 seems to be which mtx to use, with the high bit indicating
    # whether to write it back.

class ModelVtxGroup(Struct):
    _size   = 4
    _order  = '>'
    _fields = {
        'bone0':  ('B', 0x00),
        'bone1':  ('B', 0x01), # weight is 1.0 - bone0 weight
        'weight': ('B', 0x02), # for bone0
        'unk03':  ('B', 0x03), # always 0; padding?
    }

class DisplayListPtr(Struct):
    _size   = 0x1C
    _order  = '>'
    _fields = {
        'offset': ('I', 0x00),
        'size':   ('H', 0x04),
        # 'specialBitAddr': ???
        # bounding box in here somewhere?
        'unk':    ('22B', 0x06),
    }

class ShaderColor(Struct):
    _size   = 8
    _order  = '>'
    _fields = {
        'flags0': ('B',  0x00),
        'color':  ('3B', 0x04), # not plain RGB
    }

class Shader(Struct):
    _size   = 0x44
    _order  = '>'
    _fields = {
        'color0':    (ShaderColor, 0x00),
        'color1':    (ShaderColor, 0x08),
        'color2':    (ShaderColor, 0x10),
        'color3':    (ShaderColor, 0x18),
        'alpha':     ('B',  0x22),
        'unk24':     ('4I', 0x24),
        'textureId': ('2i', 0x34),
        'flags':     ('I',  0x3C),
        'flags_40':  ('B',  0x40),
        'nColors':   ('B',  0x41),
    }

class HitboxStruct(Struct):
    _size   = 0x18
    _order  = '>'
    _fields = {
        'unk00': ('I',  0x00),
        'unk04': ('4f', 0x04),
        'unk14': ('I',  0x14),
    }

class ModelStruct(Struct):
    _size   = 0xFC
    _order  = '>'
    _fields = {
        'flags':         ('H', 0x02),
        #'modelId':       ('H', 0x04), # set on load
        'fileSize':      ('I', 0x0C),
        'unk18':         ('I', 0x18),
        'extraAmapSize': ('I', 0x1C),
        'textures':      ('I', 0x20), # -> texture IDs (u32)
        'flags24':       ('B', 0x24),
        'unk25':         ('B', 0x25), # relates to lighting
        'vtxs':          ('I', 0x28), # -> vec3s
        'normals':       ('I', 0x2C), # -> vec3s
        'colors':        ('I', 0x30), # -> ??? (how many?)
        'texCoords':     ('I', 0x34), # -> vec2s
        'shaders':       ('I', 0x38), # -> Shader
        'bones':         ('I', 0x3C), # -> Bone
        'boneQuats':     ('I', 0x40), # -> Quaternion
        'unk44':         ('3I',0x44), # 0xFFC00000 x3; never read
        'unk50':         ('I', 0x50),
        'vtxGroups':     ('I', 0x54), # -> ModelVtxGroup
        'hitboxes':      ('I', 0x58), # -> HitboxStruct
        'pAltIndBuf':    ('I', 0x64), # -> ???
        'pAnimBuf':      ('I', 0x68), # -> ???
        'pModelIndBuf':  ('I', 0x6C), # -> array of u16 anim IDs - null in file, loaded from MODANIM.TAB
        'animIdxs':      ('8h',0x70), # related to animation - FFxx disables walk anim - 0x0, 0xFD, 0x112, 0x197, 0x218, 0x29B, 0x0, 0x0
        'amapTabEntry':  ('I', 0x80),
        'unk84':         ('I', 0x84),
        #'offs88':        ('I', 0x88), # astruct_54 (not a ptr)
        'unkA4':         ('I', 0xA4), # related to movement/mtxs/texmtxs
        'dlists':        ('I', 0xD0), # -> DisplayListPtr
        'renderInstrs':  ('I', 0xD4), # -> bit-packed instruction code
        'nRenderInstrs': ('H', 0xD8), # num bytes
        'offsDC':        ('I', 0xDC), # -> pointers
        'unkE0':         ('H', 0xE0),
        'flagsE2':       ('H', 0xE2), # related to textures; # texcoords?
        'nVtxs':         ('H', 0xE4),
        'nNormals':      ('H', 0xE6),
        'nColors':       ('H', 0xE8), # guessed
        'nTexCoords':    ('H', 0xEA),
        'nAnimations':   ('H', 0xEC),
        'nTextures':     ('B', 0xF2),
        'nBones':        ('B', 0xF3),
        'nVtxGroups':    ('B', 0xF4),
        'nDlists':       ('B', 0xF5),
        'unkF6':         ('B', 0xF6),
        'nHitBoxes':     ('B', 0xF7),
        'nShaders':      ('B', 0xF8),
        'nPtrsDC':       ('B', 0xF9), # #ptrs at field DC
        'nTexMtxs':      ('B', 0xFA),
        'unkFB':         ('B', 0xFB),
    }
