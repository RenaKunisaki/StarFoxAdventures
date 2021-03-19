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
GLOBALFN void cameraUpdate(int frames);
GLOBALFN void ClearArena(void);
GLOBALFN void cardShowLoadingMsg(int);
GLOBALFN float cosf(float);
GLOBALFN void* dCacheFlush(void*, uint);
GLOBALFN void* dCacheStore(void*, uint);
GLOBALFN void debugPrintDraw(void);
GLOBALFN void debugPrintf(const char*, ...);
GLOBALFN void debugPrintfxy(int x, int y, const char*, ...);
GLOBALFN void doRumble(float time);
GLOBALFN void drawScaledTexture(double x,double y,Texture *texture,uint opacity,uint scale,int width,int height,uint flags);
GLOBALFN void drawTexture(double x,double y,Texture *texture,uint opacity,uint scale);
GLOBALFN void expgfxRemoveAll(void);
GLOBALFN void free(void*);
GLOBALFN void gameBitDecrement(GameBit bit);
GLOBALFN void gameBitIncrement(GameBit bit);
GLOBALFN gametextStruct* gameTextGet(GameTextId32);
GLOBALFN char* gameTextGetStr(GameTextId32);
GLOBALFN void gameTextSetColor(u8 r, u8 g, u8 b, u8 a);
GLOBALFN void gameTextShowStr(const char*, int window, int x, int y);
GLOBALFN void GameUI_hudDraw(int,int,int);
GLOBALFN u16 getButtonsHeld(int padNo);
GLOBALFN u16 getButtonsJustPressed(int padNo);
GLOBALFN void gplayClearRestartPoint(void);
GLOBALFN void gplayGotoRestartPoint(void);
GLOBALFN void gplayRestartPoint(vec3f *pos, s16 rot, byte layer, int bSaveWithOneLessHealth);
GLOBALFN void gplaySetAct(u32 map, u32 act); //XXX which type of map ID?
GLOBALFN void gplaySetObjGroupStatus(MapDirIdx32 map,uint group,int set);
GLOBALFN void gxBeginVtxs(GXOpcode opcode,uint vat,uint nVtxs);
GLOBALFN void gxGetVtxDescr(int bit, int *out);
GLOBALFN void gxGetVtxDescrs(u32 *out);
GLOBALFN void gxResetVtxDescr(void);
GLOBALFN void gxSetBlendMode(int, int, int, int);
GLOBALFN void gxSetPeControl_ZCompLoc_(byte);
GLOBALFN void gxSetVtxDescr(int which, int val);
GLOBALFN void gxSetVtxDescrs(u32*);
GLOBALFN void gxWaitFn_80258330(void);
GLOBALFN void* heapAlloc(int region, uint size, AllocTag tag, const char* name); //don't use this, use allocTagged
GLOBALFN void* iCacheFlush(void* dest, uint size);
GLOBALFN bool isCheatActive(uint);
GLOBALFN bool isCheatUnlocked(uint);
GLOBALFN void* loadFileByPath(char *path,uint *outSize);
    // Returns pointer to allocated buffer of contents.
    // outSize: if not NULL, receives file size.
GLOBALFN void loadMapForCurrentSaveGame(void);
GLOBALFN void logSetColor(u8 r, u8 g, u8 b, u8 a);
GLOBALFN u32 mainGetBit(GameBit bit);
GLOBALFN void mainSetBits(GameBit bit, u32 val);
GLOBALFN int mapCoordsToId(int x, int z, int layer);
GLOBALFN void* mapLoadDataFile(MapDirIdx32 map, DataFileEnum file);
GLOBALFN void mapReload(void);
GLOBALFN void* memcpy(void *dest, void *src, int len); // clobbers: r0, r4, r5, r6
GLOBALFN void* memset(void *dest, u8 val, int len); // clobbers: r0, r6, r7
GLOBALFN void mm_free(void*); // wrapper for free()
GLOBALFN ModelFileHeader* modelLoad(
    int modelNum,ModelFlags_loadCharacter flags,uint *outSize);
    // if modelNum is negative, don't use MODELIND.bin
GLOBALFN float multVectorByObjMtx(float x, float y, float z, float *outX, float *outY, float *outZ, ObjInstance*);
GLOBALFN void musicTestPlaySong(int song);
GLOBALFN ObjDef* objAlloc(int size, int type);
GLOBALFN void objDisableHitbox(ObjInstance*); //only for next frame
GLOBALFN void objFree(ObjInstance*);
GLOBALFN ObjInstance* objInstantiateCharacter(ObjDef*, u32 flags, int mapId, int objNo, float *matrix);
GLOBALFN void objLoadPlayerFromSave(); //XXX verify types
GLOBALFN void objStopSound_(ObjInstance *obj, int id);
GLOBALFN void OSReport(const char *fmt, ...);
GLOBALFN s8 padGetCX(int pad);
GLOBALFN s8 padGetCY(int pad);
GLOBALFN s8 padGetStickX(int pad);
GLOBALFN s8 padGetStickY(int pad);
GLOBALFN void padReadControllers(PADStatus*);
GLOBALFN void playerDie(ObjInstance *player); //immediately kill player
GLOBALFN ObjInstance* playerGetNearestObject(int idx,ObjInstance *obj,float *outDistance);
GLOBALFN void playerInitFuncPtrs(void);
GLOBALFN void reset(int, int, int);
GLOBALFN PlayerCharState* SaveGame_getCurCharacterState(void);
GLOBALFN void saveGame_save(void);
GLOBALFN void setGpuErrorHandler(void*);
GLOBALFN void setSoundMode(u8 mode, bool force);
GLOBALFN void setSubtitlesEnabled(bool);
GLOBALFN int setWidescreen(bool); //always returns 0
GLOBALFN int shiftJisGetNextChar(char *text, char *outSize);
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
GLOBALFN void textRenderChar(s16 x1, s16 y1, s16 x2, s16 y2, double s1, double t1, double s2, double t2);
GLOBALFN void textRenderSetup(void);
GLOBALFN MapDirIdx32 unlockLevel(MapDirIdx32 mapId,int bucket,BOOL bUnlockAll);
GLOBALFN void vec3f_scale(double scale, vec3f *in, vec3f *out);
GLOBALFN void videoSetBufferSwapCb(void*);
GLOBALFN void videoSetCallback_80256460(void*);
GLOBALFN void viFn_8024d554(void);
GLOBALFN void VISetNextFrameBuffer(void*);
GLOBALFN void VIWaitForRetrace(void);
GLOBALFN void waitNextFrame(void);
GLOBALFN void warpToMap(int warpIdx, bool swapCharacter);
GLOBALFN int zlbDecompress(void *data,uint compLen,void *out); // returns with r5 = out + rawLen
