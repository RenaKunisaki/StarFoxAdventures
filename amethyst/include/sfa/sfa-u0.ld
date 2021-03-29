OUTPUT_FORMAT("elf32-powerpc", "elf32-powerpc", "elf32-powerpc")
OUTPUT_ARCH(powerpc)
ENTRY(_start)

PHDRS
{
    headers   PT_LOAD FILEHDR PHDRS ;
    dummy     PT_LOAD ;
    data      PT_LOAD ;
    dynamic   PT_DYNAMIC ;

    /* these have to be PT_SHLIB or else the VMA becomes zero
     * because reasons.  or it tries to make a 2GB file.
     * or PT_DYNAMIC but then readelf barfs since that's not
     * what PT_DYNAMIC is for.
     * it SHOULD be PT_LOAD (which might also kill the "not in segment"
     * warning) but that doesn't work.
     */
    /* bootstrap PT_SHLIB AT(BOOTPATCH_ROM_ADDR) ;
    boot      PT_SHLIB AT(BOOTSTRAP_ROM_ADDR) ; */
    text      PT_SHLIB /* AT(PATCH_ROM_ADDR) */ ;
}

SECTIONS
{
    .dynamic : { *(.dynamic) } :data :dynamic

    . = 0x80000000;
    .text . : {
        _stext = .;
        *(.text);
        *(.*data*);
        *(.*got*);
        *(.*fixup*);
        *(.*rela*);
        _etext = .;
        _ltext = _etext - _stext;
    } AT>RAM :text

    .bss . : {
        _sbss  = .;
        *(.bss);
         _ebss  = . ;
         _lbss = _ebss - _sbss;
    } AT>RAM :text

    /* get these out of the way */
    .poop : {
        *(.reginfo)
    } :dummy
    .junk : {
        *(.gnu.*)
    } :dummy
}
