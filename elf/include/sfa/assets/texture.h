typedef enum { //type:u32
    HudTextureId_BoxCorner            = 0x0A,
    HudTextureId_BoxSide              = 0x0B,
    HudTextureId_BoxInterior          = 0x0C,
	HudTextureId_BoxTopBottom         = 0x0D,
    HudTextureId_TrickyFace           = 0x55,
	HudTextureId_TrickyFoodMeterEmpty = 0x56,
	HudTextureId_TrickyFoodMeterFull  = 0x57,
} HudTextureId;

typedef enum { //type:u32
    TextureFileId_DebugFontUppercase = 0x1,
	TextureFileId_DebugFontLowercase = 0x2,
    TextureFileId_WaterFx0056        = 0x56,
	TextureFileId_DebugFontDigits    = 0x25D,
	TextureFileId_AirMeterEmpty      = 0x5D2,
    TextureFileId_AirMeterFull       = 0x5D3,
    TextureFileId_AirMeterRightSide  = 0x5D4,
    TextureFileId_AirMeterIconFox    = 0x603,
    TextureFileId_WaterFx0C2A        = 0xC2A,
	TextureFileId_WaterFx0C2C        = 0xC2C,
	TextureFileId_WaterFx0C2D        = 0xC2D,
} TextureFileId;

typedef enum { //type:u8
    TextureFormat_I4     = 0x0,
    TextureFormat_I8     = 0x1,
    TextureFormat_IA4    = 0x2,
    TextureFormat_IA8    = 0x3,
    TextureFormat_RGB565 = 0x4,
    TextureFormat_RGB5A3 = 0x5,
    TextureFormat_RGBA32 = 0x6,
    TextureFormat_C4     = 0x8,
	TextureFormat_C8     = 0x9,
	TextureFormat_C14X2  = 0xA,
    TextureFormat_BC1    = 0xE,
} TextureFormat;

typedef struct PACKED TextureGpuStruct2 {
	uint SetImage1;
	uint SetImage2;
} TextureGpuStruct2;

typedef struct PACKED TextureGpuStruct {
	dword              SetMode0;  //0x00
	dword              SetMode1;  //0x04
	dword              SetImage0; //0x08
	dword              SetImage3; //0x0C
	TextureHeader     *texture;   //0x10
    u32                unk14;     //0x14
	dword              unk18;     //0x18
	ushort             blockSize; //0x1C
	byte               pixelFmt;  //0x1E
	byte               flags;     //0x1F
	TextureGpuStruct2 *unk20;     //0x20
	uint               dataSize;  //0x24
} TextureGpuStruct;

typedef struct PACKED TextureHeader {
	word            *unk00;  //0x00
	ObjInstance     *obj;    //0x04
	byte             red;    //0x0C
	byte             green;  //0x0E
	dword            unk10;  //0x10
	byte             red2;   //0x18
	byte             green2; //0x19
	byte             unk1A;  //0x1A
	byte             unk1B;  //0x1B
	undefined       *unk1C;  //0x1C
	TextureGpuStruct gpu;    //0x20
} TextureHeader;

typedef struct PACKED Texture {
	Texture         *next;     //0x00
	uint             flags;    //0x04
	short            xOffset;  //0x08
	ushort           width;    //0x0A
	ushort           height;   //0x0C
	short            refCount; //0x0E
	short            unk10;    //0x10
	ushort           unk14;    //0x14
	u8               format;   //0x16 TextureFormat
	Color4b          unk17;    //0x17
	byte             unk1C;    //0x1C
	byte             unk1D;    //0x1D
	byte             unk1F;    //0x1F
	TextureGpuStruct gpu;      //0x20
	byte             unk49;    //0x49
	uint             size;     //0x4C
	dword            unk50;    //0x50
} Texture;

typedef struct PACKED LoadedTexture {
	int        id;       //0x0
	Texture   *texture;  //0x4
	byte       unk08;    //0x8
	undefined4 heapSize; //0xC
} LoadedTexture;
