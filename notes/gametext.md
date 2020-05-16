`gametext/<mapname>/<language>.bin` is a different format from root `gametext.bin`
that one might not even be used

u32 number of entries
characterStruct[] entries, 16 bytes each up to 0x5C4

after that, looks like 12-byte structs, probably gametextStruct

ID   nPhr Wn aH aV Ln phrases
0000 0005 2D 00 03 00 00000000  
004C 0007 7C 03 00 00 00000005  
004D 0007 7C 03 00 00 0000000C  

0x0	0x2	GameTextId	identifier	which game text is this
0x2	0x2	ushort	numPhrases	how many phrases this text has
0x4	0x1	typedef u8 byte	window	preferred window
0x5	0x1	typedef u8 byte	alignH	horizontal alignment
0x6	0x1	typedef u8 byte	alignV	vertical alignment
0x7	0x1	GameLanguageEnum	language	LANGUAGE_xxx from GameTextData.h
0x8	0x4	char * *	phrases	pointer to <numPhrases> pointers to localised strings

from 0x5C4 to 0xCC0 = 0x6FC bytes = 0x95 (149) entries
but at 0x93C the ID resets to zero (0x378 bytes, 0x4A (74) entries)
at 0xCC0 the actual text begins

actually I guess they begin at 0x5C8
also the offset seems to be an index into a string table
I think when the ID is 0, that's the beginning of the table
after the ID you have 00DF which is probably number of strings
and then offsets to each: 0x00, 0x09, 0x0E, 0x18...

00DF is at 0x942, strings at 0xCC0, so 0x37E bytes = 0xDF (223) strings plus the 00DF itself

so the entire file is:

u32 num_char_structs;
characterStruct[num_char_structs];

u16 numTexts;
u16 unknown;
gametextStruct[numTexts] texts;

u32 numStrings;
u32 strOffset[numStrings];
char[] strings[numStrings];

FrontEnd contains the credits, some unused cheats (Xmas Mode), "Play full game", "GAME SELECT", "PREVIOUSLY ON", Slippy's hints
AnimTest contains only the generic menu texts from every map
Boot contains only disc read error messages and "Loading..."
Sequences contains a ton of files... maybe only one text each

escape sequences in debug text:
0x81: set color; followed by u8 r, g, b, a
0x82: set position; followed by u16 y, u16 x
0x83: ??? clears the flag set by 0x84?
0x84: ??? sets a flag
0x85: set some (bg?) color; followed by u8 r, g, b, a
0x86: ??? followed by 2 bytes
0x87: ??? followed by 2 bytes
I don't see anywhere these are used except color.

escape sequences in gametext (maybe only after 0xEFA3):
0xB4: set style? followed by 2 bytes
0xB7: switch font texture? used to display icons. followed by 2 bytes.
    2: buttons
    4: default
    5: faces?

these would be UTF-8 for characters 0xF8F4 and 0xF8F7 which are in the Private Use area
but this isn't strictly UTF-8 as the bytes following can be zero, or bytes that don't form a valid UTF-8 sequence
is this valid Shift-JIS?
