//include/sfa/xml/Dolphin/Debug/TRK.xml
typedef enum { //type:u32
	TrkError_TrkBadMsg = 0x500,
	TrkError_TrkBufferFull = 0x301,
	TrkError_TrkBadParam = 0x703,
	TrkError_TrkCantPostEvent = 0x100,
	TrkError_OK = 0x0,
	TrkError_TrkBadRange = 0x700,
	TrkError_TrkBufferEmpty = 0x302,
	TrkError_TrkNoEvent = 0x1,
} TrkError;
//include/sfa/xml/Dolphin/Debug/TRK.xml
typedef union TrkBuffer {
	char unk00;
	struct __attribute__((packed)) { u8 _pad_writePos[0x8]; uint writePos; };
	struct __attribute__((packed)) { u8 _pad_readPos[0xC]; uint readPos; };
	struct __attribute__((packed)) { u8 _pad_msg[0x10]; char msg; };
} TrkBuffer;
//include/sfa/xml/Dolphin/Debug/TRK.xml
typedef union TrkEvent {
	u8 type;
	struct __attribute__((packed)) { u8 _pad_field_04[0x4]; undefined4 field_04; };
	struct __attribute__((packed)) { u8 _pad_bufIdx[0x8]; int bufIdx; };
} TrkEvent;
//include/sfa/xml/Dolphin/Debug/TRK.xml
typedef union TrkStruct2 {
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined4 unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined4 unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; undefined4 unk0C; };
} TrkStruct2;
