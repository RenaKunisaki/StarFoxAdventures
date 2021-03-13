
typedef enum { //type:u16
	ObjInstance_Flags06_DontTrackOldPositions = 0x8,
    ObjInstance_Flags06_DontSave              = 0x2000,
	ObjInstance_Flags06_Invisible             = 0x4000,
} ObjInstance_Flags06;

typedef enum { //type:u8
    ObjInstance_FlagsAF_CanPressAToTalk          = 0x1,
    ObjInstance_FlagsAF_FieldB8TimeCanBeNegative = 0x4,
    ObjInstance_FlagsAF_ModelDisableFlag08       = 0x8,
	ObjInstance_FlagsAF_CantBeUsed               = 0x10,
    ObjInstance_FlagsAF_ModelDisableFlag20       = 0x20,
} ObjInstance_FlagsAF;

typedef enum { //type:u16
    ObjInstance_FlagsB0_WhichParentHitbox     = 0x7,
    ObjInstance_FlagsB0_ScalingFlag08         = 0x8,
    ObjInstance_FlagsB0_CanFree               = 0x10,
    ObjInstance_FlagsB0_FallThruFloor         = 0x20,
    ObjInstance_FlagsB0_IsFreed               = 0x40,
    ObjInstance_FlagsB0_LockAnimsAndControls  = 0x80,
    ObjInstance_FlagsB0_Invisible             = 0x400,
    ObjInstance_FlagsB0_IsRendered            = 0x800,
    ObjInstance_FlagsB0_SeqActive             = 0x1000,
	ObjInstance_FlagsB0_DontMove              = 0x2000,
	ObjInstance_FlagsB0_DontUseRenderCallback = 0x4000,
    ObjInstance_FlagsB0_DontUpdate            = 0x8000,
} ObjInstance_FlagsB0;

typedef enum { //type:u8
    ObjInteractType_Attack    = 0x1,
    ObjInteractType_Talk      = 0x2,
	ObjInteractType_Activate  = 0x3,
    ObjInteractType_UseItem   = 0x4,
    ObjInteractType_Push      = 0x5,
	ObjInteractType_PickUp    = 0x6,
    ObjInteractType_UseTricky = 0x8,
	ObjInteractType_UseStaff  = 0x9,
} ObjInteractType;

typedef enum { //type:u32
	ObjSpawnFlags_KeepLoaded         = 0x1,
	ObjSpawnFlags_DontUseObjIndexBin = 0x2,
	ObjSpawnFlags_DontSave           = 0x4,
} ObjSpawnFlags;

typedef struct PACKED ObjPos {
	vec3s rotation;
	u16   flags; //ObjInstance_Flags06
	float scale;
	vec3f pos;
} ObjPos;

typedef struct PACKED ObjInstance {
	ObjPos              pos;                //0x00
	vec3f               prevPos;            //0x18
	vec3f               vel;                //0x24
	ObjInstance        *pMatrix;            //0x30
	s8                  map;                //0x34 MapId8
	u8                  mtxIdx;             //0x35
	u8                  newOpacity;         //0x36
	u8                  opacity;            //0x37
	ObjInstance        *next;               //0x38
	float               camDistVar3C;       //0x3C
	float               camDistVar40;       //0x40
	u16                 catId;              //0x44 ObjCatId
	u16                 defNo;              //0x46 ObjDefEnum
	short               defNo48;            //0x48
    short               unk4A;              //0x4A
	ObjDef             *objDef;             //0x4C
	ObjectFileStruct   *file;               //0x50
	HitState           *hitstate;           //0x54
	void               *hitboxMtx;          //0x58 HitboxMatrix*
	dword              *unk5C;              //0x5C
	void               *pEventName;         //0x60
	void               *shadow;             //0x64 Shadow*
	void               *dll;                //0x68 ObjectFuncPtrs*
	vec3f              *pVecs;              //0x6C
	Texture            *pTextures;          //0x70
	vec3f              *focusPoints;        //0x74
	void               *unk78;              //0x78
	Model             **models;             //0x7C
	vec3f               oldPos;             //0x80
	vec3f               pos_0x8c;           //0x8C
	float               animTimer;          //0x98
	float               animVal_9c;         //0x9C
	word                animId;             //0xA0
	s16                 animVal_a2;         //0xA2 GameBit16
	float               cullOffset;         //0xA4
	float               cullDistance;       //0xA8
	s8                  mapId;              //0xAC MapId8
	byte                curModel;           //0xAD
	u8                  slot;               //0xAE
	u8                  flags_0xaf;         //0xAF ObjInstance_FlagsAF
	u8                  flags_0xb0;         //0xB0 ObjInstance_FlagsB0
	s16                 objNo;              //0xB2
	short               curSeq;             //0xB4
	void               *state;              //0xB8
	ObjSeqFn           *seqFn;              //0xBC
	ObjInstance        *copyMtxFrom;        //0xC0
	ObjInstance        *parent;             //0xC4
	ObjInstance        *child[3];           //0xC8
	byte*               unkD4;              //0xD4
	word                unkD8;              //0xD8
	ObjSeqMsgQueue     *seqMsgs;            //0xDC
	byte                flags_e3;           //0xE3
	byte                unkE4;              //0xE4
	byte                flags_0xe5;         //0xE5
	short               unkE6;              //0xE6
	byte                hintTextIdx;        //0xE8
	byte                unkEA;              //0xEA
	u8                  nChildren;          //0xEB
	Color4b             colorEC;            //0xEC
	byte                unkF0;              //0xF0
	byte                brightness;         //0xF1
	byte                colorIdx;           //0xF2
	undefined4          curveNoPlus1;       //0xF4
	dword               flags_0xf8;         //0xF8
	vec3f               oldVel;             //0xFC
	void               *cbAfterUpdateBones; //0x108 objField108_func*
} ObjInstance;
