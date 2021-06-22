#define PACKED __attribute__((packed))
#define WRITE8( addr, val) (*(u8*) addr) = ((u8)(val))
#define WRITE16(addr, val) (*(u16*)addr) = ((u16)(val))
#define WRITE32(addr, val) (*(u32*)addr) = ((u32)(val))
#define WRITE64(addr, val) (*(u64*)addr) = ((u64)(val))
#define WRITEFLOAT(addr, val) (*(float*)addr) = ((float)(val))
#define WRITEDOUBLE(addr, val) (*(double*)addr) = ((double)(val))
#define READ8( addr) (*(u8* )addr)
#define READ16(addr) (*(u16*)addr)
#define READ32(addr) (*(u32*)addr)
#define READ64(addr) (*(u64*)addr)
#define READFLOAT(addr) (*(float*)addr)
#define READDOUBLE(addr) (*(double*)addr)
#define WRITE_NOP(addr) ((*(u32*)addr) = 0x60000000)
#define WRITE_BLR(addr) ((*(u32*)addr) = 0x4E800020)

#define RAM_START 0x80000000
#define RAM_SIZE  0x01800000
#define RAM_END   (RAM_START + RAM_SIZE)
#define PTR_VALID(p) (((u32)p) >= RAM_START && ((u32)p) < RAM_END)

//Prologue macro for naked functions
//Saves r0 and r3-r31, then clobbers r0
//The purpose of using this is that r2-r31 are unchanged, and that
//r3-r31 are saved to the stack (not just r14-r31), so it can be used
//to hook into the middle of a function.
#define ASM_FUNC_START(stackSize) \
    "stwu    1, -" #stackSize      "(1) \n" \
    "stw     0,                 0x10(1) \n" \
    "stmw    3,                 0x14(1) \n" \
    "mflr    0                          \n" \
    "stw     0,  (" #stackSize  "+4)(1) \n"

#define ASM_FUNC_END(stackSize) \
    "lwz     0,                0x10(1) \n" \
    "lwz     3, (" #stackSize  "+4)(1) \n" \
    "mtlr    3                         \n" \
    "lmw     3,                0x14(1) \n" \
    "addi    1, 1, " #stackSize       "\n"

//same as above, but doesn't preserve r3, for return values.
#define ASM_RFUNC_START(stackSize) \
    "stwu    1, -" #stackSize      "(1) \n" \
    "stw     0,                 0x10(1) \n" \
    "stmw    4,                 0x14(1) \n" \
    "mflr    0                          \n" \
    "stw     0,  (" #stackSize  "+4)(1) \n"

#define ASM_RFUNC_END(stackSize) \
    "lwz     0,                0x10(1) \n" \
    "lwz     4, (" #stackSize  "+4)(1) \n" \
    "mtlr    4                         \n" \
    "lmw     4,                0x14(1) \n" \
    "addi    1, 1, " #stackSize       "\n"

/** A compile time assertion check.
 *
 *  Validate at compile time that the predicate is true without
 *  generating code. This can be used at any point in a source file
 *  where typedef is legal.
 *
 *  On success, compilation proceeds normally.
 *
 *  On failure, attempts to typedef an array type of negative size. The
 *  offending line will look like
 *      typedef assertion_failed_file_h_42[-1]
 *  where file is the content of the second parameter which should
 *  typically be related in some obvious way to the containing file
 *  name, 42 is the line number in the file on which the assertion
 *  appears, and -1 is the result of a calculation based on the
 *  predicate failing.
 *
 *  \param predicate The predicate to test. It must evaluate to
 *  something that can be coerced to a normal C boolean.
 *
 *  \param file A sequence of legal identifier characters that should
 *  uniquely identify the source file in which this condition appears.
 */
#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)

#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];
