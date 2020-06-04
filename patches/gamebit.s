# gamebit patch:
# logs whenever game bits are changed.
.text
.include "common.s"

# define patches
patchList:
    # void mainSetBits(GameBit bit,uint val)
    # 8002010c 48 0C 86 A5  bl isSaveGameLoading
    PATCH_BL 0x8002010C, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x20 # how much to reserve
    .set SP_LR_SAVE,0x10

entry: # called as soon as our patch is loaded.
    blr # nothing to do here

# helpful identifying string. not actually neded.
.string "GameBits"
.align 4

main:
    stwu r1, -STACK_SIZE(r1) # get some stack space
    mflr r9
    stw  r9,  SP_LR_SAVE(r1)

    bl .getpc
    .getpc:
        mflr r14

    # check ignore list
    addi  r9, r14, (.ignoreList - .getpc)@l
.nextIgnore:
    lhzu  r8, 2(r9)
    cmpw  r8, r3
    beq   .end
    cmpwi r8, 0
    bne   .nextIgnore

.noIgnore:
    mr   r5, r4
    mr   r4, r3
    addi r3, r14, (.msg - .getpc)@l
    CALL OSReport

.end:
    lwz  r4, SP_LR_SAVE(r1)
    mtlr r4 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    JUMP 0x800e87b0, r4 # isSaveGameLoading

.ignoreList: # these change pretty much every frame
    .short 0x7511 # dummy entry
    .short 0x0960, 0x0961, 0x0964, 0x0965, 0x0969, 0x096B, 0x0986
    .short 0x0A7F, 0x0A8B
    .short 0x0EB5
    .short 0x0F10, 0x0F46
    .short 0

.msg:
    .string "Bit %04X=%X"
