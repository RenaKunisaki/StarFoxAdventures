# temporary patch to diagnose a game crash that happened to me.
.text
.include "common.s"

.set ENABLE_DRAW_HITBOX, 1

# define patches
patchList:
    PATCH_ID "HitBox " # must be 7 chars
    # 800348c8 80 83 00 54  lwz r4,0x54(r3) # hitbox = obj1->hitbox;
    PATCH_B   0x800348c8, checkHitbox
.if ENABLE_DRAW_HITBOX
    # these are do-nothing calls before and after drawing each object.
    # but it's not safe to hook them to draw things unless we figure out
    # how to properly back up and restore the graphics state, so that
    # we don't get weird artifacts.
    #PATCH_BL  0x8003bb00, drawHitbox
    #PATCH_BL  0x8003b9d4, drawHitbox
    PATCH_BL  0x8005c664, drawHitbox # replace quakeSpellTextureFn_8016dbf4()
    #PATCH_BL  0x8005c6a4, drawHitbox # replace renderGlows()
    #PATCH_BL  0x8005c52c, drawHitbox # replace renderParticles()
.endif
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x180 # how much to reserve
    .set SP_LR_SAVE,0x184 # this is what the game does
    .set SP_STR_BUF, 0x20
    .set SP_SAVE_R,  0x20
    .set SP_SAVE_G,  0x24
    .set SP_SAVE_B,  0x28
    .set SP_SAVE_A,  0x2C
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
    stfs    \x,  -0x8000(r29) # X
    stfs    \y,  -0x8000(r29) # Y
    stfs    \z,  -0x8000(r29) # Z
    stb     \r,  -0x8000(r29) # R
    stb     \g,  -0x8000(r29) # G
    stb     \b,  -0x8000(r29) # B
    stb     \a,  -0x8000(r29) # A
    #stw     \s,  -0x8000(r29) # S
    #stw     \t,  -0x8000(r29) # T
.endm


# work on rendering hitboxes. disabled for now.
# right now it just draws a rainbow square below the player.
.if ENABLE_DRAW_HITBOX
drawHitbox:
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)

    #CALL    0x8016dbf4 # replaced
    #CALL    0x800799c0
    #CALL    0x800796f0
    #CALL    0x80079804

    bl      .drawHitbox_getpc
    .drawHitbox_getpc: mflr r14

    lfs     f1,  (f_0 - .drawHitbox_getpc)(r14)
    lfs     f2,  (f_0 - .drawHitbox_getpc)(r14)
    lfs     f3,  (f_0 - .drawHitbox_getpc)(r14)
    lfs     f4,  (f_0 - .drawHitbox_getpc)(r14)
    li      r3,  0
    stw     r3,  SP_STR_BUF(r1)
    addi    r4,  r1,  SP_STR_BUF
    CALL    0x8025c2d4 # GXSetFog

    #li      r3,  0x1
    #li      r4,  0x3
    #li      r5,  0x0
    #CALL    0x80070310 #gxSetZMode_
    li      r3,  0x1
    li      r4,  0x4
    li      r5,  0x2
    li      r6,  0x5
    CALL    gxSetBlendMode
    #li      r3,  0x1
    #CALL    gxSetPeControl_ZCompLoc_
    #li      r3,  0x7
    #li      r4,  0x0
    #li      r5,  0x0
    #li      r6,  0x7
    #li      r7,  0x0
    #CALL    GXSetTevAlphaIn
    #li      r3,  0x0
    #CALL    0x80258b24 #gxSetNumTextures_80258b24

    # XXX we need to reset these afterward.
    CALL    gxResetVtxDescr
    li      r3,  0x09 # POS
    li      r4,  0x01 # DIRECT
    CALL    gxSetVtxDescr
    li      r3,  0x0B # COL0
    li      r4,  0x01 # DIRECT
    CALL    gxSetVtxDescr
    #li      r3,  0x0D # TEX0
    #li      r4,  0x00 # NONE
    #CALL    gxSetVtxDescr

    CALL    0x8000f54c # getCameraMtxs
    li      r4,  0
    CALL    0x8025d0a8 # gxSetMtx
    li      r3,  0x0
    CALL    0x8025d124 # gxSetTexMtxIdx

    LOADWH  r6,  playerMapOffsetX
    LOADFL2 f1,  playerMapOffsetX, r6
    LOADFL2 f2,  playerMapOffsetZ, r6
    LOADW   r6,  pPlayer
    cmpwi   r6,  0
    beq     .drawHitbox_end
    lfs     f3,  0x0C(r6) # player X
    lfs     f5,  0x14(r6) # player Z
    fsubs   f1,  f3,  f1  # player X - playerMapOffsetX
    fsubs   f3,  f5,  f2  # player Z - playerMapOffsetZ
    lfs     f2,  0x10(r6) # player Y
    lfs     f4,  (f_1 - .drawHitbox_getpc)(r14) # radius
    lis     r29, 0xCC01
    li      r3,  0xFF # R
    li      r4,  0x00 # G
    li      r5,  0x00 # B
    li      r6,  0x40 # A
    bl      drawSphere

.drawHitbox_end:
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr

f_0:  .float 0.0
f_1:  .float 1.0

drawSphere:
    # f1, f2, f3 = X, Y, Z
    # f4 = radius
    # r3, r4, r5, r6 = R, G, B, A
    # expects r14 = .drawHitbox_getpc, r29 = 0xCC010000
    .set NUM_SPHERE_STACKS, 4
    .set NUM_SPHERE_SLICES, 4
    .set NUM_SPHERE_VTXS, NUM_SPHERE_STACKS * NUM_SPHERE_SLICES
    .set SIZEOF_FLOAT, 4
    .set SIZEOF_VEC3F, SIZEOF_FLOAT * 3

    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r7
    stw     r7,  SP_LR_SAVE(r1)
    stw     r3,  SP_SAVE_R(r1)
    stw     r4,  SP_SAVE_G(r1)
    stw     r5,  SP_SAVE_B(r1)
    stw     r6,  SP_SAVE_A(r1)

    #addi    r7,  r14, sphereVtxs - .drawHitbox_getpc
    #li      r8,  NUM_SPHERE_VTXS

    li      r15, 1 # stack
.drawSphere_nextStack:
    li      r17, 0
    li      r3,  0x98 # DRAW_TRIANGLE_STRIP
    li      r4,  2  # vat
    li      r5,  NUM_SPHERE_SLICES * 4 # nvtxs
    CALL    gxBeginVtxs

    li      r16, 1 # slice
    lis     r29, 0xCC01
.drawSphere_nextSlice:

    # note we draw vtxs out of order: 1, 3, 2, 4

    # int v1 = (stack * nSlices) + slice;
    addi    r7,  r14, sphereVtxs - .drawHitbox_getpc
    mulli   r3,  r15, NUM_SPHERE_SLICES # v1 = stack * nSlices
    add     r3,  r3,  r16 # v1 += slice
    mulli   r3,  r3,  SIZEOF_VEC3F
    add     r3,  r3,  r7  # v1 = &vtxs[v1]
    mr      r4,  r3       # save for later
    bl      .drawSphere_drawVtx
    addi    r17, r17, 1

    # int v3 = v1 - nSlices;
    subi    r3,  r4,  NUM_SPHERE_SLICES * SIZEOF_VEC3F
    bl      .drawSphere_drawVtx
    addi    r17, r17, 1

    # int v2 = v1 - 1;
    subi    r3,  r4,  SIZEOF_VEC3F
    bl      .drawSphere_drawVtx
    addi    r17, r17, 1

    # int v4 = v3 - 1;
    # v3 is v1 - nSlices, so v4 = v1 - (nSlices + 1)
    subi    r3,  r4,  (NUM_SPHERE_SLICES + 1) * SIZEOF_VEC3F
    bl      .drawSphere_drawVtx
    addi    r17, r17, 1

    addi    r16, r16, 1 # slice++
    cmpwi   r16, NUM_SPHERE_SLICES+1
    blt     .drawSphere_nextSlice

    addi    r15, r15, 1 # stack++
    cmpwi   r15, NUM_SPHERE_STACKS+1
    blt     .drawSphere_nextStack

    # reuse function epilogue to save a few bytes
    b       .drawHitbox_end

.drawSphere_drawVtx:
    # params: r3=&vtx, f1=X, f2=Y, f3=Z, f4=radius
    # clobbers: r3, f5, f6, f7
    #li      r3,  0
    #stw     r3,  -0x8000(r29)
    #stw     r3,  -0x8000(r29)
    #stw     r3,  -0x8000(r29)
    #stb     r3,  -0x8000(r29)
    #stb     r3,  -0x8000(r29)
    #stb     r3,  -0x8000(r29)
    #stb     r3,  -0x8000(r29)
    #blr

    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r7
    stw     r7,  SP_LR_SAVE(r1)

    lfs     f5,  0(r3)
    lfs     f6,  4(r3)
    lfs     f7,  8(r3)
    #fmuls   f5,  f5,  f4
    #fmuls   f6,  f6,  f4
    #fmuls   f7,  f7,  f4
    #fadds   f5,  f5,  f1
    #fadds   f6,  f6,  f2
    #fadds   f7,  f7,  f3

    fmr     f8,  f1
    fmr     f9,  f2
    fmr     f10, f3
    fmr     f1,  f5
    fmr     f2,  f6
    fmr     f3,  f7
    mr      r4,  r3
    addi    r3,  r14, s_vtx - .drawHitbox_getpc
    mr      r5,  r17
    mr      r6,  r15
    mr      r7,  r16
    CALL    OSReport
    fmr     f1,  f8
    fmr     f2,  f9
    fmr     f3,  f10

    stfs    f5,  -0x8000(r29) # X
    stfs    f6,  -0x8000(r29) # Y
    stfs    f7,  -0x8000(r29) # Z
    lwz     r3,  SP_SAVE_R(r1)
    stb     r3,  -0x8000(r29) # R
    lwz     r3,  SP_SAVE_G(r1)
    stb     r3,  -0x8000(r29) # G
    lwz     r3,  SP_SAVE_B(r1)
    stb     r3,  -0x8000(r29) # B
    lwz     r3,  SP_SAVE_A(r1)
    stb     r3,  -0x8000(r29) # A
    #blr
    b       .drawHitbox_end

sphereVtxs: # precomputed by sphere.py
    .float  0,  1,  2,  3,  4,  5,  6,  7,  8,  9
    .float 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    .float 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    .float 30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    .float 40, 41, 42, 43, 44, 45, 46, 47, -48, -49
    .float -50, -51, -52, -53, -54, -55, -56, -57, -58, -59
    .float -60, -61, -62, -63, -64, -65, -66, -67, -68, -69

    .float  0.00000, -1.00000,  0.00000
    .float  0.00000, -1.00000,  0.00000
    .float  0.00000, -1.00000,  0.00000
    .float  0.00000, -1.00000,  0.00000
    .float  0.00000, -0.50000,  0.86603
    .float  0.86603, -0.50000,  0.00000
    .float  0.00000, -0.50000, -0.86603
    .float -0.86603, -0.50000,  0.00000
    .float  0.00000,  0.00000,  1.00000
    .float  1.00000,  0.00000,  0.00000
    .float  0.00000,  0.00000, -1.00000
    .float -1.00000,  0.00000,  0.00000
    .float  0.00000,  0.50000,  0.86603
    .float  0.86603,  0.50000,  0.00000
    .float  0.00000,  0.50000, -0.86603
    .float -0.86603,  0.50000,  0.00000

s_vtx: .string "V %08X %d %d,%d %f %f %f sc %f"
.endif # ENABLE_DRAW_HITBOX


msg_noHitbox: .string "Object %08X '%s' has no hitbox!"
