//include/sfa/xml/Video/GX.xml
typedef enum { //type:u32
	VtxDescrBit_T1MIDX = 0x2,
	VtxDescrBit_T3MIDX = 0x4,
	VtxDescrBit_T6MIDX = 0x7,
	VtxDescrBit_T4MIDX = 0x5,
	VtxDescrBit_NRM = 0xA,
	VtxDescrBit_TEX6 = 0x13,
	VtxDescrBit_TEX5 = 0x12,
	VtxDescrBit_TEX4 = 0x11,
	VtxDescrBit_PMIDX = 0x0,
	VtxDescrBit_TEX3 = 0x10,
	VtxDescrBit_TEX2 = 0xF,
	VtxDescrBit_TEX1 = 0xE,
	VtxDescrBit_COL1 = 0xC,
	VtxDescrBit_NRM3 = 0x19,
	VtxDescrBit_TEX0 = 0xD,
	VtxDescrBit_COL0 = 0xB,
	VtxDescrBit_T7MIDX = 0x8,
	VtxDescrBit_T0MIDX = 0x1,
	VtxDescrBit_POS = 0x9,
	VtxDescrBit_T2MIDX = 0x3,
	VtxDescrBit_T5MIDX = 0x6,
	VtxDescrBit_TEX7 = 0x14,
} VtxDescrBit;
//include/sfa/xml/Video/GX.xml
typedef enum { //type:u32
	VtxDescrVal_INDEX16 = 0x3,
	VtxDescrVal_INDEX8 = 0x2,
	VtxDescrVal_NONE = 0x0,
	VtxDescrVal_DIRECT = 0x1,
} VtxDescrVal;
//include/sfa/xml/Video/GX.xml
typedef enum { //type:u32
	VtxStateFlags4F4_NeedWriteVtxDescr0 = 0x8,
	VtxStateFlags4F4_TextureFlag01 = 0x1,
	VtxStateFlags4F4_NeedWriteGenMode = 0x4,
	VtxStateFlags4F4_TextureFlag02 = 0x2,
	VtxStateFlags4F4_NeedWriteVtxAttrGroups = 0x10,
} VtxStateFlags4F4;
//include/sfa/xml/Video/GX.xml
typedef union DisplayListPtr {
	void* list;
	struct __attribute__((packed)) { u8 _pad_size[0x4]; ushort size; };
	struct __attribute__((packed)) { u8 _pad_bbox[0x6]; vec3s bbox; };
	struct __attribute__((packed)) { u8 _pad_shaderId[0x13]; byte shaderId; };
	struct __attribute__((packed)) { u8 _pad_specialBitAddr[0x14]; u16 specialBitAddr; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; u16 unk16; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; u32 unk18; };
} DisplayListPtr;
