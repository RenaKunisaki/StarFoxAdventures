#define ALLOC_FAIL_LOG_SIZE 16
typedef struct {
    u32 size;
    AllocTag tag;
    u32 lr; //return address of allocTagged
} AllocFailLogItem;

#define MAX_FREEABLE_PTRS 16

//alloc.c
extern void **freeablePtrs[MAX_FREEABLE_PTRS];
extern u32 allocFailCount;
extern u32 emergFreeCount;
extern u8 allocFailLogIdx;
extern AllocFailLogItem allocFailLog[ALLOC_FAIL_LOG_SIZE];
extern u32 maxBlocksUsed, maxMemUsed;
void getFreeMemory(u32 *outTotalBlocks, u32 *outTotalBytes, u32 *outUsedBlocks, u32 *outUsedBytes, int *outBlocksPct, int *outBytesPct);
bool registerFreeablePtr(void **ptr, const char *name);
void* allocTaggedHook(u32 size, AllocTag tag, const char *name);
void allocInit();
