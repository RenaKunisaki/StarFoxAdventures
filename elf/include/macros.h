#define PACKED __attribute__((packed))
#define WRITE8( addr, val) (*(u8*) addr) = (val)
#define WRITE16(addr, val) (*(u16*)addr) = (val)
#define WRITE32(addr, val) (*(u32*)addr) = (val)
#define WRITE64(addr, val) (*(u64*)addr) = (val)
#define WRITEFLOAT(addr, val) (*(float*)addr) = (val)
#define WRITEDOUBLE(addr, val) (*(double*)addr) = (val)
#define READ8( addr) (*(u8* )addr)
#define READ16(addr) (*(u16*)addr)
#define READ32(addr) (*(u32*)addr)
#define READ64(addr) (*(u64*)addr)
#define READFLOAT(addr) (*(float*)addr)
#define READDOUBLE(addr) (*(double*)addr)

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
