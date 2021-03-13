//include/sfa/xml/Audio/Music.xml
typedef union MusicBinEntry {
	u16 id;
	struct __attribute__((packed)) { u8 _pad_songId[0x2]; u16 songId; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; u16 unk04; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; byte unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk0D[0xD]; byte unk0D; };
	struct __attribute__((packed)) { u8 _pad_flags[0xF]; byte flags; };
} MusicBinEntry;
