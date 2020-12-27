#!/usr/bin/env python3
"""Reads DLL info out of main.dol."""
import sys
import struct
import os
import os.path
import xml.etree.ElementTree as ET

MAX_DLL_ID = 0x2C1
DLLS_ADDR = {
    'kiosk':   0x802b79c0,
    '1.00_us': 0x802C6300,
    '1.01_us': 0x802c6a80,
}

sections = {'text':[], 'data':[], 'bss':{}}

def printSections():
    print("Section  Offset   Address  Size")
    for i in range(7):
        sec = sections['text'][i]
        print("Text[%2X] %08X %08X %08X" % (i, sec['offset'], sec['address'], sec['size']))
    for i in range(11):
        sec = sections['data'][i]
        print("Data[%2X] %08X %08X %08X" % (i, sec['offset'], sec['address'], sec['size']))
    sec = sections['bss']
    print("BSS      -------- %08X %08X" % (sec['address'], sec['size']))

def dolAddressToOffset(addr):
    for sec in sections['data'] + sections['text']:
        startAddr = sec['address']
        endAddr   = startAddr + sec['size']
        if addr >= startAddr and addr < endAddr:
            return (addr - startAddr) + sec['offset']
    return None

version = sys.argv[2]
with open(sys.argv[1], 'rb') as mainDol:
    # read DOL header
    header = struct.unpack('>57I', mainDol.read(19*4*3))
    for i in range(19):
        sec = {
            'offset':  header[i],
            'address': header[i+18],
            'size':    header[i+36],
        }
        if i <= 6: sections['text'].append(sec)
        elif i <= 17: sections['data'].append(sec)
        else:
            # entryPoint = bss.offset
            sections['bss'] = {
                'address': sec['size'],
                'size':    sec['address'],
            }


    # read DLLs
    dlls = {}
    base = DLLS_ADDR[version]
    for i in range(MAX_DLL_ID+1):
        mainDol.seek(dolAddressToOffset(base + (i*4)))
        ptr = struct.unpack('>I', mainDol.read(4))[0] # grumble
        if ptr != 0:
            offs = dolAddressToOffset(ptr)
            if offs is not None:
                mainDol.seek(offs)
                data = struct.unpack('>3I2H', mainDol.read(16))
                dll = {
                    'id':      i,
                    'address': ptr,
                    'dolOffs': offs,
                    'prev':    data[0],
                    'next':    data[1],
                    'unk08':   data[2],
                    'nFuncs':  data[3],
                    'unk0E':   data[4],
                    'funcs':   [],
                }
                nFuncs = dll['nFuncs']+1
                try:
                    funcs = struct.unpack('>%dI' % nFuncs, mainDol.read(nFuncs*4))
                    for func in funcs: dll['funcs'].append({
                        'address': func,
                        'stub':    False,
                        'valid':   True,
                    })
                except Exception as ex:
                    #print("Error reading function pointers for DLL 0x%04X (%d funcs)" % (
                    #    i, nFuncs), ex)
                    pass
                dlls[i] = dll
            else:
                #print("DLL 0x%04X has invalid pointer: 0x%08X"% (i, ptr))
                dlls[i] = {
                    'id':      i,
                    'address': ptr,
                    'funcs':   [],
                }
            for func in dll['funcs']:
                offs = dolAddressToOffset(func['address'])
                func['valid'] = (offs is not None)
                if func['valid']:
                    mainDol.seek(offs)
                    code = struct.unpack('>2I', mainDol.read(8))
                    if code[0] == 0x4E800020: func['stub'] = True
                    elif (code[0] & 0xFFFF0000 == 0x38600000
                    and   code[1] == 0x4E800020):
                        ret = code[0] & 0xFFFF
                        if ret >= 0x8000: ret = -(0x10000 - ret)
                        func['stub'] = True
                        func['return'] = str(ret)
        else: # ptr == 0
            dlls[i] = {
                'id':      i,
                'address': 0,
                'prev':    0,
                'next':    0,
                'unk08':   0,
                'unk0E':   0,
                'funcs':   [],
            }

    # merge with existing XML
    dllsXml = ET.parse('../data/dlls.xml').getroot()
    for dll in dlls.values():
        eDll = dllsXml.findall("./dll[@id='0x%04X']" % dll['id'])[0]
        eDll.set('address', '0x%08X' % dll['address'])
        if 'dolOffs' in dll: eDll.set('dolOffs', '0x%06X' % dll['dolOffs'])
        if dll['prev'] != 0: eDll.set('prev', '0x%08X' % dll['prev'])
        if dll['next'] != 0: eDll.set('next', '0x%08X' % dll['next'])
        eDll.set('unk08', '0x%08X' % dll['unk08'])
        eDll.set('unk0E', '0x%04X' % dll['unk0E'])

        # original file had description text directly under <dll>
        eDescr = eDll.findall('./description')
        if len(eDescr) > 0:
            eDescr = eDescr[0]
            #eDescr.clear()
        else: eDescr = ET.SubElement(eDll, 'description')
        #if eDll.text is not None and eDll.text.strip():
        #    eDescr.text = eDll.text
        #    eDll.text = None


        eFuncs = eDll.findall("./functions")
        if len(eFuncs) > 0:
            eFuncs = eFuncs[0]
            eFuncs.clear()
        else: eFuncs = ET.SubElement(eDll, 'functions')

        nNonStubs = 0
        for iFunc, func in enumerate(dll['funcs']):
            attr = {
                'idx':     '0x%X' % iFunc,
                'address': '0x%08X' % func['address'],
            }
            if not func['valid']: attr['valid'] = '0'
            if func['stub']: attr['stub'] = '1'
            if 'return' in func: attr['return'] = func['return']
            ET.SubElement(eFuncs, 'function', attr)

            if func['valid'] and not func['stub']: nNonStubs += 1
        if nNonStubs == 0:
            eDll.set('name', "Dummy%02X" % dll['id'])
            if eDescr.text is None:
                if 'dolOffs' in dll: eDescr.text = "All functions stubbed"
                elif dll['address'] == 0: eDescr.text = "Null address"
                else: eDescr.text = "Invalid address"

    print(ET.tostring(dllsXml, encoding='unicode'))
