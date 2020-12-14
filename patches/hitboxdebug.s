# temporary patch to diagnose a game crash that happened to me.
.text
.include "common.s"
.include "globals.s"

.set ENABLE_DRAW_HITBOX, 1

# define patches
patchList:
    PATCH_ID "HitBox " # must be 7 chars
    # 800348c8 80 83 00 54  lwz r4,0x54(r3) # hitbox = obj1->hitbox;
    PATCH_B   0x800348c8, checkHitbox
.if ENABLE_DRAW_HITBOX
    # these are do-nothing calls before and after drawing each object.
    #PATCH_BL  0x8003bb00, drawHitbox # after draw
    #PATCH_BL  0x8003b9d4, drawHitbox # before draw
    #PATCH_BL  0x8005c664, drawHitbox # replace quakeSpellTextureFn_8016dbf4()
    #PATCH_BL  0x8005c6a4, drawHitbox # replace renderGlows()
    #PATCH_BL  0x8005c52c, drawHitbox # replace renderParticles()
    # but let's use this one, so we get *all* objects.
    PATCH_BL  0x80041ae0, drawHitbox # objGetCurModelPtr() in objRenderModel
.endif
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,  0x200 # how much to reserve
    .set SP_LR_SAVE,  0x204 # this is what the game does
    .set SP_RET,       0x1C
    .set SP_STR_BUF,   0x20
    .set SP_SAVE_R,    0x20
    .set SP_SAVE_G,    0x24
    .set SP_SAVE_B,    0x28
    .set SP_SAVE_A,    0x2C
    .set SP_DESCR_SAVE,0x30
    .set SP_GPR_SAVE, 0x110

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


.if ENABLE_DRAW_HITBOX
drawHitbox: # r31 = ObjInstance*
    stwu    r1, -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)

    CALL    0x8002b588 # objGetCurModelPtr() (replaced)
    stw     r3,  SP_RET(r1)

    LOADW   r4,  PATCH_STATE_PTR
    lbz     r4,  DEBUG_RENDER_FLAGS(r4)
    andi.   r4,  r4,  DEBUG_RENDER_HITBOXES
    beq     .drawHitbox_end2

    addi    r3,  r1,  SP_DESCR_SAVE
    CALL    gxGetVtxDescrs

    #CALL    0x8016dbf4 # replaced: quakeSpellTextureFn_8016dbf4()
    CALL    0x800799c0
    CALL    0x800796f0
    CALL    0x80079804

    li       r3,  0
    CALL     0x8025b71c # gxResetIndCmd
    li       r3,  1
    CALL     0x80259e58 # gxSetNumColors
    li       r3,  0
    CALL     0x802581e0 # gxSetNumTextures

    bl      .drawHitbox_getpc
    .drawHitbox_getpc: mflr r14

    #lfs     f1,  (f_0 - .drawHitbox_getpc)(r14)
    #lfs     f2,  (f_0 - .drawHitbox_getpc)(r14)
    #lfs     f3,  (f_0 - .drawHitbox_getpc)(r14)
    #lfs     f4,  (f_0 - .drawHitbox_getpc)(r14)
    #li      r3,  0
    #stw     r3,  SP_STR_BUF(r1)
    #addi    r4,  r1,  SP_STR_BUF
    #CALL    0x8025c2d4 # GXSetFog

    # this is close, but the boxes' alpha changes sometimes,
    # or they turn black, and they obscure the actual model.
    # also we need to reset these when done so objects render correctly.

    li      r3,  0x1 # compare enable
    li      r4,  GX_LESS # func
    li      r5,  0x1 # update enable
    CALL    0x80070310 #gxSetZMode_
    li      r3,  GX_BM_BLEND  # type
    li      r4,  GX_BL_SRCALPHA # srcFactor
    li      r5,  GX_BL_DSTCLR # dstFactor
    li      r6,  GX_LO_NOP    # logicOp
    CALL    gxSetBlendMode
    #li      r3,  0x1
    #CALL    gxSetPeControl_ZCompLoc_
    li      r3,  0x7
    li      r4,  0x0
    li      r5,  0x0
    li      r6,  0x7
    li      r7,  0x0
    CALL    GXSetTevAlphaIn
    #li      r3,  0x0
    #CALL    0x80258b24 #gxSetNumTextures_80258b24

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
    #li      r3,  0x0
    #CALL    0x8025d124 # gxSetTexMtxIdx

    LOADWH  r6,  playerMapOffsetX
    LOADFL2 f1,  playerMapOffsetX, r6
    LOADFL2 f2,  playerMapOffsetZ, r6

    #LOADW   r6,  pPlayer
    mr      r6,  r31
    cmpwi   r6,  0
    beq     .drawHitbox_end

    lbz     r4,  0xAD(r6) # curModel
    slwi    r4,  r4,  2
    lwz     r3,  0x7C(r6) # Model**
    lwzx    r3,  r3,  r4  # Model*
    cmpwi   r3,  0
    beq     .drawHitbox_end

    lwz     r4,  0(r3)    # ModelFileHeader*
    lbz     r19, 0xF7(r4) # nSpheres
    lwz     r20, 0x50(r3) # Model->curHitSpherePos
    lwz     r21, 0x54(r6) # obj->hitstate

    #lbz     r4,  0xAC(r21)
    #lbz     r5,  0xAD(r21)
    #lbz     r6,  0xAE(r21)
    #lbz     r7,  0xAF(r21)
    #addi    r3,  r14,  s_hitState - .drawHitbox_getpc
    ##creqv   4*cr1+eq,4*cr1+eq,4*cr1+eq # magic incantation to print floats
    #CALL    debugPrintf

.drawHitbox_nextSphere:
    cmpwi   r19, 0
    beq     .drawHitbox_end
    subi    r19, r19, 1

    #addi    r3,  r3,  0x20
    lfs     f1,  0x04(r20) # sphere X
    lfs     f2,  0x08(r20) # sphere Y
    lfs     f3,  0x0C(r20) # sphere Z
    lfs     f4,  0x00(r20) # radius

    lis     r29, 0xCC01
    li      r3,  0xFF # R
    li      r4,  0x00 # G
    li      r5,  0x00 # B
    #lbz     r5,  0xAC(r21)
    #lbz     r3,  0xAF(r21)
    #slwi    r3,  r3,  4
    #slwi    r5,  r5,  4
    #slwi    r6,  r6,  4
    #sub     r4,  r4,  r6
    #lbz     r3,  0x03(r21)
    #slwi    r3,  r3,  4
    #lbz     r4,  0x14(r21)
    #slwi    r4,  r4,  4
    #lbz     r5,  0x16(r21)
    #slwi    r5,  r5,  4
    li      r6,  0x20 # A
    bl      drawSphere

    addi    r20, r20, 0x10
    #addi    r21, r21, 0x18
    b       .drawHitbox_nextSphere

.drawHitbox_end:
    addi    r3,  r1,  SP_DESCR_SAVE
    CALL    gxSetVtxDescrs
.drawHitbox_end2:
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    lwz     r3,  SP_RET(r1)
    addi    r1,  r1, STACK_SIZE # restore stack ptr
    blr

f_0:  .float 0.0

drawSphere:
    # f1, f2, f3 = X, Y, Z
    # f4 = radius
    # r3, r4, r5, r6 = R, G, B, A
    # expects r14 = .drawHitbox_getpc, r29 = 0xCC010000
    .set NUM_SPHERE_STACKS, 4
    .set NUM_SPHERE_SLICES, 4
    .set NUM_SPHERE_FACES, NUM_SPHERE_STACKS * NUM_SPHERE_SLICES
    .set NUM_SPHERE_VTXS,  NUM_SPHERE_FACES  * 4
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

    lis     r29, 0xCC01
    li      r15, 0    # vtx idx
    addi    r17, r14, sphereVtxs    - .drawHitbox_getpc
    addi    r18, r14, sphereVtxIdxs - .drawHitbox_getpc
.drawSphere_nextSlice:
    li      r16, 0    # stack idx
    li      r3,  0x98 # DRAW_TRIANGLE_STRIP
    li      r4,  2  # vat
    li      r5,  NUM_SPHERE_STACKS * NUM_SPHERE_SLICES # nvtxs
    CALL    gxBeginVtxs
.drawSphere_nextVtx:
    lbzx    r3,  r18, r15 # get vtx idx
    mulli   r3,  r3,  SIZEOF_VEC3F
    add     r3,  r3,  r17 # v1 = &vtxs[v1]
    lfs     f5,  0(r3)
    lfs     f6,  4(r3)
    lfs     f7,  8(r3)
    fmuls   f5,  f5,  f4
    fmuls   f6,  f6,  f4
    fmuls   f7,  f7,  f4
    fadds   f5,  f5,  f1
    fadds   f6,  f6,  f2
    fadds   f7,  f7,  f3

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

    addi    r15, r15, 1
    cmpwi   r15, NUM_SPHERE_VTXS
    beq     .drawHitbox_end2 # reuse function epilogue to save a few bytes

    addi    r16, r16, 1
    cmpwi   r16, NUM_SPHERE_STACKS * NUM_SPHERE_SLICES
    beq     .drawSphere_nextSlice
    b       .drawSphere_nextVtx

sphereVtxs: # precomputed by sphere.py
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

sphereVtxIdxs: # unwrapped vtx idxs
    # faces are:
    #  0  1 | 1  2 | 2  3 | 3  0
    #  4  5 | 5  6 | 6  7 | 7  4
    # ------+------+------+------
    #  4  5 | 5  6 | 6  7 | 7  4
    #  8  9 | 9 10 |10 11 |11  8
    # ------+------+------+------
    #  8  9 | 9 10 |10 11 |11  8
    # 12 13 |13 14 |14 15 |15 12
    # ------+------+------+------
    # 12 13 |13 14 |14 15 |15 12
    #  0  1 | 1  2 | 2  3 | 3  0

    .byte  0,  1,  4,  5
    .byte  1,  2,  5,  6
    .byte  2,  3,  6,  7
    .byte  3,  0,  7,  4

    .byte  4,  5,  8,  9
    .byte  5,  6,  9, 10
    .byte  6,  7, 10, 11
    .byte  7,  4, 11,  8

    .byte  8,  9, 12, 13
    .byte  9, 10, 13, 14
    .byte 10, 11, 14, 15
    .byte 11,  8, 15, 12

    .byte 12, 13,  0,  1
    .byte 13, 14,  1,  2
    .byte 14, 15,  2,  3
    .byte 15, 12,  3,  0

s_hitState: .byte 0x84
    .string "Poly:%02X ?:%02X %02X State:%02X\x83\n"
s_hitbox: .string "\x84%08X P %f %f %f R %f\x83\n"
.endif # ENABLE_DRAW_HITBOX

msg_noHitbox: .string "Object %08X '%s' has no hitbox!"
