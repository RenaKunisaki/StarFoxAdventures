void __restore_gpr(void);
void __save_gpr(void);
void* allocTagged(uint size,AllocTag tag,char *name);
void angleToVec2(int angle, float *outX, float *outY);
int atan2(float y, float x); //returns angle as s16 0-65535 (or -32768-32767?)
void audioPlaySound(ObjInstance *sourceObj,SoundId soundid);
void buttonDisable(int pad, u32 buttons);
void cameraGetFocusObjDistance(float offsY, Camera *this,
    float *outX, float *outY, float *outZ, float *outXZ, //outXZ can be null
    BOOL bUseCurPos);
void ClearArena(void);
float cosf(float);
void* dCacheFlush(void*, uint);
void* dCacheStore(void*, uint);
void debugPrintDraw(void);
void debugPrintf(const char*, ...);
void debugPrintfxy(int x, int y, const char*, ...);
void drawTexture(double x,double y,Texture *texture,uint opacity,uint scale);
void expgfxRemoveAll(void);
void free(void*);
void gameBitDecrement(GameBit bit);
void gameBitIncrement(GameBit bit);
gametextStruct* gameTextGet(GameTextId32);
char* gameTextGetStr(GameTextId32);
void gameTextSetColor(u8 r, u8 g, u8 b, u8 a);
void gameTextShowStr(char*, int window, int x, int y);
u16 getButtonsHeld(int padNo);
u16 getButtonsJustPressed(int padNo);
void gplayClearRestartPoint(void);
void gplayGotoRestartPoint(void);
void gplayRestartPoint(vec3f *pos, s16 rot, byte layer, int bSaveWithOneLessHealth);
void gplaySetAct(u32 map, u32 act); //XXX which type of map ID?
void gplaySetObjGroupStatus(MapDirIdx32 map,uint group,int set);
void gxBeginVtxs(GXOpcode opcode,uint vat,uint nVtxs);
void gxGetVtxDescr(int bit, int *out);
void gxGetVtxDescrs(u32 *out);
void gxResetVtxDescr(void);
void gxSetBlendMode(int, int, int, int);
void gxSetPeControl_ZCompLoc_(byte);
void gxSetVtxDescr(int which, int val);
void gxSetVtxDescrs(u32*);
void* heapAlloc(int region, uint size, AllocTag tag, char* name); //don't use this, use allocTagged
void* iCacheFlush(void* dest, uint size);
bool isCheatActive(uint);
bool isCheatUnlocked(uint);
void* loadFileByPath(char *path,uint *outSize);
    // Returns pointer to allocated buffer of contents.
    // outSize: if not NULL, receives file size.
void logSetColor(u8 r, u8 g, u8 b, u8 a);
u32 mainGetBit(GameBit bit);
void mainSetBits(GameBit bit, u32 val);
int mapCoordsToId(int x, int z, int layer);
void* memcpy(void *dest, void *src, int len); // clobbers: r0, r4, r5, r6
void* memset(void *dest, u8 val, int len); // clobbers: r0, r6, r7
void mm_free(void*); // wrapper for free()
ModelFileHeader* modelLoad(
    int modelNum,ModelFlags_loadCharacter flags,uint *outSize);
    // if modelNum is negative, don't use MODELIND.bin
void multVectorByObjMtx(float x, float y, float z, float *outX, float *outY, float *outZ, ObjInstance*);
ObjDef* objAlloc(int size, int type);
void objDisableHitbox(ObjInstance*); //only for next frame
void objFree(ObjInstance*);
ObjInstance* objInstantiateCharacter(ObjDef*, u32 flags, int mapId, int objNo, float *matrix);
s8 padGetStickX(int pad);
s8 padGetStickY(int pad);
void playerDie(ObjInstance *player); //immediately kill player
ObjInstance* playerGetNearestObject(int idx,ObjInstance *obj,float *outDistance);
PlayerCharState* SaveGame_getCurCharacterState(void);
void saveGame_save(void);
void setSoundMode(u8 mode, bool force);
void setSubtitlesEnabled(bool);
int setWidescreen(bool); //always returns 0
int shiftJisGetNextChar(char *text, char *outSize);
float sinf(float);
char* sprintf(char *dest, const char* fmt, ...);
int strlen(const char*);
char* strncpy(char *dest, const char* src, int len);
MapDirIdx32 unlockLevel(MapDirIdx32 mapId,int bucket,BOOL bUnlockAll);
void vec3f_scale(double scale, vec3f *in, vec3f *out);
void VISetNextFrameBuffer(void*);
void VIWaitForRetrace(void);
void waitNextFrame(void);
void warpToMap(int warpIdx, bool swapCharacter);
int zlbDecompress(void *data,uint compLen,void *out); // returns with r5 = out + rawLen
