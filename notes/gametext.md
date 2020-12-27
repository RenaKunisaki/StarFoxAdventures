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
0x83: exit fixed-width mode
0x84: enter fixed-width mode
0x85: rr gg bb aa: set background color (only for empty spaces)
0x86: xx yy: ???
0x87: xx yy: set scale (xx+1, yy+1)
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

F8F4 xx yy zz: ???
F8F5 xx yy zz: ???
F8F6 xx yy zz: ???
F8F7 xx yy zz: ???
F8F8: align 0 (left)
F8F9: align 1 (right)
F8FA: align 2 (center)
F8FB: align 3 (?)

EF A3 B2: ??? eats some following chars
EF A3 B4 xxxx: set text scale, 0100 = normal
EF A3 B5 xxxx: depending on xxxx, eats next 6 chars
EF A3 B6 xxxx: same as B5?
EF A3 B7 02 41 EF A3 B7 00 04 should display the A button icon, but it just eats the string?
and 41 -> 42 for B

I think the EF and F8 codes are the same, just a matter of Shift-JIS encoding.
EF A3 B7 xx: sets whatever is normally the `unk` flag of the language
EF A3 B8: left align
EF A3 B9: right align
EF A3 BA: center align
EF A3 BB: left align, justify across full box
EF A3 BF rrrr gggg bbbb aaaa: set text color (R16 G16 B16 A16)

B5, B6, BC, BD, BE might not be valid codes at all
the game seems to just ignore them

EF A3 B7 00 00: no effect
EF A3 B7 00 01: no effect
EF A3 B7 00 02: more space between lines
EF A3 B7 00 03: even more space between lines
EF A3 B7 00 06: same as 3
EF A3 B7 00 07: eats all lines from this one forward
EF A3 B7 01 xx: eats all lines from this one forward
EF A3 B7 20 xx: text flickers all around the screen
this must be setting a font or something.

by messing with font scaling we can see various country flag icons...

I think B7 only takes one byte parameter (the font ID) but if you select an invalid font, it breaks, and that includes fonts not currently loaded.
font 5 seems to have special handling, might be Japanese.
the effect on line spacing might be from trying to determine the height of invalid characters (01-1F probably aren't in the font, so aside from CR, LF, tab, it might just overflow looking up their sizes)


format of gametext/Boot/English.bin:
u32 numChars = 43; //number of characterStruct entries
characterStruct Boot_English_characters[43];

000002B4:  00 07 (# following texts)
01 E8  (???)

gametextStruct Boot_English_texts[]:
 ID  nPhr wn ha va ln  phrases*
0339 0003 81 00 00 00  00000000  TEXT_ERROR_CRITICAL
033A 0003 81 00 00 00  00000003  TEXT_ERROR_DISC_UNREADABLE
033B 0001 81 00 00 00  00000006  TEXT_READING_DISC
033C 0004 81 00 00 00  00000007  TEXT_CLOSE_DISC_COVER
033D 0002 81 00 00 00  0000000B  TEXT_INSERT_GAME_DISC
033E 0006 81 00 00 00  0000000D  TEXT_ERROR_WRONG_DISC
0565 0001 93 00 00 00  00000013  DiscOpenBootError_1381
0000 0014 - total phrases

phrase string offsets:
00000000 00000017 00000018 00000079
0000009A 0000009B 000000E7 000000F7
0000010F 00000110 00000132 0000014F
0000015F 0000017E 0000018E 000001A2
000001AD 000001AE 000001BE 000001DD

text: array of strings, UTF-8
TEXT_ERROR_CRITICAL:
    0360: "An error has occurred"
    0377: ""
    0378: "Turn the power OFF..."
TEXT_ERROR_DISC_UNREADABLE:
    03D9: "The Game Disc could not be read."
    03FA: ""
    03FB: "Please read the NINTENDO GAMECUBE..."
TEXT_READING_DISC:
    0447: "Reading disc..."
TEXT_CLOSE_DISC_COVER:
    0457: "The Disc Cover is open..."
    046F: ""
    0470: "If you want to continue..."
    0492: "please close the Disc Cover..."
TEXT_INSERT_GAME_DISC:
    04AF: "Please insert a"
    04BF: "Star Fox Adventures Game Disc."
TEXT_ERROR_WRONG_DISC:
    04DE: "This is not the"
    04EE: "Star Fox Adventures"
    0502: "Game Disc."
    050D: ""
    050E: "Please insert a"
    051E: "Star Fox Adventures Game Disc."
DiscOpenBootError_1381:
    053D: "Loading..."


0548: 00 00 03 F0 EE EE... this is number of padding(?) bytes, followed by padding
093A: ...EE EE 00 02 00 04 maybe texture data? some big gaps
1820: 00 00... to EOF

no idea what a lot of this data is, so, difficult to rebuild the file even from the source code that happens to be left in.
but maybe we can patch some dummy string in, that just has all letters, so they're all loaded?
I don't think that will work since it's the characterStructs that define which chars are loaded.
we'd have to make a new file.

but it looks like the padding and following data is unused?
also it looks like the game doesn't actually use this file on the title screen!?
these messages are already in main.dol (RAM: 802c9b20)
maybe Boot is unused? it doesn't look like the rest of the game's text is here.
indeed at 0x2C6874 in main.dol is a copy of the data that's actually in the source file

very strange: if we change 'A' to '9' in Boot_English_characters, then the letter A won't show up anymore as expected... but the number 9 will. Not as A, but as 9. So all the other fields are ignored!?

for that matter it looks like the function that makes the new textures is only used from init? but it's not, it affects the entire game.
unless that just means it does some init code
patching it to just iterate through all ASCII instead of looking at the actual characterStructs does let us use different chars, but they're not in order.
maybe just because it's still expecting them in the order of the original list?
"Anerohascud.TtpwOFkNIEDGMCUBilfmbPRgvy,SxL"
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,"
looks like we can change which chars load, but ones that weren't in the original list still get skipped over.
it is referencing the original list's coords so yeah.
the reason we can use different chars is that it will look up the char's coords in the list and find it.
so we would need to make a patch that either changes this logic to iterate through all chars and build coords for them, or bypasses the whole damn thing and just loads the font from ROM or shows our own texture instead.
if we do the latter we can probably get away with embedding the texture data into the patch as long as we can free the patch when done.
(nope, not enough memory. or memory system isn't set up yet?)

I guess the reason it does this whole "generate a new texture" thing is because having the entire font in memory would take up way too much space for Japanese, and/or so that it can copy in graphics from multiple different textures and not have to switch between them at render time.
