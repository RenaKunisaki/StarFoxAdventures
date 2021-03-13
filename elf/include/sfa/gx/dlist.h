typedef struct PACKED DisplayListPtr {
	void  *list;           //0x00
	ushort size;           //0x04
	vec3s  bbox;           //0x06
	byte   shaderId;       //0x13
	u16    specialBitAddr; //0x14
	u16    unk16;          //0x16
	u32    unk18;          //0x18
} DisplayListPtr;
