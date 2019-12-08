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
