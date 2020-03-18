#!/usr/bin/env python3
import sys
import struct

with open(sys.argv[1], 'rb') as sfxBin:
    i = 0
    print(" ID | Volume |  Pan   |Unk6|Rang|SfxId  0|SfxId  1|SfxId  2|SfxId  3|SfxId  4|SfxId  5|RndMx|1E|Idxs")
    while True:
        line = sfxBin.read(32)
        if len(line) < 32: break
        data = struct.unpack('>HBBBBHHHHHHHHBBBBBBHBB', line)
        entry = {
            'id':         data[0],
            'baseVolume': data[1],
            'volumeRand': data[2],
            'basePan':    data[3],
            'panRand':    data[4],
            'unk06':      data[5],
            'range':      data[6],
            'fxIds':      data[7:13],
            'fxChance':   data[13:19],
            'randMax':    data[19],
            'unk1E':      data[20],
            'numIdxs':    data[21] >> 4,
            'prevIdx':    data[21] & 0xF,
        }
        print("%04X|%3d ±%3d|%3d ±%3d|%04X|%04X|%04X %3d|%04X %3d|%04X %3d|%04X %3d|%04X %3d|%04X %3d|%5d|%02X|%d %d" % (
            entry['id'],
            entry['baseVolume'],
            entry['volumeRand'],
            entry['basePan'],
            entry['panRand'],
            entry['unk06'],
            entry['range'],
            entry['fxIds'][0], entry['fxChance'][0],
            entry['fxIds'][1], entry['fxChance'][1],
            entry['fxIds'][2], entry['fxChance'][2],
            entry['fxIds'][3], entry['fxChance'][3],
            entry['fxIds'][4], entry['fxChance'][4],
            entry['fxIds'][5], entry['fxChance'][5],
            entry['randMax'],
            entry['unk1E'],
            entry['numIdxs'],
            entry['prevIdx'],
        ))
        i += 1
    #print("Used:  ", ', '.join(used))
    #print("Unused:", ', '.join(blank))
