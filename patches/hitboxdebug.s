# temporary patch to diagnose a game crash that happened to me.
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID "HitBox " # must be 7 chars
    # 800348c8 80 83 00 54  lwz r4,0x54(r3) # hitbox = obj1->hitbox;
    PATCH_B   0x800348c8, checkHitbox
    #PATCH_BL  0x8003bb00, drawHitbox
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x180 # how much to reserve
    .set SP_LR_SAVE,0x184 # this is what the game does
    .set SP_STR_BUF, 0x20
    .set SP_GPR_SAVE,0xA0

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


checkHitbox: # called by our hook, from the patch list.
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5, SP_LR_SAVE(r1)
    stmw    r3, SP_GPR_SAVE(r1)
    mr      r28, r3
    lwz     r4, 0x54(r3)
    cmpwi   r4, 0
    bne     .checkHitbox_end

    addi    r3, r1, SP_STR_BUF
    lwz     r4, 0x50(r3) # r4 = ObjectFileStruct*
    addi    r4, r4, 0x91 # name
    li      r5, 11
    CALL    strncpy

    # r5: name
    addi    r5, r1, SP_STR_BUF
    li      r6, 0
    stb     r6, 11(r5) # ensure null terminated

    bl      .checkHitbox_getpc
    .checkHitbox_getpc: mflr r9

    addi    r3, r9, msg_noHitbox - .checkHitbox_getpc # msg
    mr      r4, r28 # addr
    CALL    OSReport

    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    lwz     r4, 0x54(r3) # replaced
    JUMP    0x80034cc4, r0

.checkHitbox_end:
    lwz     r5, SP_LR_SAVE(r1)
    mtlr    r5 # restore LR
    lmw     r3, SP_GPR_SAVE(r1)
    addi    r1, r1, STACK_SIZE # restore stack ptr
    lwz     r4, 0x54(r3) # replaced
    JUMP    0x800348cc, r0


.macro VERTEX x, y, z, r, g, b, a, s, t
    stw     \x,  -0x8000(r29) # X
    stw     \y,  -0x8000(r29) # Y
    stw     \z,  -0x8000(r29) # Z
    stb     \r,  -0x8000(r29) # R
    stb     \g,  -0x8000(r29) # G
    stb     \b,  -0x8000(r29) # B
    stb     \a,  -0x8000(r29) # A
    #stw     \s,  -0x8000(r29) # S
    #stw     \t,  -0x8000(r29) # T
.endm


# work on rendering hitboxes. disabled for now.
.if 0
drawHitbox: # replace a do-nothing call in objRender
    # r30 = ObjInstance*
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    li      r3,  0x1
    li      r4,  0x4
    li      r5,  0x1
    li      r6,  0x5
    CALL    gxSetBlendMode
    li      r3,  0x1
    CALL    gxSetPeControl_ZCompLoc_
    li      r3,  0x7
    li      r4,  0x0
    li      r5,  0x0
    li      r6,  0x7
    li      r7,  0x0
    CALL    GXSetTevAlphaIn


    CALL    gxResetVtxDescr
    li      r3,  0x09 # POS
    li      r4,  0x01 # DIRECT
    CALL    gxSetVtxDescr
    li      r3,  0x0B # COL0
    li      r4,  0x01 # DIRECT
    CALL    gxSetVtxDescr
    li      r3,  0x0D # TEX0
    li      r4,  0x00 # NONE
    CALL    gxSetVtxDescr

    li      r3,  0x80 # DRAW_QUADS
    li      r4,  2 # vat
    li      r5,  4 # nVtxs
    CALL    gxBeginVtxs

    lis     r29, 0xCC01
    lis     r3,  0x3F80
    lis     r4,  0xBF80
    li      r5,  0xFF
    li      r6,  0x7F
    li      r0,  0

    VERTEX  r3, r3, r3, r5, r5, r5, r6, r0, r0 #  1,  1,  1
    VERTEX  r3, r4, r3, r5, r5, r5, r6, r0, r0 #  1, -1,  1
    VERTEX  r4, r4, r3, r5, r5, r5, r6, r0, r0 # -1, -1,  1
    VERTEX  r4, r3, r3, r5, r5, r5, r6, r0, r0 # -1,  1,  1

    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr
.endif

msg_noHitbox: .string "Object %08X '%s' has no hitbox!"
