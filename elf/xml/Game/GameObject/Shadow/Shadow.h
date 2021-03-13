//include/sfa/xml/Game/GameObject/Shadow.xml
typedef union Shadow {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_texture[0x4]; pointer texture; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; int unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; int* unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; vec3f unk14; };
	struct __attribute__((packed)) { u8 _pad_pos[0x20]; vec3f pos; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float unk2C; };
	struct __attribute__((packed)) { u8 _pad_flags[0x30]; uint flags; };
	struct __attribute__((packed)) { u8 _pad_opacityChange[0x36]; short opacityChange; };
	struct __attribute__((packed)) { u8 _pad_color[0x38]; Color4b color; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x40]; u8 opacity; };
} Shadow;
//include/sfa/xml/Game/GameObject/Shadow.xml
typedef union ShadowStruct {
	ObjInstance* obj;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_flags[0x8]; undefined1 flags; };
} ShadowStruct;
