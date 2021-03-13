//include/sfa/xml/UI/HUD.xml
typedef enum { //type:u8
	HudText_Warp = 0x1B,
	HudText_Climb_On = 0x14,
	HudText_Scanning = 0x4F,
	HudText_PDA_On_Off = 0x52,
	HudText_Staff = 0xB,
	HudText_World_Map = 0x1E,
	HudText_Use_Key = 0x39,
	HudText_Tricky_Stay = 0x27,
	HudText_Lift = 0x2,
	HudText_Use_Gold_Key = 0x41,
	HudText_Empty4E = 0x4E,
	HudText_Throw_Ball = 0x23,
	HudText_Place_Crystal = 0x32,
	HudText_Blow_Horn = 0x38,
	HudText_Empty4A = 0x4A,
	HudText_Throw = 0x4,
	HudText_Push = 0xF,
	HudText_Exit = 0x19,
	HudText_Super_Quake = 0x5B,
	HudText_Status = 0x1D,
	HudText_Staff_Booster = 0x2D,
	HudText_Plant_MoonSeed = 0x3B,
	HudText_Find_Secret = 0x24,
	HudText_Use_Fuel_Cells = 0x49,
	HudText_Rob = 0x1C,
	HudText_Plant_Bomb_Spore = 0x3F,
	HudText_Place_Block = 0x59,
	HudText_Freeze_Blast = 0x29,
	HudText_Use_SpellStone = 0x37,
	HudText_Ground_Quake = 0x2B,
	HudText_Compass = 0x4D,
	HudText_Use_Power_Key = 0x31,
	HudText_Use_Lantern = 0x3A,
	HudText_Use_Sun_Stone = 0x45,
	HudText_Activate = 0xE,
	HudText_Place_Fire_Weed = 0x47,
	HudText_Unknown33 = 0x33,
	HudText_Use_Gate_Key = 0x57,
	HudText_Use_Silver_Tooth = 0x43,
	HudText_Open_Portal = 0x2C,
	HudText_Save = 0x21,
	HudText_Scanning_For = 0x50,
	HudText_Advice = 0x1F,
	HudText_NO_TEXT = 0x0,
	HudText_Pick_Up = 0x9,
	HudText_Put_Away = 0xC,
	HudText_Give_Scarabs = 0x53,
	HudText_Give_Gift56 = 0x56,
	HudText_Climb_Off = 0x15,
	HudText_Give_Gift54 = 0x54,
	HudText_Unknown2F = 0x2F,
	HudText_Exit_Cannon = 0x17,
	HudText_Tusk_Attack = 0x13,
	HudText_Cancel = 0xA,
	HudText_Collect = 0x8,
	HudText_Use_Silver_Key = 0x42,
	HudText_Decoy_Move = 0x26,
	HudText_Quit = 0x20,
	HudText_Place_Cog = 0x36,
	HudText_Fire_Blaster = 0x28,
	HudText_Blow_Flute = 0x3C,
	HudText_Call_Tricky = 0x22,
	HudText_Information = 0x51,
	HudText_Use_Shackle_Key = 0x34,
	HudText_SharpClaw_Disguise = 0x2A,
	HudText_Use_Prison_Key = 0x30,
	HudText_Feed_To_Tricky = 0x3E,
	HudText_Fuel_Cell = 0x4C,
	HudText_Use_Moon_Stone = 0x46,
	HudText_Slide = 0x1A,
	HudText_No_Map_Data = 0x4B,
	HudText_Release_Spirit = 0x18,
	HudText_Brake = 0x11,
	HudText_Use_Gold_Tooth = 0x44,
	HudText_Drop_Token = 0x5A,
	HudText_Fire_Cannon = 0x16,
	HudText_Give_Gold = 0x55,
	HudText_Use_Flame = 0x25,
	HudText_Buy = 0x12,
	HudText_Place_Fire_Gem = 0x48,
	HudText_Accelerate = 0x10,
	HudText_Apline_Root = 0x35,
	HudText_Attack = 0x1,
	HudText_Talk = 0x7,
	HudText_Use_Item = 0x3D,
	HudText_Jump = 0x3,
	HudText__0123456789 = 0x58,
	HudText_Put_Down = 0x5,
	HudText_Fire = 0x6,
	HudText_Select = 0xD,
	HudText_Shoot_Laser = 0x2E,
	HudText_Feed_To_Queen = 0x40,
} HudText;
//include/sfa/xml/UI/HUD.xml
typedef union AirMeter {
	struct __attribute__((packed)) { u8 _pad_step[0x4]; int step; };
	struct __attribute__((packed)) { u8 _pad_size[0x8]; int size; };
	struct __attribute__((packed)) { u8 _pad_value[0xC]; int value; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; int unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; int unk14; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x18]; byte opacity; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; float unk24; };
	struct __attribute__((packed)) { u8 _pad_iconTextureId[0x2C]; u16 iconTextureId; };
	struct __attribute__((packed)) { u8 _pad_iconTexture[0x30]; Texture* iconTexture; };
	struct __attribute__((packed)) { u8 _pad_rightSideTexture[0x34]; Texture* rightSideTexture; };
	struct __attribute__((packed)) { u8 _pad_fullTexture[0x38]; Texture* fullTexture; };
	struct __attribute__((packed)) { u8 _pad_emptyTexture[0x3C]; Texture* emptyTexture; };
	struct __attribute__((packed)) { u8 _pad_type[0x40]; int type; };
	struct __attribute__((packed)) { u8 _pad_flags[0x44]; byte flags; };
} AirMeter;
//include/sfa/xml/UI/HUD.xml
typedef union HudElementState {
	Texture* pTexture;
	struct __attribute__((packed)) { u8 _pad_time[0x4]; int time; };
	struct __attribute__((packed)) { u8 _pad_opacity[0x8]; float opacity; };
} HudElementState;
