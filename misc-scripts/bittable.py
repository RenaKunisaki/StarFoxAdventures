#!/usr/bin/env python3
"""Add BITTABLE.BIN data to gamebits.xml and improve it a bit^H^H^Htad.

args: BITTABLE.BIN gametext.xml gamebits.xml > newgamebits.xml
"""
import xml.etree.ElementTree as ET
import struct
import sys

bitTable = []
with open(sys.argv[1], 'rb') as file:
    while True:
        data = file.read(4)
        if len(data) < 4: break
        offs, flags, hint = struct.unpack('>HBB', data)
        bit = {
            'offset': offs, # bit offset into table
            'size':   (flags & 0x1F) + 1, # binary width
            'table':  flags >> 6,
            'hint':   None,
        }
        if flags & (1 << 5): # has hint
            bit['hint'] = hint + 0xF4
        bitTable.append(bit)

textXml = ET.parse(sys.argv[2]).getroot()

bitsXml = ET.parse(sys.argv[3]).getroot()
for eBit in bitsXml.findall('bit'):
    bitId = int(eBit.get('id'), 0)
    try: bit = bitTable[bitId]
    except IndexError: break
    eBit.attrib.pop('hasHint', None)
    eBit.attrib.pop('max',     None)
    eBit.attrib.pop('hint',    None)
    eBit.set('size', str(bit['size']))
    eBit.set('offset', '0x%04X' % bit['offset'])

    if bit['hint'] is not None:
        eBit.set('hintid', str(bit['hint']))
        text = textXml.find("text[@id='0x%04X']" % bit['hint'])
        if text:
            eHint = ET.SubElement(eBit, 'hint')
            for phrase in text.findall('phrase'):
                eHint.append(phrase)
        else:
            print("Text 0x%04X not found" % bit['hint'])
            sys.exit(1)

print(ET.tostring(bitsXml, encoding='unicode'))
