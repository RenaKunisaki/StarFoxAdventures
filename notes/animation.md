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
    0x80: zero
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
