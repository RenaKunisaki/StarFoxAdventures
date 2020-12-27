#!/usr/bin/env python3

with open('../notes/gamebits/gamebits.txt', 'rt') as inFile:
    for line in inFile:
        line = line.split('|')
        if not line[0].startswith('0'): continue
        print('<bit id="0x%s" table="%s" unk1="0x%s" unk2="0x%s" max="0x%s" name="%s">%s</bit>' % (
            line[0],
            line[1],
            line[2],
            line[3],
            line[4],
            line[5],
            line[6].strip(),
        ))
