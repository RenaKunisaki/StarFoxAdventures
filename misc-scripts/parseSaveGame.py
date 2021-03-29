#!/usr/bin/env python3
"""Given a savegame file or RAM dump, extract info from it."""
import sys
import os.path
import math
import struct
import xml.etree.ElementTree as ET

def printf(fmt, *args, **kwargs):
    print(fmt % args, end='', **kwargs)


NUM_HEAPS = 4
RAM_SIZE  = 0x017F_FFFF
OFFSETS = {
    'ram': { # RAM dump
        'savedata': 0x803a31c4,
        'heaps':    0x80340698,
    },
    'raw': {
        'savedata': 0,
        'heaps':    None,
    }
}

structSaveSettings = (
    (0x00, 'b',  'exists'),
    (0x01, 'b',  'unk01'),
    (0x02, 'b',  'bSubtitlesOn'),
    (0x03, 'b',  'unusedHudSetting'),
    (0x04, 'b',  'unusedCameraSetting'),
    (0x05, 'b',  'unk05'),
    (0x06, 'b',  'bWidescreen'),
    (0x07, 'b',  'unk07'),
    (0x08, 'b',  'bRumbleEnabled'),
    (0x09, 'b',  'soundMode'),
    (0x0A, 'b',  'musicVolume'),
    (0x0B, 'b',  'sfxVolume'),
    (0x0C, 'b',  'cutsceneVolume'),
    (0x0D, 'b',  'unk0D'),
    (0x0E, 'h',  'unk0E'),
    (0x10, 'I',  'unlockedCheats'),
    (0x14, 'I',  'activeCheats'),
    (0x18, 'I',  'unk18'),
    (0x1C, 'I',  'unk1C'), # related to scores?
    (0x20, '4c', 'name'), # null terminated
    (0x24, 'I',  'unk24'),
)
structPlayerCharState = (
    (0x00, 'b', 'curHealth'),
    (0x01, 'b', 'maxHealth'),
    (0x02, 'b', 'flags02'),
    (0x03, 'b', 'unk03'),
    (0x04, 'H', 'curMagic'),
    (0x06, 'H', 'maxMagic'),
    (0x08, 'B', 'money'),
    (0x09, 'b', 'curBafomDads'),
    (0x0A, 'b', 'maxBafomDads'),
    (0x0B, 'b', 'unk0B'),
)
structSavedObjPos = (
    (0x00, 'i',  'id'),
    (0x04, '3f', 'pos'),
)
structPlayerCharPos = (
    (0x00, '3f', 'pos'),
    (0x0C, 'b',  'rotX'),	# high byte of vec3s.x
    (0x0D, 'b',  'mapLayer'),
    (0x0E, 'b',  'mapId'),
    (0x0F, 'b',  'unk0F'), # probably padding
)
structSaveGame = (
    (0x0000, structPlayerCharState, 'krystalState'),
    (0x000C, structPlayerCharState, 'foxState'),
    (0x0018, 'b',	  'trickyEnergy'),
    (0x0019, 'b',   'unk19'),
    (0x0019, 'b',   'unk1A'),
    (0x0019, 'b',   'unk1B'),
    (0x001C, '4c',  'name'),
    (0x0020, 'b',   'character'),
    (0x0021, 'b',   'flags21'),
    (0x0022, 'b',   'flags22'),
    (0x0023, 'b',   'unk23'),
    (0x0024, '321B', 'gameBits2'),
    (0x0168, structSavedObjPos, 'savedObjs', 63),
    (0x0558, '5B',  'texts'),
    (0x055D, 'B',   'completion'), # out of 187
    (0x055E, 'B',   'numTexts'),
    (0x055F, 'b',   'unk55F'),
    (0x0560, 'i',	'playTime'), # frame count
    (0x0564, '116B','gameBits1'),
    (0x05D8, '172B','gameBits3'),
    (0x0684, structPlayerCharPos, 'krystalPos'),
    (0x0694, structPlayerCharPos, 'foxPos'),
    (0x06A4, 'h',    'unk6A4'), # related to camera actions?
    (0x06A6, 'h',    'unk6A6'),
    (0x06A8, 'f',    'timeOfDay'),
    (0x06AC, '5h',   'envfxActIdx'),
    (0x06B6, '3h',   'envFxActIdx2'),
    (0x06BC, '44B',  'envUnk6BC'),
    (0x06E8, 'b',    'envFlags6E8'),
    (0x06E9, 'b',    'unk6E9'),
    (0x06EA, 'b',    'unk6EA'), # unused? set to FF for new game
    (0x06EB, 'b',    'unk6EB'),
)
structSaveData = (
    (0x00, structSaveSettings, 'settings', 5), # no idea why 5 instead of 3.
    (0xE4, structSaveGame, 'game'),
)


def _structTupleToFmt(fmt, _depth=0):
    assert _depth < 10, "Maximum depth exceeded"
    if type(fmt) is str: return fmt
    if type(fmt[0]) is tuple:
        res = []
        for item in fmt:
            res.append(_structTupleToFmt(item, _depth=_depth+1))
        return ''.join(res)

    if len(fmt) > 3:
        offs, typ, name, count = fmt
    else:
        offs, typ, name = fmt
        count = 1
    if type(typ) is tuple: typ = _structTupleToFmt(typ, _depth=_depth+1)
    return typ * count

def _structUnpack(fmt, data, offs=0, _depth=0):
    """Unpack struct from data.

    @param fmt Format, which is either a struct format string, or a tuple.
    @param data Data to unpack from.
    @param offs Offset to unpack from.
    """
    assert _depth < 10, "Maximum depth exceeded"
    if type(fmt) is str:
        r = struct.unpack_from(fmt, data, offs)
        if len(r) == 1: return r[0] # grumble
        return r

    # fmt is ( offset, type, name[, count] ) or a tuple of such
    res = {}
    if type(fmt[0]) is not tuple: fmt = (fmt,)
    for item in fmt:
        if len(item) > 3:
            itemOffs, itemType, itemName, itemCount = item
        else:
            itemOffs, itemType, itemName = item
            itemCount = 1
        size = struct.calcsize(_structTupleToFmt(itemType))
        val = []
        for i in range(itemCount):
            val.append(_structUnpack(itemType, data, offs=offs, _depth=_depth+1))
            offs += size
        if len(val) == 1: res[itemName] = val[0]
        else: res[itemName] = val
    return res


def readStruct(file, fmt, offset=None):
    """Read struct from file."""
    if offset is not None: file.seek(offset)
    if type(fmt) is tuple: # ( (item), (item), ... )
        fStr = _structTupleToFmt(fmt)
        size = struct.calcsize(fStr)
        data = file.read(size)
        return _structUnpack(fmt, data)
    else:
        size = struct.calcsize(fmt)
        data = file.read(size)
        res  = struct.unpack(fmt, data)
        if len(res) == 1: return res[0] # grumble
        return res



BITTABLE = [ [], [], [], [] ]
def _readBitTable():
    with open('../discroot/BITTABLE.bin', 'rb') as file:
        iBit = 0
        while True:
            try: offs, flags, hint = readStruct(file, '>HBB')
            except struct.error: break
            BITTABLE[flags >> 6].append({
                'id':      iBit,
                'offset':  offs, # bitwise offset into table
                'table':   flags >> 6,
                'size':    (flags & 0x1F) + 1, # number of bits
                'hasHint': (flags & 0x20) != 0,
                'hint':    hint + 0xF4,
            })
            iBit += 1


def _bitTableToXml(name, val, eParent):
    if name == 'gameBits0': tbl = 0
    if name == 'gameBits1': tbl = 1
    if name == 'gameBits2': tbl = 2
    if name == 'gameBits3': tbl = 3
    eBits = ET.SubElement(eParent, name)
    for bit in BITTABLE[tbl]:
        offs = bit['offset']
        data = 0
        for i in range(bit['size']):
            iByte = (offs+i) >> 3
            iBit  = (offs+i) & 7
            if iByte >= len(val):
                print("bit", bit, "iByte=", iByte, "len", len(val))
            b     = val[iByte] >> (7 - iBit)
            data  = (data << 1) | (b & 1)

        ET.SubElement(eBits, 'bit', {
            'id': '0x%X' % bit['id'],
        }).text = '0x%X' % data


def structToXml(fmt, data, eParent, _depth=0):
    """Convert struct data to XML.

    @param fmt The struct format (tuple or string).
    @param data The value read using that format.
    @param eParent The XML Element to append to.
    """
    assert _depth < 10, "Maximum depth exceeded"
    if type(fmt) is str:
        #elem = ET.SubElement(eParent, 'value', {'type':fmt})
        eParent.text = str(data)
    elif type(fmt[0]) is tuple:
        for item in fmt:
            structToXml(item, data, eParent, _depth=_depth+1)
    else:
        if len(fmt) > 3:
            itemOffs, itemType, itemName, itemCount = fmt
        else:
            itemOffs, itemType, itemName = fmt
            itemCount = 1
        val = data[itemName]
        if itemName in ('gameBits0', 'gameBits1', 'gameBits2', 'gameBits3'): # HACK
            _bitTableToXml(itemName, val, eParent)
        elif itemCount > 1:
            for i in range(itemCount):
                elem = ET.SubElement(eParent, itemName, {'idx':str(i)})
                structToXml(itemType, val[i], elem, _depth=_depth+1)
        else:
            elem = ET.SubElement(eParent, itemName)
            structToXml(itemType, val, elem, _depth=_depth+1)


def dumpSaveData(file, offset, eData):
    eSave = ET.SubElement(eData, 'savedata')
    data  = readStruct(file, structSaveData, offset)
    # XXX convert bit tables
    structToXml(structSaveData, data, eSave)


def main(path, mode):
    """Run the app.

    @param path File path to read from.
    @param mode How to treat the file:
        - "ram": as a RAM dump
        - "card": as a memory card image (TODO)
        - "raw": as a raw savegame dump
    """
    with open(path, 'rb') as file:
        eData = ET.Element('data')
        offsets = OFFSETS[mode]
        if offsets['savedata'] is not None:
            dumpSaveData(file, offsets['savedata'] & RAM_SIZE, eData)
        print(ET.tostring(eData, encoding='unicode'))



if __name__ == '__main__':
    args = list(sys.argv[1:])
    if len(args) > 0:
        _readBitTable()
        mode = 'ram'
        if args[0] in ('-m', '--mode'):
            args.pop(0) # remove
            mode = args.pop(0)
        if len(args) > 0:
            main(args[0], mode)
            sys.exit(0)
    print("Usage: %s [-m mode] path" % sys.argv[0])
