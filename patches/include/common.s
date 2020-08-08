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
