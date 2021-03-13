//include/sfa/xml/Video.xml
typedef enum { //type:u32
	RenderFlagsEnum_EXTRA_FUZZY = 0x2,
	RenderFlagsEnum_NoFog = 0x1,
	RenderFlagsEnum_ForceFuzz = 0x8,
	RenderFlagsEnum_DrawFuzz = 0x4,
} RenderFlagsEnum;
//include/sfa/xml/Video.xml
typedef union EnvFxActEntry {
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; byte unk5C; };
} EnvFxActEntry;
//include/sfa/xml/Video.xml
typedef union FineSkinningConfig {
	struct __attribute__((packed)) { u8 _pad_numPieces[0x2]; u16 numPieces; };
	struct __attribute__((packed)) { u8 _pad_quantizeScale[0x6]; u8 quantizeScale; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; pointer unk0C; };
} FineSkinningConfig;
//include/sfa/xml/Video.xml
typedef union FineSkinningPiece {
	struct __attribute__((packed)) { u8 _pad_skinDataSrcOffs[0x60]; u32 skinDataSrcOffs; };
	struct __attribute__((packed)) { u8 _pad_weightsSrc[0x64]; u32 weightsSrc; };
	struct __attribute__((packed)) { u8 _pad_bone0[0x6C]; u8 bone0; };
	struct __attribute__((packed)) { u8 _pad_bone1[0x6D]; u8 bone1; };
	struct __attribute__((packed)) { u8 _pad_weightsBlockCount[0x6F]; u8 weightsBlockCount; };
	struct __attribute__((packed)) { u8 _pad_nVtxs[0x70]; u16 nVtxs; };
	struct __attribute__((packed)) { u8 _pad_skinMeOffs[0x72]; u8 skinMeOffs; };
	struct __attribute__((packed)) { u8 _pad_skinSrcBlockCount[0x73]; u8 skinSrcBlockCount; };
} FineSkinningPiece;
