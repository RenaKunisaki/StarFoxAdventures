//include/sfa/xml/Video/Light.xml
typedef union Light {
	u8 diffuseRed;
	struct __attribute__((packed)) { u8 _pad_diffuseGreen[0x1]; u8 diffuseGreen; };
	struct __attribute__((packed)) { u8 _pad_diffuseBlue[0x2]; u8 diffuseBlue; };
	struct __attribute__((packed)) { u8 _pad_specularRed[0x4]; u8 specularRed; };
	struct __attribute__((packed)) { u8 _pad_specularGreen[0x5]; u8 specularGreen; };
	struct __attribute__((packed)) { u8 _pad_specularBlue[0x6]; u8 specularBlue; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; ObjLight* unk08; };
	struct __attribute__((packed)) { u8 _pad_mode[0xD]; LightModeEnum mode; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; u8 unk0E; };
} Light;
