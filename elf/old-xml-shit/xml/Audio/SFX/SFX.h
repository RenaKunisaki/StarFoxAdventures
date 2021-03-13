//include/sfa/xml/Audio/SFX.xml
typedef enum { //type:u16
	SoundId_Warping = 0x467,
	SoundId_FoxFallScream2 = 0x20F,
	SoundId_Tricky_Yawn = 0x354,
	SoundId_FoxFallScream1 = 0x20E,
	SoundId_Zap = 0x41C,
	SoundId_TitleScreenCloseMenu = 0x100,
	SoundId_MenuOpen3EE = 0x3EE,
	SoundId_Blip = 0x405,
	SoundId_FoxClimbUp2 = 0x25,
	SoundId_FoxGrabLedge = 0x29,
	SoundId_TrickyCommandGoGetIt = 0x3FA,
	SoundId_TrickySpewingFire = 0x3DC,
	SoundId_BombSporePickup = 0xA7,
	SoundId_FoxAttack378 = 0x378,
	SoundId_TrickyCommandDecoy = 0x3F7,
	SoundId_RecoverHealth = 0x49,
	SoundId_GlassSmash = 0x47B,
	SoundId_PauseMenuClose = 0x3F2,
	SoundId_Tricky_GetMFox = 0x35B,
	SoundId_FoxFallScream = 0x26,
	SoundId_IceSpell = 0x382,
	SoundId_DootF4 = 0xF4,
	SoundId_Whoosh28B = 0x28B,
	SoundId_PutOutFlame = 0x395,
	SoundId_ZipDown = 0x400,
	SoundId_Drop = 0x287,
	SoundId_ClimbOutOfWater = 0x2F,
	SoundId_Whoosh288 = 0x288,
	SoundId_Tricky_Hello = 0x35E,
	SoundId_Whoosh289 = 0x289,
	SoundId_CMenuEquip = 0xF7,
	SoundId_Tricky_BadGuy = 0x358,
	SoundId_MapZoom = 0x3F0,
	SoundId_Pip3F1 = 0x3F1,
	SoundId_TrickyCommandWhistle = 0x3FB,
	SoundId_Tricky_ImStuffed = 0x364,
	SoundId_Tricky_Sniff = 0x357,
	SoundId_Ding = 0xFB,
	SoundId_TrickyCommandStay = 0x3FC,
	SoundId_Splash42B = 0x42B,
	SoundId_RobotHover = 0xE8,
	SoundId_FoxHurt = 0x24,
	SoundId_KrystalRoll2 = 0x3CE,
	SoundId_KrystalRoll1 = 0x3CD,
	SoundId_StaffTakeOut = 0xC0,
	SoundId_TitleScreenChangePage = 0x37B,
	SoundId_HagabonWoosh = 0x236,
	SoundId_MapChangeMode = 0x3ED,
	SoundId_KrystalJump = 0x2D7,
	SoundId_Clang = 0x6F,
	SoundId_FoxClimbUp = 0x1D,
	SoundId_ElectricCrackle = 0x9E,
	SoundId_TrickyFinishFlame = 0x29D,
	SoundId_ZipUp = 0x3FF,
	SoundId_BombPlantGrow = 0xA1,
	SoundId_FoxJump = 0x2D6,
	SoundId_SpellCastFailed = 0x10A,
	SoundId_Tricky_WhereAreWeGoing = 0x365,
	SoundId_TrickyCommandFind = 0x3F8,
	SoundId_CymbalCrash = 0x28D,
	SoundId_Tricky_Dumdedum = 0x360,
	SoundId_CMenuOpen = 0x408,
	SoundId_SomeKindOfWhoosh = 0x374,
	SoundId_KrystalHurt = 0x1F,
	SoundId_FuelCell = 0x403,
	SoundId_TitleScreenOpenMenu = 0xFF,
	SoundId_SmackC2 = 0xC2,
	SoundId_Drip = 0x285,
	SoundId_SmackC3 = 0xC3,
	SoundId_BombPlantBoom = 0xA3,
	SoundId_Tricky_HiFella = 0x35F,
	SoundId_BallBounce = 0x16C,
	SoundId_Thud17C = 0x17C,
	SoundId_TitleScreenBeep = 0x41A,
	SoundId_Thud17B = 0x17B,
	SoundId_EvilLaugh = 0x470,
	SoundId_WarningBeep = 0x38D,
	SoundId_Tricky_Food = 0x359,
	SoundId_Tricky_Hey = 0x34F,
	SoundId_Tricky_WaitForMe = 0x34E,
	SoundId_FoxLandOof = 0x27,
	SoundId_Trick_Chewing = 0x362,
	SoundId_Tricky_ImHungry = 0x352,
	SoundId_NO_SOUND = 0xFFFF,
	SoundId_FoxThrow = 0x379,
	SoundId_Teleport = 0x4A2,
	SoundId_MagicPickupRinging = 0x56,
	SoundId_TrickyDigging = 0x13D,
	SoundId_EnterFirstPersonNoGoggles = 0x3F3,
	SoundId_Tricky_LookAtThis = 0x351,
	SoundId_SplashBA = 0xBA,
	SoundId_Tricky_Cool = 0x356,
	SoundId_StaffPutAway = 0xC1,
	SoundId_CannonFire = 0x1FD,
	SoundId_PauseMenuOpen = 0x3E5,
	SoundId_Tricky_GetOff = 0x350,
	SoundId_Tricky_LetsPlay = 0x355,
	SoundId_Tricky_Laugh = 0x361,
	SoundId_KrystalFallScream = 0x2D0,
	SoundId_MenuOpen = 0x98,
	SoundId_RobotTalk = 0xE7,
	SoundId_Tricky_Yeah = 0x35C,
	SoundId_MenuSlide = 0x401,
	SoundId_FireyExplosion = 0x6E,
	SoundId_Tricky_TheresSomethingNear = 0x35A,
	SoundId_TrickyCommandFlame = 0x3F9,
	SoundId_TextPip = 0x397,
	SoundId_MenuClose3EF = 0x3EF,
	SoundId_TitleScreenSelect = 0x418,
	SoundId_EnterViewFinder = 0x3F5,
	SoundId_CMenuClose = 0x37C,
	SoundId_KrystalLandOof = 0x399,
	SoundId_Tricky_ImNotDoingIt = 0x35D,
	SoundId_Woosh471 = 0x471,
	SoundId_Tricky_MmmmTasty = 0x363,
	SoundId_Woosh472 = 0x472,
	SoundId_HagabonWhir = 0x3E8,
	SoundId_KrystalThrow = 0x327,
	SoundId_TickTickTick = 0x28C,
	SoundId_StaticyRadarBeep = 0x3D8,
	SoundId_RobotActivate = 0x3F4,
	SoundId_FoxPant = 0x452,
	SoundId_CameraTurnBehindPlayer = 0x286,
	SoundId_RollInWater = 0x427,
	SoundId_MenuOpenF5 = 0xF5,
	SoundId_Tricky_WaitUpFox = 0x34D,
	SoundId_Tricky_Yawn2 = 0x353,
	SoundId_TitleScreenZip = 0xFC,
	SoundId_GenericItemPickup = 0x58,
	SoundId_KrystalGrabLedge = 0x2CB,
	SoundId_Squak = 0xFD,
	SoundId_FoxRoll = 0x2E,
	SoundId_Burning = 0x394,
	SoundId_WarpIn = 0x420,
	SoundId_KrystalClimbUp = 0x398,
	SoundId_ZipUp41B = 0x41B,
	SoundId_PipF3 = 0xF3,
	SoundId_Chime = 0x402,
	SoundId_ElectronicChime3EB = 0x3EB,
	SoundId_RobotZap = 0xE9,
	SoundId_TitleScreenCancel = 0x419,
	SoundId_ElectronicChime3EC = 0x3EC,
	SoundId_ToggleDisguise = 0x69,
} SoundId;
//include/sfa/xml/Audio/SFX.xml
typedef enum { //type:u16
	SoundId2_KrystalRoll2 = 0x265,
	SoundId2_Stay = 0x261,
} SoundId2;
//include/sfa/xml/Audio/SFX.xml
typedef union SfxBinEntry {
	SoundId id;
	struct __attribute__((packed)) { u8 _pad_baseVolume[0x2]; u8 baseVolume; };
	struct __attribute__((packed)) { u8 _pad_volumeRand[0x3]; u8 volumeRand; };
	struct __attribute__((packed)) { u8 _pad_basePan[0x4]; u8 basePan; };
	struct __attribute__((packed)) { u8 _pad_panRand[0x5]; u8 panRand; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; u16 unk06; };
	struct __attribute__((packed)) { u8 _pad_range[0x8]; u16 range; };
	struct __attribute__((packed)) { u8 _pad_fxIds[0xA]; u16 fxIds; };
	struct __attribute__((packed)) { u8 _pad_fxChance[0x16]; u8 fxChance; };
	struct __attribute__((packed)) { u8 _pad_randMax[0x1C]; ushort randMax; };
	struct __attribute__((packed)) { u8 _pad_unk1E[0x1E]; u8 unk1E; };
	struct __attribute__((packed)) { u8 _pad_numIdxs[0x1F]; u8 numIdxs; };
	struct __attribute__((packed)) { u8 _pad_prevIdx[0x1F]; u8 prevIdx; };
} SfxBinEntry;
//include/sfa/xml/Audio/SFX.xml
typedef union SoundEffect {
	ushort id;
	struct __attribute__((packed)) { u8 _pad_unk02[0x2]; ushort unk02; };
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; byte unk04; };
	struct __attribute__((packed)) { u8 _pad_unk05[0x5]; byte unk05; };
	struct __attribute__((packed)) { u8 _pad_unk06[0x6]; byte unk06; };
	struct __attribute__((packed)) { u8 _pad_unk07[0x7]; byte unk07; };
	struct __attribute__((packed)) { u8 _pad_idx[0x8]; u32 idx; };
	struct __attribute__((packed)) { u8 _pad_offset[0x9]; u32 offset; };
	struct __attribute__((packed)) { u8 _pad_rate[0xC]; u16 rate; };
	struct __attribute__((packed)) { u8 _pad_pitch[0xE]; u16 pitch; };
	struct __attribute__((packed)) { u8 _pad_length[0x10]; int length; };
	struct __attribute__((packed)) { u8 _pad_repeatStart[0x14]; u32 repeatStart; };
	struct __attribute__((packed)) { u8 _pad_repeatEnd[0x18]; u32 repeatEnd; };
	struct __attribute__((packed)) { u8 _pad_variation[0x1C]; u32 variation; };
} SoundEffect;
