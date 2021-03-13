//include/sfa/xml/Game/GameState.xml
typedef enum { //type:u8
	GameStateEnum_GAME_STATE_INIT = 0x0,
	GameStateEnum_GAME_STATE_RESETNOW = 0x4,
	GameStateEnum_GAME_STATE_RUN = 0x1,
	GameStateEnum_GAME_STATE_RESET_DEFAULT = 0x6,
	GameStateEnum_GAME_STATE_RESETTING = 0x5,
	GameStateEnum_GAME_STATE_ABOUT_TO_RESET = 0x3,
	GameStateEnum_GAME_STATE_RESET = 0x2,
} GameStateEnum;
