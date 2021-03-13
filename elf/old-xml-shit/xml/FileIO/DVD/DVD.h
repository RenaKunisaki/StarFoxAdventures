//include/sfa/xml/FileIO/DVD.xml
typedef union discErrorMessage {
	u16 x;
	struct __attribute__((packed)) { u8 _pad_y[0x2]; u16 y; };
	struct __attribute__((packed)) { u8 _pad_unk[0x4]; u32 unk; };
	struct __attribute__((packed)) { u8 _pad_msg[0x8]; char* msg; };
} discErrorMessage;
//include/sfa/xml/FileIO/DVD.xml
typedef union DvdBiData {
	struct __attribute__((packed)) { u8 _pad_padSpec[0x4]; undefined4 padSpec; };
	struct __attribute__((packed)) { u8 _pad_pArgs[0x8]; void* pArgs; };
	struct __attribute__((packed)) { u8 _pad_debug[0xC]; int debug; };
} DvdBiData;
