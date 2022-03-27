v1.1 USA: unused cutscene: Krystal at Galleon
041e19d4 60000000
from https://www.youtube.com/watch?v=kfrjalwO9WQ

042B6B6C 38600000 - force to load Krystal, hopefully.
but crashes if she's not already loaded.

map types:
0 = ordinary map
1 = also ordinary?
2 = ??? unloads all objects
3 = also unloads
4 = frontend - using this on a normal map gives you fixed camera, very limited area actually loads
5 = adds the minimap
6 = "scanning for information"
    I wonder if this is really minimap type
    but it doesn't change when I cycle that.
beyond this seems to be ordinary maps but the minimap cycles modes

what we've been calling local and global files are actually main map and submap.
eg as we wander around ThornTail Hollow, 0x2B is swaphol/MODELS.bin
when we enter the shop, 0x46 is shop/MODELS.bin
when we leave the shop that remains loaded until we enter the queen's chamber, then 0x46 is swapholbot/MODELS.bin
when we head to Ice Mountain, 0x2B is icemountain/MODELS.bin and 0x46 is empty.
magiccave is also considered part of swaphol

but sometimes it switches which is which

------

globalmap.bin is 0x260 bytes, looks like s16
not vec3s (0x260 doesn't divide into 12)
(duh, vec3s is 6 bytes anyway)

globalma.bin is 0x300 bytes
the game does use globalma, not globalmap
(is globalmap ever used?)

it's 0x10 * 0x50 entries?
0x300 / 12 = 64
and it is 12-byte entries

........  0001 0203  0405 0607  0809 0A0B
00000000  FFF0 FFFF  0000 0012  FFFF FFFF
0000000C  0002 FFE6  0000 0010  000C FFFF
00000018  0000 002E  0000 0006  FFFF FFFF
00000024  0007 002D  0000 0009  FFFF FFFF
00000030  0011 0000  0000 000B  FFFF FFFF
0000003C  FFF3 0014  0000 0013  FFFF FFFF
00000048  0012 FFEB  0000 0021  FFFF FFFF
00000054  0018 FFEB  0000 0022  FFFF FFFF

offs|type|what
----|----|----
0000|s16 |x
0002|s16 |y
0004|s16 |val
0006|s16 |idx
0008|s16 |?
000A|s16 |?

at 06, if < 0, marks end of list
that's at the 61st entry
else it's an index into pMapBlocks_80382244 and others
where the value at 04 is stored, which is s16 but cast to s8
it's always in range -2 to 2
I think this is a "layer", like a Y coordinate for the cells.

every field is s16
08, 0A are stored to mapBuffer_80382240, again 06 is index

field 0, 2 are most likely X, Y

MAPS.bin is definitely some type of bitmap
looks like it's variable length entries
offs|type |what
----|-----|----
0000|s16  |width
0002|s16  |height
0004|s16  |x
0006|s16  |y
0008|?    |?
000C|s32[]|bit mask? one per width*height

for field C, the highest byte is a bit index into the output array.
if it's not 0xFF, the corresponding bit is set in the array.
maybe that array is telling whether the cell belongs to the current map?
no idea what the other bytes are

but if it's one per w*h, there should be 30 entries for the first map, but there are only 12

actually field C is ignored in the file
it's set to entry + size
where size is the next maps.tab entry minus this one
maps.tab is in groups of 7 or 8 entries

first several entries of maps.tab:
0000 0038 0188 0428 06C8 06C8 06C8
06F0 0728 0750 07A0 07F0 07F0 07F0
0810 0848 09D4 0CEC 1004 1024 1044
1070 10A8 12A0 1690 1A80 1A80 1A80
1AA0 1AD8 1B98 1D18 1E98 1EA8 1EB8
1EE0 1F18 2018 2218 2418 2448 2478
24A0 24D8 2568 2688 27A8 27A8 27A8
27D0 2808 28B0 2A00 2B50 2B60 2B70
2B90 2BC8 2BF0 2C40 2C90 2CE0 2D30
definitely looks like groups of 7
some of the latter columns repeat, giving a length of zero.
though there are 820 entries which doesn't divide into 7 or 8

820 divides into: 1, 2, 4, 5, 10, 20, 41, 82, 164, 205, 410, 820
if we assume the last 0xFFFFFFFF is an entry too then there's 821,
but that's a prime number, so would be very odd.

maybe this is 8 columns but one is implied?

mapBuffer_80382240 is pairs of s16 per map


//mapNo is dir idx
if (mapNo < 5) {
  sprintf(path,s_%s/mod%d.zlb.bin_802cc3cc,mapDirName[mapNo],mapNo);
}
else {
  sprintf(path,s_%s/mod%d.zlb.bin_802cc3cc,mapDirName[mapNo],
          mapNo + 1);
}
//same for mod%d.tab with blocks.tab


map blocks should be 16 columns according to mapGetBlockAtPos
unless in the file is different than in RAM
  00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
0000 0038 0188 0428 06C8 06C8 06C8 06F0 0728 0750 07A0 07F0 07F0 07F0 0810 0848
09D4 0CEC 1004 1024 1044 1070 10A8 12A0 1690 1A80 1A80 1A80 1AA0 1AD8 1B98 1D18
1E98 1EA8 1EB8 1EE0 1F18 2018 2218 2418 2448 2478 24A0 24D8 2568 2688 27A8 27A8
27A8 27D0 2808 28B0 2A00 2B50 2B60 2B70 2B90 2BC8 2BF0 2C40 2C90 2CE0 2D30
that doesn't look right at all


Let's try patching mapLoadDataFiles() to load different files while running around swaphol:

- ANIM: no effect
- MODELS: no effect
- BLOCKS: eventually I end up with big invisible walls around the arwing.
- VOXMAP: no effect
- ANIMCURV: no effect

what about disabling the call entirely?

- ANIM: no effect
- MODELS: no effect - I think these are still being loaded elsewhere
- BLOCKS: no effect
- VOXMAP: no effect
- ANIMCURV: no effect

let's try patching mapLoadDataFile, loading animtest
- ANIM: the animations go berzerk, making trees and flowers flap around all crazy
- BLOCKS: the objects are present but the geometry isn't. Krystal floats in place in a T pose, moving her foot a bit.
- VOXMAP: as I walk around the game tries to load the file again every few feet as well as BLOCKS
- ANIMCURV: the arwing is sunk into the ground
and that's all the files it loads for the map (but multiple times)

so is BLOCKS.BIN the geometry itself or telling which geometry to use or where to find/place it?

there is no BLOCKS.BIN anywhere...
it's actually modXX.zlb.bin
and BLOCKS.TAB is modXX.tab


803822b4 pMapLayerBlockIdxs is 5 pointers to s8[16][16]
these track the block IDs of the map cells around you
as you walk around, they move in the array, and blocks get swapped in and out.
0xFE = no block loaded.
0xFF = something
I don't know what these numbers correspond to.
I think indexes into the list of loaded blocks.

803dce94 points to s16[64] which is the ID of each loaded block.
803dce98 counts how many blocks are loaded.
803dce9c points to MapBlock*[64], the blocks themselves.
803dce8c points to bool[64] which tells whether each block slot is used. (or it might be a ref count?)

layer 0 in ThornTail Hollow:
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- 00FE 0115 0116 0112 010C 010D 0628 ---- ---- ---- ---- ---- ----
---- ---- ---- 00FF 0113 0114 0111 0100 ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- 010F 0110 0101 0102 ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- 0117 010E 0103 0104 0105 ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- 0106 0107 0108 0109 ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

so, what's entry 0x0101 in MAPS.tab? 0xA928
(but so is entry 0x0100, 0x0102, so that's odd...)
and in MAPS.bin there we find a FACEFEED
but it doesn't appear to be ZLB data following it...

```
int zlbDecompress(void *data,uint compLen,void *out)

if (*piVar6 == -0x5310113) {
  zlbDecompress(piVar6 + piVar6[2] + 0x28,piVar6[3] - 0x10,dest)
  ;
  dCacheStore(dest,piVar6[1]);
}
```

so facefeed header is:
u32 signature; //FACEFEED
u32 decLen;
u32 offset;
u32 compLen;
???

but we already knew that...

also maybe we should be looking at globalma.bin or BLOCKS.bin for this index instead
looks like that is what the game does
no idea why this data from MAPS.bin doesn't decompress properly though.
I think it's not even compressed, but then wtf is the FACEFEED header doing there?
BLOCKS.bin is modXX

swaphol/mod13.tab entry 0x101: 0x100058A0 (what does this mean?)
swaphol/mod13.zlb.bin is tiny:

0000  00 00 00 00  00 08 00 00  00 00 01 C0  00 00 00 00
0010  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0020  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0030  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0040  00 00 00 00  00 00 00 00  00 00 00 00  00 00 01 9C
0050  00 00 01 74  00 00 01 2C  00 00 01 00  00 00 01 14
0060  00 00 01 18  00 00 01 30  00 00 00 B8  00 00 00 00
0070  00 00 00 00  00 00 00 00  00 00 01 24  00 00 00 00
0080  00 00 00 00  00 07 00 00  00 00
008C                                  FD 11  FD 11 FD 11  <-- ???
0090  00 03 00 00  00 01 00 03  00 01 00 01  00 00 00 00
00A0  01 01 01 00
00A4: name: 6D 6F 64 31  33 2E 30 00  00 00 00 00  "mod13.0"
00B0: padding:  00 00 00 00  00 00 00 00
- end of header -

00B8: dlists: 1 x DisplayListPtr
    offs=000000E0  size=0020
    bbox=0000 0000 0000  1400 0000 1400
    shader=0000
    offs?=0000 0011
    ?=07000000
00D4: padding?: 00 00 00 00  00 00 00 00  00 00 00 00
00E0: dlist[0]: 0x20 bytes:
    95 00 03 00  00 00 01 00  01 02 00 02  00 00 00 00
    00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0100: vtxs: 3 x vec3s
    1400 0000 1400
    0000 0000 1400
    1400 0000 0000
0112: padding: 00 00
0114: colors: 1 x u16: 347F
0116: padding: 00 00
0118: texcoords: 3 x vec2s
    0100 0000
    0000 0000
    0100 0100
0124: renderInstrs[0]: 7 bytes: 01 0C 28 00  04 A0 00
012B: padding: 00
012C: textures: 1 x int: 00 00 02 1B
0130: shaders: 1 x Shader
    0130  00 00 00 FF  FF FF FF FF  00 00 00 00  FF 00 00 00
    0140  FF FF FF FF  00 00 00 00  00 00 00 00  FF FF FF FF
    0150  00 00 00 00  00 00 00 00  80 00 00 00  00 00 00 00
    0160  00 00 00 00  FF FF FF FF  FF FF FF FF  00 00 00 1C
    0170  06 01 00 00
0174: ptr_0x50:
    0170               00 00 00 00  02 80 00 00  00 00 00 00
    0180  02 80 00 00  00 19 00 04  00 01 00 00  00 00 00 00
    0190  00 00 00 00  00 00 00 00  00 00 00 00
019C: ptr_0x4C:
    0190                                         00 02 00 01
    01A0  00 00 FF FF  00 00 00 00  00 00 00 00  00 00 00 00
    01B0  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00

what happens if we change entry 0x0101 in swaphol/mod13.tab from 0x100058A0 to the next entry, 0x1000CC60? then we get a chunk of incorrect geometry around the waterfall. we can fall through it out of bounds (does not softlock) or hop onto it and run around.
I have no idea where the actual geometry is though if mod13 is this small...

What about 0x100058B0? then the block just doesn't load at all and we softlock on entering it. no log messages though...

so what do these numbers mean, and how does the game know which blocks to use here? and where the heck is the geometry?
the coords are roughly 0xD580 0xDF80
> Map: 0007 ThornTail Hollow             Type 00 unk 06 0000 Objs:  439 @8168E3A0
> Coords: -5552.61  -780.94 -3281.25  Cell  -16  -13 @ -10240 -8320 0

cell should be 0xFFF0, 0xFFF3 or so
that probably corresponds with an entry in MAPS.bin
but MAPS.tab is only 820 entries, so what's used to look those up?


what if we patch 80060b90 -> 38600001, to set the unused field in every map block? or 0xFACE? nothing seems to change.


https://github.com/magcius/noclip.website/blob/master/src/StarFoxAdventures/maps.ts
according to this:
multiply map ID by 0x1C to get index into MAPS.tab
each map has 7 entries (offsets into MAPS.bin):
    0: infoOffset
    1: blockTableOffset
    2-6: ?
    5 unknowns, maybe one per layer?
    entry 2 is used to get the size of the block table

at infoOffset:
    u16 nCols, nRows, originX, originY;

blockTableOffset: addressed by [y][x]; each entry is u32
```
    const blockIndex = y * mapInfo.blockCols + x;
    const blockInfo = mapsBin.getUint32(mapInfo.blockTableOffset + 4 * blockIndex);
    const sub = (blockInfo >>> 17) & 0x3F;
    const mod = (blockInfo >>> 23);
    if (mod == 0xff) {
        return null;
    }
    return {mod, sub};
```
    so each block is bits: ?mmmmmmm sssssss? ???????? ????????
    m=mod, s=sub

so if swaphol is map 7, that's 7 * 0x1C = 0xC4 in MAPS.tab:
    infoOffset       = 0x27D0 (size 0x0038)
    blockTableOffset = 0x2808 (size 0x00A8)
    ???              = 0x28B0 (size 0x0150) looks like some kind of bitmap data; not used?
    ???              = 0x2A00 (size 0x0150) looks the same as above; relates to rendering
    ???              = 0x2B50 (size 0x0010) 77777777 77777777 77777777 77777777
    ???              = 0x2B60 (size 0x0010) all zeros
    romlist          = 0x2B70
in MAPS.bin at 0x27D0:
    nCols = 7, nRows = 6, origin = 0,1 - 42 blocks total
    0x150 = 336 = 42 * 8

the last item "romlist" is very interesting.
at that offset in MAPS.BIN, there does appear to be a compressed romlist (or at least a FACEFEED header whose output size = the map's romlist size).
the game uses this for maps that don't have a separate romlist file.
though it looks like there's a copy of every map's list here? frontend at least.
maybe these lists differ from the used ones...
actually it seems like this is only a FACEFEED header for the romlist file, not an alternate romlist
the size of this is used to alloc memory for the romlist.
the other fields might not be used?

file idxs that would use this: 0x05, 0x0B, 0x43, 0x49, 0x50-0x74
which is hightop, warlock, linkc, linki and most of the maps with no dir

to allocate the map's block buffer, the game uses:
size + ((nBlocks+7) >> 3) + 0x401 + romListSize
where size = next item - this item in maps.tab,
    nBlocks is field 0x1C at infoOffset,
    romListSize is from the FACEFEED header.

changing the fourth item's data affects which areas render where, including direction, similar to MK64's master dlist table.
probably relates to the cellx/y/z shown in map debug
those only appear to range from 0 to 7, repeating several times over the map, so probably offset within a cell
maybe there are a few of these, for different Y offsets?
by changing the data I can get only certain parts to vanish

this 77777 stuff is nybbles offset by 7.
eg 7 = 0, 8 = 1, 6 = -1; actual value = val - 7
each u32 defines two rects: (or more precisely, each u16 defines one rect)
r1 = {
    x1 = (val >> 0x0C & 0xF) - 7;
    y1 = (val >> 0x08 & 0xF) - 7;
    x2 = (val >> 0x04 & 0xF) - 7;
    y2 = (val         & 0xF) - 7;
}
r2 = {
    x1 = (val >> 0x1C      ) - 7;
    y1 = (val >> 0x18 & 0xF) - 7;
    x2 = (val >> 0x14 & 0xF) - 7;
    y2 = (val >> 0x10 & 0xF) - 7;
}
the u32s are read in pairs, so four rects.
the rects seem to define which blocks are visible
and for that matter, the map's bounds (at least the first item)
there seem to be four such pointers
the first defines which blocks are actually part of the map. if we change it, we get missing chunks that, if we walk into them, we're out of bounds - the PDA map closes, the world map shows our location as blank.
the second has to do with visibility. the effect is like the first but when we walk into the missing chunks, we're not out of bounds - the blocks are still there, just invisible. they show up as we wander around.
the third doesn't seem to do anything? nor the fourth
I have no idea how it's actually interpreted.
it seems like swaphol uses the first two and animtest uses the last two


sub 1, mod 12
will want to make a script for this
so where do -16, -13 come from? maybe that's an unrelated coordinate system? (object grid?)
and what translates coordinates to map ID? I think we found that function
there's mapGetBlockAtPos(x, z, layer) which uses pMapLayerBlockIdxs
which is index into loaded block list
but that's coords relative to the map itself

then there's mapCoordsToId(int x,int z,int layer)
which uses PTR_mapRects_8038223c, pMapBlocks_80382244
maybe we should breakpoint on these functions to see what kind of coordinates are being passed in

title screen:
    mapCoordsToId(ffffffe2, 0000002c, 0)
    mapCoordsToId(ffffffe2, 00000029, 0)
    mapCoordsToId(ffffffe3, 00000029, 0)
    mapCoordsToId(ffffffe2, 0000002a, 0)
    mapCoordsToId(ffffffe3, 0000002a, 0)
    mapCoordsToId(ffffffe2, 0000002b, 0)
    mapCoordsToId(ffffffe3, 0000002b, 0)
    mapCoordsToId(ffffffe2, 0000002c, 0)
    mapCoordsToId(ffffffe3, 0000002c, 0)
    what if we patch this to always return 0xB? then it does try to load warlock at the title screen, and crashes
    let's do it just before loading a save in swaphol
    then it does load and we're standing in a void
    the world map shows our current location is Krazoa Palace
    looks like the appropriate objects loaded in too
    so we're on that map, just way outside of the actual geometry
    we're at -5552.61│  -781.00│ -3281.25 c5ad84e1 c4434000 c54d1400
    objs at +11178.90│  +189.25│  +715.58 462eab97 433d40a2 4432e51b
    if we edit our coords to that, we are indeed in the palace (1900 F100)
    Coords: +11163.94   -98.94  +774.70  Cell   10   -6 @ 6400 -3840
    now let's unpatch the function and change coords back
    we actually get blocked, just moved to a nearby wall
    even if we change both current and previous position
    maybe it's putting us at the closest place that's still in the map?

anyway, so the title screen coords are around -30, 44
        [player object coords]         [cellX/X]       [mapX/Z]
Coords: -18870.40    +9.75 +28360.43  Cell  -37   37 @ -23680 23680
that's pretty close to the cell coords
(I wonder how it chooses those coords at the start of the game? probably it chooses the map ID instead)
    dll_titleMenu is 0x34
    not sure where it actually loads the title screen map but anyway not important right now
> mapX, mapZ, cellX, cellZ = self.client.read(0x803dcdc8, '>4i')
> ffffa380 00005c80 ffffffdb 00000025

mapCoordsToId() uses PTR_mapRects_8038223c so where does that come from?
and why do we actually need this if we already know how map IDs are looked up?
- because the save file and warps table don't use map IDs, just coords

PTR_mapRects_8038223c is only written in initMaps()
the rects are just 5 s16: xMin, xMax, zMin, xMax, idx
there are 0x80 of them, seemingly in an 8x16 grid
first they're all initialized to 0x8000,0x8000,0x8000,0x8000,0x8080
globalma.bin contains x, y, val, idx, ?, ?
I think idx = map ID, val = layer
then Krazoa Palace is at 0x11, 0x00
and swaphol is 0xFFF4, 0xFFFB = -12, -5
so let's change that entry from map 7 to B
and look at that, we're in the palace
so let's try some other map IDs here.
00 (shipbattle): crash
01 (frontend2): crash
03 (animtest/old Krazoa Palace): crash
05 (animtest): crash
0F (cloudtreasure): crash
1E (insidegal): crash
1D (capeclaw): crash, so this just plain doesn't work for most maps
    maybe because it's putting the player outside the map
anyway what are the last two values?

the rects are initialized from the globalma.bin entries
rect.xMin = x - origin.x   rect.xMax = rect.xMin + w
rect.zMin = z - origin.z   rect.zMax = rect.zMin + h
then the field at entry+0xC is used somehow
it's one int per (w*h)
if the high byte isn't 0xFF, the corresponding bit is set in `out`
where `out` is PTR_mapFlags_80382248 + (map * 0x40)
so that points to a 64-byte (or 512-bit) array per map
in which each bit indicates something from this table starting at entry+0xC
I think I misread the code, field_C isn't an array, it's an int
foo = field_C + (c * 4)
if((foo >> 23) & 0xFF) != 0xFF, then set bit c
but also, field C is being set right after loading the entry
it's changed to a pointer to the block table I think!?
so I think this is just setting a flag for whether each cell is in the map or not.
and this has nothing to do with what's actually in the info field.
and the bit width = the map's width in columns
so this is just a quick way to look up whether a cell belongs to the map.
but if this is to be believed, then several maps are empty, including swaphol and krazoa
maybe "mod" is "modify" and this is whether to place some other geometry here?

anyway the only thing the last two fields seem to be used for is to store into PTR_mapBuffer_80382240
I think these are actually map IDs that they link to
eg swaphol has swapholbot and none; swapholbot has swaphol and none
it looks like these tell which romlists to load alongside this one.

dragrock has itself listed as one of them...

```
iy = 0;
while (iy < h) {
  ix = 0;
  while (ix < w) {
    c = ix + iy * w;

    fuck = (
        *(uint *)( //pointer deref
            //but what's the precedence here
            //deref is higher than + (except unary)
            (*(int *)(puVar1 + 0xc)) + (c * 4) //pointer deref
            //this is just typecast because it doesn't know the
            //type of puVar1 because it's a butt
            //so this is just field_C + (c * 4)
        ) >> 0x17 & 0xff
        //and the other deref is also a cast
    );
    if(fuck != 0xff) {
        // high byte of entry->field_C[c] is bit index
        // if not 0xFF, set corresponding bit in out
      out[c >> 3] = out[c >> 3] | 1 << (c & 7);
    }
    ix += 1;
  }
  iy += 1;
}
```


origin seems to be just which block is the "center" of the map.
this still doesn't tell anything about the global grid.
also, why is swaphol mod13 but the map data is mod 12? and where are the submodules?

swaphol info is at 0x27D0
00 07 00 06 - w, h
00 00 00 01 - x, y
BC 24 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
04 4C 00 01
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00

layer -2: DarkIce Mines bottom, boss
layer -1: Ocean Force Point bottom, ThornTail underground, Dragon Rock bottom, DarkIce Mines top to bottom
layer  0: most maps
layer  1: Drakor, ThornTail Shop
layer  2: Arwing levels

some spots from warptab:
Krazoa Palace:    +12264.56 	+211.00 	 +451.46 (cell 19, 0)
ThornTail Hollow:  -5222.53 	-634.53 	-1733.20 (cell -8, -2)


we can access some unused maps by changing a WARPTAB.bin entry to the right coordinates:
Ly    X     Y      Z
 0  -9496 -127 -19025: animtest
 0   7683    ?  14083: Rolling Demo (either nothing here, or can't find it)
 0   3200    ?  28160: Willow Grove (crashes)
 0   -640    ?  28800: Discovery Falls (empty)
 0  -6400    ?  28800: Diamond Bay (crashes - dir missing)
 0 -10240    ?  27520: Duster Cave (cloudjoin) (crashes - dir missing)
 0 -12700    ?  28000: BOSS Kamerian D (empty)
 0 -19000    ?  33920: CloudRunner2Rac (empty)
 0   7200    ?  31000: MazeTest (crashes)

022b6280 00004800 will allow you to fall out of the map instead of softlocking
if you keep going for long enough you can indeed fly from Krazoa Palace to Cape Claw, though the textures and objects don't load and it crashes soon after

802c2170 is vec3f newGameStartCoords
I think there's no var for the layer because it's 0 anyway

so to start a new game in AnimTest:
062c2170 0000000c
c6146000 c2fe7511
c694a200 00000000
(the 00000000 isn't layer, it's just padding)

some objects are listed for some unused maps:
(these are not divided by 640)

# cloudjoin
* 771.12	55.50	-841.82
* 1,217.09	1,280.00	293.76
- most are around Y=1280
- seems to be all invisible objects

others only have a setuppoint or some other single object:
(maybe not all of these are unused)
(and some of these do have more objects but lazy)
(some of these might be loaded into an existing map on the fly?)

animtest        3332.58 -329.00 3390.40 - ice mountain, middle of nowhere
ccbridge        3.60 7.00 28.92
cfcolumn        3.60 7.00 28.92
cfdungeonblock  3.60 7.00 28.92
cfgalleon       -98.08 117.00 3.67 - crash
cfgalleon       -0.91 75.47 -392.88 - way in the air in a corner of lightfoot village
cfgangplank     4.77 3.00 54.21 - crash
cfledge         3.60 7.00 28.92
cfliftplat      3.60 7.00 28.92
cfprisoncage    3.60 7.00 28.92
cfprisondoor    3.60 7.00 28.92
cloudrunnermap  3.60 7.00 28.92
cloudtrap       3.60 7.00 28.92
cloudtreasure   178.00 1835.00 722.00 - crash
dbstepstone     1.70 38.62 4.24 - crash
dfcavehatch1    3.60 7.00 28.92
dfcavehatch2    1.33 7.00 17.13
dfcradle        5.93 -89.00 13.09 - crash
dfpodium        3.60 7.00 28.92
diamondbay      -1282.95 -1581.64 -2548.56 - lightfoot village underground
discovery       1147.15 151.00 142.10 - crash
drpushcart      3.60 7.00 28.92
duster          414.46 42.00 -1472.55 - egde of lightfoot village in air
earthwalker     276.53 -707.00 -988.00 - random lightfoot village spot
frontend2       318.86 265.04 525.04 - crash
galleonship     -2.09 -139.00 568.63 - crash
goldplains      2899.79 116.32 3142.03 - crash
greatfoxworld   3.60 7.00 28.92
hightop         482.59 -11.74 1037.15 - crash
imspacecraft    89.52 3.73 -57.92 - lightfoot nowhere
insidegal       153.78 170.00 595.84 - crash
KamColumn       3.60 7.00 28.92
kamdrag         644.94 -1805.00 -845.04 - cape claw corner
krashrin2       487.57 35.60 -308.19 - random lightfoot
krazchamber     485.76 56.00 1128.61 - crash
linklevel       1398.57 -1003.00 301.33 - crash
mikelava        -236.06 -1428.00 -94.11 - random lightfoot
newicemount2    -2210.61 3665.43 -1014.69 - way above lightfoot village
newicemount3    226.12 728.38 -2650.05 - lightfoot
nwboulder       3.60 7.00 28.92
nwshcolpush     3.60 7.00 28.92
nwtreebridge    -123.52 -0.04 3.86 - crash
nwtreebridge2   12.27 86.60 140.78 - crash
scstatue        42.25 14.53 1.90 - crash
scstatue        0.74 38.00 0.87
shlily          3.60 7.00 28.92
snowmines4      -1207.59 -1400.00 454.02 - crash
snowmines5      -236.06 -1428.00 -94.11 - lightfoot
wcbouncycrate   3.60 7.00 28.92
wcpushblock     3.60 7.00 28.92
wctemplelift    3.60 7.00 28.92
willow          -555.66 -789.00 -842.88 - lightfoot
wmcolrise       3.60 7.00 28.92
wminsert        327.73 -15.00 28.84 - crash

lightfoot village is close to 0,0 so I suspect this is some scaling problem
also, maybe we need to swap Y/Z

1790.91 1263.00 1719.39 is a crate in KP and that crashes too
KP is closer to 12160, 0
if the scale is off, it's nowhere near 640
it could also be that these are relative to the map's origin.
but these might tell us the Y position of some of these:

7683 -11 14083 0 	hightop      Rolling Demo - Just In Case 	Empty
3200 -789 28160 0 	willow       ZNot Used - Willow Grove 	Crashes
-640 151 28800 0 	discovery    ZNot Used - Discovery Falls 	Empty
-6400 -1581 28800 0 	diamondbay  ZNot Used - Diamond Bay 	No files
-10240 42 27520 0 	duster     ZNot Used - Duster Cave 	No files
-12700 -1805 28000 0 	kamdrag    ZNot Used - BOSS Kamerian D 	Empty
-19000 1280 33920 0 	cloudjoin  ZNot Used - CloudRunner2Rac 	Empty
7200 134 31000 0 	mazecave     MazeTest 	Crashes
12800 192 30080 0 	krazoapalace    ZNot Used - Krazoa Palace 	Empty - grid layout suggests an early version of the palace

with these coords, willow doesn't crash, but is empty
mazecave still crashes (80030504)

for no files, crash is at 80029118

    X 	   Y 	   Z  Grid 	Internal Name 	                Description
  7683 	  -11 	14083 	0 	Rolling Demo - Just In Case 	Empty
  3200 	 -789 	28160 	0 	ZNot Used - Willow Grove 	    Empty
  -640 	  151 	28800 	0 	ZNot Used - Discovery Falls 	Empty
 -6400 	-1581 	28800 	0 	ZNot Used - Diamond Bay 	    No files
-10240 	   42 	27520 	0 	ZNot Used - Duster Cave 	    No files
-12700 	-1805 	28000 	0 	ZNot Used - BOSS Kamerian D 	Empty
-19000 	 1280 	33920 	0 	ZNot Used - CloudRunner2Rac 	Empty
  7200 	  134 	31000 	0 	MazeTest 	                    Crashes
 12800 	  192 	30080 	0 	ZNot Used - Krazoa Palace 	    Empty - grid layout suggests an early version of the palace


object loading uses map state:
if state == -1 (impossible), load nothing
if state == 0, load everything
if state < 9:
    n = mapBitIdx03 >> (state - 1 & 0x3F)
    if n & 1: do not load this object
    - state-1 can be 0 to 7
else:
    n = mapBitIdx05 >> (16 - state & 0x3F)
    if n & 1: do not load this object
    - state can be 0 to 15, so here, 16-state can be 1 to 7
this comes after a previous check for something else and can override that check
that's checking for specific object types and something to do with map blocks

so these two bytes define which map states the object should load in:
03:
    80 state 8
    40 state 7
    20 state 6
    10 state 5
    08 state 4
    04 state 3
    02 state 2
    01 state 1
05:
    80 state 9
    40 state 10
    20 state 11
    10 state 12
    08 state 13
    04 state 14
    02 state 15
    01 unused?
if bit is set, object will NOT load in that state.

# block header
for first block in animtest:
field                  |ofs/val|count|note
-----------------------|-------|-----|----
GCpolygons             |   0324|    7|
displayLists           |     B8|    3|
flags_0x4              |      8|     |
hitField_9e            |      0|     |
hits                   |      0|    0|
linehits               |      0|     |
polygonGroups          |   02D4|    3|
renderInstrsMain       |   01E8|   18|
renderInstrsReflective |      0|    0|
renderInstrsWater      |      0|    0|odd, since water looks fine
shaders                |   0208|    3|
textures               |   01FC|    3|
vertexPositions        |   0180|    8|vec3s
vertexColors           |   01B0|    2|u16
vertexTexCoords        |   01B4|   13|vec2s
yMax                   |     44|     |
yMin                   |   -128|     |
yOffset                |    -42|     |
can confirm the positions, colors, texcoords fields are correct.
also we can edit this stuff while the game is running. even vtx positions.
of course offsets are changed to pointers

## display lists
offset   |size|bbox                         |shdr|spcl|????
0000 0120|0020|0000 FD50 0000 0000 02B0 1400|0000|0000|0011|0700 0000
0000 0140|0020|0000 FD50 0000 1400 02B0 0000|0001|002D|003E|0700 0000
0000 0160|0020|0000 FD50 0000 1400 FD50 1400|0002|005A|006B|0700 0000
bounding box IS used for maps; it determines when the list should be drawn.
the other fields seem unused

FD50 = -688, 2B0 = 688
apparently this is vec3s[2] so that translates as:
0, -688, 0 |    0,  688, 5120
0, -688, 0 | 5120,  688,    0
0, -688, 0 | 5120, -688, 5120

## polygon data

all 7 GCpolygons:
    0000 0001 0005 01FF (offset 0x324)
    0000 0005 0003 01FF
    0004 0000 0003 FF01
    0004 0003 0002 FF01
    0000 0004 0006 FFFF <-- floor
    0004 0007 0006 F8FF
    0001 0000 0006 1FFF
last word looks like packed bits...

these would be referenced by the draw commands
changing these in memory doesn't seem to do anything...
but changing the pointer to them = fall through ground
so these are only for hit detection (unless this map is weird, unlikely but possible)
they apply to the first quad floor and maybe the walls
offset 344 is the first triangle we stand on, so that's the 4th idx
changing things here just makes us fall through

likely u16 x, y, z (vtx idxs)
then maybe tex coords or flags or hit info

if I change the data in the vertexPositions array it affects both geometry and hit detection.
first s16 -> 0x1111: the polygons are distorted, and you can't walk on them. the non-distorted ones are fine.
with smaller changes the vertices move and we can walk through the wall
if we change Y from FD50 to F050 the vertex does move down and the hit detection somewhat follows it
as we walk down the slope we don't move smoothly; it acts like big stairs, and about halfway along, we just fall through.

vtx positions:
    0:     0,  -688,     0
    1:  5120,  -688,     0
    2:     0,   688,  5120
    3:     0,   688,     0
    4:     0,  -688,  5120
    5:  5120,   688,     0
    6:  5120,  -688,  2560
    7:  5120,  -688,  5120
vtx colors: 0xB73F, 0xFFFF
texcoords:
     0:  654, 1277
     1: 1210, -282
     2: -145,  109
     3:  401,  256
     4: -145,  256
     5:  401,  109
     6: 1896,    0
     7:    0,  256
     8: 1896,  256
     9:    0,    0
    10:  430, -560
    11: -349, -838
    12: -905,  721

draw commands for this block:
    Select texture 1
    Set vfmt: pos=2 col=2 tex= 2
    init 1 mtxs
    Execute list 1:
        9D 00 04: TriStrip, VAT 5, 4 vtxs
            (POS_idx, COL0_idx? no visible effect, TEX0_idx)
            this is drawing the rock wall, it is 3 bytes per vtx
            01 00 06  [pos:(5120, -688, 0) col:B73F tex:(1896,   0)]
            00 00 09  [pos:(   0, -688, 0) col:B73F tex:(   0,   0)]
            05 01 08  [pos:(5120,  688, 0) col:FFFF tex:(1896, 256)]
            03 01 07  [pos:(   0,  688, 0) col:FFFF tex:(   0, 256)]
        00 00 00 00 00 00 00 00 00 00 00 00 00 NOP (padding)
    Select texture 2
    Set vfmt: pos=2 col=2 tex= 2
    init 1 mtxs
    Execute list 2:
        9D 00 04
            01 00 01
            06 01 0A
            00 00 00
            04 00 0C
        95 00 03
            06 01 0A
            07 01 0B
            04 00 0C
        00

## poly groups

animtest's first item (mod6.0) has 3 groups:
   1stP                                    flag
## 0001 0203 0405 0607 0809 0A0B  0C0D 0E0F 1011 1213 1415
   .... .... 0280 FFAA 0056 ....  .... .... 0018 0004 0002
   .... .... FFAA 0056 .... 0280  .... 0019 .... 0004 ....
   0280 FFAA FFAA .... 0280 ....  .... 0004 0007 .... ....
0x280 = 640, suspicious
0xFFAA = -86

# HITS.bin
HITS.BIN is map hitbox info
it's read by MapBlock::initHits

first several entries in HITS.bin:
## 0001 0203 0405 0607 0809 0A0B 0C0D 0E0F 1011 1213
   00F9 009C F8F3 F8F3 0280 01F8 2828 0E85 FFFF ....
   009C 011E F8F3 F8F3 01F8 0194 2828 0E85 FFFF ....
   011E 0125 F8F3 F8F3 0194 0136 2828 0E85 FFFF ....
   0125 00CC F8F3 F8F3 0136 00E6 2828 0E85 FFFF ....
   00CC 00CC F8F3 F8F3 00E6 .... 2828 0E85 FFFF ....
   009C 00F9 F837 F837 01F8 0280 00BB 8E83 FFFF ....
   .... .... .... .... 01E4 0188 F8F3 F8F3 01F8 0280
   2828 0E85 FFFF .... 0162 01E4 F8F3 F8F3 0194 01F8
   2828 0E85 FFFF .... 015B 0162 F8F3 F8F3 0136 0194
   2828 0E85 FFFF .... 01B8 015B F8F3 F8F3 00E2 0136
   2828 0E85 FFFF .... 01B4 01B8 F8F3 F8F3 .... 00E2
   2828 0E85 FFFF .... 0188 01E4 F837 F837 0280 01F8
   00BB 8E83 FFFF .... .... .... .... .... 0137 00F9
this looks like maybe variable length?

HITS.tab: (leading 0000s trimmed)
0100 0160 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0 01C0
0240 0440 0580 0580 0700
0840 0960 09A0 09A0 09A0 09A0 09A0 09A0 09A0
09E0 0A00 0A00 0A20 0A60 0A60 0A60 0A60 0A60 0A60 0A60 0A60 0A60 0A60 0A60
0AC0 0B60 0B60 0B60
0BA0 0BA0 0C00 1280 1400 1900 1A00 1AA0 1B40 1C60
total size is 6400 bytes (1600 words, which could be a 40x40 grid?)
the global map grid is much bigger than that

0x0	0x2	short	short		pos or size, range 0-640
0x2	0x2	short	short		"
0x4	0x1	??	undefined
0x5	0x1	??	undefined
0x6	0x1	??	undefined
0x7	0x1	??	undefined
0x8	0x2	short	short
0xa	0x2	short	short
0xc	0x1	??	undefined
0xd	0x1	??	undefined
0xe	0x1	??	undefined
0xf	0x1	db	byte
0x10	0x1	??	undefined
0x11	0x1	??	undefined
0x12	0x1	??	undefined
0x13	0x1	??	undefined
