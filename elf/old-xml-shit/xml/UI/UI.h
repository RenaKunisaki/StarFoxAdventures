//include/sfa/xml/UI.xml
typedef enum { //type:u32
	UiDllIdx_None0A = 0xA,
	UiDllIdx_None0B = 0xB,
	UiDllIdx_None0D = 0xD,
	UiDllIdx_DLL_32 = 0x2,
	UiDllIdx_EnterSaveName = 0x6,
	UiDllIdx_OptionsMenu = 0x7,
	UiDllIdx_None11 = 0x11,
	UiDllIdx_DLL_3F = 0xE,
	UiDllIdx_Credits = 0xF,
	UiDllIdx_Dummy3A = 0xC,
	UiDllIdx_n_rareware = 0x3,
	UiDllIdx_TitleMenu = 0x4,
	UiDllIdx_SaveSelect = 0x5,
	UiDllIdx_NO_DLL = 0xFFFFFFFF,
	UiDllIdx_DLL_16 = 0x1,
	UiDllIdx_DLL_38 = 0x8,
	UiDllIdx_None = 0x0,
	UiDllIdx_DLL_39 = 0x9,
	UiDllIdx_WarpStone = 0x10,
} UiDllIdx;
//include/sfa/xml/UI.xml
typedef union AButtonInteraction {
	short unk00;
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; short unk02; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; short unk04; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; byte unk10; };
} AButtonInteraction;
