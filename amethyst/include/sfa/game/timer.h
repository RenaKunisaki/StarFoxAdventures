typedef enum {
    GAME_TIMER_FLAG_COUNTDOWN = 0x01,
    GAME_TIMER_FLAG_DISABLED  = 0x02,
    GAME_TIMER_FLAG_RUNNING   = 0x04,
    GAME_TIMER_FLAG_EXPIRED   = 0x08,
    GAME_TIMER_FLAG_VISIBLE   = 0x10,
} GameTimerFlags;

extern float gameTimerValue, gameTimerMax;
