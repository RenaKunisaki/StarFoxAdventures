//include/sfa/xml/Animation/Model/Animation.xml
typedef enum { //type:u8
	AnimState_DoSpawnAnim = 0x2,
} AnimState;
//include/sfa/xml/Animation/Model/Animation.xml
typedef enum { //type:u8
	AnimStruct28Flags_XnotEqY = 0x4,
	AnimStruct28Flags_XcanBeNegative = 0x20,
} AnimStruct28Flags;
//include/sfa/xml/Animation/Model/Animation.xml
typedef union AnimBufStruct {
	struct __attribute__((packed)) { u8 _pad_time[0x4]; float time; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; float unk14; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; byte* unk1C; };
	struct __attribute__((packed)) { u8 _pad_pCompData[0x2C]; byte* pCompData; };
	struct __attribute__((packed)) { u8 _pad_pAnimData[0x34]; byte* pAnimData; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; undefined4 unk38; };
	struct __attribute__((packed)) { u8 _pad_unk44[0x44]; ushort unk44; };
	struct __attribute__((packed)) { u8 _pad_unk4C[0x4C]; ushort unk4C; };
	struct __attribute__((packed)) { u8 _pad_unk58[0x58]; short unk58; };
} AnimBufStruct;
//include/sfa/xml/Animation/Model/Animation.xml
typedef union AnimCurvPoint {
	float y;
	struct __attribute__((packed)) { u8 _pad_flags[0x4]; byte flags; };
	struct __attribute__((packed)) { u8 _pad_attr[0x5]; byte attr; };
	struct __attribute__((packed)) { u8 _pad_x[0x6]; short x; };
} AnimCurvPoint;
//include/sfa/xml/Animation/Model/Animation.xml
typedef union AnimHeader {
	byte length;
} AnimHeader;
//include/sfa/xml/Animation/Model/Animation.xml
typedef union EyeAnimStruct {
	struct __attribute__((packed)) { u8 _pad_state[0x1E]; byte state; };
	struct __attribute__((packed)) { u8 _pad_blinkFrameCount[0x1F]; byte blinkFrameCount; };
	struct __attribute__((packed)) { u8 _pad_eyeMoveTimer[0x20]; byte eyeMoveTimer; };
	struct __attribute__((packed)) { u8 _pad_eyeXrotSpeed[0x22]; short eyeXrotSpeed; };
	struct __attribute__((packed)) { u8 _pad_eyeXrotTarget[0x24]; int eyeXrotTarget; };
	struct __attribute__((packed)) { u8 _pad_unk2B[0x2B]; byte unk2B; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; byte unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk2D[0x2D]; byte unk2D; };
} EyeAnimStruct;
//include/sfa/xml/Animation/Model/Animation.xml
typedef union ModelAnimStruct56 {
	float timeScale0;
	struct __attribute__((packed)) { u8 _pad_timeScale1[0x4]; float timeScale1; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; float unk08; };
	struct __attribute__((packed)) { u8 _pad_start[0xC]; char start; };
	struct __attribute__((packed)) { u8 _pad_end[0xD]; char end; };
	struct __attribute__((packed)) { u8 _pad_flags[0xE]; byte flags; };
} ModelAnimStruct56;
//include/sfa/xml/Animation/Model/Animation.xml
typedef union PathCamStruct {
	undefined4 unk00;
	struct __attribute__((packed)) { u8 _pad_controlPoint1[0x4]; undefined4 controlPoint1; };
	struct __attribute__((packed)) { u8 _pad_controlPoint2[0x8]; undefined4 controlPoint2; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; undefined4 unk0C; };
} PathCamStruct;
//include/sfa/xml/Animation/Model/Animation.xml
typedef union RawAnimData {
	vec3s rotHead;
	struct __attribute__((packed)) { u8 _pad_rotTail[0x6]; vec3s rotTail; };
	struct __attribute__((packed)) { u8 _pad_scaleHead[0xC]; vec3s scaleHead; };
	struct __attribute__((packed)) { u8 _pad_scaleTail[0x12]; vec3s scaleTail; };
	struct __attribute__((packed)) { u8 _pad_xlateHead[0x18]; vec3s xlateHead; };
	struct __attribute__((packed)) { u8 _pad_xlateTail[0x1E]; vec3s xlateTail; };
} RawAnimData;
