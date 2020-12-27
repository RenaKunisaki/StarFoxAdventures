#!/usr/bin/env python3
"""Iterates all romlist files, reads gamebits from known params, and fills in the XML file."""
import sys
import struct
import os
import os.path
import zlib
import re
import xml.etree.ElementTree as ET

objNames = {}
objIndex = {}
discPath = sys.argv[1]
xmlPath  = sys.argv[2]
gameBits = ET.parse(os.path.join(xmlPath, 'gamebits.xml')).getroot()
objects  = ET.parse(os.path.join(xmlPath, 'objects.xml')).getroot()
dlls     = ET.parse(os.path.join(xmlPath, 'dlls.xml')).getroot()

def dump(data):
    r = []
    for i in range(0, len(data), 4):
        r.append(data[i:i+4].hex().upper())
    return ' '.join(r)

for i, obj in enumerate(objects.findall('./object')):
    objNames[i] = obj.get('name')

# move text to description
#for bit in gameBits.findall('./bit'):
#    text = "".join(bit.itertext()).strip()
#    if text != '':
#        bit.text = ''
#        ET.SubElement(bit, 'description').text = text

# remove existing obj refs and unhelpful names
RE_ENDS_WITH_OBJ_ID = re.compile(r'[0-9A-F]{5}$')
for bit in gameBits.findall('./bit'):
    for ref in list(bit.findall('./objref')):
        bit.remove(ref)
    name = bit.get('name', None)
    if name is not None and RE_ENDS_WITH_OBJ_ID.search(name):
        del bit.attrib['name']


_objParamsCache = {}
def getObjParams(objId):
    """Get list of parameters for given object ID."""
    if objId in _objParamsCache: return _objParamsCache[objId]
    result = []
    obj = objects.find('./object[@id="0x%04X"]' % objId)
    if obj is None:
        raise ValueError("Object 0x%04X not found" % onbjId)
    params = obj.find('./params')
    if params is not None: params = params.findall('./param')
    if params is None or len(params) == 0:
        dllId = obj.get('dll', None)
        if dllId is None: return []
        dll = dlls.find('./dll[@id="%s"]' % dllId)
        if dll is None:
            raise ValueError("DLL '%s' not found" % dllId)
        params = dll.find('./objparams')
    if params is None: return []
    for param in params:
        # offset is unknown in some cases
        if param.get('offset', None) is not None:
            result.append({
                'type':       param.get('type',   None),
                'offset': int(param.get('offset', None), 0),
                'name':       param.get('name',   None),
            })
    _objParamsCache[objId] = result
    return result


def readFile(path):
    s = path.rfind('/')
    name = path[s+1:]
    s = name.find('.')
    name = name[0:s]

    # decompress
    with open(path, 'rb') as file:
        head = file.read(4)
        assert head == b'ZLB\0'
        version, decLen, compLen = struct.unpack('>3I', file.read(12))
        assert version == 1
        data = file.read(compLen)
        fileData = zlib.decompress(data)
        assert len(fileData) == decLen

    # iterate object defs
    offs = 0
    idx  = 0
    while offs < len(fileData):
        typ, size, flags = struct.unpack_from('>hBB', fileData, offs)
        unk4, unk5, unk6, map, x, y, z, id = struct.unpack_from('>BBBBfffi', fileData, offs+4)
        realId = objIndex.get(typ, typ)
        objName = objNames.get(realId, None) # value can be None, so don't pass default here
        if objName is None: objName = "obj%04X" % realId

        uniqueId = struct.unpack_from('>I', fileData, offs+0x14)[0] # grumble
        params = getObjParams(realId)
        for param in params:
            if param['type'] == 'GameBit':
                val = struct.unpack_from('>H', fileData, offs+param['offset'])[0] & 0x7FFF # grumble
                if val not in (0x95, 0x96, 0x7FFF):
                    bit = gameBits.find('./bit[@id="0x%04X"]' % val)
                    if bit is None:
                        sys.stderr.write("Bit 0x%04X not found (map %s obj %s ID %08X)\n" % (
                            val, name, objName, uniqueId))
                    else:
                        paramName = param.get('name', None)
                        if paramName is None: paramName = '0x%02X' % param['offset']
                        ET.SubElement(bit, 'objref', {
                            'map':   name,
                            'obj':   objName,
                            'objId': '0x%08X' % uniqueId,
                            'param': paramName,
                        })
                        #print("%12s %12s %08X %04X %s" % (name, objName, uniqueId, val, param['name']))

        idx += 1
        offs += size * 4


# read OBJINDEX lookup table
with open(os.path.join(discPath, 'OBJINDEX.bin'), 'rb') as file:
    idx = 0
    while True:
        d = file.read(2)
        if len(d) < 2: break
        id = struct.unpack('>h', d)[0] # grumble
        if id != -1: objIndex[idx] = id
        idx += 1


for name in os.listdir(discPath):
    if name.endswith('.romlist.zlb'):
        readFile(os.path.join(discPath, name))

print(ET.tostring(gameBits, encoding='unicode'))
