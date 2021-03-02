# set register names
.set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
.set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31; .set f0,0; .set f2,2; .set f3,3
.set f0,0;   .set f1,1;   .set f2,2; .set f3,3;   .set f4,4
.set f5,5;   .set f6,6;   .set f7,7;   .set f8,8;   .set f9,9
.set f10,10; .set f11,11; .set f12,12; .set f13,13; .set f14,14
.set f15,15; .set f16,16; .set f17,17; .set f18,18; .set f19,19
.set f20,20; .set f21,21; .set f22,22; .set f23,23; .set f24,24
.set f25,25; .set f26,26; .set f27,27; .set f28,28; .set f29,29
.set f30,30; .set f31,31; .set f0,0; .set f2,2; .set f3,3

.set RAM_START,0x80000000
.set RAM_SIZE, 0x01800000
.set RAM_END,  RAM_START+RAM_SIZE

# I/O regs
.set IO_BASE,0xCC000000

.set IRQ_IO_BASE,0x3000
.set INTSR,    0x00 # interrupt cause
.set INTMR,    0x04 # interrupt mask
.set CPU_RESET,0x24 # write to reboot
.set HW_TYPE,  0x2C # console type in bits 28-31

# memory protection - high 16bits=pageLo, low 16bits=pageHi
# the page address can be calculated as (physical_address >> 10)
.set MEMPROT_IO_BASE,0x4000
.set MEMPROT0,      0x00
.set MEMPROT1,      0x04
.set MEMPROT2,      0x08
.set MEMPROT3,      0x0C
.set MEMPROT_TYPE,  0x10 # 2bits per channel: 01=break on write 10=on read
.set MEMPROT_IMASK, 0x1C # interrput mask
.set MEMPROT_ICAUSE,0x1E # interrput cause
.set MEMPROT_UNK20, 0x20 # interrput handler should clear
.set MEMPROT_ADDRLO,0x22 # low half of address that triggered break
.set MEMPROT_ADDRHI,0x24 # high half of address that triggered break

# DVD interface
.set DVD_IO_BASE,0x6000
.set DVD_DISR,     0x00 # status
.set DVD_DICVR,    0x04 # cover status
.set DVD_DICMDBUF0,0x08 # command
.set DVD_DICMDBUF1,0x0C # offset in 32bit words
.set DVD_DICMDBUF2,0x10 # src length
.set DVD_DIMAR,    0x14 # DMA buffer addr (must be 32byte aligned)
.set DVD_DILENGTH, 0x18 # DMA length (must be multiple of 32 bytes)
.set DVD_DICR,     0x1C # control
.set DVD_DIIMMBUF, 0x20 # immediate data buffer
.set DVD_DICFG,    0x24 # config

# serial interface
.set SI_IO_BASE,0x6400
.set SIC0OUTBUF,0x00 # channel 0 output buffer
.set SIC0INBUFH,0x04 # channel 0 input buffer hi (buttons 1)
.set SIC0INBUFL,0x08 # channel 0 input buffer lo (buttons 2)
.set SIC1OUTBUF,0x0C # channel 1 output buffer
.set SIC1INBUFH,0x10 # channel 1 input buffer hi (buttons 1)
.set SIC1INBUFL,0x14 # channel 1 input buffer lo (buttons 2)
.set SIC2OUTBUF,0x18 # channel 2 output buffer
.set SIC2INBUFH,0x1C # channel 2 input buffer hi (buttons 1)
.set SIC2INBUFL,0x20 # channel 2 input buffer lo (buttons 2)
.set SIC3OUTBUF,0x24 # channel 3 output buffer
.set SIC3INBUFH,0x28 # channel 3 input buffer hi (buttons 1)
.set SIC3INBUFL,0x2C # channel 3 input buffer lo (buttons 2)
.set SIPOLL,    0x30 # poll
.set SICOMCSR,  0x34 # comm ctrl status
.set SISR,      0x38 # status
.set SIEXILK,   0x3C # bit31=disable EXI 32MHz mode

# external interface (memory card, BBA, etc)
.set EXI_IO_BASE,0x6800
.set EXI0CSR,   0x00 # channel 0 status/param
.set EXI0MAR,   0x04 # channel 0 DMA start addr (must be 32byte aligned)
.set EXI0LENGTH,0x08 # channel 0 DMA length (must be 32byte aligned)
.set EXI0CR,    0x0C # channel 0 control
.set EXI0DATA,  0x10 # channel 0 data
.set EXI1CSR,   0x14 # channel 1 status/param
.set EXI1MAR,   0x18 # channel 1 DMA start addr (must be 32byte aligned)
.set EXI1LENGTH,0x1C # channel 1 DMA length (must be 32byte aligned)
.set EXI1CR,    0x20 # channel 1 control
.set EXI1DATA,  0x24 # channel 1 data
.set EXI2CSR,   0x28 # channel 2 status/param
.set EXI2MAR,   0x2C # channel 2 DMA start addr (must be 32byte aligned)
.set EXI2LENGTH,0x30 # channel 2 DMA length (must be 32byte aligned)
.set EXI2CR,    0x34 # channel 2 control
.set EXI2DATA,  0x38 # channel 2 data

.set EXI_CSR_CONN,       (1 << 12)
.set EXI_CSR_EXTINT,     (1 << 11)
.set EXI_CSR_EXTINTMASK, (1 << 10)
.set EXI_CSR_CS0,        (1 <<  9)
.set EXI_CSR_CS1,        (1 <<  8)
.set EXI_CSR_CS2,        (1 <<  7)
.set EXI_CSR_1MHZ,       (0 <<  4)
.set EXI_CSR_2MHZ,       (1 <<  4)
.set EXI_CSR_4MHZ,       (2 <<  4)
.set EXI_CSR_8MHZ,       (3 <<  4)
.set EXI_CSR_16MHZ,      (4 <<  4)
.set EXI_CSR_32MHZ,      (5 <<  4)
.set EXI_CSR_TCINT,      (1 <<  3)
.set EXI_CSR_TCINTMASK,  (1 <<  2)
.set EXI_CSR_EXINT,      (1 <<  1)
.set EXI_CSR_EXINTMASK,  (1 <<  0)

.set EXI_CR_LEN_1, (0 << 4)
.set EXI_CR_LEN_2, (1 << 4)
.set EXI_CR_LEN_3, (2 << 4)
.set EXI_CR_LEN_4, (3 << 4)
.set EXI_CR_READ,  (0 << 2)
.set EXI_CR_WRITE, (1 << 2)
.set EXI_CR_RW,    (2 << 2) # write, then read
.set EXI_CR_IMM,   (0 << 1)
.set EXI_CR_DMA,   (1 << 1)
.set EXI_CR_START, (1 << 0)

.set PAD_BUTTON_LEFT, 0x0001
.set PAD_BUTTON_RIGHT,0x0002
.set PAD_BUTTON_DOWN, 0x0004
.set PAD_BUTTON_UP,   0x0008
.set PAD_BUTTON_Z,    0x0010
.set PAD_BUTTON_R,    0x0020
.set PAD_BUTTON_L,    0x0040
.set PAD_BUTTON_A,    0x0100
.set PAD_BUTTON_B,    0x0200
.set PAD_BUTTON_X,    0x0400
.set PAD_BUTTON_Y,    0x0800
.set PAD_BUTTON_MENU, 0x1000 # Start
.set PAD_ALL_BUTTONS, 0x1F7F

.set GX_BM_BLEND,        1
.set GX_BM_LOGIC,        2
.set GX_BM_NONE,         0
.set GX_BM_SUBTRACT,     3
.set GX_MAX_BLENDMODE,   4
.set GX_BL_DSTALPHA,     6
.set GX_BL_DSTCLR,       GX_BL_SRCCLR
.set GX_BL_INVDSTALPHA,  7
.set GX_BL_INVDSTCLR,    GX_BL_INVSRCCLR
.set GX_BL_INVSRCALPHA,  5
.set GX_BL_INVSRCCLR,    3
.set GX_BL_ONE,          1
.set GX_BL_SRCALPHA,     4
.set GX_BL_SRCCLR,       2
.set GX_BL_ZERO,         0
.set GX_LO_AND,          1
.set GX_LO_CLEAR,        0
.set GX_LO_COPY,         3
.set GX_LO_EQUIV,        9
.set GX_LO_INV,         10
.set GX_LO_INVAND,       4
.set GX_LO_INVCOPY,     12
.set GX_LO_INVOR,       13
.set GX_LO_NAND,        14
.set GX_LO_NOOP,         5
.set GX_LO_NOR,          8
.set GX_LO_OR,           7
.set GX_LO_REVAND,       2
.set GX_LO_REVOR,       11
.set GX_LO_SET,         15
.set GX_LO_XOR,          6
.set GX_ALWAYS,          7
.set GX_EQUAL,           2
.set GX_GEQUAL,          6
.set GX_GREATER,         4
.set GX_LEQUAL,          3
.set GX_LESS,            1
.set GX_NEQUAL,          5
.set GX_NEVER,           0

# magic required to make floats print correctly
# no idea what this does
.macro MAGIC_FLOAT_INCANTATION
    creqv 4*cr1+eq,4*cr1+eq,4*cr1+eq
.endm

# we can't use `bl` instruction because it's relative and we don't
# know where our code will end up. so we use this instead.
# this clobbers lr and r0
.macro CALL addr
    lis  r0,\addr@h
    ori  r0,r0,\addr@l
    #mtlr r0
    #blrl  # branch link to the address in the link register
    mtspr CTR, r0
    bctrl
.endm
# jump to absolute address, clobber specified register
.macro JUMP addr, reg
    lis  \reg, \addr@h
    ori  \reg, \reg, \addr@l
    #mtlr \reg
    #blr
    mtspr CTR, \reg
    bctr
.endm

# expands to:
# lis reg, X
# lwz dest, Y(reg)
# where X and Y are the upper and lower halves of addr,
# adjusted as necessary when Y >= 0x8000.
.macro LOADW reg, addr
    lis \reg, ((\addr) >> 16) + (((\addr) & 0x8000) >> 15)
    lwz \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\reg)
.endm

.macro LOADH reg, addr
    lis \reg, ((\addr) >> 16) + (((\addr) & 0x8000) >> 15)
    lhz \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\reg)
.endm

.macro LOADHA reg, addr
    lis \reg, ((\addr) >> 16) + (((\addr) & 0x8000) >> 15)
    lha \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\reg)
.endm

.macro LOADB reg, addr
    lis \reg, ((\addr) >> 16) + (((\addr) & 0x8000) >> 15)
    lbz \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\reg)
.endm

.macro LOADWH reg, addr
    lis \reg, ((\addr) >> 16) + (((\addr) & 0x8000) >> 15)
.endm

.macro LOADWL reg, addr
    lwz \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\reg)
.endm

.macro LOADWL2 reg, addr, base
    lwz \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

.macro LOADHL2 reg, addr, base
    lhz \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

.macro LOADBL2 reg, addr, base
    lbz \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

.macro LOADFL2 reg, addr, base
    lfs \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

.macro LOADFDL2 reg, addr, base
    lfd \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

# reg = val
.macro LOAD reg, val
    lis \reg, \val@h
    ori \reg, \reg, \val@l
.endm

# eg:
# LOADWH r12, someAddr
# STOREW r3,  someAddr,   r12
# STOREW r4,  someAddr+4, r12
.macro STOREW reg, addr, base
    stw \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

.macro STOREH reg, addr, base
    sth \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

.macro STOREB reg, addr, base
    stb \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

.macro STOREF reg, addr, base
    stfs \reg, -((((\addr) & 0x8000) << 1) - ((\addr) & 0xFFFF))(\base)
.endm

# macros for patch files
.set PATCH_KEEP_AFTER_RUN,0x00000000 # don't automatically free patch
.set PATCH_FREE_AFTER_RUN,0x00000001 # free this patch after running it

.macro PATCH_ID name
    # insert patch ID/name into patch list.
    # name must be exactly 7 characters.
    .byte 0xFE
    .ascii "\name" # we have to use quotes even when name is already quoted...
.endm

.macro PATCH_BL addr, dest
    # insert `bl dest` at `addr`
    .int ((\addr) & 0x1FFFFFF) | 0xC6000000
    .int ((\dest) - patchList) | 1 # low bit set = bl
.endm

.macro PATCH_B addr, dest
    # insert `b dest` at `addr`
    .int ((\addr) & 0x1FFFFFF) | 0xC6000000
    .int ((\dest) - patchList) | 0 # low bit clear = b
.endm

.macro PATCH_STARTUP dest
    # call `dest` from game late init.
    .int 0xC0000001
    .int ((\dest) - patchList)
.endm

.macro PATCH_MAIN_LOOP dest
    # call `dest` from game main loop.
    .int 0xC0000000
    .int ((\dest) - patchList)
.endm

.macro PATCH_PAD_HOOK dest
    # call `dest` from controller read.
    # r4 = pointer to new controller states.
    .int 0xC0000002
    .int ((\dest) - patchList)
.endm

.macro PATCH_BYTE addr, val
    # write byte `val` to `addr`
    .int ((\addr) & 0x01FFFFFF) | 0x00000000
    .int (\val)
.endm

.macro PATCH_BYTES addr, count, val
    # write byte `val` to `addr`...`addr+count` inclusive.
    .int ((\addr) & 0x01FFFFFF) | 0x00000000
    .int (\val) | ((\count) << 16)
.endm

.macro PATCH_HWORD addr, val
    # write hword `val` to `addr`
    .int ((\addr) & 0x01FFFFFF) | 0x02000000
    .int (\val)
.endm

.macro PATCH_HWORDS addr, count, val
    # write hword `val` to `addr`...`addr+(count*2)` inclusive.
    .int ((\addr) & 0x01FFFFFF) | 0x02000000
    .int (\val) | ((\count) << 16)
.endm

# there is no PATCH_WORDS because where would the count go?
.macro PATCH_WORD addr, val
    # write word `val` to `addr`
    .int ((\addr) & 0x01FFFFFF) | 0x04000000
    .int (\val)
.endm

.macro PATCH_END flags
    # insert end-of-patch-list marker and flags
    .int 0, flags
.endm

.include "sfa.s"
.include "gecko.s"
