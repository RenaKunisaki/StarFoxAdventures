#define MAX_LOADED_OBJECTS 600

extern u16 aButtonIcon;
extern u32 audioPrevIrqFlags;
extern bool bBiggerBlurFilter; //bigger blur filter
extern bool bBlurFilterUseArea; //constrain effect by blurFilterArea
extern bool bEnableBlurFilter; //blur filter
extern BOOL bEnableColorFilter;
extern bool bEnableDistortionFilter; //distortion filter
extern bool bEnableMonochromeFilter; //monochrome
extern bool bEnableMotionBlur; //motion blur
extern bool bEnableSpiritVision; //spirit vision filter
extern bool bEnableViewFinderHud; //viewfinder
extern float bikeMoveScale;
extern vec3f blurFilterArea; //blur filter area
extern OSContext *bsodCtx;
extern u16 bsodErrorCode;
extern int bSubtitlesEnabled;
extern u32 buttonsJustPressed;
extern DLL_ID cameraMode;
extern bool cMenuEnabled;
extern bool cMenuOpen;
extern u8 cMenuState;
extern Color4b colorFilterColor;
extern u8 colorScale;
extern struct {
    int chr;
    int len; //bytes / 2
} controlCharLength[46];
extern PADStatus controllerStates[8]; //two sets of 4, alternating
extern s32 curLanguage;
extern s16 curGameText;
extern u8 curMapAct;
extern int curMapId;
extern s8 curMapLayer;
extern s8 curSaveSlot;
extern s16 curSeqIdArray[25]; //seqId+1
extern u8 curSeqNo;
extern ObjInstance *curSeqObj;
extern void *dataFileBuffers[NUM_FILES];
extern u32 dataFileSize[NUM_FILES];
extern char debugLogBuffer[1000];
extern void *debugFrameBuffer[2];
extern char *debugLogEnd;
extern s16 debugPrintXpos;
extern s16 debugPrintYpos;
extern float distortionFilterAngle1; //distortion filter angle
extern float distortionFilterAngle2; //distortion filter angle 2
extern Color4b distortionFilterColor; //distortion filter color
extern vec3f distortionFilterVector;
extern s16 dllRefCount[NUM_DLLS];
extern bool enableDebugText;
extern bool enableRumble;
extern u32 frameCount;
extern u8 framesThisStep;
extern float fovY;
extern u8 gpuHangTimer;
extern SfaHeap heaps[NUM_HEAPS];
extern int heatEffectIntensity; //0-255
extern float hudHealthBlinkTimer; //default: -30
extern bool hudHidden;
extern Texture *hudTextures[102];
extern int initFrameCount;
extern s16 irqDisableDepth;
extern bool isJapanese;
extern MapDirIdx32 levelLockBuckets[2];
extern s16 loadedFileMapIds[NUM_FILES];
extern ObjInstance **loadedObjects;
extern LoadedTexture *loadedTextures;
extern GameBit16 mapActBitIdx[NUM_MAPS];
extern u8 mapActLut[NUM_MAP_DIRS];
extern GameBit16 mapObjGroupBit[NUM_MAPS];
extern struct {
    int mapX, mapZ, cellX, cellZ;
} mapCoords;
extern const char *mapNames[NUM_MAPS];
extern u16 minimapAlpha;
extern int minimapHeight;
extern GameTextId16 minimapInfoGameText;
extern u16 minimapInfoTextAlpha;
typedef enum { //u8
    MINIMAP_MAP = 0,
    MINIMAP_COMPASS = 1,
    MINIMAP_INFO = 2,
    NUM_MINIMAP_MODES
} MinimapMode;
extern u8 minimapMode; //0=map 1=compass 2=info
extern int minimapWidth;
extern float motionBlurIntensity; //32=high, 256=unnoticeable
extern float msecsThisFrame;
extern int numLoadedObjs;
extern int numLoadedTextures;
extern int objIndexCount;
extern u32 objRenderList[1000];
extern u16 objRenderListLen;
extern bool objSeqBool [OBJ_SEQ_MAX];
extern u8   objSeqFlags[OBJ_SEQ_MAX];
extern u8   objSeqVar1 [OBJ_SEQ_MAX];
extern u8   objSeqVar2 [OBJ_SEQ_MAX];
extern u8   objSeqVar3 [OBJ_SEQ_MAX];
extern float oneOverTimeDelta;
extern bool pauseDisabled;
extern u8 pauseMenuFrameCounter;
extern ObjInstance *pBackpack;
extern Camera *pCamera;
extern MapInfoEntry *pCurMapInfo;
extern SaveGame *pCurSaveGame;
extern bool pdaOn;
extern void *pFrameBuffer[2];
extern float physicsTimeScale; //default 60.0
extern SaveGame *pLastSavedGame; //mainSetBits uses this
extern int playerId; //0=Krystal, 1=Fox XXX verify type
extern float playerMapOffsetX;
extern float playerMapOffsetZ;
extern int playerPrevPosIdx;
extern struct {
    vec3f pos;
    int time;
} playerPrevPositions[PLAYER_POS_LOG_SIZE];
extern ObjInstance *pPlayer;
extern SaveGame *pRestartPoint;
extern ObjInstance *pStaff;
extern ObjInstance *pTricky;
extern u32 randomNumber;
extern u32 renderFlags;
extern float rumbleTimer;
extern SaveData saveData;
extern s8 saveStatus; //1=can save, 0=can't, -2=???
extern s32 seqLength;
extern s8 seqNumBgCmds;
extern s16 seqGlobal1;
extern s16 seqGlobal2;
extern u8 seqGlobal3;
extern bool shouldCloseCMenu;
extern u8 shouldOpenCMenu;
extern u8 soundMode; //Stereo, Surround, Mono, Headphones
extern u64 systemBootTime;
extern u8 tempGameBits[128];
extern float timeDelta;
extern bool timeStop;
extern int titleScreen_panAwayFromMovieTimer;
extern bool titleScreenActive;
extern float viewportAspect;
extern float viewportAspectNotWidescreen;
extern float viewportAspectWidescreen;
extern float viewportAspectWidescreenShadows;
extern u8 volumeCutScenes;
extern float volumeMusic;
extern float volumeSFX;
extern u8 whichControllerFrame;
extern u32 worldMapSelected; //MapDirIdx32
extern GameBit16 yButtonItem;
