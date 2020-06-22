#!/usr/bin/env python3
"""Reads gamebits.txt and produces:
- one list per table, in gamebitsN.txt
- GameBit.h for import into Ghidra
- a list of all known bits, in stdout
"""

tables = [
    {'id':0, 'bits':[]},
    {'id':1, 'bits':[]},
    {'id':2, 'bits':[]},
    {'id':3, 'bits':[]},
]
allBits = []
longestName = 4 # "Name" itself is 4 letters

# read original file
with open('gamebits.txt', 'rt') as file:
    for line in file:
        if not line.startswith('0'): continue
        line = line.split('|')
        bit  = {
            'id': int(line[0], 16),
            'tbl': int(line[1]),
            'unk': (int(line[2], 16), int(line[3], 16)),
            'max': int(line[4], 16),
            'name': line[5].strip(),
            'note': line[6].strip(),
        }
        longestName = max(longestName, len(bit['name']))
        allBits.append(bit)
        tables[bit['tbl']]['bits'].append(bit)

namePad  = ' ' * (longestName - 4)
nameDash = '-' * (longestName - 4)

# read dumps
dumps = (
    'NewGame',
    'Chap2',
    'Chap3',
    'Chap4',
    'Chap5',
    'EndGame',
)
for dumpName in dumps:
    for line in open('gamebits%s.txt' % dumpName, 'rt'):
        if not line.startswith('\x1B[48;'): continue
        line = line.split('│') # not a '|'
        bitId = int(line[0].split('m')[1], 16)
        allBits[bitId][dumpName] = int(line[4])


# write files per table
for table in tables:
    with open('gamebits%d.txt' % table['id'], 'wt') as file:
        file.write("Bit#|Name%s|??|?|MaxValue|NewGame |Chapter2|Chapter3|Chapter4|Chapter5|EndGame |Note\n" % namePad)
        file.write("----|----%s|--|-|--------|--------|--------|--------|--------|--------|--------|----\n" % nameDash)
        for bit in table['bits']:
            #if bit['note'] != '' or bit['name'] != '':
            file.write("%04X|%s|%02X|%X|%8X|%8X|%8X|%8X|%8X|%8X|%8X|%s\n" % (
                bit['id'], bit['name'].ljust(longestName),
                bit['unk'][0], bit['unk'][1], bit['max'],
                bit['NewGame'], bit['Chap2'], bit['Chap3'],
                bit['Chap4'], bit['Chap5'], bit['EndGame'],
                bit['note'] ))

# write known bits to stdout

for table in tables:
    print("\n# Table %d" % table['id'])
    print("Bit#|Name%s|??|?|MaxValue|NewGame |Chapter2|Chapter3|Chapter4|Chapter5|EndGame |Note" % namePad)
    print("----|----%s|--|-|--------|--------|--------|--------|--------|--------|--------|----" % nameDash)
    for bit in table['bits']:
        if bit['note'] != '' or bit['name'] != '':
            print("%04X|%s|%02X|%X|%8X|%8X|%8X|%8X|%8X|%8X|%8X|%s" % (
                bit['id'], bit['name'].ljust(longestName),
                bit['unk'][0], bit['unk'][1], bit['max'],
                bit['NewGame'], bit['Chap2'], bit['Chap3'],
                bit['Chap4'], bit['Chap5'], bit['EndGame'],
                bit['note'] )
            )

# write GameBit.h
with open('GameBit.h', 'wt') as file:
    file.write("typedef enum {\n")
    for bit in allBits:
        if bit['name'] != '':
            file.write("\t%s = 0x%04X,\n" % (bit['name'], bit['id']))
    file.write('} GameBit;\n')

    # Ghidra is a butt
    file.write("typedef enum {\n")
    for bit in allBits:
        if bit['name'] != '':
            file.write("\t%s = 0x%04X,\n" % (bit['name'], bit['id']))
    file.write('} GameBit16;\n')
