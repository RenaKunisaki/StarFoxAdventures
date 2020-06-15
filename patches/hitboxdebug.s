# temporary patch to diagnose a game crash that happened to me.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID "HitBox " # must be 7 chars
    # 800348c8 80 83 00 54  lwz r4,0x54(r3) # hitbox = obj1->hitbox;
    PATCH_B   0x800348c8, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x180 # how much to reserve
    .set SP_LR_SAVE,0x184 # this is what the game does
    .set SP_STR_BUF, 0x20
    .set SP_GPR_SAVE,0xA0

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


main: # called by our hook, from the patch list.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5, SP_LR_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)
    mr      r28, r3
    lwz     r4, 0x54(r3)
    cmpwi   r4, 0
    bne     .end

    addi    r3, r1, SP_STR_BUF
    lwz     r4, 0x50(r3) # r4 = ObjectFileStruct*
    addi    r4, r4, 0x91 # name
    li      r5, 11
    CALL    strncpy

    # r5: name
    addi    r5, r1, SP_STR_BUF
    li      r6, 0
    stb     r6, 11(r5) # ensure null terminated

    bl      .getpc
    .getpc: mflr r9

    addi    r3, r9, .msg - .getpc # msg
    mr      r4, r28 # addr
    CALL    OSReport

    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    lwz     r4, 0x54(r3) # replaced
    JUMP    0x80034cc4, r0


.end:
    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    lwz     r4, 0x54(r3) # replaced
    JUMP    0x800348cc, r0

.msg: .string "Object %08X '%s' has no hitbox!"
