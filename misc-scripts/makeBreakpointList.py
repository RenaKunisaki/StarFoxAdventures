#!/usr/bin/env python3
"""Generate a breakpoint list for use with a logFuncCalls.py."""

import os
import sys
import xml.etree.ElementTree as ET

xmlPath    = '/mnt/guilmon/home/rena/projects/games/hax/sfa/data/U0'
dllsTree   = ET.parse(os.path.join(xmlPath, 'dlls.xml'))
dllsXml    = dllsTree.getroot()

with open('GSAE01.ini.template', 'rt') as file:
    template = file.read()

with open('/home/rena/.local/share/dolphin-emu/GameSettings/GSAE01.ini', 'wt') as file:
    file.write(template)
    for func in dllsXml.findall('.//function'):
        if func.get('valid', '1') != '0':
            addr = int(func.get('address'), 0)
            file.write('%02x,%03x,%03x nl\n' % (
                (addr >> 24),
                (addr >> 12) & 0xFFF,
                addr & 0xFFF))
                # WTF is this format!?


# 80,087,380 nl
