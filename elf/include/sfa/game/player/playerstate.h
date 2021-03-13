typedef enum { //type:u8
    PlayerCharacterEnum_Krystal   = 0x0,
    PlayerCharacterEnum_Fox       = 0x1,
	PlayerCharacterEnum_SharpClaw = 0x2,
} PlayerCharacterEnum;

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

typedef enum { //type:u8
	TailState_Limp = 0x0,
	TailState_Flick = 0x2,
	TailState_HalfUp = 0x5,
	TailState_Rolling = 0x4,
	TailState_Normal = 0x1,
	TailState_Wag = 0x3,
} TailState;

typedef struct PACKED PlayerCharPos {
	vec3f pos;
	byte  rotX;
	byte  mapLayer;
	u8    mapId; //MapDirIdx8
    u8    unk0F; //probably padding
} PlayerCharPos;

typedef struct PACKED PlayerCharState {
	s8   curHealth;    //0x0
	s8   maxHealth;    //0x1
	byte flags02;      //0x2
	byte field_03;     //0x3
	u16  curMagic;     //0x4
	u16  maxMagic;     //0x6
	u8   money;        //0x8
	byte curBafomDads; //0x9
	byte maxBafomDads; //0xA
	byte field_0B;     //0xB
} PlayerCharState;

//the game records this but doesn't use it
typedef struct PACKED PlayerPrevPos {
	vec3f pos;
	int time;
} PlayerPrevPos;
