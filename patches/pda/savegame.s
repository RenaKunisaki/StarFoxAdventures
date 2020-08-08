saveLoadHook:
    # hook loadSaveSettings. replace some unused settings.
    # r3 = &saveData
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r3,  SP_GPR_SAVE(r1)

    bl      .saveLoadHook_getpc
    .saveLoadHook_getpc: mflr r14
    subi    r14,  r14,  .saveLoadHook_getpc - mainLoop

    lbz     r5,  SAVEDATA_CUR_CHAR(r3)
    andi.   r5,  r5,  0x7F
    LOADWH  r9,  controllerStates
    LOADHL2 r6,  controllerStates, r9 # buttons
    andi.   r0,  r6,  PAD_BUTTON_Y | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_A | PAD_BUTTON_MENU
    cmpwi   r0,  PAD_BUTTON_X | PAD_BUTTON_MENU
    bne     .saveLoadHook_notC
    xori    r5,  r5, 0x80

.saveLoadHook_notC:
    stb     r5,  SAVEDATA_CUR_CHAR(r3)
    LOADW   r9,  PATCH_STATE_PTR

    lbz     r4,  SAVEDATA_EXTRA_OPTIONS(r3)
    stb     r4,  EXTRA_FEATURE_FLAGS(r9)
    lbz     r4,  SAVEDATA_CAMERA_OPTIONS(r3)
    stb     r4,  CAMERA_OPTIONS(r9)
    lbz     r4,  SAVEDATA_CUR_CHAR(r3)
    stb     r4,  CUR_CHAR_ADDR(r9)
    lbz     r4,  SAVEDATA_MAP_OPACITY(r3)
    cmpwi   r4,  0
    bne     .saveLoadHook_mapAlphaNotZero
    li      r4,  255
.saveLoadHook_mapAlphaNotZero:
    stb     r4,  (minimapAlphaOverride - mainLoop)(r14)

    mr      r8,  r3
    lbz     r3,  SAVEDATA_FOV(r3)
    cmpwi   r3,  0
    bne     .saveLoadHook_fovNotZero
    li      r3,  60
.saveLoadHook_fovNotZero:
    bl      intToFloat
    stfs    f1,  (fovOverride - mainLoop)(r14)
    mr      r3,  r8

    lbz     r4,  SAVEDATA_OPTIONS(r3)
    andi.   r5,  r4,  SAVEDATA_OPTION_MAP_SIZE
    stb     r5,  (menuMapSize - mainLoop)(r14)
    slwi    r5,  r5,  1
    addi    r6,  r14, .mapSizes - mainLoop
    lhzx    r5,  r5,  r6
    sth     r5,  (minimapSizeOverride - mainLoop)(r14)

    andi.   r5,  r4,  SAVEDATA_OPTION_PDA_MODE
    srwi    r5,  r5,  2
    li      r6,  1
    cmpwi   r5,  3
    bne     .saveLoadHook_pdaOn
    li      r6,  0
    li      r5,  0
.saveLoadHook_pdaOn:
    LOADWH  r7,  pdaOn
    STOREB  r6,  pdaOn, r7
    STOREB  r5,  minimapMode, r7

    #lbz     r4,  SAVEDATA_OPTIONS(r3)
    andi.   r5,  r4,  SAVEDATA_OPTION_FUR_FX
    bnel    adjItem_furEffect

    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800e7fd4, r3 # skip reading unused settings.
