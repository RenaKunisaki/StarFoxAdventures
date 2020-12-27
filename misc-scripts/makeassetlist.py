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
    'cloudrace',
    #'cloudtreasure',
    'crfort',
    'darkicemines',
    'darkicemines2',
    #'dbay',
    'dbshrine',
    'desert',
    #'dfalls',
    'dfptop',
    'dfshrine',
    'dragrock',
    'dragrockbot',
    'ecshrine',
    #'frontend',
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
    #'swapcircle',
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
    #for i in range(len(modelsTab)):
        #idx = i
        entry = modelsTab[idx]
        # i = model ID, idx = index into this MODELS.tab
        # entry = entry from this MODELS.tab
        # but somehow, this is backward!?
        # model ID can be negative to prevent this remap but is that used?
        # eg in warlock, entry 0x4E8 = 0, entry 0x54F = 0x4E8
        # and 0 doesn't mean "don't remap", it means use model 0, so...?
        if (idx != 0 and entry != 0) or i == 0:
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
        # hack to check if it's not a dummy entry
        if entry != 0 and entry != 0x81000040 and entry != 0x01000000:
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
        if entry != 0 and entry != 0x81000040 and entry != 0x01000000:
            if i not in tex1Elems:
                tex1Elems[i] = ET.SubElement(eTex1, 'texture', {'idx':'0x%04X'%i})
            eTex = tex1Elems[i]
            ET.SubElement(eTex, 'map', {
                'dir': mapName,
                'entry': '0x%08X' % entry,
            })


print(ET.tostring(root, encoding='unicode'))
