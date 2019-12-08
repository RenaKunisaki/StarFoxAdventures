.text
.include "common.s"

# patch mergeTableFiles() case MODELS.tab
# insert the offset we loaded Krystal at into the table
GECKO_BEGIN_PATCH 0x80043d78 # li r9, 0x0800
b start

.set P_MODELS_TAB1,dataFileBuffers + (MODELS_TAB  * 4)
.set P_MODELS_TAB2,dataFileBuffers + (MODELS_TAB2 * 4)
.set P_MODELS_BIN1,dataFileBuffers + (MODELS_BIN  * 4)
.set P_MODELS_BIN2,dataFileBuffers + (MODELS_BIN2 * 4)
.set KRYSTAL_MODEL_ID,0x4E8

.set STACK_SIZE,0x100 # how much to reserve
.set SP_LR_SAVE,0x104 # this is what the game does
.set SP_GPR_SAVE,0x10

do_patch:
    # r3 = models.bin file ID * 4
    # r4 = models.tab file ID * 4
    lwzx    r9,  r3, r11 # r9 = &MODELS.bin
    cmpwi   r9,  0
    beq     .end # not loaded, skip

    # get the size of this file
    lwzx    r5, r3, r12 # r5 = size
    # that's the size of the original file, before we appended
    # our model data, so we don't need to adjust it.
    add     r7, r5, r9 # r7 = the actual address
    lwz     r7, 0(r7)
    cmp     0, 0, r7, r10 # is there a model here?
    # we don't need to check for other valid headers because we're
    # only checking for a specific model
    bne     .end

    # OK, r5 is valid.
    lis     r9, 0x8000 # flags: compressed
    or      r5, r5, r9
    lwzx    r9, r4, r11 # r9 = MODELS.tab
    cmpwi   r9, 0
    beq     .end # table not loaded!? idk why this happens.
    stw     r5, (KRYSTAL_MODEL_ID*4)(r9) # store the offset

    .end:
        blr

start:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3, SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3, SP_LR_SAVE(r1)

    LOAD    r12, dataFileSize
    addi    r11, r12, dataFileBuffers - dataFileSize
    LOAD    r10, 0xFACEFEED

    li      r3, MODELS_BIN*4
    li      r4, MODELS_TAB*4
    bl      do_patch

    li      r3, MODELS_BIN2*4
    li      r4, MODELS_TAB2*4
    bl      do_patch

    # restore registers
    lwz     r3, SP_LR_SAVE(r1)
    mtlr    r3
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    li      r9, 0x0800 # replaced

GECKO_END_PATCH
