#!/usr/bin/env python3
"""Read GLOBALMA.bin and stuff the info into maps.xml."""
import sys
import os.path
import math
import struct
import xml.etree.ElementTree as ET

def printf(fmt, *args, **kwargs):
    print(fmt % args, end='', **kwargs)

def readStruct(file, fmt, offset=None):
    size = struct.calcsize(fmt)
    if offset is not None: file.seek(offset)
    data = file.read(size)
    r = struct.unpack(fmt, data)
    if len(r) == 1: return r[0] # grumble
    return r


def main(root, xmlPath):
    maps = {}
    #print("Read globalma...")
    with open(os.path.join(root, 'globalma.bin'), 'rb') as grid:
        offs = 0
        while True:
            x, y, layer, mapId, link0, link1 = readStruct(grid, '>6h', offs)
            if mapId < 0: break
            linked = []
            if link0 >= 0: linked.append(link0)
            if link1 >= 0: linked.append(link1)
            maps[mapId] = {
                'id':     mapId,
                'x':      x,
                'y':      y,
                'layer':  layer,
                'linked': linked,
            }
            offs += 12

    #print("Read MAPS.tab...")
    with open(os.path.join(root, 'MAPS.tab'), 'rb') as tab:
        idx  = 0
        while True:
            try: entries = readStruct(tab, '>7i', idx*7*4)
            except: break
            if entries[0] < 0: break
            if idx not in maps: maps[idx] = {}
            maps[idx]['infoOffset'] = entries[0]
            maps[idx]['blockTable'] = entries[1]
            maps[idx]['unk2']       = entries[2]
            maps[idx]['unk3']       = entries[3]
            maps[idx]['unk4']       = entries[4]
            maps[idx]['unk5']       = entries[5]
            maps[idx]['unk6']       = entries[6] #romlist size?
            idx += 1

    #print("Read MAPS.bin...")
    with open(os.path.join(root, 'MAPS.bin'), 'rb') as mapsBin:
        for map in maps.values():
            infoOffset = map.get('infoOffset', -1)
            blockTable = map.get('blockTable', -1)
            if infoOffset >= 0:
                w, h, x, y, unk08, unk0C, unk10, unk14, unk18, unk1C, unk1E = \
                    readStruct(mapsBin, '>4h5I2h', infoOffset)
                map['w'], map['h'] = w, h
                map['originX'], map['originY'] = x, y
                map['infoUnk08'] = '0x%08X' % unk08 # relates to objects, maybe size to allocate for them?
                map['infoUnk0C'] = '0x%08X' % unk0C
                map['infoUnk10'] = '0x%08X' % unk10
                map['infoUnk14'] = '0x%08X' % unk14
                map['infoUnk18'] = '0x%08X' % unk18
                map['infoUnk1C'] = '0x%04X' % unk1C # related to romlist? or seq?
                map['infoUnk1E'] = '0x%04X' % unk1E # related to romlist?

            if blockTable >= 0:
                map['blocks'] = []
                w = max(map['w'], 0)
                h = max(map['h'], 0)
                offs = blockTable
                for y in range(h):
                    for x in range(w):
                        try: block = readStruct(mapsBin, '>I', offs)
                        except: break
                        offs += 4
                        mod   = (block >> 23) & 0xFF
                        sub   = (block >> 17) & 0x3F
                        if mod != 0xFF:
                            map['blocks'].append({
                                'mod': str(mod),
                                'sub': str(sub),
                                'x':   str(x),
                                'y':   str(y),
                                'unk1':str(block >> 31),
                                'unk2':'0x%X' % (block & 0x1FF),
                            })

    mapsXml = ET.parse(xmlPath).getroot()

    # convert descriptions
    #for map in mapsXml.findall('./map'):
    #    if map.text is not None and map.text.strip():
    #        ET.SubElement(map, 'description').text = map.text.strip()
    #        map.text = None

    # collapse empty tags
    for map in mapsXml.findall('./map'):
        if map.text is not None and map.text.strip() == '':
            map.text = None

    for i, map in maps.items():
        node = mapsXml.find("./map[@id='0x%02X']" % i)
        if node is None: continue

        # stuff into a list first so we don't change node during iteration
        for block in list(node.findall('./block')):
            node.remove(block)

        if 'x' in map:
            node.set('worldX', str(map['x']))
            node.set('worldY', str(map['y']))
            node.set('layer',  str(map['layer']))
            if len(map['linked']) > 0: node.set('link0', '0x%02X' % map['linked'][0])
            if len(map['linked']) > 1: node.set('link1', '0x%02X' % map['linked'][1])
        if 'originX' in map:
            node.set('w',       str(map['w']))
            node.set('h',       str(map['h']))
            node.set('originX', str(map['originX']))
            node.set('originY', str(map['originY']))
            #del node.attrib['nBlocks']
            node.set('unk08',   str(map['infoUnk08']))
            node.set('unk0C',   str(map['infoUnk0C']))
            node.set('unk10',   str(map['infoUnk10']))
            node.set('unk14',   str(map['infoUnk14']))
            node.set('unk18',   str(map['infoUnk18']))
            node.set('unk1C',   str(map['infoUnk1C']))
            node.set('unk1E',   str(map['infoUnk1E']))
        if 'infoOffset' in map:
            node.set('infoOffset', '0x%08X' % map['infoOffset'])
            node.set('blockTable', '0x%08X' % map['blockTable'])
            node.set('unkOffs2',   '0x%08X' % map['unk2'])
            node.set('unkOffs3',   '0x%08X' % map['unk3'])
            node.set('unkOffs4',   '0x%08X' % map['unk4'])
            node.set('unkOffs5',   '0x%08X' % map['unk5'])
            node.set('unkOffs6',   '0x%08X' % map['unk6'])
        if 'blocks' in map:
            for block in map['blocks']:
                ET.SubElement(node, 'block', block)

        if 'dir' in node.attrib:
            try:
                files = os.listdir(os.path.join(root, node.get('dir')))
                if len(files) == 0: node.set('emptydir', '1')
            except FileNotFoundError:
                node.set('missing', '1')

    print(ET.tostring(mapsXml, encoding='unicode'))



if __name__ == '__main__':
    if len(sys.argv) > 1:
        main(sys.argv[1], sys.argv[2])
    else:
        print("Usage: %s disc-root-path maps-xml-path" % sys.argv[0])
