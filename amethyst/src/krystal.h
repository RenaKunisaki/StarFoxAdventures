#define KRYSTAL_MODEL_ID 0x4E8
#define KRYSTAL_TEXTURE_ID 0x724 //first texture
#define KRYSTAL_NUM_TEXTURES 7

typedef enum {
    PLAYER_ID_KRYSTAL = 0,
    PLAYER_ID_FOX,
    PLAYER_ID_AUTO,
    NUM_PLAYER_IDS,
} OverridePlayerId;

typedef enum {
    BACKPACK_NORMAL = 0,
    BACKPACK_OFF,
    BACKPACK_ON,
    NUM_BACKPACK_MODES,
} BackpackMode;

//krystal.c
extern s8 krystalModelNo;
extern s8 overridePlayerNo;
extern s8 backpackMode;
void krystalInit();
void krystalMainLoop();
