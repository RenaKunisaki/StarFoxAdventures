//include/sfa/xml/Audio/Musyx.xml
typedef union MidiSetupEntry {
	undefined1 programNr;
	struct __attribute__((packed)) { u8 _pad_volume[0x1]; undefined1 volume; };
	struct __attribute__((packed)) { u8 _pad_panning[0x2]; undefined1 panning; };
	struct __attribute__((packed)) { u8 _pad_reverb[0x3]; undefined1 reverb; };
	struct __attribute__((packed)) { u8 _pad_chorus[0x4]; undefined1 chorus; };
} MidiSetupEntry;
