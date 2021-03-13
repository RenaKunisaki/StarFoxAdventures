//include/sfa/xml/Game/GameText.xml
typedef enum { //type:u8
	GameLanguageEnum_ENGLISH = 0x0,
	GameLanguageEnum_ITALIAN = 0x3,
	GameLanguageEnum_SPANISH = 0x5,
	GameLanguageEnum_JAPANESE = 0x4,
	GameLanguageEnum_GERMAN = 0x2,
	GameLanguageEnum_FRENCH = 0x1,
} GameLanguageEnum;
//include/sfa/xml/Game/GameText.xml
typedef enum { //type:u32
	GameTextChar_RightJustify = 0xF8F9,
	GameTextChar_SetScale = 0xF8F4,
	GameTextChar_Center = 0xF8FA,
	GameTextChar_LeftJustify = 0xF8F8,
	GameTextChar_SetFont = 0xF8F7,
	GameTextChar_FullJustify = 0xF8FB,
	GameTextChar_SetColor = 0xF8FF,
} GameTextChar;
//include/sfa/xml/Game/GameText.xml
typedef enum { //type:u8
	GameTextJustify_Center = 0x2,
	GameTextJustify_Left = 0x0,
	GameTextJustify_Right = 0x1,
	GameTextJustify_Full = 0x3,
} GameTextJustify;
//include/sfa/xml/Game/GameText.xml
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
typedef void (*gameTextDrawFunc)(s16 param1, s16 param2, s16 param3, s16 param4, double param5, double param6, double param7, double param8);
//include/sfa/xml/Game/GameText.xml
typedef union GameTextBox {
	word unk00;
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; word unk02; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; word unk04; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; word unk06; };
	struct __attribute__((packed)) { u8 _pad_width[0x8]; word width; };
	struct __attribute__((packed)) { u8 _pad_height[0xA]; word height; };
	struct __attribute__((packed)) { u8 _pad_textScale[0xC]; float textScale; };
	struct __attribute__((packed)) { u8 _pad_justify[0x10]; GameTextJustify justify; };
	struct __attribute__((packed)) { u8 _pad_curJustify[0x12]; GameTextJustify curJustify; };
	struct __attribute__((packed)) { u8 _pad_textureId[0x13]; u8 textureId; };
	struct __attribute__((packed)) { u8 _pad_x[0x14]; word x; };
	struct __attribute__((packed)) { u8 _pad_y[0x16]; word y; };
	struct __attribute__((packed)) { u8 _pad_strX[0x18]; short strX; };
	struct __attribute__((packed)) { u8 _pad_strY[0x1A]; short strY; };
	struct __attribute__((packed)) { u8 _pad_flags[0x1C]; byte flags; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x1E]; byte opacity; };
} GameTextBox;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextBoxStruct1 {
	byte unk00;
	struct __attribute__((packed)) { u8 _pad_unk01[0x1]; byte unk01; };
	struct __attribute__((packed)) { u8 _pad_size[0x2]; vec2s size; };
	struct __attribute__((packed)) { u8 _pad_unk0A[0xA]; undefined2 unk0A; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; byte unk0E; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; byte unk0F; };
} GameTextBoxStruct1;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextCharacterStruct {
	u32 character;
	struct __attribute__((packed)) { u8 _pad_xpos[0x4]; u16 xpos; };
	struct __attribute__((packed)) { u8 _pad_ypos[0x6]; u16 ypos; };
	struct __attribute__((packed)) { u8 _pad_left[0x8]; s8 left; };
	struct __attribute__((packed)) { u8 _pad_right[0x9]; s8 right; };
	struct __attribute__((packed)) { u8 _pad_top[0xA]; s8 top; };
	struct __attribute__((packed)) { u8 _pad_bottom[0xB]; s8 bottom; };
	struct __attribute__((packed)) { u8 _pad_width[0xC]; u8 width; };
	struct __attribute__((packed)) { u8 _pad_height[0xD]; u8 height; };
	struct __attribute__((packed)) { u8 _pad_font[0xE]; u8 font; };
	struct __attribute__((packed)) { u8 _pad_texture[0xF]; u8 texture; };
} GameTextCharacterStruct;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextCharset {
	GameTextCharsetStruct1* chars;
	struct __attribute__((packed)) { u8 _pad_texts[0x4]; gametextStruct* texts; };
	struct __attribute__((packed)) { u8 _pad_numChars[0x8]; int numChars; };
	struct __attribute__((packed)) { u8 _pad_numTexts[0xC]; int numTexts; };
	struct __attribute__((packed)) { u8 _pad_textures[0x10]; Texture* textures; };
	struct __attribute__((packed)) { u8 _pad_type[0x1C]; int type; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; GameTextCharsetStruct1* unk20; };
} GameTextCharset;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextCharset2 {
	GameTextCharacterStruct* chars;
	struct __attribute__((packed)) { u8 _pad_texts[0x4]; gametextStruct* texts; };
	struct __attribute__((packed)) { u8 _pad_numChars[0x8]; int numChars; };
	struct __attribute__((packed)) { u8 _pad_numTexts[0xC]; int numTexts; };
	struct __attribute__((packed)) { u8 _pad_texture[0x10]; Texture* texture; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; Texture* unk14; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; pointer unk18; };
	struct __attribute__((packed)) { u8 _pad_gameTextState[0x1C]; undefined4 gameTextState; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; undefined4 unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; undefined1 unk24; };
	struct __attribute__((packed)) { u8 _pad_unk25[0x25]; undefined1 unk25; };
} GameTextCharset2;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextCharsetStruct1 {
	u8 fileIdx;
	struct __attribute__((packed)) { u8 _pad_language[0x1]; u8 language; };
	struct __attribute__((packed)) { u8 _pad_id[0x2]; u8 id; };
	struct __attribute__((packed)) { u8 _pad_unk03[0x3]; u8 unk03; };
	struct __attribute__((packed)) { u8 _pad_fileSize[0x4]; ushort fileSize; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; ushort unk06; };
	struct __attribute__((packed)) { u8 _pad_leftPad[0x8]; char leftPad; };
	struct __attribute__((packed)) { u8 _pad_size9[0x9]; char size9; };
	struct __attribute__((packed)) { u8 _pad_topPad[0xA]; char topPad; };
	struct __attribute__((packed)) { u8 _pad_unk0B[0xB]; char unk0B; };
	struct __attribute__((packed)) { u8 _pad_charW[0xC]; u8 charW; };
	struct __attribute__((packed)) { u8 _pad_charH[0xD]; u8 charH; };
	struct __attribute__((packed)) { u8 _pad_font[0xE]; u8 font; };
	struct __attribute__((packed)) { u8 _pad_textureIdx[0xF]; byte textureIdx; };
} GameTextCharsetStruct1;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextLanguageStruct {
	char* language;
	struct __attribute__((packed)) { u8 _pad_charset[0x4]; u8 charset; };
	struct __attribute__((packed)) { u8 _pad_unk05[0x5]; u8 unk05; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; u8 unk06; };
	struct __attribute__((packed)) { u8 _pad_unk07[0x7]; u8 unk07; };
} GameTextLanguageStruct;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextPhrase {
	undefined4 unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; undefined4 unk04; };
	struct __attribute__((packed)) { u8 _pad_curPhrase[0x8]; int curPhrase; };
	struct __attribute__((packed)) { u8 _pad_isAJustPressed[0xC]; int isAJustPressed; };
	struct __attribute__((packed)) { u8 _pad_nPhrases[0x10]; int nPhrases; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; pointer unk14; };
} GameTextPhrase;
//include/sfa/xml/Game/GameText.xml
typedef union gametextStruct {
	GameTextId identifier;
	struct __attribute__((packed)) { u8 _pad_numPhrases[0x2]; ushort numPhrases; };
	struct __attribute__((packed)) { u8 _pad_window[0x4]; u8 window; };
	struct __attribute__((packed)) { u8 _pad_alignH[0x5]; u8 alignH; };
	struct __attribute__((packed)) { u8 _pad_alignV[0x6]; u8 alignV; };
	struct __attribute__((packed)) { u8 _pad_language[0x7]; GameLanguageEnum language; };
	struct __attribute__((packed)) { u8 _pad_phrases[0x8]; char** phrases; };
} gametextStruct;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextStruct2 {
	char* str;
	struct __attribute__((packed)) { u8 _pad_x[0x4]; int x; };
	struct __attribute__((packed)) { u8 _pad_y[0x8]; int y; };
	struct __attribute__((packed)) { u8 _pad_win[0xC]; int win; };
	struct __attribute__((packed)) { u8 _pad_win10[0x10]; undefined4 win10; };
} GameTextStruct2;
//include/sfa/xml/Game/GameText.xml
typedef union GameTextStruct2_conflict {
	struct __attribute__((packed)) { u8 _pad_gameBit[0x16]; GameBit16 gameBit; };
} GameTextStruct2_conflict;
//include/sfa/xml/Game/GameText.xml
typedef union TaskTextLookup {
	u16 seqNo;
	struct __attribute__((packed)) { u8 _pad_textId[0x2]; GameTextId textId; };
	struct __attribute__((packed)) { u8 _pad_id[0x4]; u16 id; };
} TaskTextLookup;
