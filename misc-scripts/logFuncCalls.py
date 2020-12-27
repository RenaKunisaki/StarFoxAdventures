#!/usr/bin/env python3
"""Monitor the output from a modified version of Dolphin and build a list of
object types passed to functions.
"""
import os
import sys
import xml.etree.ElementTree as ET


calls = {}

def parseLine(line):
    fields = line.split(' ')
    if len(fields) != 35: return
    if fields[3] != 'BP': return
    fAddr = int(fields[4], 16)
    record = []
    for i in range(10):
        record.append((
            int(fields[5+(i*3)], 16), # register value
            int(fields[6+(i*3)], 16), # alloc tag
            int(fields[7+(i*3)], 16), # offset into heap block
        ))

    # add this record
    if fAddr not in calls: calls[fAddr] = {
        'count': 0,
    }
    calls[fAddr]['count'] += 1

    # key = concat all alloc tags from this record.
    key = ''.join(map(lambda r: str(r[1])+str(r[2]), record))
    if key not in calls[fAddr]:
        calls[fAddr][key] = record


# read the lines
for line in sys.stdin:
    if '---- Shutting down ----' in line: break
    parseLine(line)

# output the lines
eFuncs = ET.Element('functions')
for fAddr, records in calls.items():
    eFunc = ET.SubElement(eFuncs, 'function', {
        'address':'0x%08X' % fAddr,
        'count':str(records['count']),
    })
    for k, record in records.items():
        if k != 'count':
            eRecord = ET.SubElement(eFunc, 'record')
            for i, param in enumerate(record):
                ET.SubElement(eRecord, 'param', {
                    'reg':  str(i+3),
                    'val':  '0x%08X' % param[0],
                    'tag':  '0x%08X' % param[1],
                    'offs': '0x%08X' % param[2],
                })

print(ET.tostring(eFuncs, encoding='unicode'))
