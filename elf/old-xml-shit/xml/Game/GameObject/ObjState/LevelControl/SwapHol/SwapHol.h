//include/sfa/xml/Game/GameObject/ObjState/LevelControl/SwapHol.xml
typedef enum { //type:u8
	SH_LevelControl_BloopState_Lose = 0x5,
	SH_LevelControl_BloopState_Init = 0x1,
	SH_LevelControl_BloopState_Done = 0x7,
	SH_LevelControl_BloopState_Idle = 0x0,
	SH_LevelControl_BloopState_Run = 0x2,
	SH_LevelControl_BloopState_GoToState7 = 0x4,
	SH_LevelControl_BloopState_Reset = 0x6,
	SH_LevelControl_BloopState_Win = 0x3,
} SH_LevelControl_BloopState;
//include/sfa/xml/Game/GameObject/ObjState/LevelControl/SwapHol.xml
typedef enum { //type:u32
	SH_LevelControl_Flags_HaveMoonPassKey = 0x40,
	SH_LevelControl_Flags_BloopEventRunning = 0x2,
} SH_LevelControl_Flags;
//include/sfa/xml/Game/GameObject/ObjState/LevelControl/SwapHol.xml
typedef enum { //type:u8
	SH_LevelControl_Mode_BloopEvent = 0x6,
	SH_LevelControl_Mode_Start = 0x1,
	SH_LevelControl_Mode_BeaconEvent = 0x3,
	SH_LevelControl_Mode_Got4thStone = 0x8,
	SH_LevelControl_Mode_FeedQueen = 0x2,
} SH_LevelControl_Mode;
//include/sfa/xml/Game/GameObject/ObjState/LevelControl/SwapHol.xml
typedef union ObjState_SH_LevelControl {
	SH_LevelControl_Flags flags0;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; byte unk04; };
	struct __attribute__((packed)) { u8 _pad_mode[0x5]; SH_LevelControl_Mode mode; };
	struct __attribute__((packed)) { u8 _pad_bloopEventState[0x6]; SH_LevelControl_BloopState bloopEventState; };
	struct __attribute__((packed)) { u8 _pad_bloopTimer[0x8]; float bloopTimer; };
	struct __attribute__((packed)) { u8 _pad_showLevelNameTimer[0xC]; float showLevelNameTimer; };
	struct __attribute__((packed)) { u8 _pad_song1[0x10]; MusicId16 song1; };
	struct __attribute__((packed)) { u8 _pad_song2[0x12]; MusicId16 song2; };
} ObjState_SH_LevelControl;
