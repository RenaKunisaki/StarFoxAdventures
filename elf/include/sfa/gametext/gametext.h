typedef enum { //type:u8
	GameLanguageEnum_ENGLISH = 0x0,
	GameLanguageEnum_ITALIAN = 0x3,
	GameLanguageEnum_SPANISH = 0x5,
	GameLanguageEnum_JAPANESE = 0x4,
	GameLanguageEnum_GERMAN = 0x2,
	GameLanguageEnum_FRENCH = 0x1,
} GameLanguageEnum;

typedef enum { //type:u32
	GameTextChar_RightJustify = 0xF8F9,
	GameTextChar_SetScale = 0xF8F4,
	GameTextChar_Center = 0xF8FA,
	GameTextChar_LeftJustify = 0xF8F8,
	GameTextChar_SetFont = 0xF8F7,
	GameTextChar_FullJustify = 0xF8FB,
	GameTextChar_SetColor = 0xF8FF,
} GameTextChar;

typedef enum { //type:u8
	GameTextJustify_Center = 0x2,
	GameTextJustify_Left = 0x0,
	GameTextJustify_Right = 0x1,
	GameTextJustify_Full = 0x3,
} GameTextJustify;

typedef enum { //type:u32
	GameTextWindow_BottomCenter15 = 0x15,
	GameTextWindow_WarpStoneLeft = 0x47,
	GameTextWindow_TinyBox68 = 0x68,
	GameTextWindow_BottomCenter = 0xA,
	GameTextWindow_TinyBox67 = 0x67,
	GameTextWindow_TinyBox69 = 0x69,
	GameTextWindow_TinyBox64 = 0x64,
	GameTextWindow_TinyBox63 = 0x63,
	GameTextWindow_TinyBox66 = 0x66,
	GameTextWindow_TinyBox65 = 0x65,
	GameTextWindow_TinyBox60 = 0x60,
	GameTextWindow_TinyBox62 = 0x62,
	GameTextWindow_TinyBox61 = 0x61,
	GameTextWindow_TinyBox6D = 0x6D,
	GameTextWindow_TinyBox6C = 0x6C,
	GameTextWindow_TinyBox6F = 0x6F,
	GameTextWindow_TinyBox6E = 0x6E,
	GameTextWindow_UpperCenterMissingTextures = 0x12,
	GameTextWindow_BigRightCornerBox = 0x82,
	GameTextWindow_TinyBox6B = 0x6B,
	GameTextWindow_TinyBox6A = 0x6A,
	GameTextWindow_LevelTitleCentered = 0x84,
	GameTextWindow_TopLeft = 0x4,
	GameTextWindow_TopCenter0B = 0xB,
	GameTextWindow_WarpStoneMiddle = 0x48,
	GameTextWindow_TopLeftBlueBG = 0x1,
	GameTextWindow_TinyBox71 = 0x71,
	GameTextWindow_TinyBox70 = 0x70,
	GameTextWindow_TinyBox73 = 0x73,
	GameTextWindow_LowerCenterMissingTextures14 = 0x14,
	GameTextWindow_TinyBox72 = 0x72,
	GameTextWindow_WarpStoneUnused2F = 0x2F,
	GameTextWindow_CenterMissingTextures13 = 0x13,
	GameTextWindow_PauseMenuTitle = 0x49,
	GameTextWindow_TopCenter02 = 0x2,
	GameTextWindow_BigCenteredBox = 0x81,
	GameTextWindow_LeftCenter = 0x5,
	GameTextWindow_TopCenter00 = 0x0,
	GameTextWindow_BButton = 0x9,
	GameTextWindow_WarpStoneUnused33 = 0x33,
	GameTextWindow_WarpStoneUnused32 = 0x32,
	GameTextWindow_WarpStoneUnused31 = 0x31,
	GameTextWindow_WarpStoneUnused30 = 0x30,
	GameTextWindow_WarpStoneUnused35 = 0x35,
	GameTextWindow_RightCenter = 0x7,
	GameTextWindow_WarpStoneUnused34 = 0x34,
	GameTextWindow_TopOfPDA = 0x83,
	GameTextWindow_LevelTitleTop = 0x85,
	GameTextWindow_CenterMissingTextures = 0xE,
	GameTextWindow_TopLetBigBox = 0xC,
	GameTextWindow_LowerCenterMissingTextures = 0xF,
	GameTextWindow_WarpStoneRight = 0x46,
	GameTextWindow_TinyBox57 = 0x57,
	GameTextWindow_OnPDA = 0x3F,
	GameTextWindow_TinyBox59 = 0x59,
	GameTextWindow_TinyBox58 = 0x58,
	GameTextWindow_TopRight = 0x6,
	GameTextWindow_AButton = 0x8,
	GameTextWindow_WorldMapMissionBrief = 0x3,
	GameTextWindow_TinyBox5F = 0x5F,
	GameTextWindow_TinyBox5C = 0x5C,
	GameTextWindow_TinyBox5B = 0x5B,
	GameTextWindow_TinyBox5E = 0x5E,
	GameTextWindow_TinyBox5D = 0x5D,
	GameTextWindow_AboveMap = 0xD,
	GameTextWindow_TinyBox5A = 0x5A,
} GameTextWindow;

typedef void (*gameTextDrawFunc)(
    s16 param1,
    s16 param2,
    s16 param3,
    s16 param4,
    double param5,
    double param6,
    double param7,
    double param8
);

typedef struct PACKED GameTextBox {
	word  unk00;      //0x00
	word  unk02;      //0x02
	word  unk04;      //0x04
	word  unk06;      //0x06
	word  width;      //0x08
	word  height;     //0x0A
	float textScale;  //0x0C
	u16   justify;    //0x10 GameTextJustify
	u16   curJustify; //0x12 GameTextJustify
	u8    textureId;  //0x13
	word  x;          //0x14
	word  y;          //0x16
	short strX;       //0x18
	short strY;       //0x1A
	byte  flags;      //0x1C
	byte  opacity;    //0x1E
} GameTextBox;
CASSERT(sizeof(GameTextBox) == 0x1F, sizeof_GameTextBox);

typedef struct PACKED GameTextCharacterStruct {
	u32 character; //0x0 UCS32
	u16 xpos;      //0x4
	u16 ypos;      //0x6
	s8  left;      //0x8
	s8  right;     //0x9
	s8  top;       //0xA
	s8  bottom;    //0xB
	u8  width;     //0xC
	u8  height;    //0xD
	u8  font;      //0xE
	u8  texture;   //0xF
} GameTextCharacterStruct;
CASSERT(sizeof(GameTextCharacterStruct) == 0x10, sizeof_GameTextCharacterStruct);

typedef struct PACKED GameTextPhrase {
	undefined4 unk00;          //0x00
	undefined4 unk04;          //0x04
	int        curPhrase;      //0x08
	int        isAJustPressed; //0x0C
	int        nPhrases;       //0x10
	void      *unk14;          //0x14
} GameTextPhrase;
CASSERT(sizeof(GameTextPhrase) == 0x18, sizeof_GameTextPhrase);

typedef struct PACKED gametextStruct {
	u16    identifier; //0x00 GameTextId
	ushort numPhrases; //0x02
	u8     window;     //0x04
	u8     alignH;     //0x05
	u8     alignV;     //0x06
	u8     language;   //0x07 GameLanguageEnum
	char **phrases;    //0x08
} gametextStruct;
CASSERT(sizeof(gametextStruct) == 0xC, sizeof_gametextStruct);
