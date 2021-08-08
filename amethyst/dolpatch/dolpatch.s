# patches main.dol file to load the mod from an external file.
.set DEBUG,0
.text
.include "common.s"

# patch at 802408a8: 4B FF FB 59  bl ClearArena (in _osInit())
# or better, 80021038: 48 11 6C F1  bl installBsodHandlers (in init())
# or, 80020e98: 4B FF 46 15  bl initControllers() (in init())

# can probably insert at 80003E6C - 80004338, room for 307 instructions
# 80003100 in RAM => 0x100 in DOL file
# but that's only text0
# 80021038 is text1, which should start at 0x2620, and 800066E0
# the proper offset is 1cf78

# for now I'm gonna be lazy and just hardcode the jump
# eventually a script should use these ints to know where to patch.
.int 0x80020E98 #, 0x4BFE2FD5 # where to patch the jump
.int 0x80003E6C # where to insert the code

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104
    .set SP_R0_SAVE,0x14
    .set SP_HEAP1_SAVE,0x18
    .set SP_HEAP3_SAVE,0x1C
    .set SP_FILE_SIZE,0x20
    .set SP_GPR_SAVE,0x24

_start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)
    #stw     r0,  SP_R0_SAVE(r1)
    stmw    r14, SP_GPR_SAVE(r1)

    bl      .getpc
    .getpc:
        mflr    r14

.if DEBUG
    addi    r3,  r14, s_start - .getpc
    CALL    OSReport
.endif

    #CALL    0x80137d28 # installBsodHandlers()
    CALL    0x800154ac # initControllers
    #lwz     r0,  SP_R0_SAVE(r1)

    # allow loading this file into other heaps
    li      r5,  0
    lwz     r4,  (vars - .getpc)(r14)
    lwz     r3,  0(r4)
    stw     r3,  SP_HEAP1_SAVE(r1)
    stw     r5,  0(r4)
    lwz     r4,  ((vars+4) - .getpc)(r14)
    lwz     r3,  0(r4)
    stw     r3,  SP_HEAP3_SAVE(r1)
    stw     r5,  0(r4)

.if DEBUG
    addi    r3,  r14, s_loading - .getpc
    CALL    OSReport
.endif

    # get filePath address
    addi    r3,  r14, (filePath - .getpc)@l

    # load the file
    #li      r4,  0 # we don't need size
    addi    r4,  r1,  SP_FILE_SIZE
    CALL    0x80015ab4 # loadFileByPath

    # restore heap settings
    lwz     r4,  (vars - .getpc)(r14)
    lwz     r5,  SP_HEAP1_SAVE(r1)
    stw     r5,  0(r4)
    lwz     r4,  ((vars+4) - .getpc)(r14)
    lwz     r5,  SP_HEAP3_SAVE(r1)
    stw     r5,  0(r4)

    cmpwi   r3,  0
    beq     .done # file doesn't exist

    # we loaded the file, so jump to it.
    # the file's code can free the buffer if it wants when done.
    lwz     r4,  SP_FILE_SIZE(r1)
    mtspr   CTR, r3
    bctrl

.done:
.if DEBUG
    addi    r3,  r14, s_done - .getpc
    CALL    OSReport
.endif

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3  # restore LR
    lmw     r14, SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    blr

vars:
    .int 0x803db434 #bOnlyUseHeaps1and2
    .int 0x803dcb08 #bOnlyUseHeap3
filePath: .string "boot.bin"
s_start:   .string "Bootstrap..."
s_loading: .string "Load patch..."
s_done: .string "Loaded patch OK"
