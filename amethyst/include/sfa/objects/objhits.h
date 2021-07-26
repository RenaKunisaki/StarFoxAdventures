typedef enum { //type:u16
    HitboxFlags60_Disabled      = 0x1,
	HitboxFlags60_NeedPosUpdate = 0x40,
	HitboxFlags60_LockRotY      = 0x800,
    HitboxFlags60_LockRotZ      = 0x1000,
} HitboxFlags60;

typedef enum { //type:u8
	HitboxFlags62_ScaleBySize     = 0x1,
	HitboxFlags62_SizeFlag2       = 0x2,
    HitboxFlags62_DontUpdate      = 0x8,
    HitboxFlags62_UseModelField14 = 0x20,
} HitboxFlags62;

typedef struct PACKED ObjHitsEntry {
	short animId;
} ObjHitsEntry;

typedef struct PACKED HitSpherePos {
	float radius;
	vec3f pos;
} HitSpherePos;

typedef struct PACKED HitSphere {
	u16          bone;   //0x00
	u16          unk02;  //0x02
    HitSpherePos pos;    //0x04
	ushort       unk14;  //0x14
	char         unk16;  //0x16
	char         unk17;  //0x17
} HitSphere;
CASSERT(sizeof(HitSphere) == 0x18, sizeof_HitSphere);

typedef struct PACKED HitState {
	ObjInstance  *curTouchedObject;  //0x00
	short         objHitsSize;       //0x04
	short         maxHitsSize;       //0x06
	ObjHitsEntry *objHits;           //0x08
	float         sizeXY_0C;         //0x0C
	vec3f         pos;               //0x10
	vec3f         prevPos;           //0x1C
	vec3f         size;              //0x28
	float         minSizeY;          //0x34
	float         sizeY;             //0x38
	vec3f         unk3C;             //0x3C
	undefined4    unk48;             //0x48
	dword         unk4C;             //0x4C
	ObjInstance  *lastTouchedObject; //0x50
    u32           unk54;             //0x54
	u16           lowestDist;        //0x58
	short         sizeXY;            //0x5A
	short         sizeX1;            //0x5C
	short         sizeX2;            //0x5E
	u16           flags_0x60;        //0x60 HitboxFlags60
	u16           flags_0x62;        //0x62 HitboxFlags62
	short         sizeZ;             //0x64
	short         sizeZ1;            //0x66
	short         sizeZ2;            //0x68
	byte          unk6A;             //0x6A
	undefined1    unk6B;             //0x6B
	byte          damage;            //0x6C
	byte          unk6D;             //0x6D
	byte          unk6E;             //0x6E
	byte          unk6F;             //0x6F
	byte          unk70;             //0x70
	byte          nHits;             //0x71
	byte          sphereIdxs[3];     //0x72
	byte          recordedHitType[3];//0x75
	byte          recordedDamage[3]; //0x78
    byte          unk7B;             //0x7B probably padding
	ObjInstance  *objs[3];           //0x7C
	float         objX;              //0x88 XXX is this a matrix?
    u32           unk8C;             //0x8C
    u32           unk90;             //0x90
	float         objY;              //0x94
    u32           unk98;             //0x98
    u32           unk9C;             //0x9C
	float         objZ;              //0xA0
    u32           unkA4;             //0xA4
    u32           unkA8;             //0xA8
	u8            typeOfPolyHit;     //0xAC PolygonType
	byte          unkAD;             //0xAD
	byte          disable;           //0xAE
	char          state;             //0xAF
	byte          unkB0;             //0xB0
	byte          unkB1;             //0xB1
	short         unkB2;             //0xB2
	byte          unkB4;             //0xB4
	byte          unkB5;             //0xB5
	u16           flagsB6;           //0xB6 HitboxFlags62
} HitState;
CASSERT(sizeof(HitState) == 0xB8, sizeof_HitState);
