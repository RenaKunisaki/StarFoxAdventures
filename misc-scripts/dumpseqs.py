#!/usr/bin/env python3
"""Dump sequence data from ANIMCURV.bin."""
import sys
import struct
import os
import os.path
import zlib
import re
import xml.etree.ElementTree as ET

OUTPUT_XML = False
discPath = sys.argv[1]
xmlPath  = sys.argv[2]
eTexts   = ET.parse(os.path.join(xmlPath, 'gametext.xml')).getroot()
eSeqs    = ET.Element('sequences')

MAIN_CMDS = {
    0x00: 'SetCurvePos',
    0x01: 'CameraAnim',
    0x02: 'PlayAnim',
    0x03: 'AnimCmd03',
    0x04: 'ModelCmd04',
    0x06: 'PlaySound',
    0x07: 'ToggleCmd07',
    0x09: 'SetLoopPoint',
    0x0B: 'QueueActions',
    0x0D: 'SubCmd0D',
    0x0E: 'ShowDialogue',
    0x0F: 'StopSound',
    0xFF: 'SetCurveLen',
}
CMDS_0B = { # for MAIN_CMDS
    0x01: 'Goto',
    0x02: 'SubCmd02',
    0x03: 'AddParamToField60',
    0x04: 'While',
    0x05: 'End',
    0x06: 'SubCmd06',
    0x07: 'FindObjOrAnimate',
    0x08: 'SetEventLabel',
    0x09: 'Label',
    0x0A: 'GotoIf',
}
CMDS_02 = { # for CMDS_0B
    0x00: 'EnqueuePlayerCmd',
    0x01: 'SetField60',
    0x02: 'NOP',
    0x03: 'SetGlobal1',
    0x04: 'SetGlobal2',
    0x05: 'SetBool',
    0x06: 'SetGameBit',
}
CMDS_06 = { # for CMDS_06
    0x00: 'End',
    0x02: 'FindCurve',
    0x07: 'Earthquake',
    0x09: 'SetFlag7F_1',
    0x0A: 'StartCountupTimer',
    0x0B: 'StartCountdownTimer',
    0x0C: 'SetTimerToCountUp',
    0x0D: 'SoundCmd0D',
    0x0E: 'FadeFromBlackForWarp',
    0x0F: 'FadeFromBlack',
    0x10: 'SetField7D',
    0x13: 'ClearSomeFlag',
    0x14: 'CameraModeTestStrength',
    0x15: 'CamModeStatic', # XXX verify
    0x17: 'SwitchModel',
    0x18: 'PullOutStaff',
    0x19: 'PutAwayStaff',
    0x1A: 'CameraModeNormal',
    0x1B: 'SetObjGroupStatusTo1',
    0x1C: 'SetObjGroupStatusTo0',
    0x1D: 'SetMapAct',
    0x1E: 'SetSomeFlag',
    0x1F: 'ClearRestartPoint',
    0x20: 'GotoRestartPoint',
    0x21: 'SetFlag6E_0400_andField136',
    0x22: 'ClearFlag6E_0400_andField136',
    0x23: 'SetIsSavePoint',
    0x24: 'SavePoint',
    0x25: 'StopAndDisableTimer',
    0x26: 'PlayerLock',
    0x27: 'PlayVoice',
    0x28: 'PlayStream',
    0x2C: 'EnableMotionBlur',
    0x2D: 'DisableMotionBlur',
    0x2E: 'EnableMonochromeFilter',
    0x2F: 'DisableMonochromeFilter',
    0x30: 'EnvFxCmd30',
    0x31: 'EnvFxCmd31',
    0x32: 'EnvFxCmd32',
    0x33: 'SetCamVar',
}
CONDITIONS = { # for CMDS_0B
    0x00: 'True',
    0x01: 'Field60 < 1',
    0x02: 'Field60 > 0',
    0x03: 'IsDaytime',
    0x04: 'IsNight',
    0x05: 'Bool == 0',
    0x06: 'Bool == 1',
    0x07: 'Var == 0',
    0x08: 'Var != 0',
    0x09: 'Global1 < 1',
    0x0A: 'Global1 > 0',
    0x0B: 'Global2 < 1',
    0x0C: 'Global2 > 0',
    0x0D: 'TimerDisabled',
    0x0E: 'TimerEnabled',
    # skip 0x0F
    0x10: 'Global3 != 0',
    0x11: 'Global3 == 0',
}
CMDS_0D = { # for MAIN_CMDS
    0x00: 'PlaySong',
    0x02: 'GetEnvFxAct',
    0x03: 'ParticleFx',
    0x04: 'NOP04',
    0x05: 'LoadDll',
    0x06: 'WarpToMap',
    0x07: 'NOP07',
    0x08: 'SetEyeAndEyelid',
    0x0B: 'SetGameBit',
    0x0C: 'ClearGameBit',
    0x0D: 'SetInputOverride',
    0x0E: 'SetEyeState',
    0x0F: 'SetEyelidState',
}
EYE_STATES = {
    0x00: 'Normal',
    0x01: 'HalfClosed',
    0x02: 'Closed',
    0x03: '3QuarterClosed',
    0x04: 'WideOpen',
    0x05: 'Confused',
    0x06: 'Annoyed',
}
CURVE_ATTRS = {
    0x00: 'rot9E_Z',
    0x01: 'rot9E_X',
    0x02: 'rot9E_Y',
    0x03: 'Opacity',
    0x04: 'TimeOfDay',
    0x05: 'Scale',
    0x06: 'RotZ',
    0x07: 'RotX',
    0x08: 'RotY',
    0x09: 'AnimTimer',
    0x0A: 'pointSound',
    0x0B: 'PosZ',
    0x0C: 'PosY',
    0x0D: 'PosX',
    0x0E: 'Unk0E',
    0x0F: 'EyeX',
    0x10: 'EyeY',
    0x11: 'Unk11_X',
    0x12: 'Unk12', # sound related
}
EVENTS = {
    0x12: "A Pressed",
    0x13: "B Pressed",
    0x1A: "Talking to NPC",
}

def readCmd0B(data, offs, action):
    # XXX do XML
    for iCmd in range(action[2]):
        try:
            param, lengthAndOp = struct.unpack_from('>HH', data, offs)
            idx = lengthAndOp >> 6
            cmd = lengthAndOp & 0x3F
            if cmd in (2,3): param = struct.unpack_from('>h', data, offs)[0] # grumble
        except struct.error:
            break
        if not OUTPUT_XML:
            if   cmd == 0x02: name = CMDS_02.get(idx, '?')
            elif cmd == 0x04: name = 'While(%s)' % CONDITIONS.get(idx, 'Cond %02X' % idx)
            elif cmd == 0x06: name = CMDS_06.get(idx, '?')
            elif cmd == 0x08: name = "SetEvent(%s)" % EVENTS.get(idx, '0x%02X' % idx)
            elif cmd == 0x0A: name = 'If(%s) Goto' % CONDITIONS.get(idx, 'Cond %02X' % idx)
            else: name = CMDS_0B.get(cmd, '?')
            print("    @%06X #%04X | %02X %-12s %02X %04X" % (
                offs, iCmd, cmd, name, idx, param,
            ))
        offs += 4
    return offs


def readCommands(data, header, offs, eSeq):
    curTime = 0
    iAct = 0
    for iAct in range(header[2]): # read actions
        try:
            action = struct.unpack_from('>BBH', data, offs)
        except struct.error:
            break
        cmd, time, param = action[0], action[1], action[2]
        ET.SubElement(eSeq, 'act', {
            'idx':    str(iAct),
            #'offset': '0x%04X' % offs,
            'cmd':    '0x%02X' % action[0],
            'time':   '0x%02X' % action[1],
            'param':  '0x%04X' % action[2],
        })

        if not OUTPUT_XML:
            if cmd == 0x0D:
                cnd = param >> 12
                param &= 0x0FFF
                name = CMDS_0D.get(cmd, '?')
                print("@%06X #%04X t=%02X [%04X-%04X] | %02X %-12s %04X" % (
                    offs, iAct, time, curTime, time+curTime, cmd, name, param,
                ))
            else:
                name = MAIN_CMDS.get(cmd, '?')
                print("@%06X #%04X t=%02X [%04X-%04X] | %02X %-12s %04X" % (
                    offs, iAct, time, curTime, time+curTime, cmd, name, param,
                ))
                if cmd == 0x0E:
                    text = eTexts.find('./text[@id="0x%04X"]' % param)
                    if text:
                        for phrase in text.findall('./phrase'):
                            text = phrase.text
                            if text is None: text = "<EMPTY PHRASE>"
                            print("    "+text)
                    else: print("  <NOT FOUND>")

        offs += 4
        if cmd == 0x0B: offs = readCmd0B(data, offs, action)
        if cmd not in (0x00, 0x03, 0x09, 0x0B, 0x0F): curTime += time
    return offs


def readCurves(data, header, offs, eSeq):
    #if not OUTPUT_XML:
    #    print("    Curve points:")
    idx   = 0
    attr  = 0
    start = 0
    attrs = []
    while offs < len(data):
        try:
            item = struct.unpack_from('>fBBh', data, offs)
        except struct.error:
            break
        point, flags, field, xpos = item
        if field & 0x1F != attr: # end of curve
            attrs.append({
                'id':    attr,
                'name':  CURVE_ATTRS.get(attr, 'unk%02X' % attr),
                'start': start,
                'end':   idx-1,
            })
            start = idx
            attr = field & 0x1F

        ET.SubElement(eSeq, 'point', {
            'idx':   str(idx),
            'f':     '%6.4f'  % point,
            'flags': '0x%02X' % flags,
            'field': '0x%02X' % field,
            'xpos':  '0x%04X' % xpos,
        })
        #if not OUTPUT_XML:
        #    print("    @%06X #%04X x=%6d y=%7.2f fld=%02X flg=%02X" % (
        #        offs, idx, xpos, point, field, flags))
        offs += 8
        idx  += 1

    attrs.append({
        'id':    attr,
        'name':  CURVE_ATTRS.get(attr, 'unk%02X' % attr),
        'start': start,
        'end':   idx-1,
    })

    if not OUTPUT_XML:
        print("    Attr Curves:")
        for attr in attrs:
            print("    %02X %-12s %d - %d" % (attr['id'], attr['name'], attr['start'], attr['end']))


def readAnims(path):
    s = path.rfind('/')
    name = path[s+1:]

    # read table
    entries = []
    idx = 1
    with open(os.path.join(path,'ANIMCURV.tab'), 'rb') as file:
        offs = struct.unpack('>I', file.read(4))[0] # grumble
        while True:
            next = struct.unpack('>I', file.read(4))[0] # grumble
            if offs == 0xFFFFFFFF or next == 0xFFFFFFFF: break
            if offs & 0x80000000:
                entries.append({
                    'offset': offs & 0xFFFFFF,
                    'idx': idx,
                    'len': (next & 0xFFFFFF) - (offs & 0xFFFFFF),
                })
            idx += 1
            offs = next

    # read data
    with open(os.path.join(path,'ANIMCURV.bin'), 'rb') as file:
        iEntry = 0
        for entry in entries:
            file.seek(entry['offset'])
            data = file.read(entry['len'])
            header = struct.unpack_from('>4sHH', data, 0)
            if header[0] in (b'SEQA', b'SEQB'):
                size, nAct = header[1], header[2]
                eSeq = ET.SubElement(eSeqs, 'seq', {
                    'dir':      name,
                    'idx':      str(entry['idx']),
                    'sig':      header[0].decode('ascii'),
                    'offset':   '0x%06X' % entry['offset'],
                    'size':     '0x%04X' % header[1],
                    #'nActions': '%d' % header[2],
                })
                if not OUTPUT_XML:
                    print("\nScript 0x%02X @ 0x%06X: %d actions, size 0x%X:" % (
                        iEntry, entry['offset'], nAct, size))
                offs = readCommands(data, header, 8, eSeq)
                readCurves(data, header, offs, eSeq)

            else:
                sys.stderr.write("Unknown header: %r\n" % header[0].hex())
            iEntry += 1
        # for entry


#for name in os.listdir(discPath):
#    path = os.path.join(discPath, name, 'ANIMCURV.tab')
#    if os.path.isfile(path):
#        readAnims(os.path.join(discPath, name))

readAnims(discPath)

if OUTPUT_XML:
    print(ET.tostring(eSeqs, encoding='unicode'))
