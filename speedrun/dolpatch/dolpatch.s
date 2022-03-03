# patches main.dol file to load the mod from an external file.
# to avoid changing memory allocations, that file is LACTIONS.bin
# which is loaded by the game but unused.
.set DEBUG,0
.text
.include "common.s"

# patch at 0x8003BAC0: 48 27 96 11 bl playerRender()
.int 0x8003BAC0 # where to patch the jump
.int 0x80003E6C # where to insert the code

constants:
    .set STACK_SIZE,    0xC0 # how much to reserve
    .set SP_LR_SAVE,    STACK_SIZE+4
    .set SP_GPR_SAVE,   0x10

_start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r9
    stw     r9,  SP_LR_SAVE(r1)
    #stw     r0,  SP_R0_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    bl      .getpc
    .getpc:
        mflr    r14

    # check if LACTIONS.bin is loaded
    LOADW   r3,  dataFileBuffers + (0xC * 4)
    cmpwi   r3,  0
    beq     .done

.if DEBUG
    mr      r15, r3
    addi    r3,  r14, s_start - .getpc
    CALL    OSReport
    mr      r3,  r15
.endif

    # get the size
    LOADW  r4, dataFileSize + (0xC * 4)

    # jump to it
    mtspr   CTR, r3
    bctrl

.if DEBUG
    addi    r3,  r14, s_done - .getpc
    CALL    OSReport
.endif

.done:
    # jump to the render function we hooked
    LOAD    r3,  0x802b50d0 # playerRender (XXX why can't we use the symbol?)
    mtspr   CTR, r3
    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3  # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    bctr

.if DEBUG
    s_start: .string "Bootstrap..."
    s_done:  .string "Loaded patch OK"
.endif
