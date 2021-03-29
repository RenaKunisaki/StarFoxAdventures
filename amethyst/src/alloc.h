#define ALLOC_FAIL_LOG_SIZE 16
typedef struct {
    u32 size;
    AllocTag tag;
} AllocFailLogItem;

//alloc.c
extern u32 allocFailCount;
extern u32 emergFreeCount;
extern u8 allocFailLogIdx;
extern AllocFailLogItem allocFailLog[ALLOC_FAIL_LOG_SIZE];
void* allocTaggedHook(u32 size, AllocTag tag, const char *name);
