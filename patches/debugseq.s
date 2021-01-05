# Sequence debug patch:
# logs some sequence operations.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID  "seqDbug" # must be 7 chars
    PATCH_B   0x80083784, onSubCmd0B
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x80 # how much to reserve
    .set SP_LR_SAVE, 0x84
    .set SP_GPR_SAVE,0x10

entry: # called as soon as our patch is loaded.
    blr # nothing to do here

onSubCmd0B:
    li      r3,  0 # replaced
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    bl .onSubCmd0B
    .onSubCmd0B: mflr r14

    mr      r4, r25 # idx
    mr      r5, r30 # count
    mr      r6, r29 # op
    mr      r7, r28 # len
    mr      r8, r27 # param
    addi    r3, r14, (.msg_cmd0B - .onSubCmd0B)@l
    CALL    OSReport

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3  # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    JUMP    0x80083788, r0 # return to original code

.msg_cmd0B: .string "SEQCMD 0B[%d/%d] op %02X len %02X param %04X"
