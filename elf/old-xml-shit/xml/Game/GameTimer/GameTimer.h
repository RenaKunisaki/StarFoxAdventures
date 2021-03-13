//include/sfa/xml/Game/GameTimer.xml
typedef enum { //type:u8
	GameTimerFlags_Running = 0x4,
	GameTimerFlags_Visible = 0x10,
	GameTimerFlags_Expired = 0x8,
	GameTimerFlags_Disabled = 0x2,
	GameTimerFlags_CountDown = 0x1,
} GameTimerFlags;
