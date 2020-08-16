from common.xstruct import Struct
from common.math import Vec2s, Vec3s, Vec3f, Quaternion, Mtx43, Mtx44

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
        'offset':         ('I', 0x00),
        'size':           ('H', 0x04),
        'bbox':           ('6h',0x06),
        'unk12':          ('B', 0x12),
        'shaderId':       ('B', 0x13), # maybe u16
        'specialBitAddr': ('H', 0x14), # related to shader
        'unk16':          ('H', 0x16),
        'unk18':          ('I', 0x18), # always 07 00 00 00?
    }

class ShaderLayer(Struct):
    _size   = 0x08
    _order  = '>'
    _fields = {
        'texture':             ('I',  0x00),
        'tevMode':             ('B',  0x04),
        'enableTexChainStuff': ('B',  0x05),
        'scrollingTexMtx':     ('B',  0x06),
    }

class Shader(Struct):
    _size   = 0x44
    _order  = '>'
    _fields = {
        'auxTex0':   ('I',  0x08),
        'auxTex1':   ('I',  0x14),
        'alpha':     ('B',  0x22),
        'unk24':     ('4I', 0x24),
        'auxTex2':   ('I',  0x34),
        'furTexture':('I',  0x38),
        'flags':     ('I',  0x3C),
        'flags_40':  ('B',  0x40),
        'nTextures': ('B',  0x41),
    }

class HitboxStruct(Struct):
    _size   = 0x18
    _order  = '>'
    _fields = {
        'bone':  ('h',  0x00),
        'unk02': ('H',  0x02),
        'radius':('f',  0x04),
        'pos':   ('3f', 0x08),
        'unk14': ('H',  0x14),
        'unk16': ('B',  0x16),
        'unk17': ('B',  0x17),
    }

class ModelStruct(Struct):
    _size   = 0xFC
    _order  = '>'
    _fields = {
        # 0x00: ref count, set on load
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
        'posFineSkinningConfig': ('I', 0x84),
        'posFineSkinningPieces': ('I', 0xA4), # ObjInstance*
            # field_88.nVtxs = how many
        'posFineSkinningWeights': ('I', 0xA8),
        'nrmFineSkinningWeights': ('I', 0xAC),
        'dlists':        ('I', 0xD0), # -> DisplayListPtr
        'renderInstrs':  ('I', 0xD4), # -> bit-packed instruction code
        'nRenderInstrs': ('H', 0xD8), # num bytes
        'offsDC':        ('I', 0xDC), # -> pointers
        'unkE0':         ('H', 0xE0),
        'flagsE2':       ('H', 0xE2), # related to textures; # texcoords?
        'nVtxs':         ('H', 0xE4),
        'nNormals':      ('H', 0xE6),
        'nColors':       ('H', 0xE8),
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

class MapBlockStruct(Struct):
    _size   = 0xB0 # XXX verify
    _order  = '>'
    _fields = {
        'unused00':      ('I',   0x00), # ignored in file
        'flags_4':       ('H',   0x04),
        'fileSize':      ('I',   0x08),
        'unknown':       ('64B', 0x0C),
        'unk4C':         ('I',   0x4C), # relates to hit detection
        'unk50':         ('I',   0x50), # relates to hit detection
        'textures':      ('I',   0x54), # -> texture IDs
        'vtxs':          ('I',   0x58),
        'colors':        ('I',   0x5C),
        'texcoords':     ('I',   0x60),
        'shaders':       ('I',   0x64),
        'dlists':        ('I',   0x68),
        'hits':          ('I',   0x70),
        'renderInstrs':  ('3I',  0x78),
        'nRenderInstrs': ('3H',  0x84), # num bytes
        'nVtxs':         ('H',   0x90),
        'unk92':         ('H',   0x92),
        'nColors':       ('H',   0x94),
        'nTexCoords':    ('H',   0x96),
        'unk98':         ('2H',  0x98),
        'nHits':         ('H' ,  0x9C),
        'unk9E':         ('H' ,  0x9E),
        'nTextures':     ('B',   0xA0),
        'nDlists':       ('B',   0xA1),
        'nShaders':      ('B',   0xA2),
        'unkA3':         ('B',   0xA3),
        'name':          ('11s', 0xA4), # length guessed
    }
