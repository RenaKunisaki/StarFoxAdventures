#!/usr/bin/env python3
"""Read OBJECTS.bin and OBJECTS.tab and make a list of ID => name."""
import sys
import os
import os.path
import struct
import xml.etree.ElementTree as ET

#print("Obj#|Def#|Obj Name   |Models              |DLL |NP|Description")
#print("----|----|-----------|--------------------|----|--|-----------")
#print("enum ObjDef {")

USE_PROTO = False

tree = ET.parse('data/U0/objects.xml')
objects = tree.getroot()

basePath = sys.argv[1]

objInd = {}
with open(os.path.join(basePath, 'OBJINDEX.bin'), 'rb') as file:
    idx = 0
    while True:
        entry = file.read(2)
        if len(entry) < 2: break
        entry = struct.unpack('>H', entry)[0] # grumble
        objInd[idx] = entry
        idx += 1

dllObjs = {}
seenObjs = {}
with open(os.path.join(basePath, 'OBJECTS.tab'), 'rb') as tabFile:
    with open(os.path.join(basePath, 'OBJECTS.bin'), 'rb') as binFile:
        idx = 0
        while True:
            d = tabFile.read(4)
            if len(d) < 4: break
            offs = struct.unpack('>I', d)[0] # grumble
            if offs == 0xFFFFFFFF: break

            try:
                if USE_PROTO:
                    nSeq = 0
                    binFile.seek(offs + 0x50)
                    dllId = struct.unpack('>H', binFile.read(2))[0] # grumble

                    #binFile.seek(offs + 0x55)
                    #nModels, nPlayerObjs = struct.unpack('>BB', binFile.read(2))
                    nModels, nPlayerObjs = 0, 0

                    #binFile.seek(offs + 0x78)
                    #mapId = struct.unpack('>H', binFile.read(2))[0] # grumble
                    mapId = 0xFFFF

                    #binFile.seek(offs + 0x7C)
                    #texts = struct.unpack('>4H', binFile.read(8))
                    texts = (0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF)

                    binFile.seek(offs + 0x58)
                    name = binFile.read(15)
                else:
                    binFile.seek(offs + 0x1C)
                    pSeq = struct.unpack('>I', binFile.read(4))[0] # grumble
                    binFile.seek(offs + 0x5E)
                    nSeq = struct.unpack('>B', binFile.read(1))[0] # grumble

                    binFile.seek(offs + 0x50)
                    dllId, objId = struct.unpack('>Hh', binFile.read(4))

                    binFile.seek(offs + 0x55)
                    nModels, nPlayerObjs = struct.unpack('>BB', binFile.read(2))

                    binFile.seek(offs + 0x78)
                    mapId = struct.unpack('>H', binFile.read(2))[0] # grumble

                    binFile.seek(offs + 0x7C)
                    texts = struct.unpack('>4H', binFile.read(8))

                    binFile.seek(offs + 0x91)
                    name = binFile.read(11)
            except struct.error: break
            name = name.replace(b'\0', b' ').decode('ascii').strip()

            if dllId not in dllObjs: dllObjs[dllId] = []
            dllObjs[dllId].append((idx, name))

            d = binFile.read(4 * nModels)
            if len(d) < 4 * nModels: break
            modelIds = struct.unpack('>%dI'%nModels, d)
            #if nModels > 4: modelIds = modelIds[0:4]
            #models = ' '.join(map(lambda m: '%04X'%m, modelIds))
            #if nModels > 4: models += '…'

            try: objDef = [k for k, v in objInd.items() if v == idx][0] #or 0xFFFF
            except IndexError: objDef = 0xFFFF
            #print('%04X|%04X|%-11s|%-20s|%04X|%2d|' % (
            #    idx, objDef, name, models, dllId, nPlayerObjs))

            enumName = name
            if enumName in seenObjs: enumName += '_%04X' % objDef
            seenObjs[enumName] = objDef
            #print("\t%s = 0x%04X," % (enumName, objDef))

            try: obj = objects.findall("./object[@id='0x%04X']" % idx)[0]
            except IndexError:
                obj = ET.SubElement(objects, 'object', {
                    'id': '0x%04X' % idx
                })

            # remove existing children before re-adding
            for child in list(obj.findall('./model')): obj.remove(child)
            for child in list(obj.findall('./seq')): obj.remove(child)

            obj.set('def',   '0x%04X' % objDef)
            obj.set('clsId', '0x%04X' % objId)
            obj.set('name', name)
            if nPlayerObjs > 0: obj.set('nPlayerObjs', str(nPlayerObjs))
            if dllId != 0xFFFF: obj.set('dll', '0x%04X' % dllId)
            if mapId != 0xFFFF: obj.set('map', '0x%02X' % mapId)
            for i, mdl in enumerate(modelIds):
                eModel = ET.SubElement(obj, 'model')
                eModel.set('id', '0x%04X' % mdl)
                if i < 4 and texts[i] != 0xFFFF:
                    eModel.set('text', '0x%04X' % texts[i])

            #obj.set('nSeq', '%d' % nSeq)
            #obj.set('pSeq', '0x%04X' % pSeq)
            for i in range(nSeq):
                binFile.seek(offs + pSeq + (i*2))
                iSeq = struct.unpack('>H', binFile.read(2))[0] # grumble
                ET.SubElement(obj, 'seq', {'id':'0x%04X' % iSeq})
            idx += 1

#for id, objs in dllObjs.items():
#    objList = ', '.join(map(lambda obj: "%04X %11s " % obj, objs))
#    print("%04X: %s" % (id, objList))

#print("};")

print(ET.tostring(objects, encoding='unicode'))
