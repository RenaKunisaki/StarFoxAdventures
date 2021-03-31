# patches main.dol (rather, default.dol) file to load debug code as needed
.text
.include "common.s"

.int 0x80077da8 #, 0x4BFE2FD5 # where to patch the jump
.int 0x80003E6C # where to insert

constants:
    .set STACK_SIZE,0x40 # how much to reserve
    .set SP_LR_SAVE,0x10
    .set SP_R0_SAVE,0x14


start:
    stwu r1, -STACK_SIZE(r1) # get some stack space
    mflr r3
    stw  r3,  SP_LR_SAVE(r1)

    CALL audioInit # replaced

    # get filePath address
    bl .getpc
    .getpc:
        mflr r3
        addi r3, r3, (filePath - .getpc)@l

    # load the file
    li      r4, 0 # we don't need size
    li      r5, 1 # we do want logging
    CALL    dvdLoadFile
    cmpwi   r3, 0
    beq     .done # file doesn't exist

    # we loaded the file, so jump to it.
    # the file's code can free the buffer if it wants when done.
    mtspr CTR, r3
    bctrl

.done:
    lwz  r3, SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr


filePath:
    .string "debug.bin"
