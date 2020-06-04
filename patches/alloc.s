# alloc debug patch:
# logs allocations and flags any that are absurd.
.text
.include "common.s"

# define patches
patchList:
    PATCH_B 0x80023D00, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x20 # how much to reserve
    .set SP_LR_SAVE, 0x10
    .set SP_R28_SAVE,0x14
    .set SP_R29_SAVE,0x18
    .set SP_R30_SAVE,0x1C

entry: # called as soon as our patch is loaded.
    blr # nothing to do here

# helpful identifying string. not actually neded.
.string "Alloc"
.align 4

main:
    # patch into allocTagged
    stwu r1, -STACK_SIZE(r1) # get some stack space
    mflr r9
    stw  r9,  SP_LR_SAVE(r1)
    stw  r28, SP_R28_SAVE(r1) # save func's temp regs
    stw  r29, SP_R29_SAVE(r1)
    stw  r30, SP_R30_SAVE(r1)

    bl .getpc
    .getpc:
        mflr r31

    addi r3, r31, (.msg - .getpc)@l
    mr   r4, r0  # lr
    mr   r5, r28 # size
    mr   r6, r29 # tag
    mr   r7, r30 # name
    CALL OSReport

    # check size
    srwi  r4, r28, 16
    cmpwi r4, 0x0180
    blt   .ok

    # trigger a BSOD
    li    r3, 0
    stw   r3, 0(r3)

.ok:
    lwz  r4, SP_LR_SAVE(r1)
    mtlr r4 # restore LR
    lwz  r28, SP_R28_SAVE(r1)
    lwz  r29, SP_R29_SAVE(r1)
    lwz  r30, SP_R30_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr
    li   r0, 1 # replaced
    JUMP 0x80023d04, r31 # return to original code

.msg:
    .string "%08X alloc %08X %08X %s"
