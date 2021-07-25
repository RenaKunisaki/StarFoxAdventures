//If we use `-mlongcall` in the makefile it will use that even for functions
//within the ELF file. This is not only inefficient, but causes problems when
//one of those functions gets placed at offset zero, because then it becomes
//impossible for the loader to tell whether it's an offset or an absolute
//NULL pointer.
//The best way I can find to avoid this problem is to put the attribute manually
//on every function that isn't part of the ELF.
//Unfortunately it doesn't look like there's a way to apply it to all functions
//except by adding it to every single one.
//Hopefully this workaround still works if a variable gets put at 0. Or maybe
//that can't happen because .text comes first?
#define GLOBALFN __attribute__((longcall))
GLOBALFN void __restore_gpr(void);
GLOBALFN void __save_gpr(void);

GLOBALFN void* allocTagged(uint size,AllocTag tag,const char *name);
GLOBALFN void angleToVec2(int angle, float *outX, float *outY);
GLOBALFN int atan2(float y, float x); //returns angle as s16 0-65535 (or -32768-32767?)
GLOBALFN void audioPlaySound(ObjInstance *sourceObj,SoundId soundid);
GLOBALFN void buttonDisable(int pad, u32 buttons);
GLOBALFN void cameraCheckEnterFirstPerson(void *param1, void *param2); //XXX signature
GLOBALFN void cameraGetFocusObjDistance(float offsY, Camera *this,
    float *outX, float *outY, float *outZ, float *outXZ, //outXZ can be null
    BOOL bUseCurPos);
GLOBALFN void camera_overridePos(float x, float y, float z);
GLOBALFN void cameraUpdate(int frames);
GLOBALFN void cameraUpdateViewMtx(Camera *camera);
GLOBALFN void ClearArena(void);
GLOBALFN void cardShowLoadingMsg(int);
GLOBALFN float cosf(float);
GLOBALFN void* dCacheFlush(void*, uint);
GLOBALFN void* dCacheStore(void*, uint);
GLOBALFN void debugPrintDraw(void);
GLOBALFN void debugPrintf(const char*, ...);
GLOBALFN void debugPrintfxy(int x, int y, const char*, ...);
GLOBALFN void debugPrintSetColor(u8 r, u8 g, u8 b, u8 a);
GLOBALFN void dll_MCUpgradeMa_init(ObjInstance *obj);
GLOBALFN void doQueuedLoads(void);
GLOBALFN void doRumble(float time);
GLOBALFN void drawHudBox (short x, short y, short w, short h, uint opacity, BOOL param_6);
GLOBALFN void drawScaledTexture(float x,float y,Texture *texture,uint opacity,uint scale,int width,int height,uint flags);
GLOBALFN void drawTexture(float x,float y,Texture *texture,uint opacity,uint scale);
GLOBALFN void expgfxRemoveAll(void);
GLOBALFN void fearTestMeterDraw(void);
GLOBALFN void free(void*);
GLOBALFN void gameBitDecrement(GameBit bit);
GLOBALFN void gameBitIncrement(GameBit bit);
GLOBALFN gametextStruct* gameTextGet(GameTextId32);
GLOBALFN GameTextBox* gameTextGetBox (int box);
GLOBALFN char* gameTextGetStr(GameTextId32);
GLOBALFN void gameTextLoadDir(GameTextDir32);
GLOBALFN void gameTextSetColor(u8 r, u8 g, u8 b, u8 a);
GLOBALFN void gameTextShowStr(const char*, int window, int x, int y);
GLOBALFN void gameTimerInit(GameTimerFlags flags, int max);
GLOBALFN void gameTimerRun(void); //takes a param but doesn't use it
GLOBALFN void gameTimerStop(void);
GLOBALFN void GameUI_hudDraw(int,int,int);
GLOBALFN ObjInstance* getArwing(void);
GLOBALFN u16 getButtonsHeld(int padNo);
GLOBALFN u16 getButtonsJustPressed(int padNo);
GLOBALFN Texture* getLoadedTexture(int id);
GLOBALFN ObjPos* getCameraPos(void);
GLOBALFN void gplayClearRestartPoint(void);
GLOBALFN void gplayGotoRestartPoint(void);
GLOBALFN void gplayGotoSavegame(void);
GLOBALFN void gplayRestartPoint(vec3f *pos, s16 rot, byte layer, int bSaveWithOneLessHealth);
GLOBALFN void gplaySetAct(u32 map, u32 act); //XXX which type of map ID?
GLOBALFN void gplaySetObjGroupStatus(MapDirIdx32 map,uint group,int set);
GLOBALFN void gxBeginVtxs(GXOpcode opcode,uint vat,uint nVtxs);
GLOBALFN void GXFlush_(int, int); //real GXFlush takes no params...
GLOBALFN void gxGetVtxDescr(int bit, int *out);
GLOBALFN void gxGetVtxDescrs(u32 *out);
GLOBALFN void gxResetVtxDescr(void);
GLOBALFN void gxSetBlendMode(int, int, int, int);
GLOBALFN void gxSetPeControl_ZCompLoc_(byte);
GLOBALFN void gxSetVtxDescr(int which, int val);
GLOBALFN void gxSetVtxDescrs(u32*);
GLOBALFN void gxWaitFn_80258330(void);
GLOBALFN void* heapAlloc(int region, uint size, AllocTag tag, const char* name); //don't use this, use allocTagged
GLOBALFN void hudDrawAirMeter(void);
GLOBALFN void hudDrawButtons(int,int,int); //XXX params
GLOBALFN void hudDrawRect(int x1,int y1,int x2,int y2,Color4b *color);
GLOBALFN void hudDrawTriangle(float x1,float y1,float x2,float y2,float x3, float y3,Color4b *color); //WARNING: changes color->a
GLOBALFN void* iCacheFlush(void* dest, uint size);
GLOBALFN void initFn_800534f8(void);
GLOBALFN bool isCheatActive(uint);
GLOBALFN bool isCheatUnlocked(uint);
GLOBALFN void* loadFileByPath(char *path,uint *outSize);
    // Returns pointer to allocated buffer of contents.
    // outSize: if not NULL, receives file size.
GLOBALFN void loadMapForCurrentSaveGame(void);
GLOBALFN void loadSaveSettings(void);
GLOBALFN u32 mainGetBit(GameBit bit);
GLOBALFN void mainLoopDoQueuedSounds(void);
GLOBALFN void mainSetBits(GameBit bit, u32 val);
GLOBALFN int mapCoordsToId(int x, int z, int layer); //MapId32
GLOBALFN int mapGetDirIdx(int id); //MapId32 -> MapDirIdx32
GLOBALFN void* mapLoadDataFile(MapDirIdx32 map, DataFileEnum file);
GLOBALFN void mapReload(void);
GLOBALFN void mapScreenDrawHud(void);
GLOBALFN void mapUnload(MapDirIdx32 map, uint flags);
GLOBALFN void* memcpy(void *dest, void *src, int len); // clobbers: r0, r4, r5, r6
GLOBALFN void* memset(void *dest, u8 val, int len); // clobbers: r0, r6, r7
GLOBALFN void mm_free(void*); // wrapper for free()
GLOBALFN ModelFileHeader* modelLoad(
    int modelNum,ModelFlags_loadCharacter flags,uint *outSize);
    // if modelNum is negative, don't use MODELIND.bin
GLOBALFN void multByObjMtx(double x, double y, double z, float *outX, float *outY, float *outZ, ObjInstance *obj);
GLOBALFN float multVectorByObjMtx(float x, float y, float z, float *outX, float *outY, float *outZ, ObjInstance*);
GLOBALFN void musicTestPlaySong(int song);
GLOBALFN ObjDef* objAlloc(int size, int type);
GLOBALFN void objDisableHitbox(ObjInstance*); //only for next frame
GLOBALFN void objFree(ObjInstance*);
GLOBALFN void objFreeAll(void);
GLOBALFN ObjInstance* objInstantiateCharacter(ObjDef*, u32 flags, int mapId, int objNo, float *matrix);
GLOBALFN void objLoadPlayerFromSave(); //XXX verify types
GLOBALFN void objRunSeq(uint seqIdx, ObjInstance *obj, u32 flags);
GLOBALFN void objSetModel(ObjInstance *obj, int modelIdx);
GLOBALFN void objStopSound_(ObjInstance *obj, int id);
GLOBALFN bool objUpdateOpacity(ObjInstance *obj);
GLOBALFN BOOL OSDisableInterrupts(void);
GLOBALFN BOOL OSEnableInterrupts(void);
GLOBALFN OSInterruptMask OSGetInterruptMask(void);
GLOBALFN u64 __OSGetSystemTime();
GLOBALFN void OSReport(const char *fmt, ...);
GLOBALFN BOOL OSRestoreInterrupts(BOOL level); //wtf Nintendo why a bool param?
GLOBALFN OSInterruptMask OSSetInterruptMask(OSInterruptMask mask);
GLOBALFN s8 padGetCX(int pad);
GLOBALFN s8 padGetCY(int pad);
GLOBALFN s8 padGetStickX(int pad);
GLOBALFN s8 padGetStickY(int pad);
GLOBALFN void padReadControllers(PADStatus*);
GLOBALFN void padUpdate(void);
GLOBALFN void pauseMenuDraw(int,int,int); //XXX params
GLOBALFN void pauseMenuDrawText(void);
GLOBALFN void playerDie(ObjInstance *player); //immediately kill player
GLOBALFN ObjInstance* playerGetNearestObject(int idx,ObjInstance *obj,float *outDistance);
GLOBALFN ObjInstance** playerGetObject(int idx, int *outIdx);
GLOBALFN void playerInitFuncPtrs(void);
GLOBALFN BOOL playerSetHeldObject(ObjInstance *player, ObjInstance *obj);
GLOBALFN u32 randomGetNext(void);
GLOBALFN u32 randomGetRange(u32 min, u32 max);
GLOBALFN void randomSetSeed(u32);
GLOBALFN void reset(int, int, int);
GLOBALFN PlayerCharState* SaveGame_getCurCharacterState(void);
GLOBALFN int saveGame_load(s8 slot);
GLOBALFN void saveGame_save(void);
GLOBALFN void saveSelect_drawText(void *unused, u32 alpha);
GLOBALFN void sceneRender(void);
GLOBALFN void setGpuErrorHandler(void*);
GLOBALFN void setSoundMode(u8 mode, bool force);
GLOBALFN void setSubtitlesEnabled(bool);
GLOBALFN int setWidescreen(bool); //always returns 0
GLOBALFN int shiftJisGetNextChar(char *text, int *outSize);
GLOBALFN bool shouldForceMotionBlur(void);
GLOBALFN float sinf(float);
GLOBALFN char* sprintf(char *dest, const char* fmt, ...);
GLOBALFN void streamPlay(int id, void (*callback)(void));
GLOBALFN void streamPlay_defaultCallback();
GLOBALFN void streamStop(void);
GLOBALFN void streamStopAll(void);
GLOBALFN char* strcpy(char *dest, const char *src);
GLOBALFN int strlen(const char*);
GLOBALFN char* strncpy(char *dest, const char* src, int len);
GLOBALFN void texRestructRefs(void);
GLOBALFN void textRenderChar(s16 x1, s16 y1, s16 x2, s16 y2, double s1, double t1, double s2, double t2);
GLOBALFN void textRenderSetup(void);
GLOBALFN Texture* textureIdxToPtr(int);
GLOBALFN int titleLoadSaveFiles(void);
GLOBALFN void timeListDraw(void);
GLOBALFN void unloadMap(void);
GLOBALFN MapDirIdx32 unlockLevel(MapDirIdx32 mapId,int bucket,BOOL bUnlockAll);
GLOBALFN void updateViewMatrix(void);
GLOBALFN float vec3f_distance(vec3f *a, vec3f *b);
GLOBALFN void vec3f_scale(double scale, vec3f *in, vec3f *out);
GLOBALFN float vec3f_xzDistance(vec3f *a, vec3f *b);
GLOBALFN void videoSetBufferSwapCb(void*);
GLOBALFN void videoSetCallback_80256460(void*);
GLOBALFN void viFn_8024d554(void);
GLOBALFN void VISetNextFrameBuffer(void*);
GLOBALFN void VIWaitForRetrace(void);
GLOBALFN void waitNextFrame(void);
GLOBALFN void warpToMap(int warpIdx, bool swapCharacter);
GLOBALFN int zlbDecompress(void *data,uint compLen,void *out); // returns with r5 = out + rawLen
