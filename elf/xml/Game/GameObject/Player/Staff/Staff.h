//include/sfa/xml/Game/GameObject/Player/Staff.xml
typedef enum { //type:u8
	StaffGlowColor_Green2 = 0x2,
	StaffGlowColor_White = 0x7,
	StaffGlowColor_Blue = 0x1,
	StaffGlowColor_Yellow2 = 0x3,
	StaffGlowColor_LightBlue = 0x8,
	StaffGlowColor_Orange2 = 0x5,
	StaffGlowColor_Blue2 = 0x6,
	StaffGlowColor_Orange = 0x4,
	StaffGlowColor_Green = 0x0,
} StaffGlowColor;
//include/sfa/xml/Game/GameObject/Player/Staff.xml
typedef union StaffAnimStruct1 {
	void* swipes;
	struct __attribute__((packed)) { u8 _pad_firstSwipe[0xC]; short firstSwipe; };
	struct __attribute__((packed)) { u8 _pad_numSwipes[0xE]; short numSwipes; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; short unk10; };
	struct __attribute__((packed)) { u8 _pad_flags_0x14[0x14]; u8 flags_0x14; };
} StaffAnimStruct1;
//include/sfa/xml/Game/GameObject/Player/Staff.xml
typedef union StaffSwipe {
	vec3f pos;
	struct __attribute__((packed)) { u8 _pad_alpha[0x10]; undefined2 alpha; };
} StaffSwipe;
