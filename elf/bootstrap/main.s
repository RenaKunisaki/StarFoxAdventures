# Contents of debug.bin.
# This file is loaded at startup by our modified main.dol and executed.
# No header, just code. This code must be position-independent.
# This code loads in PATCHES.BIN and PATCHES.TAB and executes the programs
# found within.

.set DEBUG,1
.text
.include "common.s"

constants:
    .set STACK_SIZE,   0xC0 # how much to reserve
    .set SP_LR_SAVE,   0xC4
    .set SP_TMP1,      0x10
    .set SP_PATCH_ADDR,0x14
    .set SP_FLAGS_ADDR,0x18
    .set SP_GPR_SAVE,  0x1C

_start:
    # entry point of this file.
    b      _start2

    # header, to be filled in by build script.
offsGOT:   .int 0 # offset of GOT (-4 for lwzu)
sizeGOT:   .int 0 # size of GOT in words
offsEntry: .int 0 # entry point of ELF
sizeBoot:  .int 0 # size of this bootstrap code

_start2:
    mr      r14, r3 # save address of this code
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    # say something to show we're alive.
    .if DEBUG
        addi    r3,  r14, s_hello - _start
        CALL    OSReport

        lwz     r4,  (offsGOT - _start)(r14)
        lwz     r6,  (sizeGOT - _start)(r14)
        add     r5,  r4,  r14 # r15 = address of GOT - 4
        addi    r3,  r14, s_gotOffs - _start
        CALL    OSReport
    .endif

    # get GOT offset and size
    lwz     r15, (offsGOT  - _start)(r14)
    lwz     r16, (sizeGOT  - _start)(r14)
    lwz     r17, (sizeBoot - _start)(r14)
    add     r17, r17, r14 # correct for bootstrap size
    add     r15, r15, r14 # r15 = address of GOT - 4

.nextGot:
    cmpwi   r16, 0 # any entries left?
    beq     .doneGot
    lwz     r4,  4(r15) # get GOT entry
    # HACK: there are absolute addresses and even code in here.
    # this seems to work well enough to avoid changing those.
    # XXX figure out the correct way to do this.
    # is there anything that tells us which entries here actually
    # need to be relocated?
    srwi    r8,  r4,  24 # get high byte
    cmpwi   r8,  0
    bne     .setGot
    add     r4,  r4,  r17 # to absolute address
.setGot:
    stwu    r4,  4(r15)
    subi    r16, r16, 1
    b       .nextGot
.doneGot:

.if DEBUG
    lwz     r4,  (offsEntry - _start)(r14)
    add     r4,  r4,  r14
    addi    r3,  r14, s_aboutToExec - _start
    CALL    OSReport
.endif

    # jump to code
    lwz     r4,  (offsEntry - _start)(r14)
    add     r4,  r4,  r14
    mtctr   r4
    crclr   4*cr1+eq # not sure, but gcc does this...
    bctrl
.if DEBUG
    addi    r3,  r14, s_doneExec - _start
    CALL    OSReport
.endif

.done:
endSub:
    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    blr

panic: # r5 = msg
    # apparently OSPanic doesn't work right in Dolphin or something...
    #addi    r3,  r14, s_file - _start
    #li      r4,  0 # line
    #JUMP    OSPanic, r0 # never returns
    mr       r3,  r5
    mr       r4,  r6
    CALL     OSReport
    b        .done

s_patchesBin: .string "PATCHES.BIN"
s_patchesTab: .string "PATCHES.TAB"
s_noMem:      .string "Out of memory"
s_file:       .string "debug.bin" # for OSPanic
.if DEBUG
    s_hello:        .string "hello, cruel world"
    s_gotOffs:      .string "GOT Offs=0x%08X (0x%08X) size=0x%04X"
    s_aboutToExec:  .string "Executing at 0x%08X"
    s_doneExec:     .string "Exec OK!"
.endif
.align 4
