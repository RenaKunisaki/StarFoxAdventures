what the heck is at field D4 in ModelFileHeader?
it's parsed at 800403c0
data at 80f84240
seems to be a series of 3-byte values (high byte first)
processed one bit at a time, lowest to highest
first 4 are an opcode; next bits depend on opcode
it's not really 3-byte values, but the game always grabs the current and next 2 bytes even if it doesn't need that many. (probably this is the compiler playing it safe)
it's 0x168 (360) bytes (0xB40 (2880) bits)

I wonder if pModAnim is an array of bit indices into this?
that appears to have 864 (0x360) items
or 0x359 which is nAnimations
but in the file, pModAnim is zero!?
it's set by model_loadAnimations(), from MODANIM.tab
first few entries: 207h,   217h,   21Eh,   225h,    CDh,   230h,   231h,   21Dh
testing suggests these aren't bit offsets

first few bytes are: 01 CC 49 05 8A 00 2E 2C
0000 0001
1100 1100
0100 1001
0000 0101
1000 1010
0000 0000
0010 1110
0010 1100
bits are read right to left so this decodes as:
opcode 1, param 0
opcode 3, param 1110 (pos=16 nrm=16 col=16 tex=8)
opcode 2, param 0001 0101 (call list 0x15)
opcode 8 ???
since it's right to left the decoding really depends on how many bits are read at once
it looks like it will just skip other opcodes
but that leads it into calling nonexisting lists and initializing nonexisting mtxs
also the format it sets, I think would be 0x0001FE00, which doesn't work with dlist 21 or 0 (it reads past the end of the vtx data or is missing required fields)
dlist 21 itself only draws a single triangle?

it looks like the game is reading more here...
starting from 80F84240
80F84240 op 1
80F84241 op 3
80F84242 op 4
80F8424D op 2
80F8424E op 4
80F84259 op 2
80F8425B op 4
(it turned out to be because opcode 3 isn't always 4 bits param)

opcode 0: I think same as 4 but never happens?
opcode 1: select texture and material
    6 bits idx (same idx for both texture and material)

opcode 2: call dlist
    8 bits dlist idx

opcode 3: set vtx descriptors
    1 bit: vtx positions are 8 or 16 bits
    1 bit: normals are 8 or 16 bits
    1 bit: COL0 is 8 or 16 bits (ONLY PRESENT IF COLORS ARE USED)
    1 bit: TEXn is 8 or 16 bits (n=0 to 7; single bit for all)

opcode 4: renderOpMatrix
    4 bits # mtxs
        error if >20, but this is impossible...
    for each mtx:
        8 bits idx (model->mtxs)
        each mtx is initialized in some way involving the camera mtx

opcode 5: end of data

is it possible these are also describing number of vtxs per primitive? 1 to 4, 5=end, 0=invalid?

It reaches opcode 5 after only 34 bits but there are 360 bytes here...
other places that read this:

modelAnimFn_8003f7f4
    ....pt....nnnn[mmmmmmmm]............dddddddd
    p=pos size, t=tex size, n=#mtxs, m=mtxIdx (repeated n times)
    d=dlist idx, .=skip

    skip 4 bits
    modelSetupPosTexFmt(header,header->materials,&state);
        1 bit pos size
        1 bit tex size
        PMIDX = INDEXED

    skip 4 bits
    modelInitMtxs(header,model,&state,auStack136);
        reads 4 bits, a counter; for each:
            8 bits mtx idx
            init that mtx (maybe different from renderOpMatrix)

    skip 12 bits
    next 8 bits are a display list idx; call it

modelRenderFn_8005d4ec
    inits, sets current bit to param1->curBit + 4
    materialFn_8005e560
        6 bits, material idx
    4 bits unused?
    vtxDescrFn_8005f920
        1 bit pos size
        1 bit COL0 size
        1 bit TEXn size for every n in material->nTextures (1 bit for all of them)
    4 bits ???
        skips ahead by 0x40 bits at a time until four 0 bits?
    4 bits unused?

more?

the bigger question is under what circumstances these are called and how they determine where in the stream to start reading


field 0x80 is the animation offset but it's empty in the file
MODANIM.TAB is indexed by model ID (times 2)
but it's only 0x9E0 bytes (0x4F0 entries)
which might be the number of models?
for Krystal: 0x1E60
for Fox: 0x0000 or 0x06B4
it loads 0x10 bytes but that might just be the minimum it can load
the ((modelId & 0xFFFFFFFC) << 2) and (modelId & 3) stuff is just working around minimum file read size, it's really just indexing by (modelId << 2) since it's one int per model

AMAP.TAB is indexed by model ID
it points to some animation data for each model
it's a global file
Fox:     04 05 06 07
Krystal: 18 19 1A 1B
the whole file is strings of incrementing bytes like this
besides a few variations it mostly repeats 0x00 to 0x23 followed by 0x00000000

animIdxs=(0, 254, 275, 408, 537, 668, 0, 0)
for Krystal each is one less
for Fox and SharpClaw they're the same

maybe it's because, although we append Krystal model, we don't actually "load" it. it's there, but nothing references it, so it never gets correctly initialized.
although its refcount is 1...

it has to be ANIM.bin/tab
it looks like Krystal uses different animation IDs
when walking it calls animLoadFromTable with IDs:
Kry: 0x211, 0x212, 0x20D, 0x5F1, 0x207
Fox: 0x014, 0x015, 0x010, 0x003, 0x55E
0x211 * 4 = 0x844
at that offset in ANIM.TAB:
swaphol 00 04 B9 00  00 04 B9 20  00 04 B9 40
warlock 10 05 26 A0  10 05 2F 00  10 05 37 E0
seems like pModAnim in the model header points to the animation IDs
if we change Krystal's pointer to Fox's, she animates just fine, except the loincloth.
should try to actually load her animations in though.
(pModAnim was previously named pModelIndBuf)

pModAnim is empty in the file, so where does this data come from?
it's set in modelLoadAnimations
- get an offset from MODANIM.TAB[modelId << 1]
- copy from that offset in MODANIM.BIN
- size is (model.nAnimations * 2) + 8, max 0x800
for Krystal:
    nAnimations = 857
    offset = 0x1E60 (at 0x9D0 in MODANIM.TAB)
    size = 0x6BA
but MODANIM.tab/bin are global, so, this data isn't missing
the data that these animation IDs refer to is
(this data is the IDs themselves)

after loading that, it looks in AMAP.TAB at offset (modelId << 2)
it takes the offset found there and the one right after it (A, B)
model.amapTabEntry = A
it reads from AMAP.BIN, size B-A, offset A, into model.pAnimBuf
AMAP.BIN/TAB are also global

read from MODANIM.BIN from this model's offset onward
2 bytes per animation (the animation IDs?)
this value is an index into ANIM.BIN
the size is the corresponding entry from ANIM.TAB (next offset - this offset)
whatever is read, the first byte is a reference count, ignored in the file

so, consider animation ID 0x211 * 4 = 0x844
warlock/ANIM.TAB: 0x100526A0 0x10052F00 size=0x860
swaphol/ANIM.TAB: 0x0004B900 0x0004B920 size=0x20
warlock/ANIM.BIN:
    000526A0  00 16 00 E8  08 20 24 13  61 00 FF 57  FF 69 FC D9
    000526B0  FC CA F9 1A  FE C8 01 C7  FA 6A F9 4A  FD E8 F6 8B
    000526C0  FB CA F1 88  F4 2B 2E CA  F3 CA E3 8B  FB CA F9 09
    000526D0  F3 EB F2 AA  00 00 00 00  F0 43 00 00  0E A0 00 00 ...
swaphol/ANIM.BIN:
    0004B900  00 04 00 10  00 00 01 02  00 00 00 00  00 00 00 00
    0004B910  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
    seems like just blank data, not sure what the 0x10 means though in the high byte of the offset
no idea what this data means, but we shouldn't have to interpret it, just copy it like the other stuff
but we'd need to copy all 857 entries, looking them up in ANIM.TAB
and we'd need to grab the IDs from MODANIM.BIN
can make a script to do all this, but the resulting code might be huge.
even if we just copy the entire block from ANIM.BIN, we still have to set every entry in ANIM.TAB
it might be simpler to have the patch do this?
at least, have it load warlock/ANIM.TAB, read the offsets, add to them as necessary, and store them to the loaded ANIM.TAB
with any luck the IDs are contiguous
they range from 0x0000 to 0x0554, and a few are 0xFFFF
several are repeated too, especially 0x0001
not sure if any besides 1 are repeated?
there are only 0x359 animations so there must be gaps

so we'd need to:
- hook into ANIM.BIN loading
    - copy warlock/ANIM.BIN regions 0 - 0x000EB140
- hook into some init function after the anim IDs are loaded
    - load warlock/ANIM.TAB
    - iterate the anim IDs in the model
    - for each ID, if it's not FFFF:
        - get its entry from warlock/ANIM.TAB
        - add the offset we copied ANIM.BIN at
        - store that in the loaded ANIM.TAB
    - unload warlock/ANIM.TAB

this seems to work except the game crashes after being Krystal for a few seconds
not even a BSOD, but a bunch of noise on the screen
when pausing there's lots of graphical glitches
it seems to be when she runs. if we swap mid-run she glides for a second before it crashes.
maybe her run animation is different from Fox's?
MODANIM.BIN has indices into ANIM.BIN so maybe we need to change those?
if we don't actually store the indices into ANIM.TAB it doesn't crash until we try to load a new area and run out of memory...
right now we just copy a huge chunk of ANIM.BIN, maybe we can parse ANIM.TAB and only copy the parts we actually need, to save RAM?
and/or reuse Fox's animations where possible?
worst case maybe we can swap the model pointers?

it's crashing on animation 0x227 because the length is negative
0x227 << 2 = 0x89C
entry = 0x000b13a0
the corresponding ANIM.TAB entry is 0x10193e20
the next entry is 0x0004bbe0
not sure how we can fix that...
ANIM.TAB needs to be sorted
would it be completely insane to try to rebuild ANIM.BIN on the fly? load both of them, and copy entry by entry? is there even enough RAM?

apparently, copying Krystal's model over Fox, the animations work fine. So why does changing the pointer not work?
rolling is 0x47B
it works without changing the pointer
by switching between the characters mid-roll I got the softlock and when switching back to Fox, he's still stuck walking forward in animation 0x47B
it looks like changing the animIdxs works

in the meantime I patched it to set Krystal's pModAnim to Fox's but it doesn't quite work. rolling softlocks
maybe instead we can memcpy Fox's anim IDs over hers
or just change that pointer?
but isn't that what we just did?
and some of her other animations break, including crawling through a tunnel, which does work if we don't do this...

some of the animations that don't work right:
0x40D crawling through hole
0x40F standing up after crawling
0x431 walking slowly while holding staff

trying to fix it by changing header fields:
    0x80: zero (amapTab)
        actually in Krystal it's not and this is what we needed to change
    0xA4: zero
    0xDC: no change

80aa7240 Krystal model
    80f76be0 header
        80f76cbc 0xDC -> 80f76cdc
80ac1e00 Fox model
    80f338c0 header
        80f339bc 0xDC -> 80f33a78

Krystal header:
      +0 +1 +2 +3  +4 +5 +6 +7  +8 +9 +A +B  +C +D +E +F
0000  01 00 00 40  04 E8 00 00  00 00 00 00  00 02 1D E0
0010  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0020  80 F9 16 68  05 00 00 00  80 F9 19 20  80 F9 4D 20
0030  00 00 00 00  80 F9 68 A0  80 F9 16 80  80 F9 81 A0
0040  80 F9 85 90  FF C0 00 00  FF C0 00 00  FF C0 00 00
0050  00 00 00 00  80 F9 18 18  80 F9 87 D0  00 00 00 00
0060  00 00 00 00  00 00 00 00  00 00 00 00  80 F6 0D 60
0070  00 00 00 FE  01 13 01 98  02 19 02 9C  00 00 00 00
0080  00 02 19 98  21 F0 00 00  00 00 00 00  00 00 00 00
0090  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00A0  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00B0  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00C0  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00D0  80 F7 94 94  80 F9 15 00  01 68 00 00  80 F3 3A 78
00E0  00 24 04 04  08 A9 09 25  00 00 06 40  03 59 00 00
00F0  00 00 06 24  3B 1F 1F 14  06 2F 01 00

Fox header:
      +0 +1 +2 +3  +4 +5 +6 +7  +8 +9 +A +B  +C +D +E +F
0000  01 00 00 40  00 01 00 00  00 00 00 00  00 02 D4 A0
0010  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0020  80 F5 A5 E8  05 00 00 00  80 F5 A8 20  80 F5 D2 40
0030  00 00 00 00  80 F5 E8 80  80 F5 A5 FC  80 F6 05 44
0040  80 F6 09 34  FF C0 00 00  FF C0 00 00  FF C0 00 00
0050  00 00 00 00  80 F5 A7 50  80 F6 0B 74  00 00 00 00
0060  00 00 00 00  00 00 00 00  00 00 00 00  80 F6 0D 60
0070  00 00 00 FE  01 13 01 98  02 19 02 9C  00 00 00 00
0080  00 00 00 00  21 F0 00 00  00 00 00 00  00 00 00 00
0090  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00A0  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00B0  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00C0  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
00D0  80 F4 63 54  80 F5 A4 C0  01 25 00 00  80 F3 39 BC
00E0  00 21 04 04  07 05 07 63  00 00 07 31  03 5A 00 00
00F0  00 00 05 24  30 19 19 14  05 2F 01 00



80A07A40 Sabre
    80A07B4C model ptrs: 80a08ec0, 80a23a80, 80a3cd00
        Krystal model header: 80e6aac0
            pModAnim: 80e6ab2c -> 80e8c8a0
            animTabEntry: 80e6ab40 -> 21998
        Fox model header: 80e277a0
            pModAnim: 80e2780c -> 80e54c40
            animTabEntry: 80e27820 -> 0


```
modAnimOffs = MODANIM.TAB[(modelId << 1)]  // 0x9D0 -> 0x1E60
amapTabOffs = ((modelNo & 0xFFFFFFFC) << 2) // 0x13A0
amapIdx = (modelId & 3) << 2 // 0
model->amapTabEntry = AMAP.TAB[amapTabOffs + amapIdx] //0x21998
amap4 = AMAP.TAB[amapTabOffs + amapIdx + 4] //0x29F80

if ((model->flags & UseLocalModAnimTab) == 0) {
  model->pModAnim = MODANIM.BIN[modAnimOffs]
  //02 07 02 17  02 1E 02 25  00 CD 02 30  02 31 02 1D...
}
else {
  model->pModAnim = modelIndBuf; //function parameter
  size = bufSz; //model->nAnimations * 2 + 8
  round size up to 8;
  modelIndBuf = (short *)((int)modelIndBuf + size);
  load MODANIM.BIN into model->pModAnim
    with modAnimOffs, size
  //really, both cases do the same thing here, just one uses a buffer
  //provided as a function parameter and copies MODANIM.BIN into it,
  //while the other just uses a global buffer.
}

pModAnim is u16 * model->nAnimations
model->animIdxs is an array which stores each N
for which pModAnim[N] == -1

if ((model->flags & UseLocalModAnimTab) != 0) done;
```

model->pAnimBuf = AMAP.BIN
    offset = model->amapTabEntry
    size = amap4 - model->amapTabEntry
    ie read up until the next entry.


AMAP.BIN is a global file

Fox AMAP.TAB entry 00021998
Fox model 80a08ec0
Kry model 80a23a80
changing Krystal's model file header doesn't help
most other animation-related fields in Model and ModelFileHeader are 0

ANIM.TAB has the same entries present in both warlock and swaphol
the offsets are different but none are missing from only one file


climbing is animations (alternating):
0x6F, 0xBF (up cliff)
0x6E, 0xBE (down cliff)
0x2D, 0x2E (up ladder)
0x38, 0x39 (down ladder)
tied up 0348 0337 0351 others?


field_0xDC points to pointers to short[]
47 (0x2F) entries - field 0xF9 gives this count

in Krystal model file they're offsets (or IDs?)
000000FC  00 00 01 B8  00 00 02 F2  00 00 04 86  00 00 04 8C
0000010C  00 00 04 92  00 00 04 98  00 00 04 9E  00 00 06 4A
0000011C  00 00 06 50  00 00 06 56  00 00 06 5C  00 00 06 62
0000012C  00 00 09 76  00 00 09 7C  00 00 0A 58  00 00 0A 5E
0000013C  00 00 0C 24  00 00 0E BA  00 00 11 54  00 00 14 02
0000014C  00 00 14 08  00 00 14 0E  00 00 16 36  00 00 18 42
0000015C  00 00 1A F6  00 00 1A FC  00 00 1B 02  00 00 1B 08
0000016C  00 00 1B 0E  00 00 1B 14  00 00 1B 1A  00 00 1D 2A
0000017C  00 00 1D 30  00 00 1F 56  00 00 1F 5C  00 00 21 F2
0000018C  00 00 24 88  00 00 24 8E  00 00 26 B4  00 00 26 BA
0000019C  00 00 26 C0  00 00 26 C6  00 00 26 CC  00 00 26 D2
000001AC  00 00 26 D8  00 00 28 A8  00 00 28 AE
each is an offset of one of these entries.


length of each entry:
157, 202, 3, 3, 3, 3,
214, 3, 3, 3, 3,
394, 3,
110, 3,
227, 331, 333, 343, 3, 3,
276, 262, 346, 3, 3, 3, 3, 3, 3,
264, 3,
275, 3,
331, 331, 3,
275, 3, 3, 3, 3, 3, 3,
232, 3, 3,

every 3x entry is 0x44C0, 0x0015, 0x08AA
every longer entry is 0x2022, ..., 0x08AA
they're very similar but different lengths
this isn't PPC code

first entry: 157 (0x9D) entries
2022    27  ; x, y, z?
2023    28
2024    31

2025    25
2026    27
2027    28

2028    30
2029    30
202A    29

2040   -27  ; these are negative of previous,
2041   -31  ; except this one is swapped
2042   -28  ; with this one

2043   -25
2046   -27
2047   -28

204B   -30
204D   -30
204E   -29

C2A9    -6   -19
C2AA    -6   -19

42AE    -7
42AF   -16

62B0     8    -9
62B1    14    -7

42CC   -10
42CE     4

82D1    -5

42D5     2
42FB     3
42FC     4
42FD     4
42FE     5
42FF   -10
4300   -11

E346    12    -7   -35
634F    11   -10
2357     5
83A5    -5
C3E6    -6   -19
6458   -18   -10

4463     7
4464     3
4465     8
4466     8
A46B    -9   -40

2471   -12
2487   -10
2488    -7
24A7   -22

64A8    -6    -6
C4D5     5    -4

44D6     8
44EA     4
44EE     4
44F3     4
4522     4
4525     4
452C     4
452D     4
4531     2

C6D2    -8   -19
C6D8   -39   -35
C6DB   -38   -34
C6E0    -7   -19
E6E1     6   -38   -34
C6E3    -8   -18

66E4     8    11
26E6    -7
08AA
observations:
- looks like a (key, value) list except values are various length
- last entry is only one beginning with 0
- looks like first 6(?) bits are what to do, next 10 are an index (register?)
- then values for that, which depend on what it's doing
- second nybble isn't length since 24 and 44 are different length
- index mostly increases
- at 0x80f69ad8 in RAM with save file 1
    - 80f699fc -> 80f69a1c -> 80f69ad8
these aren't accessed at all during the game?
the pointer is accessed during loading but changing it does nothing?
80027460 accesses it during game
80027818 also
both seem to only check if it's null
if it is, nothing seems to change...
setting it to null when loading does crash the game
even changing it to set the same pointer for every entry (80029150 -> 3800xxxx) doesn't change anything... even with totally bogus offsets
so it really seems like whatever this data is, it's not important for loading the model.
either that or some shenanigans are going on, maybe it's affecting something that Dolphin doesn't care about?

the top 3 bits are probably x, y, z flags
the number of them that are set = the number of following values
where did we get 10 from? looks like all 13 remaining bits are one value.

these are probably XF register idxs
if you consider only the low 10 bits it looks like they all resolve to mtx regs
0x8AA is probably used as "end" because it doesn't map to any reg
-> 0x0800 | 0x00AA
    none of the top 3 bits are set, so 0 values = done

the top 3 bits are flags, see FUN_800244dc:
```
    val = *param
    next = param + 1
    if val & 0x2000: next++
    if val & 0x4000: next++
    if val & 0x8000: next++
    return next
```
    so this function advances to the next entry
the remaining bits are...?
I think a vertex count.
so these specify which vertices to modify, whether to change the X, Y, Z, and what to add to them.
a bone idx seems like it'd make more sense but the values are too big...
the function is also given a weight value to scale the change by.
not sure if this is necessary for just applying the bone transforms or if it's only related to animation
this function is called on the title screen (at 800270a4 -> 8002438c) but not in-game? not even for idle animations. 800270a4 isn't called either.
800270a4 NOP makes the title characters explode; restoring it doesn't fix them

at 80136224 the game chooses a random number between 0 and nPtrsDC
forcing it to choose a specific number (even 7FFF) doesn't appear to do anything...

fuzzFn_80027404 seems to be the only reference to field 0xDC other than just checking if it's null or converting offsets to pointers
it is called during game
disabling it does nothing? even during loading.
it's only related to shadows? I wonder if reflections are rendered like shadows...

8002430c modelApplyBoneTransforms receives some of the values as params
so, these values do have something to do with those...
this also isn't called during game or loading...

anyway, this data can't be related to animations because there's way more of those than there are entries here.
it might be used for _some_ animations?

============================================================

what happens if we replace swaphol's ANIM.BIN,TAB with animtest's?
Krystal and Tricky still animate normally, but the trees, flowers, NPCs etc go wacky, like they're just playing random animation frames

some Krystal anim IDs: (or really Fox's IDs)
0000 standing
0002 starting to run
0003 running
0016 walking
001B idle, looking at arm
001C idle, looking around
001E stopping walking
002D climb up ladder
002E climb up ladder
0038 climb down ladder
0039 climb down ladder
0045 stopping running/shielding
004C swimming slowly
004F shielding
005B strafing (holding L)
006E climb down wall
006F climb up wall
0070 hang on wall
0071 climb up from wall
0073 swimming
0074 swimming
00BE climb down wall
00BF climb up wall
00C0 hang on wall
00E6 stopping running
00E7 turning while running
00E8 turning
00F2 stopping running with staff
00F8 riding bike
0337 tied up
0348 tied up
0351 tied up
035A get item
040D crawling through hole
040F standing up after crawling
0413 floating in water
041D stopping walking with staff
0431 walking with staff
043A running with staff
0469 attacking with staff
047B rolling
047F rolling with staff

swaphol/ANIM.TAB entries almost all increase by 0x20 from 0
there are a few jumps, and a few entries have 0x10 as high byte

ANIM.BIN is indeed the animation data, as seen in loadAnimation()
and ANIM.TAB is the offset into ANIM.BIN as normal
and the size comes from subtracting from the next offset

ANIM.BIN is the same entries repeating:
00040010 00000102 00000000 00000000 00000000 00000000 00000000 00000000
until 0x6E0 which is the first jump in ANIM.TAB, at entry 0x37 (offs 0xDC)
which is also the first one to have the high byte be 0x10
next entry is 0x9C0 giving a size of 0x2E0

it looks like the high byte of the animation ID is index into ModelFileHeader.animIdxs and low byte is added to that entry
so for Fox: 0000 00FE 0113 0198 0219 029C 0000 0000
Krystal:    0000 00FD 0112 0197 0218 029B 0000 0000
and then the result of that is looked up in MODANIM
MODANIM.TAB seems to be 16-bit entries, 0x4EC (1260) of them
one per model ID (see modelLoadAnimations())
model->pAnimBuf is read from AMAP.BIN, with model->amapTabEntry being the offset
and amapTabEntry is from AMAP.TAB which is also indexed by model ID
and the pAnimBuf size is found by subtracting from the next AMAP.TAB entry
so Fox model ID is 1, Krystal is 0x4E8
so let's look at entry 1
in MODANIM.TAB it's 0000 and next is 06B4
in AMAP.TAB it's 00000000 and next is 00008610
in AMAP.BIN it's mostly bytes that increment from 00 to 23, then four 00, then repeat, with some exceptions
so the pAnimBuf data is eg 00 01 02 03...

so what's the flow here
animation ID
high byte is index into ModelFileHeader.animIdxs
low byte is added to that
result is index into MODANIM.TAB
that value is the actual animation ID to load
then I'm not sure where it's going but I think that ID is an index into PREANIM.TAB
if the high byte has 0x10 set then it uses PREANIM.BIN instead of ANIM.BIN
otherwise it ignores the value in PREANIM.TAB and uses the one in ANIM.TAB instead
in both cases it also looks something up in AMAP.BIN
first 3 entries in PREANIM.TAB: 10000000 10000180 00000740
so entry 1 is in PREANIM.BIN
I'm not sure what to make of that data
fucking around with it doesn't seem to make any difference
nor does changing the entries in PREANIM.TAB
this would seem to be because MODANIM.BIN maps these IDs to others
the idle animation is actually 0003
changing it to 001A (the next animation listed in MODANIM.BIN) replaces the idle animation with the walking animation
entries 3 and 4 in PREANIM.TAB: 10000760 10000D20
the first few bytes seem to relate to rotation, the rest, ????
this might be some kind of bytecode
the first 0x100 bytes or so look a lot less dense
changing the first 0016 to 0000 makes it not repeat; the anim timer stays at 1.0
as do 0001, 0006
but with 0010 it repeats, so this is some kind of flags
the ear fluttering and tail flicking aren't part of this animation

after that, 00EA to 0000 makes her move around her center a bit during the whole animation
pretty much any value does this

then 00002413 -> 00000000 = crash at 80007534 (reading next entry)
00002414 = a shuffle at the end of the anim
00002400 = no movement, and game freeze when trying to walk
00002420 = big shuffle at end
this must be length
0x2414 = 9236 which is just over 256 bytes per bone

then 41 -> 01 or 40 or 42 or 43 = a lot of wriggling about
00 = no movement, except the eyes (and ears/tail which are separate anim)
82 = some wriggling after a bit
pretty much any change causes the same effect
0x41 = 65
this may be an offset into the animation data, so changing it causes movements to be applied wrong?

then 00 which seems to have no effect

then 0010 -> 0020 = weird T pose (legs crossed, arms bent a bit)
0011 = shuffling
0012 = shuffling and game freeze
001C = shuffling and leaning back
0000 = weird T pose
0001 = weird T pose, turning, shuffling
000F = weird T pose and doing 360 flips back and forth
0030 = nothing
0070 = nothing
0040 = weird T pose
00F0 = nothing
0F10 = leaning back
3F10 = leaning back more

then 000A -> 000B or pretty much anything else = shuffling
00FF, 007F, 003F = stretching insanely wide
000F = gliding away to her right
010A = nothing
0F0A = slide to the right
3F0A = slide further right
FF0A = slide slightly left
00FA, 007A, 009A = flattening on X axis
004A, 008A = no effect
009B, 009C, 009D, 009E = flattening and stretching
0099 = flattening and not really coming back from it
0090, 0094 = flattening and staying that way
009F = stretching insane
004F = sliding away right
005F = stretching
006F = sliding
001A = flattening

seems like the low bits have meaning
0123 4567  89AB CDEF
0000 0000  0000 0101  000A default
0000 0000  0000 0110  000B shuffling
0000 0000  0000 1111  000F moving right
0000 0000  0001 0101  001A flattening
0000 0000  0100 1111  004F moving right
0000 0000  0101 1111  005F stretching

0-7=how much to move on X axis
B=stretch or slide - maybe whether to mirror this movement
might be the low 3 bits are telling whether the *next* entry applies to each of the 3 axes?

next FE37 -> 0E37: she turns to the left a bit
7E37 = spins 180 degrees right and stays that way
FE00 = weird T pose
FE36, FE38 = shuffling
FE3F = spinning
FE3E = less spinning
FE3C = quarter turn left
FE30 = shuffling
FE01, FE02 = weird T pose
FE0C = weird T pose + quarter turn left
FE07 = weird T pose
FE27 = weird T pose
FE2C = weird T pose + quarter turn left
FE4C = weird T pose + quarter turn left and back
F037 = slight turn right
so here the high byte is an X rotation and then there's two 4-bit values?

next FFA7 -> 00A7 = slight rise
F0A7 = sink into ground
05A7 = more rise
FF07 = slight sink
FFA0, FFA8 = shuffling
FFAF = liftoff and bouncing in air
FFAC = small rise and shuffling

next FC58 -> FF58 = slight tilt left
7F58 = flip completely upside down to the right
FC98, FCF8 = no effect?
FC57 = shuffling

next 0007 -> 000F = shuffling and sliding backward
0001 = shuffling
0027 = no effect?
00F7 = squish and pseudo egyptian pose
0107 = no effect
0407 = slight slide back
0F07 = sliding further back
7007 = sliding very far back

next FF66 -> FF00 = shuffling
FF77 = flattened egyptian
FF67 = shuffling
FF76 = flattened egyptian
6666 = bending backward so far her ears touch her butt
9966 = bending forward so far she could eat herself out

next 0008 -> 000A = shuffling
0018, 0038, 00F8 = flattened egyptian
0108 = nothing
0F08 = turning left, only upper body
2F08 = turning more left
2F09 = turning left and shuffling
2F07 = turning left, shuffling, bobbing head

this seems like it's just an offset and rotation for each bone but with the low bits indicating something

let's go further in to offset 0x5C
DCAA -> DC00 = shuffling
DC99 = shuffling, legs crossing a bit
DCFF = shuffling, legs crossing, nodding
DDAA, DFAA = nothing
D0AA, CCAA, 0CAA = nothing

this code is insane

it looks like pAnimBuf points to bytes which are assigned to model->bones[i].idx[0..n] where n can be 1 or 2
Krystal and Fox have 36 bones
at 736 (0x2E0) bytes of data here that gives just over 20 bytes per bone
but where did that number come from? 0xD20-0x760 = 0x5C0 (1472), almost 41 bytes per bone.

0x81580ae0 is the animation data in RAM right now

000A ;X stretch/offset
FE37 ;X rotation
FFA7 ;Y offset
FC58 ;Z rotation
0007 ;Z offset
FF66 ;X rotation
0008 ;Z rotation?

func 800074ec seems to operate on this data but after it's been copied into some other buffer and maybe transformed a bit
first byte is multiplied by 3
next 3 are unused?
then it reads 16-bit items where the low 4 bits and the high 12 bits are separated
if low bits aren't 0, they're added to a counter
then if the counter is >= 0x20, this lowest bit, interpreted as signed 4-bit int, is added to param10 and something is fetched from there
then entry & 0x10 is checked
not entirely sure what it's doing
looks like the same checks for 0x20 but not 0x40
- would make sense to have bits that tell whether a 2nd and 3rd field are present, but you must have at least one field.
this function takes a vec3s* in r31
the X value is the high bits of the entry
then if the next entry has 0x10 set, then that entry & 0xFFC0 is the X value for out[2] (ie 0xC+out)
which makes me suspect this isn't really vec3s
then if 0x20 is set we do another thing
which looks like probably the same with out[4].x (ie 0x18+out)
in some situations, out = out+0x3A
then at the end of the loop (whether above applies or not), out=out+2

the first byte (times 3) is a counter
not clear what it counts down by
when it hits 0, param6 is used
that points to some 8-byte structs (u16, ?, s16, ?)
it ends when the first member is 0x1000
then the function returns

some of this data probably defines how long each animation "frame" lasts?

let's breakpoint at 80007538 and see what it reads
not sure which animation it's reading but
r14=addr, r26=data, u16
812f8f8a 0000
812f8f8c 0000
812f8f8e 0000
812f8f90 fc89
812f8f92 0000
812f8f94 0000
812f8f96 fc69
812f8f98 0000
812f8f9a 0000
812f8f8a 0000
812f8f8c 0000
812f8f8e 0000
812f8f90 fc89
812f8f92 0000...
seems like it's always 2 bytes

r12 = param10 which is another pointer
r14         r26  r12
812f924a    19f0 812f9404
812f924e +4 0172 812f9404 +0
812f9252 +4 0133 812f9404 +0
812f9256 +4 eee6 812f9404 +0
812f9258 +2 fb86 812f9404 +0 - r26 is Fxxx but r12 not increased
812f925a +2 fe85 812f9404 +0

812f925c +2 fc49 812f9407 +3
812f925e +2 0069 812f9407 +0
812f9260 +2 fee8 812f9407 +0

812f9262 +2 0069 812f940a +3
812f9264 +2 fa2a 812f940a +0
812f9266 +2 35c8 812f940a +0

812f9268 +2 fb05 812f940d +3
812f926a +2 dcaa 812f940d +0
812f926c +2 fdc3 812f940d +0

812f926e +2 dca4 812f9410 +3
so the address increases by 2 or 4 a a time
r26 sometimes increases by 3
- seems like after the first few, it increases by 3 every 3rd entry
- so, it probably references some data that's 1 byte per entry

at 812f9404:
 X  Y  Z data at r14
95 5e 24 19f0 0172 0133 eee6 fb86 fe85
6d 0a 70 fc49 0069 fee8
05 07 5a 0069 fa2a 35c8
9f 02 b4 fb05 dcaa fdc3
5e 52 14 dca4 ...
9a df b0
d3 e3 a4
e9 3c 24
dd aa dd
8e 50 ac
66 11 1b
17 56 a1 ...

eg:
19F0: lo=0, hi=19F0
lo is 0 so just store hi directly to out[0], out[6], out[12]

let's do this in AnimTest since only the player animations will be loaded.
r14=80744dba r12=80744e96 should be first frame of idle anim
80744e96: 0010bce0 035001f1 d0011ea3 7601878b
80744dba: 0018feaa 0417ffa7 fba9ff66 fdc9fc69
all of this data changes. looks like it's swapped in from ANIM.BIN on the fly when switching animations.

for the idle anim the data actually is:
80744e96: 00000000 00386ff0 0d400445 00000a37
80744dba: 0010000a fe37ffa7 fc580007 ff660008
made a save state on what should be the first frame. let's mess with it.

80744dba 00  (r14)
    ff: no change
80744dbb 10
    18: lots of fidgeting
    20: partial T pose, legs crossed
    08: partial T pose and fidgeting
    11: fidgeting
    0F: partial T pose, legs crossed, spinning on own X axis
    01: same as 08
    02: same as 08
    04: same as 08
    40: same as 20
    80: same as 20
    FF: spinning
    00: same as 08
    90: no effect
    50: no effect
    suspect this is flags for which fields are present in following data.
    changing number of bits = all following data is misread.
    would make sense: 3 bits for X,Y,Z translation, 3 for rotation, top 2 unused
80744dbc 000a
    0000: some fidgeting
    0008: same as 0000
    0002: different fidgeting
    010a: no effect
    ff0a: no effect
    0077: becomes flat
    0011: fidgeting
    000b: same as 0011
    0009: same as 0002
    007a: alternates between flat/normal
    001a: same as 007a
    00fa: same as 007a but not completely flat
    0a0a: moves to the right
    070a: same as 0a0a
    0f0a: same as 0a0a
    7f0a: moves far to the right
    800a: moves far to the left
    007f: becomes very wide
    0080: fidgeting
    003f: same as 007f (possibly not as wide)
    001f: same as 007f
    000f: slides far to right
    000b: same as 0000
    could be flags for whether X,Y,Z scale are present?
80744dbe fe37
    0000: partial T pose, legs crossed, fidgeting
    fe00: same as 0000
    0037: no effect
    0038: fidgeting
    003f: spinning on Z axis
    fe3f: same as 003f
    8037: spins 180 on Z axis
    7e37: same as 8037
    f037: turns slightly to the right
    e037: turns more to the right
    ff37: no effect
so far looks like:
    00: ???
    10: which fields are present (1)
    00: X offset
    0a: X scale?
    fe: ???
    37: which fields are present
this might even be bitflags
reading 3 bytes at a time reminds me of how it parses render ops, where it overreads just in case.
looking at the code, it does seem to be doing a lot of bit manipulating.
at r14 is u16s:
    FFF0: ??? lowest 2 bits are flags
    000F: length
if length is 0, just write directly.
maybe this is just RLE? but it doesn't seem to loop copy/fill ops.
could also besome custom LZ variant (but why? easier to decompress on the fly?)
might explain where the 2413 comes from (output length)
definitely does smell like compression.

also, the fact that the output pointer advances by 2, but it writes to out+0, out+6, out+12, suggests the data may be interleaved XXX YYY ZZZ
- actually it writes 0 to out+6, out+12

so, binary:
00000000 ???
00010000 00000000 00001010
1111111 Z rotation
0 00110111

r14=pEntry r20=pData
entry is u16

sample of decompressed data:
head            tail
0010 ffec 0008  0008 ffcc 0010 rotation
0000 0000 0000  0000 0000 0000 scale (why zero!?)
0003 000c 0006  0000 fff2 0000 translation
00000000 00000000 00000000 00000000
00000000 00000000 00000000 ???
fffc fffd fffb  0000 fffd ffff rotation
0000 0000
the decompression is being done every frame and the result is gradually changing, so it might be a delta
more likely, this is the raw transformation data for this frame, and the values are being interpolated over time. not sure where, though...
if we disable the whole decompression call, the staff instantly grows to full length. and now we can freely edit the decompressed data, though it does get reset by a different decompress call when we move.

data is: s16 X rotation, s16 Y rotation, s16 Z rotation (on local axes)
s16 ???, ???, ???
u16 X scale, Y scale, Z scale (0x0400 = normal)
s16 ???, ???, ???
s16 X translate, Y translate, Z translate
then 34 bytes that seem to have no effect (17 shorts, 5 2/3 vec3s)
then it appears to repeat for the next bone
my guess:
s16 rotXHead, rotYHead, rotZHead, rotXTail, rotYTail, rotZTail
u16 scaleXHead, scaleYHead, scaleZHead, scaleXTail, scaleYTail, scaleZTail
s16 transXHead, transYHead, transZHead, transXTail, transYTail, transZTail
34 bytes ???
but it doesn't look like the tail does anything...

there's a case that skips 0x3A (58) bytes of output. that's 64-6
each entry is 64 bytes, and vec3s is 6
it looks like that case is after every three fields (rot, scale, xlate).

I suspect the eye movement isn't handled here, though.
if we disable animDecompress there's still some eye movement but not when buying an item
but I don't recall there being eye control bones so how is it being done?
maybe some other routine writes to the skipped fields? might be a shader param?
I think the eye texture/shader are separate from the rest...
they're dlist 24 and the only ones using shader 3.
eyelids are TEX1 0724 "mipmaps" 0-3; eyeballs are 0661 or 0728
Fox's eyelids might be 065E
Fox's and Krystal's textures actually have half-closed eyes on them.
both have green eyes so it's hard to tell which is actually used
the eyes do also move around even in the idle animation
065E isn't loaded in swaphol, 0661 is, so that's probably it
swaphol/TEX1.tab 0x1984 is the entry for 0661: 8109F3A0
that is Fox's eyeball texture, but not Krystal's (or is the mod just overriding it?)
0661, 0728 are the only eyeball textures in animtest
0728 is right after Krystal's textures (0724, 0725, possibly 0726) so that's probably it
so knowing that, what do we do?
0724 is Krystal's eyelid: full open, half closed, full closed, 3/4 closed
065E is Fox's     eyelid: 1/4 closed, half closed, closed, 3/4 closed, wide open, confused, annoyed
(so what does Krystal do in the latter two cases? I guess just full open, or it wraps?)
so we need to figure when the game swaps these textures and how

  0 1 2 3 4  <-- n/4 closed
F 4 0 1 3 2
K 0 - 1 3 2
maybe for Fox the "1/4 closed" is actually "open"

the blink anim for both would be: 0, 1, 3, 2, 3, 1
the sequence 0 1 3 2 doesn't show up in memory (u8, u16, u32) in any meaningful place

it might be enough to just change the texture order in Krystal's objdef?
no, because it's the mipmap IDs, not the texture IDs

the eye animation functions don't seem to be involved at all during scripted eye movements, only idle animations.
it's probably the animation (sequence 0) changing the texture directly.
should locate it and watch for writes to its params.
if we spawn several Krystals they all blink independently, so it's not the texture that's changed.
so some parameter of the model or object is being changed to select the texture.

the EyeAnim data isn't changing
it's probably the Override doing it
there are two of those. one seems to control the camera. the other I can't delete. it just doesn't actually disappear.
- previously I was able to delete, and it made me able to walk during the scene.
deleting the AnimCamera doesn't change the eye movements.
I was pushing the wrong button again -.-
actually, deleting the camera makes it impossible to see the eyes, because now it's behind me.

suspect the Override object is just copying the player and hiding the real one which is why the eye anim variables aren't being used.

the seq that runs this animation is:
00000000 0001 FFFF Override
00000000 8010 FFFE AnimCamera
00000000 C010 0443 VariableObj
(where is this data? somewhere in OBJSEQ.bin)

VariableObj is fairly simple: OBJECTS.bin @ 0x03E720:
0003E720  00 00 00 00  3F 80 00 00  00 00 00 9C  00 00 00 A0   .... ?... .... ....
0003E730  00 00 00 A0  00 00 00 00  00 00 00 00  00 00 00 00   .... .... .... ....
0003E740  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 A0   .... .... .... ....
0003E750  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00   .... .... .... ....
0003E760  00 00 00 00  00 02 00 21  00 00 00 00  00 00 00 00   .... ...! .... ....
0003E770  00 C6 00 10  00 01 00 0F  00 00 00 00  00 FF 00 00   .... .... .... ....
0003E780  00 00 0A 00  00 01 00 00  FF FB 00 0A  FF FB 00 0A   .... .... .... ....
0003E790  00 00 00 00  00 00 00 00  FF FF FF FF  FF FF FF FF   .... .... .... ....
0003E7A0  FF FF FF FF  00 00 00 00  00 00 00 00  02 00 00 00   .... .... .... ....
0003E7B0  01 56 61 72  69 61 62 6C  65 4F 62 6A  00 00 04 5F   .Var iabl eObj ..._

OBJSEQ2C.tab might just be mapping IDs to other IDs like OBJINDEX.bin
ACRomTab = ANIMCURV ROM Table
ANIMCURV.tab is where the index is looked up after checking OBJSEQ2C (seq to curve?)
that points to somewhere in ANIMCURV.bin which should start with SEQA or SEQB strings (not compressed)

when we buy an item, the sequence IDs are: ffffa3e0, ffffa3e1, ffffa3e2
since those have bit 0x8000 set, we look them up in OBJSEQ2C.tab:
((idx & 0x7FF0) >> 4) << 1
or more efficiently: (idx & 0x7FF0) >> 3
giving 0x47C for all three
add the entry there (0x979) to (idx & 0xF): 0x979, 0x97A, 0x97B
look those up in ANIMCURV.tab: (0x25E4)
80004638 80004874 80004998 - look those up in ANIMCURV.bin

command data: 1byte command, 1byte len?, 2byte param
commands:
    00: if field_58 < xxxx, stop; else, set field_68 to xxxx
        field_58 might be "current seq pos"?
    02: play animation xxxx
    03: related to animation
    04: related to models
    06: play sound xxxx from current obj
    07: ?
    0B: condition: if ???, skip next xxxx actions
        xxxx can be <= 0 in which case, do nothing?
    0D: subcommand (high 4 bits of xxxx=cmd, low 12=param)
        0: play song param (but only certain ones?)
        2: getEnvFxAct
        6: warpToMap param
        7: nop
        8: set eye state and texture 5 state (eyes = texture 4)
        E: set texture 5 state
        F: set eye state
    0E: show dialogue
    0F: ???
        for all commands except 0F and 00, field_68 += ll
    FF: related to anim looping?
when field_58 < field_68, stop
for now I've named field_68 stopAtAction and field_58 curAction58 (curAction already exists)

maybe we can hook at 80087144 and check, if Krystal, change the eye anims
breakpoint at 80086214
seq data is at 812f49c0
