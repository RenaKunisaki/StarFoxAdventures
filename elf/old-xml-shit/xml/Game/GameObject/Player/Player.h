//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u32
	ObjSeqCmd06_PlayStream = 0x28,
	ObjSeqCmd06_SetIsSavePoint = 0x23,
	ObjSeqCmd06_SoundCmd0D = 0xD,
	ObjSeqCmd06_EnableMotionBlur = 0x2C,
	ObjSeqCmd06_CamStatic = 0x15,
	ObjSeqCmd06_SetCamSpeed = 0x33,
	ObjSeqCmd06_GotoRestartPoint = 0x20,
	ObjSeqCmd06_DisableMonochromeFilter = 0x2F,
	ObjSeqCmd06_SetFlag7F_1 = 0x9,
	ObjSeqCmd06_DisableLetterbox = 0x1E,
	ObjSeqCmd06_ToggleLetterbox = 0x12,
	ObjSeqCmd06_Earthquake = 0x7,
	ObjSeqCmd06_PlayerEnvfxCmd30 = 0x30,
	ObjSeqCmd06_DisableMotionBlur = 0x2D,
	ObjSeqCmd06_PlayerEnvfxCmd32 = 0x32,
	ObjSeqCmd06_PlayerEnvfxCmd31 = 0x31,
	ObjSeqCmd06_StopAndDisableTimer = 0x25,
	ObjSeqCmd06_CamNormal = 0x1A,
	ObjSeqCmd06_ClrObjGroup = 0x1C,
	ObjSeqCmd06_StartCountupTimer = 0xA,
	ObjSeqCmd06_SwitchModel = 0x17,
	ObjSeqCmd06_StartCountdownTimer = 0xB,
	ObjSeqCmd06_SavePointCmd24 = 0x24,
	ObjSeqCmd06_SetTargetMapAct = 0x1D,
	ObjSeqCmd06_SetFlag6E_0400 = 0x21,
	ObjSeqCmd06_PullOutStaff = 0x18,
	ObjSeqCmd06_SetTimerToCountUp = 0xC,
	ObjSeqCmd06_ClrFlag6E_0400 = 0x22,
	ObjSeqCmd06_EnableMonochromeFilter = 0x2E,
	ObjSeqCmd06_CamTestStrength = 0x14,
	ObjSeqCmd06_SetObjGroup = 0x1B,
	ObjSeqCmd06_FadeFromBlack = 0xF,
	ObjSeqCmd06_FadeToBlack = 0xE,
	ObjSeqCmd06_ClrRestartPoint = 0x1F,
	ObjSeqCmd06_PlayVoice = 0x27,
	ObjSeqCmd06_PlayerLock = 0x26,
	ObjSeqCmd06_PutAwayStaff = 0x19,
	ObjSeqCmd06_End = 0x0,
	ObjSeqCmd06_GetCurve = 0x2,
	ObjSeqCmd06_SetField7D = 0x10,
	ObjSeqCmd06_EnableLetterbox = 0x13,
} ObjSeqCmd06;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u8
	PlayerBuyItemEnum_DumbleDangPod4x = 0x1,
	PlayerBuyItemEnum_FireflyLantern = 0x15,
	PlayerBuyItemEnum_SnowHornArtifact = 0x16,
	PlayerBuyItemEnum_PukPukEgg = 0x2,
	PlayerBuyItemEnum_Viewfinder = 0x1B,
	PlayerBuyItemEnum_Firefly = 0x7,
	PlayerBuyItemEnum_PukPukEgg7x = 0x3,
	PlayerBuyItemEnum_PDA = 0x1A,
	PlayerBuyItemEnum_DumbleDangPod = 0x0,
	PlayerBuyItemEnum_FuelCell = 0x8,
	PlayerBuyItemEnum_BafomdadHolder = 0x17,
	PlayerBuyItemEnum_TrickyFood = 0x6,
	PlayerBuyItemEnum_Bomb = 0x4,
	PlayerBuyItemEnum_MoonSeed = 0x5,
} PlayerBuyItemEnum;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u8
	PlayerCharacterEnum_SharpClaw = 0x2,
	PlayerCharacterEnum_Krystal = 0x0,
	PlayerCharacterEnum_Fox = 0x1,
} PlayerCharacterEnum;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u16
	PlayerCharacterEnum16_SharpClaw = 0x2,
	PlayerCharacterEnum16_Krystal = 0x0,
	PlayerCharacterEnum16_Fox = 0x1,
} PlayerCharacterEnum16;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u32
	PlayerCharacterEnum32_SharpClaw = 0x2,
	PlayerCharacterEnum32_Krystal = 0x0,
	PlayerCharacterEnum32_Fox = 0x1,
} PlayerCharacterEnum32;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u16
	PlayerStateEnum_Activate_35 = 0x35,
	PlayerStateEnum_Push_1D = 0x1D,
	PlayerStateEnum_StartAiming = 0x2A,
	PlayerStateEnum_ClimbLedge = 0xC,
	PlayerStateEnum_ClimbOntoBike = 0x16,
	PlayerStateEnum_StaffBooster = 0x32,
	PlayerStateEnum_LiftRockWithStaff = 0x33,
	PlayerStateEnum_Idle = 0x1,
	PlayerStateEnum_Moving = 0x2,
	PlayerStateEnum_CastFireball = 0x2E,
	PlayerStateEnum_CastLaser = 0x2F,
	PlayerStateEnum_ClimbOntoWall = 0x12,
	PlayerStateEnum_NoChange = 0x0,
	PlayerStateEnum_ClimbWall = 0x13,
	PlayerStateEnum_GrabWall = 0x11,
	PlayerStateEnum_SlideDownLadder = 0x10,
	PlayerStateEnum_Throwing = 0x7,
	PlayerStateEnum_NOP_3E = 0x3E,
	PlayerStateEnum_ClimbOntoLadder = 0xE,
	PlayerStateEnum_SuperQuake = 0x36,
	PlayerStateEnum_Dead = 0x3,
	PlayerStateEnum_Attack = 0x26,
	PlayerStateEnum_StopAiming = 0x2B,
	PlayerStateEnum_ClimbUpFromWall = 0x14,
	PlayerStateEnum_ClimbDownFromWall = 0x15,
	PlayerStateEnum_LiftActivate_08 = 0x8,
	PlayerStateEnum_OnWindLift = 0x41,
	PlayerStateEnum_ClimbLadder = 0xF,
	PlayerStateEnum_TryCastSpell = 0x2D,
	PlayerStateEnum_Aiming = 0x2C,
	PlayerStateEnum_RideBike = 0x18,
} PlayerStateEnum;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u16
	SpellGameBit_GroundQuake2 = 0xC55,
	SpellGameBit_NO_SPELL = 0xFFFF,
	SpellGameBit_Disguise = 0x40,
	SpellGameBit_Fireball = 0x2D,
	SpellGameBit_GroundQuake = 0x107,
	SpellGameBit_Ice = 0x5CE,
	SpellGameBit_RapidFireLaser = 0x958,
	SpellGameBit_Unk5BD = 0x5BD,
} SpellGameBit;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u32
	SpellIdx_IceBlast = 0x4,
	SpellIdx_SharpClawDisguise = 0x1,
	SpellIdx_OpenPortal = 0x3,
	SpellIdx_GroundQuake = 0x9,
	SpellIdx_NUM_SPELLS = 0xC,
	SpellIdx_RapidFireLaser = 0x8,
	SpellIdx_Deleted1D7 = 0x2,
	SpellIdx_StaffBooster = 0x7,
	SpellIdx_Deleted000 = 0xB,
	SpellIdx_Fireball = 0x0,
	SpellIdx_Deleted777 = 0x6,
	SpellIdx_Deleted5FC = 0x5,
	SpellIdx_SuperQuake = 0xA,
} SpellIdx;
//include/sfa/xml/Game/GameObject/Player.xml
typedef enum { //type:u8
	TailState_Limp = 0x0,
	TailState_Flick = 0x2,
	TailState_HalfUp = 0x5,
	TailState_Rolling = 0x4,
	TailState_Normal = 0x1,
	TailState_Wag = 0x3,
} TailState;
typedef int (*PlayerStateFunc)(ObjInstance* param1, ObjState_Player* param2, double param3);
//include/sfa/xml/Game/GameObject/Player.xml
typedef union PlayerAnimStruct67 {
	vec3f unk00;
	struct __attribute__((packed)) { u8 _pad_unk12[0x12]; byte unk12; };
	struct __attribute__((packed)) { u8 _pad_count[0x1B]; byte count; };
} PlayerAnimStruct67;
//include/sfa/xml/Game/GameObject/Player.xml
typedef union PlayerCharPos {
	vec3f pos;
	struct __attribute__((packed)) { u8 _pad_rotX[0xC]; byte rotX; };
	struct __attribute__((packed)) { u8 _pad_mapLayer[0xD]; byte mapLayer; };
	struct __attribute__((packed)) { u8 _pad_mapId[0xE]; MapDirIdx8 mapId; };
} PlayerCharPos;
//include/sfa/xml/Game/GameObject/Player.xml
typedef union PlayerCharState {
	s8 curHealth;
	struct __attribute__((packed)) { u8 _pad_maxHealth[0x1]; s8 maxHealth; };
	struct __attribute__((packed)) { u8 _pad_flags02[0x2]; byte flags02; };
	struct __attribute__((packed)) { u8 _pad_field_03[0x3]; byte field_03; };
	struct __attribute__((packed)) { u8 _pad_curMagic[0x4]; u16 curMagic; };
	struct __attribute__((packed)) { u8 _pad_maxMagic[0x6]; u16 maxMagic; };
	struct __attribute__((packed)) { u8 _pad_money[0x8]; u8 money; };
	struct __attribute__((packed)) { u8 _pad_curBafomDads[0x9]; byte curBafomDads; };
	struct __attribute__((packed)) { u8 _pad_maxBafomDads[0xA]; byte maxBafomDads; };
	struct __attribute__((packed)) { u8 _pad_field_0B[0xB]; byte field_0B; };
} PlayerCharState;
//include/sfa/xml/Game/GameObject/Player.xml
typedef union PlayerPrevPos {
	vec3f pos;
	struct __attribute__((packed)) { u8 _pad_time[0xC]; int time; };
} PlayerPrevPos;
