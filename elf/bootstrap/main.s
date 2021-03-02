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

    mr      r14, r3 # save address of this code
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    # say something to show we're alive.
    .if DEBUG
        addi r3, r14, s_hello - _start
        CALL OSReport
    .endif

    # load PATCHES.BIN
    addi    r3,  r14, s_patchesBin - _start
    addi    r4,  r1,  SP_TMP1 # out size
    CALL    loadFileByPath
    cmpwi   r3,  0
    beq     .done # file doesn't exist
    mr      r15, r3

    # load PATCHES.TAB
    addi    r3,  r14, s_patchesTab - _start
    addi    r4,  r1,  SP_TMP1 # out size
    CALL    loadFileByPath
    cmpwi   r3,  0
    beq     .done # file doesn't exist
    mr      r16, r3

.if DEBUG
    addi    r3,  r14, s_loadedFiles - _start
    mr      r4,  r15
    mr      r5,  r16
    CALL    OSReport
.endif

    # iterate patches.tab
    # r14: _start
    # r15: PATCHES.BIN
    # r16: PATCHES.TAB
    # r17: current offset into PATCHES.TAB
    subi    r17, r16, 4
.nextPatch:
    lwzu    r3,  4(r17) # get cur offset
    cmpwi   r3,  0
    blt     .done
    add     r3,  r3,  r15 # r3 = header address
    lwz     r18, 8(r3)    # r18 = entry point offset
    add     r18, r18, r3  # r18 = entry point address

    # update the global offset table.
    lwz     r4,  0(r3)     # get GOT offset
    lwz     r5,  4(r3)     # get GOT size (# words)
    add     r6,  r3,  r4   # r6 = GOT ptr
    subi    r6,  r6,  4    # for stwu
    addi    r3,  r3,  0x10 # skip header
.nextGot:
    cmpwi   r5,  0
    beq     .doneGot
    lwz     r7,  4(r6) # get GOT entry
    # HACK: there are absolute addresses and even code in here.
    # this seems to work well enough to avoid changing those.
    # XXX figure out the correct way to do this.
    # is there anything that tells us which entries here actually
    # need to be relocated?
    srwi    r8,  r7,  24 # get high byte
    cmpwi   r8,  0
    bne     .doneGot
    add     r7,  r7,  r3 # to absolute address
    stwu    r7,  4(r6)
    subi    r5,  r5,  1
    b       .nextGot
.doneGot:

.if DEBUG
    mr      r4,  r18
    addi    r3,  r14, s_aboutToExec - _start
    CALL    OSReport
.endif

    # jump to code
    mtctr   r18
    crclr   4*cr1+eq # not sure, but gcc does this...
    bctrl
.if DEBUG
    addi    r3,  r14, s_doneExec - _start
    CALL    OSReport
.endif
    b       .nextPatch

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
    s_loadedFiles:  .string "Loaded PATCHES.BIN=%08X TAB=%08X"
    s_aboutToExec:  .string "Executing at 0x%08X"
    s_doneExec:     .string "Exec OK!"
.endif
.align 4
