MEMORY
{
    /* null: used for segments we don't need */
    null       (rwx) : ORIGIN = 0x00000000, LENGTH = 1M

    /* rest are GC memory space */
    PHYSRAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 24M /* actual physical RAM */
    RAM        (rwx) : ORIGIN = 0x80000000, LENGTH = 24M /* most games map RAM here */
    UNCACHED   (rwx) : ORIGIN = 0xC0000000, LENGTH = 24M /* uncached RAM */
    /* apparently ld doesn't like EFB */
    /* EFB        (rw)  : ORIGIN = 0xC8000000, LENGTH = 2M   embedded framebuffer */
    CP         (rw)  : ORIGIN = 0xCC000000, LENGTH = 4K  /* GX Command Processor */
    PE         (rw)  : ORIGIN = 0xCC001000, LENGTH = 4K  /* GX Pixel Engine */
    VI         (rw)  : ORIGIN = 0xCC002000, LENGTH = 4K  /* GX Video Interface */
    PI         (rw)  : ORIGIN = 0xCC003000, LENGTH = 4K  /* Processor Interface */
    MI         (rw)  : ORIGIN = 0xCC004000, LENGTH = 4K  /* Memory Interface */
    AI         (rw)  : ORIGIN = 0xCC005000, LENGTH = 4K  /* Audio Interface */
    DI         (rw)  : ORIGIN = 0xCC006000, LENGTH = 1K  /* DVD Interface */
    SI         (rw)  : ORIGIN = 0xCC006400, LENGTH = 1K  /* Serial Interface */
    EXI        (rw)  : ORIGIN = 0xCC006800, LENGTH = 1K  /* External Interface */
    STREAM     (rw)  : ORIGIN = 0xCC006C00, LENGTH = 1K  /* Streaming Interface */
    GXFIFO     (rw)  : ORIGIN = 0xCC008000, LENGTH = 1K  /* GX Display List FIFO (XXX size?) */
    L2CACHE    (rw)  : ORIGIN = 0xE0000000, LENGTH = 16K /* XXX executable? */
    IPL        (rw)  : ORIGIN = 0xFFF00000, LENGTH = 1M  /* only mapped at bootup */
}
