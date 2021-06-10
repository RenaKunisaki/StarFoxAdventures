#define ALLOC_FAIL_LOG_SIZE 16
typedef struct {
    u32 size;
    AllocTag tag;
    u32 lr; //return address of allocTagged
} AllocFailLogItem;

//alloc.c
extern u32 allocFailCount;
extern u32 emergFreeCount;
extern u8 allocFailLogIdx;
extern AllocFailLogItem allocFailLog[ALLOC_FAIL_LOG_SIZE];
void getFreeMemory(u32 *outTotalBlocks, u32 *outTotalBytes, u32 *outUsedBlocks, u32 *outUsedBytes, int *outBlocksPct, int *outBytesPct);
void* allocTaggedHook(u32 size, AllocTag tag, const char *name);
void allocInit();
