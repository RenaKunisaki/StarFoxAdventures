//include/sfa/xml/Video/ExpGfx.xml
typedef union ExpGfxObjStruct {
	ObjInstance* obj;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; ObjInstance* unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; ObjInstance* unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; undefined2 unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; undefined2 unk0E; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; undefined2 unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; ObjInstance* unk14; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; ObjInstance* unk18; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; ObjInstance* unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; ObjInstance* unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; ObjInstance* unk24; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; ObjInstance* unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; ObjInstance* unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; ObjInstance* unk30; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; ObjInstance* unk34; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; ObjInstance* unk38; };
	struct __attribute__((packed)) { u8 _pad_scale[0x3C]; float scale; };
	struct __attribute__((packed)) { u8 _pad_texId[0x42]; short texId; };
	struct __attribute__((packed)) { u8 _pad_flags44[0x44]; u32 flags44; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; ObjInstance* unk48; };
	struct __attribute__((packed)) { u8 _pad_unk4C[0x4C]; ObjInstance* unk4C; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; ObjInstance* unk50; };
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; ObjInstance* unk54; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; ushort unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5A[0x5A]; ushort unk5A; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; ushort unk5C; };
	struct __attribute__((packed)) { u8 _pad_unk61[0x61]; byte unk61; };
} ExpGfxObjStruct;
//include/sfa/xml/Video/ExpGfx.xml
typedef union ExpGfxStruct {
	struct __attribute__((packed)) { u8 _pad_unk26[0x26]; undefined2 unk26; };
	struct __attribute__((packed)) { u8 _pad_dx[0x70]; undefined1 dx; };
	struct __attribute__((packed)) { u8 _pad_dy[0x74]; undefined1 dy; };
	struct __attribute__((packed)) { u8 _pad_dz[0x78]; undefined1 dz; };
	struct __attribute__((packed)) { u8 _pad_flags7C[0x7C]; undefined4 flags7C; };
	struct __attribute__((packed)) { u8 _pad_flags80[0x80]; undefined4 flags80; };
	struct __attribute__((packed)) { u8 _pad_scale[0x84]; undefined1 scale; };
	struct __attribute__((packed)) { u8 _pad_scaleS16[0x86]; undefined1 scaleS16; };
	struct __attribute__((packed)) { u8 _pad_invScale[0x88]; undefined1 invScale; };
	struct __attribute__((packed)) { u8 _pad_texIdx[0x8A]; byte texIdx; };
	struct __attribute__((packed)) { u8 _pad_flags8B[0x8B]; byte flags8B; };
} ExpGfxStruct;
//include/sfa/xml/Video/ExpGfx.xml
typedef union ExpGfxStruct2 {
	ExpGfxStruct unk00;
} ExpGfxStruct2;
//include/sfa/xml/Video/ExpGfx.xml
typedef union ExpGfxTexture {
	ObjInstance* obj;
	struct __attribute__((packed)) { u8 _pad_id[0x4]; undefined4 id; };
	struct __attribute__((packed)) { u8 _pad_tex[0x8]; Texture* tex; };
	struct __attribute__((packed)) { u8 _pad_refCnt[0xC]; s16 refCnt; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; undefined2 unk0E; };
} ExpGfxTexture;
