//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u8
	ASSET_TYPE_CHARACTER = 0x4,
	ASSET_TYPE_DLL = 0x5,
	ASSET_TYPE_DUMMY = 0x6,
	ASSET_TYPE_FILE = 0x0,
	ASSET_TYPE_TEXTURE = 0x3,
	ASSET_TYPE_ANIMATION = 0x7,
	ASSET_TYPE_FILE_WITH_ID = 0x1,
	ASSET_TYPE_FILE_WITH_ID_SIZE = 0x2,
} AssetTypeEnum;
//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u32
	BaddieFlags02DC_MoveOnZAxisWhileFrozen = 0x40,
	BaddieFlags02DC_TargetIsTricky = 0x200000,
} BaddieFlags02DC;
//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u32
	ItemType_MidHealthPickup = 0x2,
	ItemType_GenericItem = 0xFF,
	ItemType_BigHealthPickup = 0x3,
	ItemType_SmallHealthPickup = 0x1,
	ItemType_MoonSeed = 0x6,
} ItemType;
//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u16
	ObjCatId_CampFire = 0x32,
	ObjCatId_StaticCamera = 0x12,
	ObjCatId_collectible = 0x6,
	ObjCatId_TrigBits = 0x1B,
	ObjCatId_MagicCaveTop = 0x7D,
	ObjCatId_SPShopKeeper = 0x6F,
	ObjCatId_ShopItem = 0x70,
	ObjCatId_AppleOnTree = 0x5D,
	ObjCatId_CCriverflow = 0x35,
	ObjCatId_TrigButt = 0x19,
	ObjCatId_SH_thorntail = 0x44,
	ObjCatId_WaterFallSpray = 0x73,
	ObjCatId_CFPrisonUncle = 0x3E,
	ObjCatId_MoonSeedPlantingSpot = 0x68,
	ObjCatId_TrigPln = 0x15,
	ObjCatId_texscroll = 0x21,
	ObjCatId_VisAnimator = 0x4E,
	ObjCatId_ArwingLevelObj = 0x7C,
	ObjCatId_PressureSwitch52 = 0x52,
	ObjCatId_TriggSetp = 0x1A,
	ObjCatId_XYZAnimator = 0x4D,
	ObjCatId_AreaFXEmit = 0x67,
	ObjCatId_ProjectedLight = 0x7B,
	ObjCatId_CmbSrc = 0x7E,
	ObjCatId_Pushable = 0x3A,
	ObjCatId_various0030 = 0x30,
	ObjCatId_AlphaAnimator = 0x49,
	ObjCatId_TrigTime = 0x18,
	ObjCatId_sideload = 0x1E,
	ObjCatId_curve = 0x2C,
	ObjCatId_DFropenode = 0x36,
	ObjCatId_CloudRunner = 0x40,
	ObjCatId_MagicPlant = 0x65,
	ObjCatId_SidekickBall = 0x3C,
	ObjCatId_AnimatedObj = 0x10,
	ObjCatId_fallingRock = 0x28,
	ObjCatId_DigTunnel = 0x42,
	ObjCatId_TrigPnt = 0x14,
	ObjCatId_SeqObject = 0x55,
	ObjCatId_ProjectileSwitch = 0x51,
	ObjCatId_bombPlant = 0x2B,
	ObjCatId_DeathGas = 0x80,
	ObjCatId_fireball = 0x24,
	ObjCatId_ExplodeAnimator = 0x4F,
	ObjCatId_InfoPoint = 0x8,
	ObjCatId_Tricky = 0x2,
	ObjCatId_KT_Rex = 0x6D,
	ObjCatId_torch = 0x1D,
	ObjCatId_StaffActivated = 0x7A,
	ObjCatId_DummyObject = 0x0,
	ObjCatId_IceSmash = 0x50,
	ObjCatId_InfoText = 0x31,
	ObjCatId_Lock = 0x54,
	ObjCatId_WaveAnimator = 0x48,
	ObjCatId_CFPrisonCage = 0x3F,
	ObjCatId_LFXEmitter = 0x60,
	ObjCatId_GroundAnimator = 0x4A,
	ObjCatId_TrigArea = 0x17,
	ObjCatId_DIM2PathGen = 0x47,
	ObjCatId_setuppoint = 0x4,
	ObjCatId_HitAnimator = 0x4B,
	ObjCatId_WalkCamera = 0x13,
	ObjCatId_FogControl = 0x81,
	ObjCatId_Light = 0x79,
	ObjCatId_LevelControl = 0x39,
	ObjCatId_Tree = 0x6A,
	ObjCatId_baddie = 0x1C,
	ObjCatId_Player = 0x1,
	ObjCatId_CurveFish = 0x69,
	ObjCatId_SeqObj2 = 0x56,
	ObjCatId_WallAnimator = 0x4C,
	ObjCatId_DIMLavaBall = 0x45,
	ObjCatId_FXEmit = 0x6B,
	ObjCatId_checkpoint4 = 0x5,
	ObjCatId_mammoth = 0x26,
	ObjCatId_playerWeapon = 0x2D,
	ObjCatId_Area = 0x41,
	ObjCatId_Door53 = 0x53,
	ObjCatId_LevelName = 0x43,
	ObjCatId_EffectBox = 0x9,
	ObjCatId_warp = 0x25,
	ObjCatId_Cannon = 0x71,
	ObjCatId_PressureSwitch = 0x33,
	ObjCatId_sfxPlayer = 0x5C,
	ObjCatId_SPShop = 0x6E,
	ObjCatId_GuardClaw = 0x82,
	ObjCatId_bike = 0x2E,
	ObjCatId_TrigCyl = 0x16,
	ObjCatId_InvHit = 0x27,
	ObjCatId_CFPerch = 0x3D,
	ObjCatId_siderepel = 0x1F,
	ObjCatId_edibleMushroom = 0x29,
	ObjCatId_TrigCrve = 0x3B,
	ObjCatId_various0061 = 0x61,
	ObjCatId_Decorative = 0x7F,
	ObjCatId_DIMSnowBall = 0x46,
	ObjCatId_Explodable = 0x63,
	ObjCatId_IMIcePillar = 0x66,
	ObjCatId_Door38 = 0x38,
	ObjCatId_WM_Column = 0x59,
	ObjCatId_IMMultiSeq = 0x57,
	ObjCatId_ARWGenerator = 0x78,
	ObjCatId_ObjCreator = 0x37,
	ObjCatId_Lightning = 0x83,
	ObjCatId_SkeetlaWall = 0x72,
	ObjCatId_KT_RexFloorSwitch = 0x6C,
	ObjCatId_enemyMushroom = 0x2A,
} ObjCatId;
//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u8
	ObjInteractType_Activate = 0x3,
	ObjInteractType_PickUp = 0x6,
	ObjInteractType_UseStaff = 0x9,
	ObjInteractType_UseTricky = 0x8,
	ObjInteractType_Attack = 0x1,
	ObjInteractType_Talk = 0x2,
	ObjInteractType_UseItem = 0x4,
	ObjInteractType_Push = 0x5,
} ObjInteractType;
//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u32
	ObjSpawnFlags_KeepLoaded = 0x1,
	ObjSpawnFlags_DontUseObjIndexBin = 0x2,
	ObjSpawnFlags_DontSave = 0x4,
} ObjSpawnFlags;
//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u8
	StaffActivatedFlags1D_Enabled = 0x40,
	StaffActivatedFlags1D_IsToggled = 0x20,
	StaffActivatedFlags1D_PowerOn = 0x80,
} StaffActivatedFlags1D;
//include/sfa/xml/Game/GameObject.xml
typedef enum { //type:u8
	WarpStoneState_AreYouReadyToWarp = 0x4,
	WarpStoneState_AreYouReadyToGoKP = 0x3,
	WarpStoneState_ShowMenu = 0x1,
} WarpStoneState;
//include/sfa/xml/Game/GameObject.xml
typedef union AttachPoint {
	vec3f pos;
	struct __attribute__((packed)) { u8 _pad_rot[0xC]; vec3s rot; };
	struct __attribute__((packed)) { u8 _pad_bone[0x12]; byte bone; };
	struct __attribute__((packed)) { u8 _pad_unk13[0x13]; byte unk13; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; byte unk14; };
	struct __attribute__((packed)) { u8 _pad_unk15[0x15]; u8 unk15; };
	struct __attribute__((packed)) { u8 _pad_unk16[0x16]; u8 unk16; };
	struct __attribute__((packed)) { u8 _pad_unk17[0x17]; u8 unk17; };
} AttachPoint;
//include/sfa/xml/Game/GameObject.xml
typedef union ObjAnimState_60 {
	ObjInstance* obj00;
	struct __attribute__((packed)) { u8 _pad_nObjs[0x1C]; byte nObjs; };
	struct __attribute__((packed)) { u8 _pad_unk1E[0x1E]; byte unk1E; };
	struct __attribute__((packed)) { u8 _pad_gameBitId[0x20]; short gameBitId; };
} ObjAnimState_60;
//include/sfa/xml/Game/GameObject.xml
typedef union ObjAnimState61 {
	ObjInstance* unk00;
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; float unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; float unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; byte unk14; };
	struct __attribute__((packed)) { u8 _pad_unk15[0x15]; byte unk15; };
} ObjAnimState61;
//include/sfa/xml/Game/GameObject.xml
typedef union ObjAnimState62 {
	float unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; float unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; char unk0F; };
	struct __attribute__((packed)) { u8 _pad_gameBit_0x10[0x10]; GameBit16 gameBit_0x10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; short unk14; };
	struct __attribute__((packed)) { u8 _pad_unk1D[0x1D]; char unk1D; };
	struct __attribute__((packed)) { u8 _pad_unk1E[0x1E]; char unk1E; };
	struct __attribute__((packed)) { u8 _pad_unk37[0x37]; byte unk37; };
	struct __attribute__((packed)) { u8 _pad_frameCountDown_0x3c[0x3C]; undefined2 frameCountDown_0x3c; };
	struct __attribute__((packed)) { u8 _pad_unk3E[0x3E]; char unk3E; };
	struct __attribute__((packed)) { u8 _pad_animTimer_0x44[0x44]; float animTimer_0x44; };
	struct __attribute__((packed)) { u8 _pad_unk48[0x48]; undefined2 unk48; };
} ObjAnimState62;
//include/sfa/xml/Game/GameObject.xml
typedef union object_struct_16 {
	struct __attribute__((packed)) { u8 _pad_objID[0x44]; undefined2 objID; };
	struct __attribute__((packed)) { u8 _pad_defno[0x46]; undefined2 defno; };
	struct __attribute__((packed)) { u8 _pad_unkDC[0xDC]; uint* unkDC; };
} object_struct_16;
//include/sfa/xml/Game/GameObject.xml
typedef union ObjListItem {
	short count;
	struct __attribute__((packed)) { u8 _pad_length[0x2]; short length; };
	struct __attribute__((packed)) { u8 _pad_obj[0x4]; ObjInstance* obj; };
} ObjListItem;
//include/sfa/xml/Game/GameObject.xml
typedef union ObjModLine {
	short x1;
	struct __attribute__((packed)) { u8 _pad_x2[0x2]; short x2; };
	struct __attribute__((packed)) { u8 _pad_y1[0x4]; short y1; };
	struct __attribute__((packed)) { u8 _pad_y2[0x6]; short y2; };
	struct __attribute__((packed)) { u8 _pad_z1[0x8]; short z1; };
	struct __attribute__((packed)) { u8 _pad_z2[0xA]; short z2; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; undefined1 unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk0D[0xD]; undefined1 unk0D; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; undefined1 unk0E; };
	struct __attribute__((packed)) { u8 _pad_unk0F[0xF]; undefined1 unk0F; };
} ObjModLine;
//include/sfa/xml/Game/GameObject.xml
typedef union ObjState_StaffActivated {
	float distX;
	struct __attribute__((packed)) { u8 _pad_distZ[0x4]; float distZ; };
	struct __attribute__((packed)) { u8 _pad_flags_1d[0x1D]; StaffActivatedFlags1D flags_1d; };
} ObjState_StaffActivated;
//include/sfa/xml/Game/GameObject.xml
typedef union WarpStoneDestination {
	GameBit16 bit;
	struct __attribute__((packed)) { u8 _pad_actNo[0x2]; u8 actNo; };
	struct __attribute__((packed)) { u8 _pad_padding[0x3]; u8 padding; };
} WarpStoneDestination;
//include/sfa/xml/Game/GameObject.xml
typedef union wmSeqpointStruct1 {
	GameBit bit;
	struct __attribute__((packed)) { u8 _pad_objId[0x4]; ObjUniqueId objId; };
} wmSeqpointStruct1;
