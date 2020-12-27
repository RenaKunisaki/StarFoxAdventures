#!/usr/bin/env python3
"""Iterates all romlist files to determine parameter length of objects."""
import sys
import struct
import os
import os.path
import zlib
import xml.etree.ElementTree as ET

objNames = {}
objIndex = {}
objParamLen = {}
exampleParams = {}

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

    with open(path, 'rb') as file:
        head = file.read(4)
        assert head == b'ZLB\0'
        version, decLen, compLen = struct.unpack('>3I', file.read(12))
        assert version == 1
        data = file.read(compLen)
        fileData = zlib.decompress(data)
        assert len(fileData) == decLen

    offs = 0
    idx  = 0
    while offs < len(fileData):
        typ, size, flags = struct.unpack_from('>hBB', fileData, offs)
        unk4, unk5, unk6, map, x, y, z, id = struct.unpack_from('>BBBBfffi', fileData, offs+4)
        realId = objIndex.get(typ, typ)
        objName = objNames.get(realId, '?')
        if realId in objParamLen:
            if objParamLen[realId] != size:
                objParamLen[realId] = "varies"
        else: objParamLen[realId] = size

        if size > 6 and realId not in exampleParams:
            exampleParams[realId] = fileData[offs+0x18 : offs+(size*4)]

        idx += 1
        offs += size * 4

path = sys.argv[1]
with open(path+'/OBJECTS.tab', 'rb') as tabFile:
    with open(path+'OBJECTS.bin', 'rb') as binFile:
        idx = 0
        while True:
            d = tabFile.read(4)
            if len(d) < 4: break
            offs = struct.unpack('>I', d)[0] # grumble
            if offs == 0xFFFFFFFF: break
            binFile.seek(offs + 0x91)
            name = binFile.read(11).replace(b'\0', b' ')
            name = name.decode('ascii')
            objNames[idx] = name
            idx += 1

with open(path+'/OBJINDEX.bin', 'rb') as file:
    idx = 0
    while True:
        d = file.read(2)
        if len(d) < 2: break
        id = struct.unpack('>h', d)[0] # grumble
        if id != -1: objIndex[idx] = id
        idx += 1


#print("file;idx;obj;real;name;size;03;04;05;06;mp;x;y;z;id;data")
for name in os.listdir(path):
    if name.endswith('.romlist.zlb'):
        readFile(path + '/' + name)

basePath = os.path.dirname(os.path.realpath(__file__))
objs = ET.parse(os.path.join(basePath, '../data/objects.xml')).getroot()

for objId in range(0x5C5):
    obj = objs.findall("./object[@id='0x%04X']" % objId)[0]
    if objId not in objParamLen: continue

    params = obj.findall('./params')
    if len(params) == 0: params = ET.SubElement(obj, 'params')
    else: params = params[0]
    pLen = objParamLen[objId]
    if type(pLen) is int:
        pLen = '0x%02X' % ((pLen * 4))
    params.set('length', pLen)

    if objId in exampleParams:
        example = ET.SubElement(obj, 'exampleParams')
        data    = exampleParams[objId]
        res     = []
        for i in range(0, len(data), 4):
            res.append(data[i:i+4].hex().upper())
        example.text = ' '.join(res)

print(ET.tostring(objs, encoding='unicode'))
