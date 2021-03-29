/* This is edited from some random vxworks link script I found. */

OUTPUT_FORMAT("elf32-powerpc", "elf32-powerpc", "elf32-powerpc")
OUTPUT_ARCH(powerpc)
ENTRY(_start)
/* Do we need any of these for elf?
__DYNAMIC = 0;    */

PROVIDE (__stack = 0);
SECTIONS {
    /* Read-only sections, merged into text segment: */
    . = 0x01800000 + SIZEOF_HEADERS;
    .interp        : { *(.interp) }
    .hash          : { *(.hash)      }
    .dynsym        : { *(.dynsym)        }
    .dynstr        : { *(.dynstr)        }
    .gnu.version   : { *(.gnu.version)      }
    .gnu.version_d : { *(.gnu.version_d)  }
    .gnu.version_r : { *(.gnu.version_r)  }
    .rela.text     : { *(.rela.text) *(.rela.gnu.linkonce.t*) }
    .rela.data     : { *(.rela.data) *(.rela.gnu.linkonce.d*) }
    .rela.rodata   : { *(.rela.rodata) *(.rela.gnu.linkonce.r*) }
    .rela.got      : { *(.rela.got)  }
    .rela.got1     : { *(.rela.got1) }
    .rela.got2     : { *(.rela.got2) }
    .rela.ctors    : { *(.rela.ctors)    }
    .rela.dtors    : { *(.rela.dtors)    }
    .rela.init     : { *(.rela.init) }
    .rela.fini     : { *(.rela.fini) }
    .rela.bss      : { *(.rela.bss)  }
    .rela.plt      : { *(.rela.plt)  }
    .rela.sdata    : { *(.rela.sdata)    }
    .rela.sbss     : { *(.rela.sbss) }
    .rela.sdata2   : { *(.rela.sdata2)   }
    .rela.sbss2    : { *(.rela.sbss2)    }

    /* Adjust the address for the data segment.  We want to adjust up to
    the same address within the page on the next page up.  It would
    be more correct to do this:
    . = ALIGN(0x40000) + (ALIGN(8) & (0x40000 - 1));
    The current expression does not correctly handle the case of a
    text segment ending precisely at the end of a page; it causes the
    data segment to skip a page.  The above expression does not have
    this problem, but it will currently (2/95) cause BFD to allocate
    a single segment, combining both text and data, for this case.
    This will prevent the text segment from being shared among
    multiple executions of the program; I think that is more
    important than losing a page of the virtual address space (note
    that no actual memory is lost; the page which is skipped can not
    be referenced).  */

    /* All offsets that may need relocation go here.
     * Put the GOT before any symbols so that no symbol will end up at
     * offset zero, since that causes issues with relocation.
     */
    PROVIDE (_GOT_START_ = .);
    .got : {
        *(.offsetzero)
        *(.dynamic)
        *(.data.rel)
        *(.data.rel.*)
        *(.got*)
        *(.ctors)
        *(.dtors)
        *(.fixup)
    }
    PROVIDE (_GOT_END_ = .);

    . =  ALIGN(8) + 0x40000;
    .data : {
        *(.data)
        *(.gnu.linkonce.d*)
        CONSTRUCTORS
    }

    .text : {
        *(.text)
        /* .gnu.warning sections are handled specially by elf32.em.  */
        *(.gnu.warning)
        *(.gnu.linkonce.t*)
    } =0
    .init    : { *(.init) } =0
    .fini    : { *(.fini) } =0
    .rodata  : { *(.rodata) *(.gnu.linkonce.r*) }
    .rodata1 : { *(.rodata1) }
    _etext = .;
    PROVIDE (etext = .);

    .sdata2  : { *(.sdata2) }
    .sbss2   : { *(.sbss2) }

    /* We want the small data sections together, so single-instruction offsets
    can access them all, and initialized data all before uninitialized, so
    we can shorten the on-disk segment size.  */
    .sdata : { *(.sdata) }
    _edata = .;
    PROVIDE (edata = .);
    .sbss : {
        PROVIDE (__sbss_start = .);
        *(.sbss)
        *(.scommon)
        *(.dynsbss)
        PROVIDE (__sbss_end = .);
    }
    .plt : { *(.plt) }
    .bss : {
        PROVIDE (__bss_start = .);
        *(.dynbss)
        *(.bss)
        *(COMMON)
    }
    _end = . ;
    PROVIDE (end = .);
}
