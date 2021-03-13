//include/sfa/xml/UI/Menu/PauseMenu.xml
typedef enum { //type:u8
	PauseMenuPage_Status = 0x1,
	PauseMenuPage_WorldMap = 0x2,
	PauseMenuPage_Quit = 0x4,
	PauseMenuPage_Save = 0x5,
	PauseMenuPage_Advice = 0x3,
} PauseMenuPage;
//include/sfa/xml/UI/Menu/PauseMenu.xml
typedef enum { //type:u8
	PauseMenuState_Status = 0x5,
	PauseMenuState_NotPaused = 0x0,
	PauseMenuState_CubeMenu = 0x4,
	PauseMenuState_Quitting = 0x6,
	PauseMenuState_Unpause = 0x2,
	PauseMenuState_WorldMap = 0x3,
	PauseMenuState_QuittingGame = 0xA,
	PauseMenuState_Saving = 0x7,
	PauseMenuState_Closing = 0xB,
	PauseMenuState_Dead_AskSave = 0x9,
	PauseMenuState_MainMenu = 0x1,
	PauseMenuState_Dead_AskUseBafomdad = 0x8,
} PauseMenuState;
