//include/sfa/xml/Game/Arwing.xml
typedef union WorldMapStruct {
	byte idx;
	struct __attribute__((packed)) { u8 _pad_bit[0x2]; GameBit16 bit; };
} WorldMapStruct;
