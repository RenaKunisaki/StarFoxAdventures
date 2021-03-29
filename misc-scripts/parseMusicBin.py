#!/usr/bin/env python3
"""Dump all entries from MUSIC.BIN."""
import sys
import os.path
import struct

def readStr(file):
    r = []
    while True:
        b = file.read(1)
        if len(b) == 0 or b[0] == 0: break
        r.append(chr(b[0]))
    return ''.join(r)

def getSongNames():
    songs = []
    with open("/home/rena/.local/share/dolphin-emu/Dump/mem1.raw", 'rb') as mem:
        for i in range(99):
            mem.seek(0x2c5700 + (i*16))
            entry = mem.read(16)
            id, unk2, unk3, pName, unk8, unkC = struct.unpack('>HBBIII', entry)
            mem.seek(pName & 0x017F_FFFF)
            name = readStr(mem)
            songs.append({
                'name': name,
                'idx':  i,
                'id':   id,
                'unk2': unk2,
                'unk3': unk3,
                'unk8': unk8,
                'unkC': unkC,
            })
    return songs

def main():
    if len(sys.argv) < 2:
        print("Usage: %s music.bin" % sys.argv[0])
        sys.exit(1)
    songs = getSongNames()
    with open(sys.argv[1], 'rb') as file:
        idx = 0
        print("Idx |ID   Song|04   06 07|08 09 0A 0B|0C 0D 0E Flg|Song")
        while True:
            entry = file.read(16)
            if len(entry) < 16: break
            id, sid, unk4, unk6, unk7, unk8, unk9, unkA, unkB, \
            unkC, unkD, unkE, flags = struct.unpack('>HHHBBBBBBBBBB', entry)
            try:
                song = songs[sid]
                songId, songName = song['id'], song['name']
            except:
                songId, songName = 0xFF, 'dynamic'
            #print("%04X|%04X %04X|%04X %02X %02X|%02X %02X %02X %02X|%02X %02X %02X %02X |%02X %s" % (
            #    idx, id, sid, unk4, unk6, unk7, unk8, unk9, unkA, unkB,
            #    unkC, unkD, unkE, flags, songId, songName))
            print("#define %s_%02X 0x%02X" % (songName, idx, id))
            idx += 1

if __name__ == '__main__':
    main()
