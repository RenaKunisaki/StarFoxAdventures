//include/sfa/xml/Game/Map/Environment.xml
typedef enum { //type:u8
	SaveGameEnvFxFlags40_TimeOfDayFlag20 = 0x20,
	SaveGameEnvFxFlags40_DrawLights = 0x8,
	SaveGameEnvFxFlags40_Cloudy = 0x1,
} SaveGameEnvFxFlags40;
//include/sfa/xml/Game/Map/Environment.xml
typedef union SkyAmbientColor {
	Color4b ambientColor;
	struct __attribute__((packed)) { u8 _pad_unk49[0x49]; char unk49; };
} SkyAmbientColor;
//include/sfa/xml/Game/Map/Environment.xml
typedef union SkyStruct {
	Texture* unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; Texture* unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; Texture* unk08; };
	struct __attribute__((packed)) { u8 _pad_ambientColors[0x78]; SkyAmbientColor ambientColors; };
	struct __attribute__((packed)) { u8 _pad_unk165[0x165]; char unk165; };
	struct __attribute__((packed)) { u8 _pad_unk209[0x209]; byte unk209; };
	struct __attribute__((packed)) { u8 _pad_timeOfDay[0x20C]; float timeOfDay; };
	struct __attribute__((packed)) { u8 _pad_timeSpeed[0x214]; float timeSpeed; };
	struct __attribute__((packed)) { u8 _pad_unk218[0x218]; int unk218; };
	struct __attribute__((packed)) { u8 _pad_unk23C[0x23C]; float unk23C; };
	struct __attribute__((packed)) { u8 _pad_unk240[0x240]; float unk240; };
	struct __attribute__((packed)) { u8 _pad_unk244[0x244]; float unk244; };
	struct __attribute__((packed)) { u8 _pad_unk248[0x248]; float unk248; };
	struct __attribute__((packed)) { u8 _pad_unk24C[0x24C]; byte unk24C; };
	struct __attribute__((packed)) { u8 _pad_unk24E[0x24E]; byte unk24E; };
} SkyStruct;
//include/sfa/xml/Game/Map/Environment.xml
typedef union SnowStruct {
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; short unk24; };
	struct __attribute__((packed)) { u8 _pad_cloudId[0x26]; ushort cloudId; };
	struct __attribute__((packed)) { u8 _pad_divisor2A[0x2A]; ushort divisor2A; };
	struct __attribute__((packed)) { u8 _pad_divisor2C[0x2C]; ushort divisor2C; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; byte unk58; };
	struct __attribute__((packed)) { u8 _pad_unk5C[0x5C]; char unk5C; };
	struct __attribute__((packed)) { u8 _pad_unk5D[0x5D]; char unk5D; };
} SnowStruct;
//include/sfa/xml/Game/Map/Environment.xml
typedef union SnowStruct2 {
	ObjInstance* player;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; pointer unk04; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk1008[0x1008]; SnowStruct3 unk1008; };
	struct __attribute__((packed)) { u8 _pad_cloudId[0x13F0]; int cloudId; };
	struct __attribute__((packed)) { u8 _pad_unk1430[0x1430]; float unk1430; };
} SnowStruct2;
//include/sfa/xml/Game/Map/Environment.xml
typedef union timestruct_12 {
	struct __attribute__((packed)) { u8 _pad_count_0x33[0x33]; byte count_0x33; };
} timestruct_12;
//include/sfa/xml/Game/Map/Environment.xml
typedef union timestruct_74 {
	struct __attribute__((packed)) { u8 _pad_count_0xe[0xE]; byte count_0xe; };
} timestruct_74;
//include/sfa/xml/Game/Map/Environment.xml
typedef union WaterFxStruct {
	vec3f unk00;
	struct __attribute__((packed)) { u8 _pad_unk1B4[0x1B4]; float unk1B4; };
} WaterFxStruct;
//include/sfa/xml/Game/Map/Environment.xml
typedef union waterfx_struct_51 {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; undefined2 unk16; };
} waterfx_struct_51;
