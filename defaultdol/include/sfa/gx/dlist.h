typedef struct PACKED DisplayListPtr {
	void  *list;           //0x00
	ushort size;           //0x04
	vec3s  bbox;           //0x06
    u32    unk0C;          //0x0C
    u8     unk10;          //0x10
    u8     unk11;          //0x11
    u8     unk12;          //0x12
	byte   shaderId;       //0x13
	u16    specialBitAddr; //0x14
	u16    unk16;          //0x16
	u32    unk18;          //0x18
} DisplayListPtr;
CASSERT(sizeof(DisplayListPtr) == 0x1C, sizeof_DisplayListPtr);
