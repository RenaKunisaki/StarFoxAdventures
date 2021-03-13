//include/sfa/xml/UI/Menu/TitleScreen.xml
typedef enum { //type:u8
	TitleMenuItemFlags_Increased = 0x8,
	TitleMenuItemFlags_Zero = 0x0,
	TitleMenuItemFlags_Wrap = 0x2,
	TitleMenuItemFlags_AudioRelated40 = 0x40,
	TitleMenuItemFlags_Changed = 0x10,
	TitleMenuItemFlags_Enabled = 0x1,
	TitleMenuItemFlags_Reduced = 0x4,
	TitleMenuItemFlags_NoAButtonToggle = 0x20,
	TitleMenuItemFlags_AlwaysUsePhrase0 = 0x80,
} TitleMenuItemFlags;
//include/sfa/xml/UI/Menu/TitleScreen.xml
typedef enum { //type:u8
	TitleMenuItemType_NoText = 0x1,
	TitleMenuItemType_WithText = 0x0,
	TitleMenuItemType_WithWindow = 0x2,
} TitleMenuItemType;
//include/sfa/xml/UI/Menu/TitleScreen.xml
typedef enum { //type:u8
	TitleMenuState_Init = 0x0,
	TitleMenuState_GameCantSave = 0x3,
	TitleMenuState_Inactive = 0xFF,
	TitleMenuState_Run = 0x1,
	TitleMenuState_PreviouslyOn = 0x2,
} TitleMenuState;
//include/sfa/xml/UI/Menu/TitleScreen.xml
typedef union TitleCameraPos {
	float y;
	struct __attribute__((packed)) { u8 _pad_z[0x4]; float z; };
	struct __attribute__((packed)) { u8 _pad_rot[0x8]; vec3s rot; };
} TitleCameraPos;
//include/sfa/xml/UI/Menu/TitleScreen.xml
typedef union TitleMenuItem {
	s16 x;
	struct __attribute__((packed)) { u8 _pad_y[0x2]; s16 y; };
	struct __attribute__((packed)) { u8 _pad_flags[0x4]; TitleMenuItemFlags flags; };
	struct __attribute__((packed)) { u8 _pad_type[0x5]; TitleMenuItemType type; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; byte unk06; };
	struct __attribute__((packed)) { u8 _pad_min[0x8]; short min; };
	struct __attribute__((packed)) { u8 _pad_max[0xA]; short max; };
	struct __attribute__((packed)) { u8 _pad_value[0xC]; short value; };
	struct __attribute__((packed)) { u8 _pad_text[0xE]; GameTextId text; };
	struct __attribute__((packed)) { u8 _pad_window[0x10]; GameTextWindow16 window; };
} TitleMenuItem;
//include/sfa/xml/UI/Menu/TitleScreen.xml
typedef union TitleMenuStruct {
	LinkStruct* items;
	struct __attribute__((packed)) { u8 _pad_nItems[0x4]; u8 nItems; };
	struct __attribute__((packed)) { u8 _pad_topText[0x6]; GameTextId topText; };
	struct __attribute__((packed)) { u8 _pad_nItems08[0x8]; u8 nItems08; };
} TitleMenuStruct;
