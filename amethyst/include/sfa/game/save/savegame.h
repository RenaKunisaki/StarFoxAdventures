typedef struct PACKED {
    float timeOfDay;
    short envfxActIdx[5];
    short envFxActIdx2[3];
    u8    unk14[0x2C];
    u8    flags_0x40; //SaveGameEnvFxFlags40
} SaveGameEnvState;
CASSERT(sizeof(SaveGameEnvState) == 0x41, sizeof_SaveGameEnvState);

typedef struct PACKED {
    u32 id; //ObjUniqueId
    vec3f pos;
} SaveGameObjectPos;

typedef struct PACKED {
    PlayerCharState   charState[2];     //0x00
    u8                trickyEnergy;     //0x18
    u8                maxTrickyEnergy;  //0x19 HUD always shows 5 shrooms
    byte              unk1A;            //0x1a
    byte              unk1B;            //0x1b
    char              saveFileName[4];  //0x1c null terminated
    u8          	  character;        //0x20 PlayerCharEnum
    u8	              flags21;          //0x21 SaveGameFlags21 80=erase me; 60=save slot
    byte              flag_0x22;        //0x22
    byte              unk23;            //0x23
    u8                gameBits2[324];   //0x24
    SaveGameObjectPos objs[63];         //0x168
    u8                texts[5];	        //0x558 hint texts (+0xF4 = text ID)
    byte              completion;	    //0x55d percent = (this / 187) * 100
    byte              numTexts;         //0x55e number of hint texts?
    byte              unk55F;           //0x55f
    float             playTime;	        //0x560 frame count
    u8                gameBits1[116];   //0x564
    u8                gameBits3[172];   //0x5d8
    PlayerCharPos     charPos[2];       //0x684
    short             camVar6A4;        //0x6a4 related to camera actions?
    byte              unk6A6;           //0x6a6
    byte              unk6A7;           //0x6a7
    SaveGameEnvState  env;              //0x6a8
    byte              unk6E9;           //0x6e9
    byte              unk6EA;           //0x6ea unused? set to FF for new game
    byte              unk6EB;           //0x6eb
} SaveGame;
CASSERT(sizeof(SaveGame) == 0x6EC, sizeof_SaveGame);

typedef struct PACKED {
    bool  exists;              //0x0
    u8    unused01;            //0x1
    u8    bSubtitlesOn;        //0x2
    u8    unusedHudSetting;	   //0x3 copied to a never-read variable
    u8    unusedCameraSetting; //0x4 copied to bitmask 0x18 of camera->flags_0x141
    u8    unused05;            //0x5
    u8    bWidescreen;         //0x6
    u8    unused07;            //0x7
    u8    bRumbleEnabled;      //0x8
    u8    soundMode;           //0x9 SoundMode
    s8    musicVolume;         //0xa
    s8    sfxVolume;           //0xb
    s8    cutsceneVolume;      //0xc
    u8    unused0D;            //0xd
    u8    unused0E;            //0xe
    u8    unused0F;            //0xf
    u32   unlockedCheats;      //0x10
    u32   activeCheats;        //0x14
    u32   unk18;               //0x18
} SaveGameSettings;
CASSERT(sizeof(SaveGameSettings) == 0x1C, sizeof_SaveGameSettings);

typedef struct PACKED {
    u32 score;
    char name[4];
} SavedHighScore;

typedef struct {
    SaveGameSettings saveSettings; //0x0
    SavedHighScore score1[5]; //XXX which score is for which level?
    SavedHighScore score2[5]; //and why 5 entries? should only be 4?
    SavedHighScore score3[5];
    SavedHighScore score4[5];
    SavedHighScore score5[5];
    SaveGame curSaveGame; //0xE4
} SaveData;
CASSERT(sizeof(SaveData) == 0x7d0, sizeof_SaveData);
