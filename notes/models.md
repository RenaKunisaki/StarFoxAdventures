0x811a1680 is where Krystal ObjectFileStruct gets loaded to
0x8002bb94 is where it's called
0x8002d5e0 is the first instruction after the file is loaded (but this function is used for many objects)
you can just save state once the second breakpoint hits (remove it first)
as long as Dolphin is paused when you load state, it remains paused

objs in fifo log that render Krystal:
obj 113 - 143
41 sets up textures, else she's all black (against a black background)

41:
00018818 98 0008 [...lots of data...]
0001BC3B BP 49 000000
0001BC40 BP 4A 007C1F
0001BC45 BP 4D 000010
0001BC4A BP 4B 0BE544
0001BC4F BP 52 010063
0001BC54 XF 000F0600 [...lots of data...]
0001BC99 XF 000F0610 [...lots of data...]
0001BCDE XF 0000100E 0000070E
0001BCE7 XF 00001010 00000400
0001BCF0 XF 0000100F 00000400
0001BCF9 XF 00001011 00000400
0001BD02 XF 00001009 00000001
0001BD0B XF 0000100A 0A141A00
0001BD14 BP 00 000010
the other objs actually render her

There must be a setting for the hair and loincloth to prevent face culling. Maybe an aspect of the material?
maybe not. I can't see her hair behind her head in-game either.

Krystal dlist unknown params:

List  12 13  1415  1617  1819  1A1B  note
   0  00 00  0000  0011  0700  0000
   1  00 00  0000  0075  0700  0000
   2  00 00  0000  00D9  0700  0000
   3  00 00  0000  013D  0700  0000
   4  00 00  0000  01A1  0700  0000
   5  00 00  0000  0205  0700  0000
   6  00 00  0000  026D  0700  0000
   7  00 00  0000  02CD  0700  0000
   8  00 00  0000  0331  0700  0000
   9  00 00  0000  0395  0700  0000
  10  00 00  0000  03F9  0700  0000
  11  00 00  0000  045D  0700  0000
  12  00 00  0000  04C1  0700  0000
  13  00 00  0000  0525  0700  0000
  14  00 00  0000  0589  0700  0000
  15  00 00  0000  05ED  0700  0000
  16  00 00  0000  0651  0700  0000
  17  00 00  0000  06B5  0700  0000
  18  00 00  0000  0719  0700  0000
  19  00 00  0000  077D  0700  0000
  20  00 00  0000  07D9  0700  0000
  21  00 00  0000  082D  0700  0000
  22  00 01  0859  086A  0700  0000  hair front
  23  00 02  0886  0897  0700  0000  hair back
  24  00 03  08BB  08CC  0700  0000  eyes
  25  00 04  08E8  08F9  0700  0000  armor, tiara, necklace
  26  00 05  0955  0966  0700  0000  cloth, shoes
  27  00 05  0955  09CA  0700  0000  cloth, shoes
  28  00 05  0955  0A2E  0700  0000  cloth, shoes
  29  00 05  0955  0A92  0700  0000  top, shoes
  30  00 05  0955  0AEE  0700  0000  cloth
   0  00 00  0000  0011  0700  0000  only dlist from test cube model
13: shader idx (could be u16)
14: offset of something relating to shader
16: offset of something
18: always 07 00 00 00

80e7357c list 22

changing these fields during play doesn't seem to do anything
nor does changing them just before they're read

8004bf50 reads some of this but this is just ZLB decompress
even trashing all of them immediately after that has no effect

stripping the FACEFEED header off the Krystal model makes it not load.

FACEFEED sig
00021DE0 uncompressed size
0000000C ZLB offset
00012B00 compressed size
00000018
00000028
00000001
00000359
00002140
this is 0x24 bytes but the struct is normally only 0x1C
maybe this is like with the textures where there are multiple LODs?

ZLB header at 0x24, data at 0x34
0xC * 4 = 0x30
