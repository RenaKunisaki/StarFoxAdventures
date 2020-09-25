#!/usr/bin/env python3
"""Read OBJECTS.bin and OBJECTS.tab and make a list of ID => name."""
import struct
import xml.etree.ElementTree as ET

#print("Obj#|Def#|Obj Name   |Models              |DLL |NP|Description")
#print("----|----|-----------|--------------------|----|--|-----------")
#print("enum ObjDef {")

tree = ET.parse('../dump/objects.xml')
objects = tree.getroot()

objInd = {}
with open('../discroot/OBJINDEX.bin', 'rb') as file:
    idx = 0
    while True:
        entry = file.read(2)
        if len(entry) < 2: break
        entry = struct.unpack('>H', entry)[0] # grumble
        objInd[idx] = entry
        idx += 1

dllObjs = {}
seenObjs = {}
with open('../discroot/OBJECTS.tab', 'rb') as tabFile:
    with open('../discroot/OBJECTS.bin', 'rb') as binFile:
        for idx in range(0x5C5):
            d = tabFile.read(4)
            if len(d) < 4: break
            offs = struct.unpack('>I', d)[0] # grumble
            if offs == 0xFFFFFFFF: break

            binFile.seek(offs + 0x50)
            dllId = struct.unpack('>H', binFile.read(2))[0] # grumble

            binFile.seek(offs + 0x55)
            nModels, nPlayerObjs = struct.unpack('>BB', binFile.read(2))

            binFile.seek(offs + 0x78)
            mapId = struct.unpack('>H', binFile.read(2))[0] # grumble

            binFile.seek(offs + 0x7C)
            texts = struct.unpack('>4H', binFile.read(8))

            binFile.seek(offs + 0x91)
            name = binFile.read(11).replace(b'\0', b' ')
            name = name.decode('ascii')

            if dllId not in dllObjs: dllObjs[dllId] = []
            dllObjs[dllId].append((idx, name))

            d = binFile.read(4 * nModels)
            if len(d) < 4 * nModels: break
            modelIds = struct.unpack('>%dI'%nModels, d)
            #if nModels > 4: modelIds = modelIds[0:4]
            #models = ' '.join(map(lambda m: '%04X'%m, modelIds))
            #if nModels > 4: models += '…'

            objDef = [k for k, v in objInd.items() if v == idx][0] #or 0xFFFF
            #print('%04X|%04X|%-11s|%-20s|%04X|%2d|' % (
            #    idx, objDef, name, models, dllId, nPlayerObjs))

            enumName = name
            if enumName in seenObjs: enumName += '_%04X' % objDef
            seenObjs[enumName] = objDef
            #print("\t%s = 0x%04X," % (enumName, objDef))

            obj = objects.findall("./object[@id='0x%04X']" % idx)[0]
            obj.set('def', '0x%04X' % objDef)
            if nPlayerObjs > 0: obj.set('nPlayerObjs', str(nPlayerObjs))
            if dllId != 0xFFFF: obj.set('dll', '0x%04X' % dllId)
            if mapId != 0xFFFF: obj.set('map', '0x%02X' % mapId)
            for i, mdl in enumerate(modelIds):
                eModel = ET.SubElement(obj, 'model')
                eModel.set('id', '0x%04X' % mdl)
                if i < 4 and texts[i] != 0xFFFF:
                    eModel.set('text', '0x%04X' % texts[i])
                    # XXX eModel.text = the actual text

#for id, objs in dllObjs.items():
#    objList = ', '.join(map(lambda obj: "%04X %11s " % obj, objs))
#    print("%04X: %s" % (id, objList))

#print("};")

print(ET.tostring(objects, encoding='unicode'))
