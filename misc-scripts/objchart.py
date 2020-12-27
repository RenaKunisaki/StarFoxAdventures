#!/usr/bin/env python3
"""Iterates all romlist files and builds chart of which objects are present
in which maps.
"""
import sys
import struct
import os
import os.path
import zlib
import xml.etree.ElementTree as ET
from objdefs import ObjDefs

NUM_MAPS = 0x75
NUM_OBJS = 0x5C5

objIndex = {}
objects  = []
for i in range(NUM_OBJS):
    ls = []
    for j in range(NUM_MAPS):
        ls.append(0)
    objects.append(ls)

missingMaps = set()

tree = ET.parse(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../dump/maps.xml'))
maps = tree.getroot()
mapIds = {}
for i in range(NUM_MAPS):
    m = maps.findall("./map[@id='0x%02X']" % i)
    if m:
        name = m[0].get('romlist')
        mapIds[name] = i

def dump(data):
    r = []
    for i in range(0, len(data), 4):
        r.append(data[i:i+4].hex().upper())
    return ' '.join(r)

def readFile(path):
    s = path.rfind('/')
    name = path[s+1:]
    s = name.find('.')
    name = name[0:s]
    try: mapId = mapIds[name]
    except KeyError:
        missingMaps.add(name)
        return


    with open(path, 'rb') as file:
        head = file.read(4)
        assert head == b'ZLB\0'
        version, decLen, compLen = struct.unpack('>3I', file.read(12))
        assert version == 1
        data = file.read(compLen)
        fileData = zlib.decompress(data)
        assert len(fileData) == decLen

    offs = 0
    while offs < len(fileData):
        typ, size, flags = struct.unpack_from('>hBB', fileData, offs)
        unk4, unk5, unk6, map, x, y, z, id = struct.unpack_from('>BBBBfffi', fileData, offs+4)
        realId = objIndex.get(typ, typ)
        objects[realId][mapId] += 1
        offs += size * 4

path = sys.argv[1]
with open(path+'/OBJINDEX.bin', 'rb') as file:
    idx = 0
    while True:
        d = file.read(2)
        if len(d) < 2: break
        id = struct.unpack('>h', d)[0] # grumble
        if id != -1: objIndex[idx] = id
        idx += 1


def printCsv():
    print("obj;name;%s" % (';'.join(list(map(lambda b: '%02X'%b, range(NUM_MAPS))))))
    for i, obj in enumerate(objects):
        print("%03X;%-11s;" % (i, objNames[i]), end='')
        for j in range(NUM_MAPS):
            print('%d;' % obj[j], end='')
        print('')

def printMd():
    for i, obj in enumerate(objects):
        objDef = ObjDefs[i]
        if objDef[1] == '':
            print("## %04X %-11s" % (i, objDef[0]))
        else:
            print("## %04X %-11s - %s" % (i, objDef[0], objDef[1]))

        s = sum(obj)
        #if s == 0: print("- Not present in any maps")
        if s == 0: pass
        elif s == NUM_MAPS: print("- Present in every map")
        else:
            maps = []
            for mapId, present in enumerate(obj):
                if present:
                    maps.append(mapNames[mapId])
            maps = ', '.join(maps)
            print("- Present in: %s" % maps)
        print('')

def printXml():
    eRoot = ET.Element('objects')
    for i, obj in enumerate(objects):
        objDef = ObjDefs[i]
        eObj = ET.SubElement(eRoot, 'object')
        eObj.set('id', '0x%04X' % i)
        eObj.set('name', objDef[0])
        eDesc = ET.SubElement(eObj, 'description')
        eDesc.text = objDef[1]
        for mapId, count in enumerate(obj):
            if count:
                eMap = ET.SubElement(eObj, 'map')
                eMap.set('id', '0x%02X' % mapId)
                eMap.set('count', str(count))
                #eMap.text = mapNames[mapId]
    print(ET.tostring(eRoot, encoding='unicode'))


for name in os.listdir(path):
    if name.endswith('.romlist.zlb'):
        readFile(path + '/' + name)


printXml()
#for name in missingMaps:
#    print("No ID for", name)
