//include/sfa/xml/Game/GameObject/Tricky.xml
typedef enum { //type:u8
	MoveStateEnum_MOVESTATE_WALK_END_PATCH = 0x3,
	MoveStateEnum_MOVESTATE_WALK_FREE = 0x1,
	MoveStateEnum_MOVESTATE_WALK_WAIT = 0x0,
	MoveStateEnum_MOVESTATE_WALK_NODES = 0x7,
	MoveStateEnum_MOVESTATE_JUMP_PREP = 0x9,
	MoveStateEnum_MOVESTATE_WALK_TO_NODE = 0x6,
	MoveStateEnum_MOVESTATE_JUMP_RUN_UP = 0x8,
	MoveStateEnum_MOVESTATE_JUMPING = 0xA,
	MoveStateEnum_MOVESTATE_JUMPUP = 0xE,
	MoveStateEnum_MOVESTATE_JUMP_DOWN = 0xC,
	MoveStateEnum_MOVESTATE_WALK_PATCH_EXIT = 0x4,
	MoveStateEnum_MOVESTATE_WALK_START_PATCH = 0x2,
	MoveStateEnum_MOVESTATE_CURVE_SETUP = 0x5,
	MoveStateEnum_MOVESTATE_JUMPDOWN_RUNUP = 0xD,
	MoveStateEnum_MOVESTATE_JUMP_UP_RUN_UP = 0xB,
} MoveStateEnum;
//include/sfa/xml/Game/GameObject/Tricky.xml
typedef enum { //type:u8
	TrickyGrowlAtEnum_GROWLAT_GOTO = 0x0,
	TrickyGrowlAtEnum_GROWLAT_FLAME = 0x3,
	TrickyGrowlAtEnum_GROWLAT_GOTOFLAME = 0x2,
	TrickyGrowlAtEnum_GROWLAT_GROWLING = 0x1,
} TrickyGrowlAtEnum;
//include/sfa/xml/Game/GameObject/Tricky.xml
typedef enum { //type:u32
	TrickyStateFlags54_IsImpressed = 0x80000000,
	TrickyStateFlags54_IsEating = 0x40000000,
	TrickyStateFlags54_SpewingFire = 0x800,
	TrickyStateFlags54_IsStaying = 0x40000,
} TrickyStateFlags54;
//include/sfa/xml/Game/GameObject/Tricky.xml
typedef union TrickyBallAnimState {
	struct __attribute__((packed)) { u8 _pad_unk70[0x70]; vec3f unk70; };
	struct __attribute__((packed)) { u8 _pad_unk1B4[0x1B4]; undefined4 unk1B4; };
	struct __attribute__((packed)) { u8 _pad_unk1BC[0x1BC]; undefined4 unk1BC; };
	struct __attribute__((packed)) { u8 _pad_timeSinceThrown[0x26C]; float timeSinceThrown; };
	struct __attribute__((packed)) { u8 _pad_state[0x274]; byte state; };
	struct __attribute__((packed)) { u8 _pad_unk2B0[0x2B0]; float unk2B0; };
	struct __attribute__((packed)) { u8 _pad_unk2B4[0x2B4]; float unk2B4; };
	struct __attribute__((packed)) { u8 _pad_unk2B8[0x2B8]; float unk2B8; };
	struct __attribute__((packed)) { u8 _pad_unk2C0[0x2C0]; float unk2C0; };
	struct __attribute__((packed)) { u8 _pad_unk2C4[0x2C4]; float unk2C4; };
} TrickyBallAnimState;
//include/sfa/xml/Game/GameObject/Tricky.xml
typedef union MoveStruct {
	struct __attribute__((packed)) { u8 _pad_movementState[0x9]; MoveStateEnum movementState; };
	struct __attribute__((packed)) { u8 _pad_velocity[0x14]; float velocity; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; int unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; float unk30; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; float unk3C; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; float unk48; };
	struct __attribute__((packed)) { u8 _pad_curveField_0x54[0x54]; uint curveField_0x54; };
	struct __attribute__((packed)) { u8 _pad_unk64[0x64]; float unk64; };
	struct __attribute__((packed)) { u8 _pad_unk68[0x68]; float unk68; };
	struct __attribute__((packed)) { u8 _pad_unk6C[0x6C]; float unk6C; };
	struct __attribute__((packed)) { u8 _pad_unk70[0x70]; float unk70; };
	struct __attribute__((packed)) { u8 _pad_unk74[0x74]; float unk74; };
	struct __attribute__((packed)) { u8 _pad_unk78[0x78]; float unk78; };
	struct __attribute__((packed)) { u8 _pad_unk7C[0x7C]; undefined4 unk7C; };
	struct __attribute__((packed)) { u8 _pad_unk80[0x80]; undefined4 unk80; };
	struct __attribute__((packed)) { u8 _pad_unk98[0x98]; short unk98; };
	struct __attribute__((packed)) { u8 _pad_unk9A[0x9A]; undefined2 unk9A; };
	struct __attribute__((packed)) { u8 _pad_unk9C[0x9C]; undefined2 unk9C; };
	struct __attribute__((packed)) { u8 _pad_unk9E[0x9E]; undefined2 unk9E; };
	struct __attribute__((packed)) { u8 _pad_unkA0[0xA0]; float unkA0; };
	struct __attribute__((packed)) { u8 _pad_unkA4[0xA4]; float unkA4; };
	struct __attribute__((packed)) { u8 _pad_unkA8[0xA8]; float unkA8; };
	struct __attribute__((packed)) { u8 _pad_unkAC[0xAC]; float unkAC; };
	struct __attribute__((packed)) { u8 _pad_unkB0[0xB0]; float unkB0; };
	struct __attribute__((packed)) { u8 _pad_unkB4[0xB4]; float unkB4; };
	struct __attribute__((packed)) { u8 _pad_walkGrounp[0xD0]; short walkGrounp; };
	struct __attribute__((packed)) { u8 _pad_unkD2[0xD2]; short unkD2; };
	struct __attribute__((packed)) { u8 _pad_lastPatchPoint[0xD4]; vec3f lastPatchPoint; };
	struct __attribute__((packed)) { u8 _pad_unkE0[0xE0]; float unkE0; };
	struct __attribute__((packed)) { u8 _pad_unkE4[0xE4]; float unkE4; };
	struct __attribute__((packed)) { u8 _pad_unkE8[0xE8]; float unkE8; };
	struct __attribute__((packed)) { u8 _pad_unk418[0x418]; undefined4 unk418; };
	struct __attribute__((packed)) { u8 _pad_unk430[0x430]; int unk430; };
	struct __attribute__((packed)) { u8 _pad_unk4A0[0x4A0]; int unk4A0; };
	struct __attribute__((packed)) { u8 _pad_unk4BC[0x4BC]; int unk4BC; };
	struct __attribute__((packed)) { u8 _pad_unk4C0[0x4C0]; undefined* unk4C0; };
	struct __attribute__((packed)) { u8 _pad_unk4C4[0x4C4]; uint unk4C4; };
	struct __attribute__((packed)) { u8 _pad_unk528[0x528]; int unk528; };
	struct __attribute__((packed)) { u8 _pad_unk52C[0x52C]; int unk52C; };
	struct __attribute__((packed)) { u8 _pad_unk530[0x530]; undefined2 unk530; };
	struct __attribute__((packed)) { u8 _pad_unk532[0x532]; undefined2 unk532; };
	struct __attribute__((packed)) { u8 _pad_unk534[0x534]; undefined2 unk534; };
	struct __attribute__((packed)) { u8 _pad_unk536[0x536]; byte unk536; };
	struct __attribute__((packed)) { u8 _pad_unk7A0[0x7A0]; undefined4 unk7A0; };
} MoveStruct;
//include/sfa/xml/Game/GameObject/Tricky.xml
typedef union TrickyStruct {
	short unk00;
	struct __attribute__((packed)) { u8 _pad_state[0xA]; undefined1 state; };
	struct __attribute__((packed)) { u8 _pad_vec_0x0C[0xC]; vec3f vec_0x0C; };
	struct __attribute__((packed)) { u8 _pad_pos[0x18]; vec3f pos; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; float unk24; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; float unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; short* unk30; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; short unk44; };
	struct __attribute__((packed)) { u8 _pad_unk54[0x54]; int unk54; };
	struct __attribute__((packed)) { u8 _pad_unk80[0x80]; undefined4 unk80; };
	struct __attribute__((packed)) { u8 _pad_unk84[0x84]; undefined4 unk84; };
	struct __attribute__((packed)) { u8 _pad_unk88[0x88]; undefined4 unk88; };
	struct __attribute__((packed)) { u8 _pad_unk8C[0x8C]; float unk8C; };
	struct __attribute__((packed)) { u8 _pad_unk90[0x90]; float unk90; };
	struct __attribute__((packed)) { u8 _pad_unk94[0x94]; float unk94; };
	struct __attribute__((packed)) { u8 _pad_unk98[0x98]; float unk98; };
	struct __attribute__((packed)) { u8 _pad_unkA0[0xA0]; short unkA0; };
	struct __attribute__((packed)) { u8 _pad_unkB8[0xB8]; int unkB8; };
} TrickyStruct;
