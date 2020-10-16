#!/usr/bin/env python3
"""Builds list of which assets are present in which maps."""
import sys
import struct
import os
import os.path
import zlib
import xml.etree.ElementTree as ET

def readTab16(path):
    entries = []
    with open(path, 'rb') as file:
        while True:
            d = file.read(2)
            if len(d) < 2: break
            offs = struct.unpack('>h', d)[0] # grumble
            entries.append(offs)
    return entries

def readTab32(path):
    entries = []
    with open(path, 'rb') as file:
        while True:
            d = file.read(4)
            if len(d) < 4: break
            offs = struct.unpack('>I', d)[0] # grumble
            if offs == 0xFFFFFFFF: break
            entries.append(offs)
    return entries

maps = (
    'animtest',
    'arwing',
    'arwingcity',
    'arwingcloud',
    'arwingdarkice',
    'arwingdragon',
    'arwingtoplanet',
    'bossdrakor',
    'bossgaldon',
    'bosstrex',
    'capeclaw',
    'clouddungeon',
    #'cloudjoin',
    'cloudrace',
    #'cloudtreasure',
    'crfort',
    'darkicemines',
    'darkicemines2',
    #'dbay',
    'dbshrine',
    'desert',
    'dfptop',
    'dfshrine',
    'dragrock',
    'dragrockbot',
    'ecshrine',
    'gamefront',
    'gpshrine',
    'greatfox',
    'icemountain',
    #'insidegal',
    'lightfoot',
    'linka',
    'linkb',
    'linkc',
    'linkd',
    'linke',
    'linkf',
    'linkg',
    'linkh',
    'linki',
    'linkj',
    #'linklevel',
    'magiccave',
    'mazecave',
    'mmpass',
    'mmshrine',
    'nwastes',
    'nwshrine',
    'shipbattle',
    'shop',
    'swaphol',
    'swapholbot',
    'volcano',
    'wallcity',
    'warlock',
    'worldmap',
)

path = sys.argv[1]
mapTables = {}
for name in maps:
    mapTables[name] = {
        'anim':     readTab32(path+'/'+name+'/ANIM.TAB'),
        'animcurv': readTab32(path+'/'+name+'/ANIMCURV.tab'),
        'modelInd': readTab16(path+'/'+name+'/MODELIND.bin'),
        'models':   readTab32(path+'/'+name+'/MODELS.tab'),
        'objseq':   readTab16(path+'/'+name+'/OBJSEQ.tab'),
        'objseq2C': readTab16(path+'/'+name+'/OBJSEQ2C.tab'),
        'tex0':     readTab32(path+'/'+name+'/TEX0.tab'),
        'tex1':     readTab32(path+'/'+name+'/TEX1.tab'),
        'voxmap':   readTab32(path+'/'+name+'/VOXMAP.tab'),
    }

root = ET.Element('assets')


# list models
modelElems = {}
eModels = ET.SubElement(root, 'models');
for mapName, table in mapTables.items():
    modelsTab = table['models']
    for i, idx in enumerate(table['modelInd']):
        entry = modelsTab[idx]
        if idx != 0 and entry != 0:
            if idx not in modelElems:
                modelElems[idx] = ET.SubElement(eModels, 'model', {'idx':'0x%04X'%idx})
            eModel = modelElems[idx]
            ET.SubElement(eModel, 'map', {
                'dir':   mapName,
                'entry': '0x%08X' % entry,
                'idx':   '0x%04X' % i,
            })

# list TEX0
tex0Elems = {}
eTex0 = ET.SubElement(root, 'tex0');
for mapName, table in mapTables.items():
    for i, entry in enumerate(table['tex0']):
        if entry != 0:
            if i not in tex0Elems:
                tex0Elems[i] = ET.SubElement(eTex0, 'texture', {'idx':'0x%04X'%i})
            eTex = tex0Elems[i]
            ET.SubElement(eTex, 'map', {
                'dir': mapName,
                'entry': '0x%08X' % entry,
            })

# list TEX1
tex1Elems = {}
eTex1 = ET.SubElement(root, 'tex1');
for mapName, table in mapTables.items():
    for i, entry in enumerate(table['tex1']):
        if entry != 0:
            if i not in tex1Elems:
                tex1Elems[i] = ET.SubElement(eTex1, 'texture', {'idx':'0x%04X'%i})
            eTex = tex1Elems[i]
            ET.SubElement(eTex, 'map', {
                'dir': mapName,
                'entry': '0x%08X' % entry,
            })


print(ET.tostring(root, encoding='unicode'))
