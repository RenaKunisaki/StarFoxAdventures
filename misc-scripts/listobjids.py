#!/usr/bin/env python3
"""Iterates all romlist files and build an XML file of unique object IDs."""
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
objIds   = ET.Element('objids')

def dump(data):
    r = []
    for i in range(0, len(data), 4):
        r.append(data[i:i+4].hex().upper())
    return ' '.join(r)

for i, obj in enumerate(objects.findall('./object')):
    objNames[i] = obj.get('name')


def readRomlist(path):
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
        typ, size, acts0, loadFlags, acts1, bound, slot, x, y, z, id = \
            struct.unpack_from('>hBBBBBBfffI', fileData, offs)
        realId = objIndex.get(typ, typ)
        objName = objNames.get(realId, None) # value can be None, so don't pass default here
        if objName is None: objName = '?'

        uniqueId = struct.unpack_from('>I', fileData, offs+0x14)[0] # grumble
        ET.SubElement(objIds, 'obj', {
            'id': '0x%08X' % id,
            'romlist': name,
            'idx': '%d' % idx,
            'offset': '0x%06X' % offs,
            'type': '0x%04X' % typ,
            'real': '0x%04X' % realId,
            'name': objName,
            'acts': '0x%02X%02X' % (acts0, acts1),
            'bound': '%d' % bound,
            'slot': '0x%02X' % slot,
            'flags': '0x%02X' % loadFlags,
            'x': str(x),
            'y': str(y),
            'z': str(z),
        })

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
        readRomlist(os.path.join(discPath, name))

print(ET.tostring(objIds, encoding='unicode'))
