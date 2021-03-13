//include/sfa/xml/Game/SaveGame.xml
typedef enum { //type:u32
	gplaySavePointFlags_SetFlag22 = 0x2,
	gplaySavePointFlags_ClearFlag22 = 0x4,
	gplaySavePointFlags_UpdateRestartPoint = 0x1,
} gplaySavePointFlags;
//include/sfa/xml/Game/SaveGame.xml
typedef enum { //type:u8
	SaveGameFlags21_SlotMask = 0x70,
	SaveGameFlags21_EraseMe = 0x80,
} SaveGameFlags21;
//include/sfa/xml/Game/SaveGame.xml
typedef enum { //type:u32
	SaveGameStatus_LOADING = 0x8,
	SaveGameStatus_NO_CARD = 0x2,
	SaveGameStatus_NOT_SAME_CARD = 0xB,
	SaveGameStatus_NEED_INIT = 0xD,
	SaveGameStatus_CARD_DAMAGED = 0x4,
	SaveGameStatus_CARD_WRONG_REGION = 0x6,
	SaveGameStatus_NO_DATA = 0x0,
	SaveGameStatus_RETRY = 0x9,
	SaveGameStatus_NOT_MEMCARD = 0x1,
	SaveGameStatus_CORRUPT = 0xA,
	SaveGameStatus_NO_CARD2 = 0x3,
	SaveGameStatus_CARD_NEEDS_FORMAT = 0x5,
	SaveGameStatus_NOT_SAME_CARD2 = 0xC,
	SaveGameStatus_CARD_INCOMPATIBLE = 0x7,
} SaveGameStatus;
//include/sfa/xml/Game/SaveGame.xml
typedef enum { //type:u8
	SoundMode_Headphones = 0x3,
	SoundMode_Surround = 0x1,
	SoundMode_Mono = 0x2,
	SoundMode_Stereo = 0x0,
} SoundMode;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveGameObjectPos {
	u32 id;
	struct __attribute__((packed)) { u8 _pad_pos[0x4]; vec3f pos; };
} SaveGameObjectPos;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveGameEnvState {
	float timeOfDay;
	struct __attribute__((packed)) { u8 _pad_envfxActIdx[0x4]; short envfxActIdx; };
	struct __attribute__((packed)) { u8 _pad_envFxActIdx2[0xE]; short envFxActIdx2; };
	struct __attribute__((packed)) { u8 _pad_flags_0x40[0x40]; SaveGameEnvFxFlags40 flags_0x40; };
} SaveGameEnvState;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveGameSettings {
	struct __attribute__((packed)) { u8 _pad_unk01[0x1]; byte unk01; };
	struct __attribute__((packed)) { u8 _pad_bSubtitlesOn[0x2]; byte bSubtitlesOn; };
	struct __attribute__((packed)) { u8 _pad_unusedHudSetting[0x3]; byte unusedHudSetting; };
	struct __attribute__((packed)) { u8 _pad_unusedCameraSetting[0x4]; byte unusedCameraSetting; };
	struct __attribute__((packed)) { u8 _pad_unk05[0x5]; undefined1 unk05; };
	struct __attribute__((packed)) { u8 _pad_bWidescreen[0x6]; byte bWidescreen; };
	struct __attribute__((packed)) { u8 _pad_unk07[0x7]; undefined1 unk07; };
	struct __attribute__((packed)) { u8 _pad_bRumbleEnabled[0x8]; byte bRumbleEnabled; };
	struct __attribute__((packed)) { u8 _pad_soundMode[0x9]; SoundMode soundMode; };
	struct __attribute__((packed)) { u8 _pad_musicVolume[0xA]; byte musicVolume; };
	struct __attribute__((packed)) { u8 _pad_sfxVolume[0xB]; byte sfxVolume; };
	struct __attribute__((packed)) { u8 _pad_cutsceneVolume[0xC]; byte cutsceneVolume; };
	struct __attribute__((packed)) { u8 _pad_unk0D[0xD]; byte unk0D; };
	struct __attribute__((packed)) { u8 _pad_unk0E[0xE]; short unk0E; };
	struct __attribute__((packed)) { u8 _pad_unlockedCheats[0x10]; u32 unlockedCheats; };
	struct __attribute__((packed)) { u8 _pad_activeCheats[0x14]; u32 activeCheats; };
	struct __attribute__((packed)) { u8 _pad_score[0x1C]; uint score; };
	struct __attribute__((packed)) { u8 _pad_name[0x20]; char name; };
} SaveGameSettings;
//include/sfa/xml/Game/SaveGame.xml
typedef union GameSaveTimeLog {
	u32 objId;
	struct __attribute__((packed)) { u8 _pad_time[0x4]; float time; };
} GameSaveTimeLog;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveFileStruct3 {
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; byte unk14; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; byte unk1C; };
} SaveFileStruct3;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveGame {
	PlayerCharState charState;
	struct __attribute__((packed)) { u8 _pad_trickyEnergy[0x18]; byte trickyEnergy; };
	struct __attribute__((packed)) { u8 _pad_unk19[0x19]; byte unk19; };
	struct __attribute__((packed)) { u8 _pad_saveFileName[0x1C]; char saveFileName; };
	struct __attribute__((packed)) { u8 _pad_character[0x20]; PlayerCharacterEnum character; };
	struct __attribute__((packed)) { u8 _pad_flags21[0x21]; SaveGameFlags21 flags21; };
	struct __attribute__((packed)) { u8 _pad_flag_0x22[0x22]; byte flag_0x22; };
	struct __attribute__((packed)) { u8 _pad_unk23[0x23]; byte unk23; };
	struct __attribute__((packed)) { u8 _pad_gameBits2[0x24]; u8 gameBits2; };
	struct __attribute__((packed)) { u8 _pad_cameraVar[0xC2]; u8 cameraVar; };
	struct __attribute__((packed)) { u8 _pad_staffUpgrades[0x103]; u8 staffUpgrades; };
	struct __attribute__((packed)) { u8 _pad_gameBits0[0x118]; byte gameBits0; };
	struct __attribute__((packed)) { u8 _pad_objs[0x168]; SaveGameObjectPos objs; };
	struct __attribute__((packed)) { u8 _pad_texts[0x558]; u8 texts; };
	struct __attribute__((packed)) { u8 _pad_completion[0x55D]; byte completion; };
	struct __attribute__((packed)) { u8 _pad_numTexts[0x55E]; byte numTexts; };
	struct __attribute__((packed)) { u8 _pad_playTime[0x560]; float playTime; };
	struct __attribute__((packed)) { u8 _pad_gameBits1[0x564]; u8 gameBits1; };
	struct __attribute__((packed)) { u8 _pad_gameBits3[0x5D8]; u8 gameBits3; };
	struct __attribute__((packed)) { u8 _pad_charPos[0x684]; PlayerCharPos charPos; };
	struct __attribute__((packed)) { u8 _pad_unk6A4[0x6A4]; short unk6A4; };
	struct __attribute__((packed)) { u8 _pad_env[0x6A8]; SaveGameEnvState env; };
	struct __attribute__((packed)) { u8 _pad_unk6E9[0x6E9]; byte unk6E9; };
	struct __attribute__((packed)) { u8 _pad_unk6EA[0x6EA]; byte unk6EA; };
	struct __attribute__((packed)) { u8 _pad_unk6EB[0x6EB]; byte unk6EB; };
} SaveGame;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveData {
	SaveGameSettings unk00;
	struct __attribute__((packed)) { u8 _pad_curSaveGame[0xE4]; SaveGame curSaveGame; };
} SaveData;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveGameBufChunk {
	uint unk00;
	struct __attribute__((packed)) { u8 _pad_unk04[0x4]; uint unk04; };
	struct __attribute__((packed)) { u8 _pad_unk08[0x8]; uint unk08; };
	struct __attribute__((packed)) { u8 _pad_unk0C[0xC]; uint unk0C; };
	struct __attribute__((packed)) { u8 _pad_unk10[0x10]; uint unk10; };
	struct __attribute__((packed)) { u8 _pad_unk14[0x14]; uint unk14; };
	struct __attribute__((packed)) { u8 _pad_unk18[0x18]; uint unk18; };
	struct __attribute__((packed)) { u8 _pad_unk1C[0x1C]; uint unk1C; };
	struct __attribute__((packed)) { u8 _pad_unk20[0x20]; uint unk20; };
	struct __attribute__((packed)) { u8 _pad_unk24[0x24]; uint unk24; };
	struct __attribute__((packed)) { u8 _pad_unk28[0x28]; uint unk28; };
	struct __attribute__((packed)) { u8 _pad_unk2C[0x2C]; uint unk2C; };
	struct __attribute__((packed)) { u8 _pad_unk30[0x30]; uint unk30; };
	struct __attribute__((packed)) { u8 _pad_unk34[0x34]; uint unk34; };
	struct __attribute__((packed)) { u8 _pad_unk38[0x38]; uint unk38; };
	struct __attribute__((packed)) { u8 _pad_unk3C[0x3C]; uint unk3C; };
} SaveGameBufChunk;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveGameBuffer {
	SaveGameBufChunk chunk;
	struct __attribute__((packed)) { u8 _pad_unkA40[0xA40]; uint unkA40; };
	struct __attribute__((packed)) { u8 _pad_unkA44[0xA44]; uint unkA44; };
	struct __attribute__((packed)) { u8 _pad_cksum2[0x1FF8]; u32 cksum2; };
	struct __attribute__((packed)) { u8 _pad_cksum1[0x1FFC]; u32 cksum1; };
} SaveGameBuffer;
//include/sfa/xml/Game/SaveGame.xml
typedef union SaveGameInfo {
	char fileName;
	struct __attribute__((packed)) { u8 _pad_percent[0x4]; byte percent; };
	struct __attribute__((packed)) { u8 _pad_nSpirits[0x5]; byte nSpirits; };
	struct __attribute__((packed)) { u8 _pad_nSpellStones[0x6]; byte nSpellStones; };
	struct __attribute__((packed)) { u8 _pad_playTimeSeconds[0x8]; int playTimeSeconds; };
	struct __attribute__((packed)) { u8 _pad_texts[0xC]; gametextStruct* texts; };
	struct __attribute__((packed)) { u8 _pad_flags[0x20]; SaveGameFlags21 flags; };
	struct __attribute__((packed)) { u8 _pad_unk21[0x21]; byte unk21; };
} SaveGameInfo;
