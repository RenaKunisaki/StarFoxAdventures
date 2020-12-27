Krystal uses textures 0xD4 - 0xD9 inclusive
warlock/TEX0.tab for these: (offset 0x350)
8F017690 01000000 01000000 88017C50 88017F90 8F018200
TEX1: all are 01000000
TEX0.bin is only 0x50EA0 bytes long so the first byte must be flags
these offsets seem to be partway through ZLB archives...
maybe they're from the global TEX0?
the texture IDs seem to be treated as 16-bit
looks like ID & 0x8000 tells whether to use TEX0 or TEX1

high byte & 0x3F is length?
if it's 1, it does something different
    many entries are set to this; it probably means "don't use"
it uses `(length+1)*4`, which would give a max 256 bytes (64 entries) and a minimum of 2 entries
it uses offset*2

at that offset in TEX0.bin is a few ints, then a ZLB archive:
(length = how many entries before ZLB)

0xD4 @ 0x2ED20 (0x17690*2)
0x0040  0x0100  0x01C0  0x0280
0x0340  0x0400  0x04C0  0x0580
0x0640  0x0700  0x07C0  0x0880
0x0940  0x0A00  0x0AC0  0x0B80
looks like every entry is 0xC0 + previous
ZLB output size 0x260

0xD5, 0xD6 entries are 0x01000000

0xD7 @ 0x2F8A0 (0x17C50*2)
0x0024  0x00E4  0x01A4  0x0264
0x0324  0x0404  0x04C4  0x05A4
0x0664
offset 0xC0 again
output 0x860

0xD8 @ 0x2FF20 (0x17F90*2)
0x0024  0x00A4  0x0124  0x01C4
0x0264  0x0304  0x0384  0x0424
0x04C4
offset 0x80
output 0x860

0xD9 @ 0x30400 (0x18200*2)
0x0040  0x0320  0x05E0  0x0880
0x0B00  0x0D60  0x1000  0x12C0
0x15A0  0x1880  0x1B40  0x1DC0
0x2020  0x22C0  0x2580  0x2860
offset 0x2E0
output 0x460

so what's left:
- what do the remaining 2 high bits mean?
- what do these entries mean?
    - they're much too big to be offsets into the ZLB-packed data, unless they're right shifted first? but that would be unusual, it doesn't do that elsewhere.

decompressing the first one to `dump/tex0-d4.bin`
it looks like it has a header:
0000  00 00 00 00  00 00 00 00  00 00 00 10  00 10 00 01
0010  0F 00 00 00  00 00 05 00  00 01 01 00  00 00 00 00
0020  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0030  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0040  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
0050  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00
then what looks like pixel data, 0x200 bytes, 16bpp

we may not need to understand the data
if we just copy enough from TEX0.bin into memory and update the table
    this doesn't seem to work; we've successfully added them but they're not being used
    and there's already other offsets at 0xD4
    I don't see Krystal textures in any unusual places...

warlock/TEX1.bin: 0x143280 Krystal fur/clothes
it's ZLB, and then another layer with a header like above
so where did we get 0x2ED20? it's 0x17690 << 1 which is what TEX0.tab has
0x143280 >> 1 = 0xA1940

TEX1.tab @ 0x1C94: 810A1940; that's index 0x725
where does that number come from? not a multiple of 0xD4
0x1AA8A in krystal.bin has this number and other similar ones
I've seen this before, where?
field 0x20 points to it
this is the "textures" field...
but in memory it's 0xD4, 0xD5...
maybe those are the slots they got assigned to?
values: 0x725, 0x726, 0x729, 0x72A, 0x724, 0x728
so maybe the problem is just that we're loading the wrong textures.
and we're patching TEX0 instead of TEX1

TEX1.bin entries 0x724 - 0x72B
00001C90  84 09 EB 90  81 0A 19 40  81 0A A9 10  01 00 00 00
00001CA0  81 0A B2 70  81 0A B6 00  81 0A B7 40  01 00 00 00
these get multiplied by 2

so we need to copy from 0x13D720 to 0x1570f6 which is 0x199d6 bytes
81134870 tex1.tab entry 0x724
    840f82a0 810fb050 81104020 10000008
    81104980 81104d10 81104e50
80526020 tex1.bin entry 0
    original size is 0x1f0540
    new      size is 0x209f40
    so our new textures go at 0x80716560
    0x1f0540 >> 1 = 0xf82a0 which matches the table
    but what's there is gibberish
80002944 is where we memcpy the textures
    r3 = 8053fa20 r4 = 8086d680 r5 = 00019a00
    r3 is 0x19A00 + 0x80526020
    we're adding the wrong offset here
    lwz     r5, SP_EXTRA_SIZE(r1)  <-- we needed SP_ORIG_SIZE here.
    so it works now.
    unless you load in as Krystal (forcing player ID to 0) in which case it crashes while loading texture 0x724
    even if we don't actually change TEX1.tab
    is it possible this isn't caused by our patch?
    without it, forcing the ID loads krazoa instead
    but with it we can load file 1 just fine...
    and if we force the ID and load file 2, with or without the patch, it's all fine there.

looks like whatever is at 816a87c0 is wrong
read at 80048610
r28 (length?) = 00000000 r29 = 816a87c0
data is:
9790abd5 617b37f0 7d0b4161 6b587e26  looks like ASCII but is gibberish
35811f8b 000026d8 00001000 000012c0
the first 5 words come from swaphol/TEX1.bin @ 0x1BB940
which is 0xddca0 << 1
which is the entry we wrote into TEX1.tab
so why is it copying that here? and before it is all zeros
what should be here is the data we copied from warlock/TEX1.bin
is this before we've copied it?
80054764 writes 0 here, 800549f0 reads it
that repeats a zillion times
8004865c memcpys to it - now there's a few offsets here
80048610 reads it, still offsets
80054a44 reads it
80054764 writes 0 again, 80054a44 reads again, repeat...
then 8004865c again
this area seems to be a scratch buffer
8004865c is where it copies this data in
our code hasn't executed yet
and then crash
so maybe we're not patching the right place?

800023e4 is where we memcpy the model

80054a7c crash:
    PC  80054A7C stw r0, 0(r24)
    r0  00000000 803F7FC0 803E6500 F0459BE0
    r4  04F0B50F 00000000 00000000 00000000
    r8  00000729 0000C200 0000C200 803F7FC0
    r12 8036F880 803E31E0 00000000 FFFF78D7
    r16 D802E59C 00000000 00000001 00209A20
    r20 00000100 41104D10 00000000 00000000
    r24 522EFA00 9E16A1C0 00000729 00000001
    r28 00000020 00000001 00000000 00000001
    stack trace:
    8002960C 8002D8C8 8002BB98 800577F8 80020914 80020D74 8002136C 80003274

512x256 = 128K, and that's assuming 1 byte per pixel
it would be interesting if we could edit the texture itself with some small patches to the compressed data. would be a lot of work though. have to be able to decompress it and basically trial-and-error change the data and see how close the result is to what we want.

> Yes, the textures are, in most cases CMPR (some outliers for transparent and greyscale), which is already a compressed format.
> That file is then compressed with whatever Rare's compression is, which is prominent in their 64 titles, and then that is packed up into a ZLB archive, which is the last layer of compression.

looking at the file log, the global MODELS.bin/TEXn.bin isn't loaded at all when we boot the game and load the save file

during gameplay the ModelFileHeader texture IDs aren't read at all, except as overread when parsing the render instructions before them.


first few ZLB entries in warlock/TEX0.bin are at offsets:
0x0000, 0x0080, 0x0960, 0x1080, 0x1760, 0x1840

TEXTABLE.bin looks like a list of u16 texture IDs?
if so, the 6 entries from 0xD4*2 are:
01C1 01C2 01C3 0000 00CF 00D0
if we look at those entries in TEX0.tab the first 3 are 01000000 in both TEX0 and TEX1

TEXPRE.bin is only 0x330 bytes, so these aren't there...

for our save file Krystal materials are at:
0x80f4e7c0, 0x80f4e804, 0x80f4e848, 0x80f4e88c, 0x80f4e8d0, 0x80f4e914


------------------------------------------------------------------------

let's try to understand the data for Krystal texture (0x724) in animtest/TEX1.bin (offset 0x46860 len 0x19a00)
size is 512x256

the entry in TEX1.tab and the following entries:
00001C90  84 02 34 30  81 02 61 E0  81 02 F1 B0  01 00 00 00  
00001CA0  81 02 FB 10  81 02 FE A0  81 02 FF E0  01 00 00 00  

the beginning of the texture data:
00000000  00 00 00 14  00 00 15 B4  00 00 2C 94  00 00 43 F4  
00000010  00 00 5B 54  5A 4C 42 00  00 00 00 01  00 00 2B 00  
00000020  00 00 15 75  58 85 ED 59  5D 6C 1B 57  76 26 ED 2C  

0000: offset of ZLB header
0004: decimal 1556, 11412, 17396, 23380

the high byte of the TAB entry & 0x3F is the length
it's 4, so there's 4 ints between the ZLB offset and the actual ZLB header
the upper 2 bits are probably like in other tables, telling whether to use one slot or the other

in fact, at each one of these offsets is a ZLB header
except 5B54, that has 12 0x00 bytes beforehand
that one is also much larger
I assume these are mipmaps
all are 11008 bytes except the last which is 87488
512x256 = 131072
this size isn't even a power of two, so definitely a funny format

the files have an 0x60-byte header which is probably a Texture struct
they do give the size at 0x0A, 0x0C
the smaller ones are listed as 64x64
0x16 is apparently format, it's 0x0E here, which is apparently http://wiki.tockdom.com/wiki/Image_Formats#CMPR

 but we should be able to just patch in a new texture file which specifies a format like RGB565 (0x04); we don't need alpha anyway

we may not need to, as this appears to be BC1 which Gimp can export (but byteswapped), but swizzle is still an issue, and I'm not sure how that works with BC1

anyway I got the swizzle working with RGB565, but for some reason the lighting is all fucked up. mipmaps?
how can they bey 64x64 anyway? that's not the same aspect ratio.
maybe they're something else?

well it looks like the byte at 0x19 is how many mipmaps (including the base, so it's 5 here, and changing it to 1 fixes the issue)

other fields that aren't zero:
0x0F = 1 - no effect if changed to 0
0x10 = 1
0x17 = 1
0x18 = 1
0x1A = 1
0x1D = 6 - no effect if changed to 1

726 = eyes
