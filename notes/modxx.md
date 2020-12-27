modXX.zlb.bin and modXX.tab
XX is the map's directory index, except if >= 5, 1 is added.

these contain either the map geometry or info essential to loading it.

the file name list has BLOCKS.BIN and BLOCKS.TAB in place of these names.

# animtest (mod6)

tab file is mostly empty:
(everything before here is zero)
000001E0  00 00 00 00  00 00 00 00  10 00 00 00  10 00 01 80  
000001F0  10 00 06 C0  10 00 1B A0  10 00 21 20  10 00 2A 80  
00000200  10 00 30 60  10 00 32 40  10 00 35 E0  10 00 3A A0  
00000210  10 00 3F 00  10 00 44 40  10 00 4E C0  10 00 53 00  
00000220  10 00 58 A0  10 00 5D 60  10 00 60 60  10 00 61 A0  
00000230  10 00 68 60  10 00 69 C0  10 00 6B 00  10 00 6E 60  
00000240  FF FF FF FF  00 00 00 00  00 00 00 00  00 00 00 00  
most of these are beyond EOF

bin file is pretty sparse: (0x360 bytes)
00000000  00 00 00 00  00 08 00 00  00 00 03 60  00 00 00 00
00000010  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00000020  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00000030  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00000040  00 00 00 00  00 00 00 00  00 00 00 00  00 00 03 24
00000050  00 00 02 D4  00 00 01 FC  00 00 01 80  00 00 01 B0
00000060  00 00 01 B4  00 00 02 08  00 00 00 B8  00 00 00 00
00000070  00 00 00 00  00 00 00 00  00 00 01 E8  00 00 00 00
00000080  00 00 00 00  00 12 00 00  00 00 FF 80  00 2C FF D6
00000090  00 08 00 00  00 02 00 0D  00 07 00 03  00 00 00 00
000000A0  03 03 03 00  6D 6F 64 36  2E 30 00 00  00 00 00 00 "mod6.0"
000000B0  00 00 00 00  00 00 00 00  

null: field 70: hits

00B8: field 68: 0x1C byte structs - dlist ptrs? count = 3
offs  +0 01 02 03  04 05 06 07  08 09 0A 0B  0C 0D 0E 0F  10 11 12 13  14 15 16 17  18 19 1A 1B
00B8  00 00 01 20  00 20 00 00  FD 50 00 00  00 00 02 B0  14 00 00 00  00 00 00 11  07 00 00 00
00D4  00 00 01 40  00 20 00 00  FD 50 00 00  14 00 02 B0  00 00 00 01  00 2D 00 3E  07 00 00 00
00F0  00 00 01 60  00 20 00 00  FD 50 00 00  14 00 FD 50  14 00 00 02  00 5A 00 6B  07 00 00 00

010C: ??? maybe referenced at 01E8
010C  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
011C  00 00 00 00  

0120: dlists
0120  9D 00 04 02  01 03 03 01  04 04 00 05  00 00 02 00
0130  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00

0140  9D 00 04 01  00 06 00 00  09 05 01 08  03 01 07 00
0150  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00

0160  9D 00 04 01  00 01 06 01  0A 00 00 00  04 00 0C 95
0170  00 03 06 01  0A 07 01 0B  04 00 0C 00  00 00 00 00
9D = draw tri strips from VAT 5
so would be 3 sets of 4 tris as strips
but we don't know the VAT format

0180: field 58: vtxs, or something related to them
bytes             | as s16           | as s8
00 00 FD 50 00 00 |    0, -688,    0 |  0, 0, -3,  80,  0, 0
14 00 FD 50 00 00 | 5120, -688,    0 | 20, 0, -3,  80,  0, 0
00 00 02 B0 14 00 |    0,  688, 5120 |  0, 0,  2, -80, 20, 0
00 00 02 B0 00 00 |    0,  688,    0 |  0, 0,  2, -80,  0, 0
00 00 FD 50 14 00 |    0, -688, 5120 |  0, 0, -3,  80, 20, 0
14 00 02 B0 00 00 | 5120,  688,    0 | 20, 0,  2, -80,  0, 0
14 00 FD 50 0A 00 | 5120, -688, 2560 | 20, 0, -3,  80, 10, 0
14 00 FD 50 14 00 | 5120, -688, 5120 | 20, 0, -3,  80, 20, 0
no other width seems to make much sense.

01B0: field 5C: colors or texture coords
000001B0  B7 3F FF FF  

01B4: field 60: looks like texcoords?
02 8E  04 FD
04 BA  FE E6
FF 6F  00 6D
01 91  01 00
FF 6F  01 00
01 91  00 6D
07 68  00 00
00 00  01 00
07 68  01 00
00 00  00 00
01 AE  FD D0
FE A3  FC BA
FC 77  02 D1  

01E8: field 78: critical
000001E8  01 0C 28 00  04 20 82 01
000001F0  05 80 04 84  30 A0 00 10  81 02 00 00

01FC: field 54: textures: 0x14, 0x0E, 0x07

0208: field 64: materials? count = 3 = 0x44 bytes each
Material is the only known struct of 0x44 bytes
00000208  00 00 00 FF  FF FF FF FF
00000210  00 00 00 00  FF 00 00 00  FF FF FF FF  00 00 00 00
00000220  00 00 00 00  FF FF FF FF  00 00 00 00  00 00 00 01
00000230  80 00 00 00  00 00 00 00  00 00 00 00  FF FF FF FF
00000240  FF FF FF FF  00 00 00 0C  06 01 00 00  00 00 00 FF
00000250  FF FF FF FF  00 00 00 00  FF 00 00 00  FF FF FF FF
00000260  00 00 00 00  00 00 00 00  FF FF FF FF  00 00 00 00
00000270  00 00 00 00  80 00 00 00  00 00 00 00  00 00 00 00
00000280  FF FF FF FF  FF FF FF FF  00 00 00 1C  06 01 00 00
00000290  00 00 00 FF  FF FF FF FF  00 00 00 00  FF 00 00 00
000002A0  FF FF FF FF  00 00 00 00  00 00 00 00  FF FF FF FF
000002B0  00 00 00 00  00 00 00 02  80 00 00 00  00 00 00 00
000002C0  00 00 00 00  FF FF FF FF  FF FF FF FF  00 00 00 1C
000002D0  06 01 00 00  

02D4: field 50: hit related
000002D4  00 00 00 00  02 80 FF AA  00 56 00 00
000002E0  00 00 00 00  00 18 00 04  00 02 00 00  00 00 FF AA
000002F0  00 56 00 00  02 80 00 00  00 19 00 00  00 04 00 00
00000300  02 80 FF AA  FF AA 00 00  02 80 00 00  00 00 00 04
00000310  00 07 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00000320  00 00 00 00  

0324: field 4C: hit related
00000324  00 00 00 01  00 05 01 FF  00 00 00 05
00000330  00 03 01 FF  00 04 00 00  00 03 FF 01  00 04 00 03
00000340  00 02 FF 01  00 00 00 04  00 06 FF FF  00 04 00 07
00000350  00 06 F8 FF  00 01 00 00  00 06 1F FF  00 00 00 00

- 0008 is file size
- 00A4 is name

blocks.tab contains a max of 0x800 entries according to `mergeTableFiles`.
the high byte is flags. like other tables, it tells whether to force using one slot or the other.

`checkLoadBlock` seems to expect that ZLB archives may appear inside blocks.bin

the game does seem to treat the table as just offsets, and `mergeTableFiles` doesn't transform them. so is animtest's incomplete?

several fields are offsets into the file, which get converted to pointers, like with models.
