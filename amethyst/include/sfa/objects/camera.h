typedef struct PACKED Camera {
	ObjPos              pos;                  //0x00
	vec3f               prevPos;              //0x18
	vec3f               vel;                  //0x24
	ObjInstance        *pMatrix;              //0x30
    int                 unk34;                //0x34
	float               unk38;                //0x38
    int                 unk3C;                //0x3C
	short              *unk40;                //0x40
	vec3f               prevPos44;            //0x44
	vec3s               prevRot;              //0x50
    u16                 unk56;                //0x56
    u32                 unk58;                //0x58
    u32                 unk5C;                //0x5C
    u32                 unk60;                //0x60
    u32                 unk64;                //0x64
    u32                 unk68;                //0x68
    u32                 unk6C;                //0x6C
    u32                 unk70;                //0x70
	float               unk74;                //0x74
    u32                 unk78;                //0x78
    u32                 unk7C;                //0x7C
    u32                 unk80;                //0x80
    u32                 unk84;                //0x84
    u32                 unk88;                //0x88
    u32                 unk8C;                //0x8C
    u32                 unk90;                //0x90
    u32                 unk94;                //0x94
    u32                 unk98;                //0x98
    u32                 unk9C;                //0x9C
    u16                 unkA0;                //0xA0
	byte                unkA2;                //0xA2
	byte                unkA3;                //0xA3
	ObjInstance        *focus;                //0xA4 obj camera is looking at
	vec3f               pos_0xa8;             //0xA8
	float               fov;                  //0xB4
	vec3f               prevPos2;             //0xB8
	float               dist_0xc4;            //0xC4
	float               unkC8;                //0xC8
	float               unkCC;                //0xCC
	float               unkD0;                //0xD0
	float               unkD4;                //0xD4
	float               dist_0xd8;            //0xD8
	vec3f               focusOverridePos;     //0xDC
    u32                 unkE8;                //0xE8
    u32                 unkEC;                //0xEC
    u32                 unkF0;                //0xF0
	float               zoomTimer;            //0xF4
	float               zoomSpeed;            //0xF8
    u32                 unkFC;                //0xFC
	vec3s               unk100;               //0x100
	vec3s               firstPersonRot;       //0x106
	vec3f               firstPersonPos;       //0x10C
	float               fovFirstPerson;       //0x118
	ObjInstance        *overrideTarget;       //0x11C
	undefined4          unk120;               //0x120
	ObjInstance        *target;               //0x124 obj player is locked onto
	ObjInstance        *prevTarget;           //0x128
	float               unk12C;               //0x12C
	float               unk130;               //0x130
	float               targetHealthFraction; //0x134
	u8                  targetType;           //0x138 ObjInteractType
	byte                interpMode;           //0x139
	byte                unk13A;               //0x13A
	char                letterboxSize;        //0x13B
	byte                unk13C;               //0x13C
	byte                doFocusOverride;      //0x13D
	byte                unk13E;               //0x13E
	byte                rotFlags_0x13f;       //0x13F
	byte                unk140;               //0x140
	byte                flags_0x141;          //0x141
	byte                unk142;               //0x142
	byte                unk143;               //0x143
} Camera;
CASSERT(sizeof(Camera) == 0x144, sizeof_Camera);

typedef struct PACKED CameraStruct57 {
    Mtx44 mtx0;         //0x00
    Mtx44 mtx1;         //0x40
    vec3s rot0;         //0x80
    vec3s rot1;         //0x86
    float targetHeight;	//0x8c offset Y pos to point to player's head, not feet
    float unk90;        //0x90
    float unk94;        //0x94
    float unk98;        //0x98
    float unk9C;        //0x9c
    float unkA0;        //0xa0
    float unkA4;        //0xa4
    float unkA8;        //0xa8
    float unkAC;        //0xac
    float unkB0;        //0xb0
    float unkB4;        //0xb4
    float unkB8;        //0xb8
    float yPosBC;       //0xbc
    s8    unkC0;        //0xc0
    s8    unkC1;        //0xc1
    s8    unkC2;        //0xc2
    s8    unkC3;        //0xc3
    s8    unkC4;        //0xc4
    s8    unkC5;        //0xc5
    s8    unkC6;        //0xc6
    s8    unkC7;        //0xc7
    u8    flagsC8;      //0xc8
    s8    unkC9;        //0xc9
    s8    unkCA;        //0xca
    s8    unkCB;        //0xcb
} CameraStruct57;
CASSERT(sizeof(CameraStruct57) == 0xCC, sizeof_CameraStruct57);
extern CameraStruct57 *cameraMtxVar57;

extern void *CameraParamsViewfinder; //XXX type
extern float firstPersonFovY;
