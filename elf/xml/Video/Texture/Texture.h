//include/sfa/xml/Video/Texture.xml
typedef enum { //type:u32
	HudTextureId_BoxTopBottom = 0xD,
	HudTextureId_TrickyFoodMeterEmpty = 0x56,
	HudTextureId_TrickyFace = 0x55,
	HudTextureId_BoxSide = 0xB,
	HudTextureId_TrickyFoodMeterFull = 0x57,
	HudTextureId_BoxInterior = 0xC,
	HudTextureId_BoxCorner = 0xA,
} HudTextureId;
//include/sfa/xml/Video/Texture.xml
typedef enum { //type:u32
	TextureFileId_DebugFontLowercase = 0x2,
	TextureFileId_AirMeterFull = 0x5D3,
	TextureFileId_DebugFontDigits = 0x25D,
	TextureFileId_AirMeterIconFox = 0x603,
	TextureFileId_DebugFontUppercase = 0x1,
	TextureFileId_WaterFx0056 = 0x56,
	TextureFileId_WaterFx0C2A = 0xC2A,
	TextureFileId_AirMeterEmpty = 0x5D2,
	TextureFileId_WaterFx0C2C = 0xC2C,
	TextureFileId_AirMeterRightSide = 0x5D4,
	TextureFileId_WaterFx0C2D = 0xC2D,
} TextureFileId;
//include/sfa/xml/Video/Texture.xml
typedef enum { //type:u8
	TextureFormat_BC1 = 0xE,
	TextureFormat_C4 = 0x8,
	TextureFormat_IA8 = 0x3,
	TextureFormat_RGB565 = 0x4,
	TextureFormat_C8 = 0x9,
	TextureFormat_C14X2 = 0xA,
	TextureFormat_RGBA32 = 0x6,
	TextureFormat_I4 = 0x0,
	TextureFormat_IA4 = 0x2,
	TextureFormat_RGB5A3 = 0x5,
	TextureFormat_I8 = 0x1,
} TextureFormat;
//include/sfa/xml/Video/Texture.xml
typedef union TextureGpuStruct {
	dword SetMode0;
	struct __attribute__((packed)) { u8 _pad_SetMode1[0x4]; dword SetMode1; };
	struct __attribute__((packed)) { u8 _pad_SetImage0[0x8]; dword SetImage0; };
	struct __attribute__((packed)) { u8 _pad_SetImage3[0xC]; dword SetImage3; };
	struct __attribute__((packed)) { u8 _pad_texture[0x10]; TextureHeader* texture; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; dword unk18; };
	struct __attribute__((packed)) { u8 _pad_blockSize[0x1C]; ushort blockSize; };
	struct __attribute__((packed)) { u8 _pad_pixelFmt[0x1E]; byte pixelFmt; };
	struct __attribute__((packed)) { u8 _pad_flags[0x1F]; byte flags; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; TextureGpuStruct2* unk20; };
	struct __attribute__((packed)) { u8 _pad_dataSize[0x24]; uint dataSize; };
} TextureGpuStruct;
//include/sfa/xml/Video/Texture.xml
typedef union LoadedTexture {
	int id;
	struct __attribute__((packed)) { u8 _pad_texture[0x4]; Texture* texture; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; byte unk08; };
	struct __attribute__((packed)) { u8 _pad_heapSize[0xC]; undefined4 heapSize; };
} LoadedTexture;
//include/sfa/xml/Video/Texture.xml
typedef union Texture {
	Texture* next;
	struct __attribute__((packed)) { u8 _pad_flags[0x4]; uint flags; };
	struct __attribute__((packed)) { u8 _pad_xOffset[0x8]; short xOffset; };
	struct __attribute__((packed)) { u8 _pad_width[0xA]; ushort width; };
	struct __attribute__((packed)) { u8 _pad_height[0xC]; ushort height; };
	struct __attribute__((packed)) { u8 _pad_refCount[0xE]; short refCount; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; short unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; ushort unk14; };
	struct __attribute__((packed)) { u8 _pad_format[0x16]; TextureFormat format; };
	struct __attribute__((packed)) { u8 _pad_unk17[0x17]; Color4b unk17; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; byte unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk1D[0x1D]; byte unk1D; };
	struct __attribute__((packed)) { u8 _pad_unk1F[0x1F]; byte unk1F; };
	struct __attribute__((packed)) { u8 _pad_gpu[0x20]; TextureGpuStruct gpu; };
	struct __attribute__((packed)) { u8 _pad_unk49[0x49]; byte unk49; };
	struct __attribute__((packed)) { u8 _pad_size[0x4C]; uint size; };
	struct __attribute__((packed)) { u8 _pad_unk50[0x50]; dword unk50; };
} Texture;
//include/sfa/xml/Video/Texture.xml
typedef union TextureGpuStruct2 {
	uint SetImage1;
	struct __attribute__((packed)) { u8 _pad_SetImage2[0x4]; uint SetImage2; };
} TextureGpuStruct2;
//include/sfa/xml/Video/Texture.xml
typedef union TextureHeader {
	word* unk00;
	struct __attribute__((packed)) { u8 _pad_obj[0x4]; ObjInstance* obj; };
	struct __attribute__((packed)) { u8 _pad_red[0xC]; byte red; };
	struct __attribute__((packed)) { u8 _pad_green[0xE]; byte green; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; dword unk10; };
	struct __attribute__((packed)) { u8 _pad_red2[0x18]; byte red2; };
	struct __attribute__((packed)) { u8 _pad_green2[0x19]; byte green2; };
	struct __attribute__((packed)) { u8 _pad_unk1A[0x1A]; byte unk1A; };
	struct __attribute__((packed)) { u8 _pad_unk1B[0x1B]; byte unk1B; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; undefined* unk1C; };
	struct __attribute__((packed)) { u8 _pad_gpu[0x20]; TextureGpuStruct gpu; };
} TextureHeader;
