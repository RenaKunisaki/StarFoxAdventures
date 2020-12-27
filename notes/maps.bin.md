MAPS.tab first few entries:
00000000  00 00 00 00  00 00 00 38  00 00 01 88  00 00 04 28  
00000010  00 00 06 C8  00 00 06 C8  00 00 06 C8  00 00 06 F0  
00000020  00 00 07 28  00 00 07 50  00 00 07 A0  00 00 07 F0  
00000030  00 00 07 F0  00 00 07 F0  00 00 08 10  00 00 08 48  
00000040  00 00 09 D4  00 00 0C EC  00 00 10 04  00 00 10 24  
00000050  00 00 10 44  00 00 10 70  00 00 10 A8  00 00 12 A0  
00000060  00 00 16 90  00 00 1A 80  00 00 1A 80  00 00 1A 80  
00000070  00 00 1A A0  00 00 1A D8  00 00 1B 98  00 00 1D 18  
00000080  00 00 1E 98  00 00 1E A8  00 00 1E B8  00 00 1E E0  
00000090  00 00 1F 18  00 00 20 18  00 00 22 18  00 00 24 18  
000000A0  00 00 24 48  00 00 24 78  00 00 24 A0  00 00 24 D8  
000000B0  00 00 25 68  00 00 26 88  00 00 27 A8  00 00 27 A8  
000000C0  00 00 27 A8  00 00 27 D0  00 00 28 08  00 00 28 B0  
000000D0  00 00 2A 00  00 00 2B 50  00 00 2B 60  00 00 2B 70  
000000E0  00 00 2B 90  00 00 2B C8  00 00 2B F0  00 00 2C 40  
000000F0  00 00 2C 90  00 00 2C E0  00 00 2D 30  00 00 2D 50  
ends at 0xCD0 = 820 entries

there are 0x4A maps with directories and 0x74 total
820 doesn't divide into either of those
map cell scale = 640.0

first entry 0 - 38: seems to be shipbattle or frontend
00 0C 00 07  00 06 00 05  06 28 00 00  00 00 00 00  
00 00 00 00  00 00 00 00  00 00 00 00  00 1D 00 00  
00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  
00 00 00 00  00 00 00 00

second entry 38 - 188:
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  06 84 00 7F  
06 82 00 7F  06 82 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  06 88 00 7F  
06 86 00 7F  06 82 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  06 8C 00 7F  
06 8A 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
10 80 00 7F  10 80 00 7F  10 80 00 7F  10 80 00 7F  
this is 0x150 (336) bytes, or 86 ints, or 168 shorts
86 doesn't divide nicely into much of anything
168 = 12x14 which forms some almost square pattern
the data almost looks like pairs of (min, max)?
12x7 = 84 which would make sense if one is an end marker?

apparently there can be FACEFEED in here
and there is at 0x6C8, 0x7F0, 0x1044...
I guess it's just the same data but compressed
not sure why only some are compressed

third entry 188 - 428:
77776688 00000000
77776688 00000000
77776688 00000000
77776688 00000000
386A77ED 00000000
285A67DD 00000000
184A57CD 00000000
000047BD 00000000
000037AD 00000000
0000279D 00000000
0000178D 00000000
0000077D 00000000
77776688 00000000
77776688 00000000
77776688 00000000
77776688 00000000
376976EC 00000000
275966DC 00000000
174956CC 00000000
000046BC 00000000
000036AC 00000000
0000269C 00000000
0000168C 00000000
0000067C 00000000
77776688 00000000
77776688 00000000
77776688 00000000
77776688 00000000
366875EB 77777777
265865DB 77777777
164855CB 77777777
777745BB 77777777
35AB35AB 35AB35AB
0000259B 00000000
0000158B 00000000
0000057B 00000000
777765F9 00000000
777764FA 00000000
777754FA 00000000
777744FA 00000000
256734EA 77777777
255734DA 77775758
154724CA 00000000
44BA14BA 00000000
000034AA 00000000
0000249A 00000000
0000148A 00000000
0000047A 00000000
77776688 00000000
77776688 00000000
77776688 00000000
777744F9 00000000
969643E9 77777777
85A643D9 77775777
13C913C9 13C913C9
13A91389 03990399
03A903A9 06A903A9
03990399 03990399
00001389 00000000
00000379 00000000
77776688 00000000
77776688 00000000
77776688 00000000
777742F8 00000000
000042E8 00000000
000022D8 00000000
229812C8 22A82288
029802B8 02980298
02A802A8 02A802A8
00000298 00000000
02880288 02880288
00000278 00000000
77776688 00000000
77776688 00000000
77776688 00000000
777741F7 00000000
000031E7 00000000
000021D7 00000000
000011C7 00000000
000041B7 00000000
000031A7 00000000
00002197 00000000
00001187 00000000
00000177 00000000

```
*param_4 = MAPS_BIN[MAPS_TAB[idx+6]+1]
```
should be groups of 6 ints

seems like it looks at MAPS.tab entries in groups of 7
then it looks at <mapname>.romlist.zlb

seems like at 0x20 is a GameObjectDef or pointers to some



animtest.romlist.zlb:
0000  00 0D 08 FF  08 04 00 00  45 50 49 34  C3 A4 80 00   .... .... EPI4 ....
0010  45 53 E6 54  00 00 00 00  00 00 00 FF  00 00 00 00   ES.T .... .... ....
I think the text here is coincidence.

warlock.romlist.zlb: (size 0x71BC)
0000  00 0D 08 00  08 04 00 00  44 80 D7 30  44 9D E0 00
0010  45 2A BB 27  00 00 19 EE  05 06 00 40  00 00 0C 00
0020  06 1B 10 00  04 04 20 10  45 0E 01 34  C2 09 CB 96
0030  45 33 F9 A1  00 04 B4 95  00 00 00 FF  FF 00 FF FF
0040  00 00 00 3C  00 50 01 00  D4 D4 00 00  00 00 00 01
0050  01 00 00 00  00 00 00 32  00 00 FF FF  FF 80 00 00
0060  02 5F 09 00  04 04 20 10  44 86 A9 B0  43 9B 1E C3
0070  45 16 4F 73  00 04 B2 D9  00 01 00 00  00 0D FF FF
0080  FF FF 00 00  02 5F 09 00  04 04 20 10  44 A9 42 68
0090  42 9C 58 3F  44 F8 E0 72  00 04 B2 DA  00 01 00 00
00A0  00 0B FF FF  FF FF 00 00  02 5F 09 00  04 04 20 10
00B0  44 AB 49 A8  44 09 1F 03  45 1B 22 9B  00 04 B2 DB
00C0  00 01 00 00  00 0E FF FF  FF FF 00 00  02 5F 09 BF
00D0  01 F4 1F 10  44 84 96 78  44 AC 60 00  45 0C 53 9A
00E0  00 04 B3 22  00 01 00 00  00 0A 04 B1  FF FF 00 00
00F0  05 E0 09 00  01 04 03 6A  43 45 9A 00  C2 C6 00 00

looks a lot like vec3f mixed in here.
             00           01           02           03           04           05           06           07           08           09           0A           0B           0C           0D           0E           0F
[2/08] 00 0D 08 00  08 04 00 00  44 80 D7 30  44 9D E0 00  45 2A BB 27
[5/14] 00 00 19 EE  05 06 00 40  00 00 0C 00  06 1B 10 00  04 04 20 10  45 0E 01 34  C2 09 CB 96  45 33 F9 A1
[D/34] 00 04 B4 95  00 00 00 FF  FF 00 FF FF  00 00 00 3C  00 50 01 00  D4 D4 00 00  00 00 00 01  01 00 00 00  00 00 00 32  00 00 FF FF  FF 80 00 00  02 5F 09 00  04 04 20 10  44 86 A9 B0  43 9B 1E C3  45 16 4F 73
[6/18] 00 04 B2 D9  00 01 00 00  00 0D FF FF  FF FF 00 00  02 5F 09 00  04 04 20 10  44 A9 42 68  42 9C 58 3F  44 F8 E0 72
[6/18] 00 04 B2 DA  00 01 00 00  00 0B FF FF  FF FF 00 00  02 5F 09 00  04 04 20 10  44 AB 49 A8  44 09 1F 03  45 1B 22 9B
[6/18] 00 04 B2 DB  00 01 00 00  00 0E FF FF  FF FF 00 00  02 5F 09 BF  01 F4 1F 10  44 84 96 78  44 AC 60 00  45 0C 53 9A
[6/18] 00 04 B3 22  00 01 00 00  00 0A 04 B1  FF FF 00 00  05 E0 09 00  01 04 03 6A  43 45 9A 00  C2 C6 00 00 ...
 ^ vec3f at word idx/byte idx

00 01 02 03 0405 0607 08090A0B 0C0D0E0F 10111213 14151617 18 19 1A 1B 1C 1D 1E1F  2021 2223  2425 2627  2829 2A2B  2C2D 2E2F  3031 3233  3435 3637  3839 3A3B  3C3D 3E3F
                         X        Y        Z      offset?
00 0D 08 00 0804 0000 4480D730 449DE000 452ABB27 000019EE 05 06 00 40 00 00 0C00
06 1B 10 00 0404 2010 450E0134 C209CB96 4533F9A1 0004B495 00 00 00 FF FF 00 FFFF  0000 003C  0050 0100  D4D4 0000  0000 0001  0100 0000  0000 0032  0000 FFFF  FF80 0000
02 5F 09 00 0404 2010 4486A9B0 439B1EC3 45164F73 0004B2D9 00 01 00 00 00 0D FFFF  FFFF 0000
02 5F 09 00 0404 2010 44A94268 429C583F 44F8E072 0004B2DA 00 01 00 00 00 0B FFFF  FFFF 0000
02 5F 09 00 0404 2010 44AB49A8 44091F03 451B229B 0004B2DB 00 01 00 00 00 0E FFFF  FFFF 0000
02 5F 09 BF 01F4 1F10 44849678 44AC6000 450C539A 0004B322 00 01 00 00 00 0A 04B1  FFFF 0000
05 E0 09 00 0104 036A 43459A00 C2C60000 ...

really seems like there must be variable-length data here
bytes after the offset: 8, 40, 12, 12, 12, 12...
normalized: 0, 32, 4, 4, 4, 4...
words: 0, 8, 1, 1, 1, 1...
total line length bytes: 0x20, 0x40, 0x24, 0x24, 0x24, 0x24
words: 0x08, 0x10, 0x09, 0x09, 0x09, 0x09

so field 2 is the length in words
offset might be an ID?

in warlock, every entry's length is at least 6, up to "offset?"
in animtest there's one entry:
0000 [0000]: 00 0D 08 FF 0804 0000 +3332.58  -329.00 +3390.40 00000000 000000FF 00000000

presumably the first few bytes specify an object ID, coordinates, ???, and then additional params depending on object type
highest valid object ID is 0x5C4
WM_xxx is mostly 0x376 to 0x3BC

I wonder if these "offsets" are converted from when the game used a cartridge
they're stored to the save file along with some coordinates at offset 0x168
for chapter 2 this is: 0x00046fd8  12373.7  211  176.534
apparently there are 63 such entries in the save
when processing a romlist entry, if one of these saved entries has the same ID, then the saved coords overwrite the entry's.
so this mechanism allows to save the coords of up to 63 unique objects.
and the "offset" allows to assign a globally unique ID to an object instance, so that its position can persist forever.

supposedly this is a GameObjectDef
0x00  short	      type
0x02  u8          allocatedSize
0x03  undefined		
0x04  ObjectId	  objectId
0x05  byte	      flags_0x5
0x06  undefined1  maybeSize_6
0x07  undefined1  mapId2
0x08  vec3f       pos
0x14  int		
0x18  float*      pX
0x1c  float*      pY
0x20  float*      pZ
0x24  undefined		
0x25  undefined		
0x26  undefined		
0x27  undefined		
0x28  undefined		
0x29  undefined		
0x2a  undefined		
0x2b  undefined		
0x2c  undefined		
0x2d  undefined		
0x2e  undefined		
0x2f  undefined		
0x30  float[8]    mtx
0x50  char*       name
this ID looks a lot more like flags.
that would explain why objLoadLanternFire sets the same ID as IceBlast
but where do these IDs come from? there's more of them than in OBJECTS.BIN
In that, Fox=2, Krystal=3, but for these, Fox=0, Krystal=1F
these aren't model IDs either, those are 0x4E8 for both
they're at 0x46 in ObjInstance and they define the behaviour IIRC
they might be just a big array somewhere
they go up to at least 0x773 (FoxShield)

the object IDs are translated by OBJINDEX.bin (unless a flag says not to)
this file is just an array of u16, 0x890 (2192) elements
then it uses that ID as index into OBJECTS.bin which gives ObjectFileStruct

in OBJINDEX: Fox=2, Krystal=3, so that works out.

animtest contains only 0x0D => 0x0493 => setuppoint
but I think when you actually load it, there are more objects loaded, so presumably things like the player and camera are automatically added everywhere
frontend2 contains only AnimCamera
frontend seems to be the ship battle
gamefront is title screen
    includes a FrontSeq object
    and four of "FrontPilots" (maybe FrontPilotseat?)
one thing to do would be editing OBJINDEX to change some objects so we can see which ones they are
I think that's what the cheat to change the fireball spell does

setuppoint could be the player spawn point?

globalma.bin (not globalmap) might be objects loaded for every map
