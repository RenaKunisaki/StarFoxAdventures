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
GLOBALFN void* dvdLoadFile(const char *path, int *outSize, BOOL bLog);
GLOBALFN int lzoDecompress(byte *src,int compLen,byte *dest,int *outLen);
GLOBALFN void* memcpy(void *dest, void *src, int len);
GLOBALFN void OSReport(const char *fmt, ...);
