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
GLOBALFN void* allocTagged(u32 size, u32 tag, const char *name);
GLOBALFN void audioInit(void);
GLOBALFN void* dCacheStore(void*, uint);
GLOBALFN s32 DVDConvertPathToEntrynum(const char *path);
GLOBALFN BOOL DVDGetCurrentDir(char *path, u32 maxlen);
GLOBALFN void* dvdLoadFile(const char *path, int *outSize, BOOL bLog);
GLOBALFN void __flush_cache(void *addr, u32 len);
GLOBALFN void free(void*);
GLOBALFN void GXCopyDisp2 (void *frameBuf, u8 param2);
GLOBALFN void* heapAlloc(int region, uint size, AllocTag tag, const char* name); //don't use this, use allocTagged
GLOBALFN void* iCacheFlush(void *addr, int size);
GLOBALFN int lzoDecompress(byte *src,int compLen,byte *dest,int *outLen);
GLOBALFN void* memcpy(void *dest, void *src, int len);
GLOBALFN void* memset(void *dest, u8 val, int len); // clobbers: r0, r6, r7
GLOBALFN void* model_load(int id); //Model*
GLOBALFN int model_lookupModelInd(int id);
GLOBALFN ObjData * objLoadData(int objType);
GLOBALFN void OSReport(const char *fmt, ...);
GLOBALFN char* strcpy(char *dest, const char *src);
GLOBALFN char* strncpy(char * __dest, char * __src, uint __n);
GLOBALFN BOOL tex0count(u32 fileId);
GLOBALFN void tex0GetMipmap(u32 offset, u32 mipIdx, u32* outSize, u32* outCompSize, int size, void *dest, int doWhat);
GLOBALFN BOOL tex1count(u32 fileId);
GLOBALFN void tex1GetMipmap(u32 offset, u32 mipIdx, u32* outSize, u32* outCompSize, int size, void *dest, int doWhat);
