every sound channel seems to have a structure like:
FFFFFFFF
00004000 or 00000000 or 00010000
volume
volume copy
0
0
0
1 volume again?
0
0
0
0
there seem to be several channels enabled but not used, maybe because I turned off music
setting volume negative is weird, it becomes very loud and dings?
maybe it's inverting volume of disabled effects?
for global it's just silent

code at 80270AE8, 80270AF0 reads these with offset 0x5D4 but from bases 803BD180, 803BD1B0 which are < 0x5D4 apart, so they don't seem to be fields of an object of that size.

803398b0 controller inputs
803a32c8 set to 0101 to become Fox
	as Krystal it's 0001 so maybe only first byte matters
803428f8 Y velocity
803a33ab u16 staff upgrades
803a32a8 u8 current health, max health (4 = 1 heart)
803a32b0 u8 money or something
803a32ac u8 set to 1 to enable Sharpclaw Disguise
	using it as Krystal turns you back into Fox but keeps her voice
803a336a something to do with camera - set above 7F to pan somewhere else
803a32c4 save file name
803a32d0 some item unlocks

80270ae8 code that reads global volume (from R4 + 0x5D4)
R4 values:
803bd360 - global volume
803bd210 - does look like volume for something (in fact it's wind effect)

`(*809F4038) + 0x81A` should be the voice flag (16-bit)
but this address might not be constant (it's not)
    I think it's field 0xB8 in the Player object
    and Krystal's voice is indeed loaded, but changing this doesn't fix the animations

80270af0 code that reads SFX volume (from R3 + 0x5D4)
R3 values:
803bd1b0 - SFX
803bd180 - music
	call stack:
	80270970 [480128E5] - no effect if NOP'd
	802713F4 - NOP kills all sound (was 4E800021 "blrl")
		"branch to link register and link", ie tail call
		changing to just blr freezes the game
		this is a computed jump (to 80270938) - thread state?
		params:
			r0: unused
			r1: 803f8040 - stack pointer
			r2: 803e6500 - many floats which do nothing?
			r3: first argument; seen: 1, 0x28, 0x2D
			r4: 0xB4B4, 0x6464, 0xACAC...
			r5: unused (overwritten with 803c0000)
	80271508
	802830F0
	802846E8
	8024FD3C
	80243F60
	802461AC
	80336E88
	80246B28
	8004A9F8
80270afc [41820034] - NOP here prevents any new sound effects from playing.
	maybe checking if volume > 0, but restoring the opcode doesn't bring them back
	before that it's getting a byte from 0x8139edbc + 0x120 (0x8139eedc) and checking if it's 0xFF
	if so, it takes the branch


sfx.bin:
FoxFallScream
 ID |Ofs|Max|Of2|Mx2|?   |range| unknown table               |randTbl          |rndMx| ?|flg
0026 5F  00  7F  00  0064  0280 00F2 0000 0000 0000 0000 0000 64 00 00 00 00 00  0064 43 10
assumption:
	n = randInRange(0, randMax)
	for i, v in enumerate(randTbl):
		if n >= v:
			# use unkTbl[i]
			break
so in this case only one possible value

8110eb20 sound 0x3CE (KrystalRoll2)
Offset           +0 +1 +2 +3  +4 +5 +6 +7  +8 +9 +A +B  +C +D +E +F
8110EB20 [0000]  03 CE 5A 00  7F 00 00 64  02 80 02 65  00 00 00 00
8110EB30 [0010]  00 00 00 00  00 00 64 00  00 00 00 00  00 64 43 10
offset         = 0x5A
maxOffsetRand  = 0
offset2        = 0x7F
maxOffset2Rand = 0
unk06          = 0x0064
range          = 0x0280
randVals       = 0x0265, 0, 0, 0, 0, 0
randTbl        = 0x64, 0, 0, 0, 0, 0
randMax        = 0x0064
unk1E          = 0x43
flags          = 0x10

randVals 0x0261 = "Stay!"


the SfxBinEntry structure is something like:
SoundId id; //the actual sound effect ID
u8  baseVolume;
u8  volumeRand; //volume = rand(baseVolume - volumeRand, baseVolume + volumeRand)
u8  baseVol2;
u8  vol2Rand;
u16 unk06;
u16 range;       //how far from source object to silence
u16 randVals[6]; //actual sound to play (not same as SoundId)
u8  randTbl[6];  //chance to pick each sound
u16 randMax;     //sum of randTbl
u8  unk1E; //iiii a??b
	//iiii: index into sfxTable_803db248
	//a, b: unknown
u8 numIdxs : 4; //number of items in randVals/randTbl
u8 prevIdx : 4; //previously played idx

to play a sound:
look up the entry with the desired ID
if entry->numIdxs == 0, don't play

if the ID is 0xAB:
	//no idea what this is, just some kind of whoosh or creak.
	//it alternates between two different sounds every time it's played.

	entry->prevIdx ^= 1
	idx = entry->prevIdx
else:
	n = rand(1, entry->randMax)
	idx = the index of value n in entry->randVals
		//eg if randVals = [10, 20, 30] and n = 22,
		//then idx = 2, since randVals[2] >= n

	//avoid playing the same sound twice in a row.
	//if we chose the same one as last time, use the next one.
	if entry->prevIdx == idx:
		idx += 1
		if idx > entry->numIdxs: idx = 0

entry->prevIdx = idx
outId = entry->randVals[idx]
if outId is 0, don't play

compute the volume:
	if entry->volumeRand == 0:
		outVolume = entry->baseVolume
	else: outVolume = rand(
		entry->baseVolume - entry->volumeRand,
		entry->baseVolume + entry->volumeRand)

	//same calculation here, but result is cast to float.
	maxOfs2 = entry->vol2Rand
	if entry->vol2Rand == 0:
		outVol2 = (float)entry->baseVol2
	else: outVol2 = rand(
		entry->baseVol2 - entry->vol2Rand,
		entry->baseVol2 + entry->vol2Rand)

outField6  = (float)unk06
outRange   = (float)range
outTable1E = sfxTable_803db248[unk1E >> 4]
out1ELow   = unk1E & 1
out1EHigh  = (unk1E >> 3) & 1

u8 sfxTable_803db248[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0};

8000bb18 void audioPlaySound(ObjInstance *sourceObj, SoundId soundId);
8000c400 SfxBinEntry * audioGetSfxBinEntry(uint soundId);
	//given a sound ID, look up its entry from SFX.BIN
802751b8 SoundEffect * audioGetSoundEffectById(SoundId2 id);
	//SoundId2 -> SoundEffect*

struct SoundEffect {
	u16 id;
	u16 unk02;
	u8  unk04;
	u8  unk05;
	u8  unk06;
	u8  unk07;
	u32 offset; //u8 idx, u24 offset - not sure
	u16 rate;
	u16 pitch;
	int length;
	u32 repeatStart;
	u32 repeatEnd;
	u32 variation;
}

files under /audio:
midi.wad
starfox.h.bak - old list of SFX IDs (don't match final)
starfoxm.poo, starfoxs.poo - pool file
starfoxm.pro, starfoxs.pro - project file
starfoxm.sam, starfoxs.sam - sample file (the actual audio data)
starfoxm.sdi, starfoxs.sdi - sample directory
m=music, s=sfx?
https://github.com/axiodl/amuse is able to parse the audio data
of course, it does not build, and has some utterly insane dependencies (why the shit does an audio decoder want a 3D graphics library!?)

.proj: project structure, what belongs to which group
.pool: all data except samples
.sdir: locations of samples needed by groups
.samp: sample data

/audio/data:
EmptyN .bin 256K where N is 0..7
Music  .bin   2.5K
Sfx    .bin  38K
Streams.bin  20K

SoundMacro:
0x0 	4 	Chunk Size (note: includes the size value itself)
0x4 	2 	SoundMacro ObjectID
0x6 	2 	Padding
commands... 8 bytes each

Table:
0x0 	4 	Chunk Size
0x4 	2 	Table ObjectID
0x6 	2 	Padding

Keymap:
0x0 	4 	Chunk Size; (usually 0x1032)
0x4 	2 	Keymap ObjectID
0x6 	2 	Padding

Keymap Entry:
0x0 	2 	ObjectID
0x2 	1 	Transpose
0x3 	1 	Pan
0x4 	1 	Priority Offset
0x8 	Padded to 8 bytes

Layer:
0x0 	4 	Chunk Size
0x4 	2 	Layer ObjectID
0x6 	2 	Padding
Chunk Size 	Layer data

Layer Data:
0x0 	2 	ObjectID
0x2 	1 	Key Lo
0x3 	1 	Key Hi
0x4 	1 	Transpose
0x5 	1 	Volume
0x6 	1 	Priority Offset
0x7 	1 	Surround Pan; (0: extreme forward, 64: center, 127: extreme rearward)
0x8 	1 	Pan; (0: extreme left, 64: center, 127: extreme right)
0xC 	Padded to 12 bytes

Project:
0x0 	4 	Group end offset (points to next group in project)
0x2 	2 	Group ID
0x4 	2 	Group Type; 0 for SongGroup (for use with CSNG), 1 for SFXGroup.
0x8 	4 	SoundMacro ID table offset
0xC 	4 	Sample ID table offset
0x10 	4 	Tables table offset
0x14 	4 	Keymaps table offset
0x18 	4 	Layers table offset
0x1C 	4 	Normal page table (SongGroup) / SFX table offset (SFXGroup)
0x20 	4 	Drum page table offset (SongGroup)
0x24 	4 	MIDI Setup table offset (SongGroup)
0x20 	End of group header

Normal/Drum Page Entry:
0x0 	2 	ObjectID
0x2 	1 	Priority; voices are limited, so priority is used to play more important sounds over others
0x3 	1 	Max number of voices
0x4 	1 	GM Program Number
0x5 	1 	Padding

SFX Entry:
0x0 	2 	DefineID; referenced by game code
0x2 	2 	ObjectID
0x4 	1 	Priority; voices are limited, so priority is used to play more important sounds over others
0x5 	1 	Max number of voices
0x6 	1 	Definite Velocity; volume (usually 7F)
0x7 	1 	Panning
0x8 	1 	Definite Key; The default pitch - usually 0x3C (MIDI C4)
0x9 	1 	Padding

MIDI Setup Entry:
0x0 	1 	Program Number
0x1 	1 	Volume
0x2 	1 	Panning
0x3 	1 	Reverb
0x4 	1 	Chorus

SampleDir Table A:
0x0 	2 	Sound ID
0x2 	2 	Padding; always 0
0x4 	4 	Sound start offset, relative to the start of the ADPCM chunk
0x8 	4 	Unknown
0xC 	1 	Base Note; Corresponds to the MIDI note played in the sample, at the native sample-rate (which MusyX obtains from the INST chunk of .aiff files or SMPL chunk of .wav files, along with looping info). To play at a specified pitch in cents, set the playback sample rate using this formula: sampleRate * 2((pitch - baseNote * 100) / 1200.0)
0xD 	1 	Padding; always 0
0xE 	2 	Sample rate
0x10 	1 	Audio format
    DSP-ADPCM
    DSP-ADPCM (Drum Sample)
    PCM
    N64-VADPCM (Legacy Format)
0x11 	3 	Number of samples
0x14 	4 	Loop start sample
0x18 	4 	Loop length, in samples. To get the loop end sample, add this to the start sample and subtract 1.
0x1C 	4 	Table B entry offset, relative to the start of the sound metadata chunk

Table B:
0x0 	2 	Unknown; always 8
0x2 	1 	Initial predictor/scale (matches first frame header)
0x3 	1 	Loop predictor/scale (matches loop start frame header)
0x4 	2 	Loop context sample history 2
0x6 	2 	Loop context sample history 1
0x8 	2 × 16 	Decode coefficients
