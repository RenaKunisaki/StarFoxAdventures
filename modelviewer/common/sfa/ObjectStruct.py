import logging; log = logging.getLogger(__name__)
from common.xstruct import Struct
import struct # this is not confusing at all

class ObjectStruct(Struct):
    """OBJECTS.bin entry."""
    _size   = 0x9C
    _order  = '>'
    _fields = {
        'unk00':              (  'f', 0x00), #copied to shadow field 0
        'scale':              (  'f', 0x04),
        'pModelList':         (  'I', 0x08), #-> list of model IDs
        'unk0C':              (  'I', 0x0c), #pointer
        'unk10':              (  'I', 0x10), #pointer
        'unk14':              (  'I', 0x14), #pointer
        'offset_0x18':        (  'I', 0x18), #[OPTIONAL] a file containing functions
        'pSeq':               (  'I', 0x1c), #[OPTIONAL] -> seq IDs
        'pEvent':             (  'I', 0x20), #[OPTIONAL] offset into the file. changed to pointer on load
        'pHits':              (  'I', 0x24), #[OPTIONAL]
        'pWeaponDa':          (  'I', 0x28), #[OPTIONAL]
        'hitboxes':           (  'I', 0x2c),
        #'pModLines':          (  'I', 0x30), #ignored in file (zeroed on load)
        #'unk34':              (  'I', 0x34), #ignored in file (zeroed on load)
        'unk38':              (  'I', 0x38),
        'unk3C':              (  'I', 0x3c), #void*
        'aButtonInteraction': (  'I', 0x40), #[OPTIONAL] count in field 0x72 -> something 0x12 or 0x18 bytes per model
        'flags':              (  'I', 0x44),
        'shadowType':         (  'h', 0x48),
        'shadowTexture':      (  'h', 0x4a),
        'unk4C':              (  'B', 0x4c),
        'unk4D':              (  'B', 0x4d),
        'unk4E':              (  'h', 0x4e),
        'dll_id':             (  'h', 0x50), #if not -1, load this DLL; func 0 is a model callback
        'unk52':              (  'B', 0x52),
        'unk53':              (  'B', 0x53),
        'unk54':              (  'B', 0x54),
        'nModels':            (  'B', 0x55),
        'numPlayerObjs':      (  'B', 0x56), #if > 0, objAddObjectType(obj, 8)
        'unk57':              (  'B', 0x57), #never read?
        'nHitboxes':          (  'B', 0x58),
        'count59':            (  'B', 0x59), #count of sth 0x10 bytes
        'numVecs':            (  'B', 0x5a), #count of something 0x12 bytes; crashes if reduced
        'unk5B':              (  'B', 0x5b),
        #'modLinesSize':       (  'B', 0x5c), #ignored in file
        'modLinesIdx':        (  'b', 0x5d),
        'numSeqs':            (  'B', 0x5e),
        'flags_0x5f':         (  'B', 0x5f),
        'unk60':              (  'B', 0x60),
        'unk61':              (  'B', 0x61), #crash if 0; hitbox related
        'hitboxSize':         (  'B', 0x62),
        'unk63':              (  'B', 0x63),
        'hitboxField6B':      (  'B', 0x64),
        'hitboxFlags_0x65':   (  'B', 0x65), #8=has something 0x1C8 bytes
        'unk66':              (  'B', 0x66),
        'hasHitbox':          (  'B', 0x67),
        'hitboxField5C':      (  'h', 0x68),
        'shadowVar6A':        (  'h', 0x6a), #> 0x169 = no shadow; also hitbox related
        'unk6C':              (  'h', 0x6c),
        'unk6E':              (  'h', 0x6e),
        'hitboxVar70':        (  'B', 0x70), #related to hitbox (height?)
        'flags_0x71':         (  'B', 0x71), #related to hitbox
        'count72':            (  'B', 0x72), #count of something 0x18 bytes; changing crashes
        'stateVar73':         (  'B', 0x73), #1=translucent; 3=invincible - not flags
        'unk74':              (  'B', 0x74),
        'unk75':              (  'B', 0x75),
        'flags76':            (  'B', 0x76),
        'unk77':              (  'B', 0x77),
        'map':                (  'h', 0x78), #crashes loudly if invalid
        'unk7A':              (  'B', 0x7a),
        'unk7B':              (  'B', 0x7b),
        'helpTexts':          ( '4h', 0x7c), #one per model
        'unk84':              (  'h', 0x84),
        'unk86':              (  'h', 0x86),
        'lagVar88':           (  'f', 0x88), #causes lag at ~65536.0; GPU hang at much more; related to shadow; maybe causing excessive map loads?
        'nLights':            (  'B', 0x8c),
        'lightIdx':           (  'B', 0x8d),
        'unk8E':              (  'B', 0x8e), #related to textures; 1=dark, 2=default, 3+=corrupt, 77=crash, 0=normal
        'unk8F':              (  'B', 0x8f), #related to hitbox
        'hitboxVar90':        (  'B', 0x90), #< 0xE = invincible
        'name':               ('11s', 0x91),
    }

    def __init__(self, data):
        super().__init__(data)
        self.name = self.name.replace('\0', '')

    @classmethod
    def readFromFile(cls, file, offset=None, count=None):
        offs = offset
        if offs is None: offs = file.tell()
        self = super().readFromFile(file, offset, count)

        self.modelIds = []
        file.seek(offs + self.pModelList)
        for i in range(self.nModels):
            self.modelIds.append(struct.unpack('>i', file.read(4))[0]) # grumble

        return self
