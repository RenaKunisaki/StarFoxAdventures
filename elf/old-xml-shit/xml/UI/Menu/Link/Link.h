//include/sfa/xml/UI/Menu/Link.xml
typedef enum { //type:u16
	LinkStructFlags_HasTexture = 0x4,
	LinkStructFlags_Colored = 0x80,
	LinkStructFlags_Hidden = 0x4000,
	LinkStructFlags_Bright = 0x800,
	LinkStructFlags_TitleColored = 0x100,
} LinkStructFlags;
//include/sfa/xml/UI/Menu/Link.xml
typedef enum { //type:u32
	LinkUpdateResult_ACCEPT = 0x1,
	LinkUpdateResult_CANCEL = 0x0,
	LinkUpdateResult_NOTHING = 0xFFFFFFFF,
} LinkUpdateResult;
//include/sfa/xml/UI/Menu/Link.xml
typedef union LinkStruct {
	GameTextId title;
	struct __attribute__((packed)) { u8 _pad_window[0x2]; u16 window; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined2 unk04; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; short unk06; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; undefined2 unk08; };
	struct __attribute__((packed)) { u8 _pad_textureX[0xA]; short textureX; };
	struct __attribute__((packed)) { u8 _pad_textureY[0xC]; short textureY; };
	struct __attribute__((packed)) { u8 _pad_texture[0x10]; undefined4 texture; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; ushort unk14; };
	struct __attribute__((packed)) { u8 _pad_flags[0x16]; LinkStructFlags flags; };
	struct __attribute__((packed)) { u8 _pad_nUpLink[0x1A]; char nUpLink; };
	struct __attribute__((packed)) { u8 _pad_nDownLink[0x1B]; char nDownLink; };
	struct __attribute__((packed)) { u8 _pad_nLeftLink[0x1C]; char nLeftLink; };
	struct __attribute__((packed)) { u8 _pad_nRightLink[0x1D]; char nRightLink; };
	struct __attribute__((packed)) { u8 _pad_someOtherLeftRightLink[0x1E]; byte someOtherLeftRightLink; };
	struct __attribute__((packed)) { u8 _pad_textures[0x1F]; u8 textures; };
	struct __attribute__((packed)) { u8 _pad_unk37[0x37]; byte unk37; };
	struct __attribute__((packed)) { u8 _pad_count38[0x38]; byte count38; };
} LinkStruct;
//include/sfa/xml/UI/Menu/Link.xml
typedef union LinkTexture {
	Texture* tex;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined2 unk04; };
	struct __attribute__((packed)) { u8 _pad_width[0x6]; u8 width; };
} LinkTexture;
