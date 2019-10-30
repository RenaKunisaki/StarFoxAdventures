from xstruct import Struct

class ObjectStruct(Struct):
    """OBJECTS.bin entry.

    This isn't actually used at the moment.
    """
    SIZE = 0x9C
    _fields = {
        'scale':      ('f', 0x00),
        'unk04':      ('f', 0x04),
        'offs08':     ('I', 0x08),
        'offs0C':     ('I', 0x0C), # relates to textures?
        'offs10':     ('I', 0x10),
        'unk14':      ('I', 0x14),
        'offs18':     ('I', 0x18),
        'offs1C':     ('I', 0x1C),
        'offs20':     ('I', 0x20),
        'offs24':     ('I', 0x24),
        'offs28':     ('I', 0x28),
        'offs2C':     ('I', 0x2C), # relates to models
        # fields 0x30, 0x34 are ignored in file
        'unk38':      ('I', 0x38),
        'offs3C':     ('I', 0x3C),
        'offs40':     ('I', 0x40),
        'flags':      ('I', 0x44),
        'shadowType': ('H', 0x48),
        'unk4A':      ('B', 0x4A),
        'unk4B':      ('B', 0x4B),
        'unk4C':      ('I', 0x4C),
        'unk50':      ('I', 0x50),
        'unk54':      ('B', 0x54),
        'nModels':    ('B', 0x55),
        'unk56':      ('B', 0x56),
        'unk57':      ('B', 0x57),
        'unk58':      ('B', 0x58),
        'unk59':      ('B', 0x59), # relates to textures?
        'numVecs':    ('B', 0x5A),
        'unk5B':      ('B', 0x5B),
        # 0x5C: ignored in file
        'modLinesIdx':('b', 0x5D), # is this ignored too?
        'unk5E':      ('B', 0x5E), # num items at offset 0x2C?
        'flags5F':    ('B', 0x5F), # num items at offset 0x2C?
        'unk60':      ('B', 0x60),
        'unk61':      ('B', 0x61),
        'unk62':      ('B', 0x62),
        'unk63':      ('B', 0x63),
        'unk64':      ('B', 0x64),
        'unk65':      ('B', 0x65),
        'unk66':      ('B', 0x66),
        'unk67':      ('B', 0x67),
        'unk68':      ('B', 0x68),
        'unk69':      ('B', 0x69),
        'unk6A':      ('B', 0x6A),
        'unk6B':      ('B', 0x6B),
        'unk6C':      ('B', 0x6C),
        'unk6D':      ('B', 0x6D),
        'unk6E':      ('B', 0x6E),
        'unk6F':      ('B', 0x6F),
        'unk70':      ('B', 0x70),
        'unk71':      ('B', 0x71),
        'unk72':      ('B', 0x72), # conut of something?
        'unk73':      ('B', 0x73),
        'unk74':      ('B', 0x74),
        'unk75':      ('B', 0x75),
        'unk76':      ('B', 0x76),
        'unk77':      ('B', 0x77),
        'mapId':      ('h', 0x78),
        'unk7A':      ('h', 0x7A),
        'unk7C':      ('4h',0x7C),
        'unk84':      ('B', 0x84),
        'unk85':      ('B', 0x85),
        'unk86':      ('B', 0x86),
        'unk87':      ('B', 0x87),
        'unk88':      ('f', 0x88),
        'nLights':    ('b', 0x8C),
        'lightIdx':   ('b', 0x8D),
        'unk8E':      ('b', 0x8E),
        'unk8F':      ('b', 0x8F),
        'unk90':      ('b', 0x90),
        'name':       ('11s',0x91),
    }
