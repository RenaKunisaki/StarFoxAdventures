# gamebit patch:
# logs whenever game bits are changed.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID "GameBit" # must be 7 chars
    # void mainSetBits(GameBit bit,uint val)
    # 8002010c 48 0C 86 A5  bl isSaveGameLoading
    PATCH_BL 0x8002010C, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE, 0x30 # how much to reserve
    .set SP_LR_SAVE, 0x34
    .set SP_R3_SAVE, 0x10
    .set SP_R4_SAVE, 0x14
    .set SP_R14_SAVE,0x18
    .set SP_R15_SAVE,0x1C
    .set SP_R16_SAVE,0x20

entry: # called as soon as our patch is loaded.
    blr # nothing to do here


main:
    stwu r1, -STACK_SIZE(r1) # get some stack space
    mflr r9
    stw  r9,  SP_LR_SAVE(r1)
    stw  r3,  SP_R3_SAVE(r1)
    stw  r4,  SP_R4_SAVE(r1)
    stw  r14, SP_R14_SAVE(r1)
    stw  r15, SP_R15_SAVE(r1)
    stw  r16, SP_R16_SAVE(r1)

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
    # is value being changed?
    CALL mainGetBit
    mr   r5, r3  # r5 = current val
    lwz  r4,  SP_R3_SAVE(r1) # r4 = bit
    lwz  r6,  SP_R4_SAVE(r1) # r6 = new val

    cmpw r5, r6
    beq  .end # no change

    # changed, so print msg
    addi r3, r14, (.msg - .getpc)@l
    CALL OSReport

.end:
    lwz  r4, SP_LR_SAVE(r1)
    mtlr r4 # restore LR
    lwz  r3,  SP_R3_SAVE(r1)
    lwz  r4,  SP_R4_SAVE(r1)
    lwz  r14, SP_R14_SAVE(r1)
    lwz  r15, SP_R15_SAVE(r1)
    lwz  r16, SP_R16_SAVE(r1)
    addi r1, r1, STACK_SIZE # restore stack ptr
    JUMP 0x800e87b0, r4 # isSaveGameLoading

.ignoreList: # these change pretty much every frame
    .short 0x7511 # dummy entry
    #.short 0x0960, 0x0961, 0x0964, 0x0965, 0x0969, 0x096B, 0x0986
    #.short 0x0A7F, 0x0A8B
    #.short 0x0EB5
    #.short 0x0F10, 0x0F46
    .short 0

.msg:
    .string "Bit %04X %X -> %X"
