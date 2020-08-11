drawItem_FOV:
    mr      r7,  r3
    lfs     f1,  (fovOverride - mainLoop)(r14)
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r5,  (SP_FLOAT_TMP+4)(r1)
    mr      r3,  r7
    addi    r4,  r14, (s_FOV - mainLoop)
    blr

adjItem_FOV: # r3 = amount to adjust by
    #LOAD    r5, fovY
    addi    r5,  r14, fovOverride - mainLoop
    lfs     f2,  (five   - mainLoop)(r14) # f2 = step
    lfs     f3,  (five   - mainLoop)(r14) # f3 = min
    lfs     f4,  (fovMax - mainLoop)(r14) # f4 = max
    mflr    r8
    bl      adjItem_float
    mtlr    r8

    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r6,  (SP_FLOAT_TMP+4)(r1)
    LOADWH  r5,  saveData
    STOREB  r6,  SAVEDATA_FOV+saveData, r5
    blr

fovOverride: .float 60

##########################################################################

drawItem_player:
    addi    r4,  r14, (s_Player - mainLoop)
    addi    r5,  r14, (s_Krystal - mainLoop)
    LOADW   r6,  PATCH_STATE_PTR
    lbz     r6,  CUR_CHAR_ADDR(r6)
    andi.   r6,  r6,  0x7F
    cmpwi   r6,  0
    beq     .drawPlayer_krystal
    addi    r5,  r14, (s_Fox - mainLoop)
.drawPlayer_krystal:
    blr

adjItem_player: # r3 = amount to adjust by
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  CUR_CHAR_ADDR(r5)
    xori    r6,  r6, 1
    stb     r6,  CUR_CHAR_ADDR(r5)
    LOADWH  r5,  saveData
    STOREB  r6,  SAVEDATA_CUR_CHAR+saveData, r5
    blr

##########################################################################

drawItem_gameSpeed:
    addi    r4,  r14, (s_gameSpeed - mainLoop)
    LOADWH  r5,  physicsTimeScale
    LOADFL2 f1,  physicsTimeScale, r5 # f1 = speed
    lfs     f4,  (gameSpeedDiv - mainLoop)(r14) # f4 = 60/100
    fdivs   f1,  f1,  f4 # f1 = speed%
    fctiw   f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r5,  (SP_FLOAT_TMP+4)(r1)
    blr

adjItem_gameSpeed: # r3 = amount to adjust by
    LOAD    r5,  physicsTimeScale
    lfs     f2,  (gameSpeedMin - mainLoop)(r14) # f2 = 15
    fmr     f3,  f2 # f3 = 15
    lfs     f4,  (gameSpeedMax - mainLoop)(r14) # f4 = 240
    b       adjItem_float

##########################################################################

drawItem_autoSave:
    addi    r4,  r14, (s_autoSave - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    #LOADW   r7,  PATCH_STATE_PTR
    #lbz     r6,  AUTOSAVE_ENABLED(r7)
    LOADWH  r7,  saveData
    LOADBL2 r6,  (saveData+0x10), r7
    andi.   r6,  r6, 0x80
    cmpwi   r6,  0
    beq     .drawAutoSave_off
    addi    r5,  r14, (s_on - mainLoop)

.drawAutoSave_off:
    blr

adjItem_autoSave: # r3 = amount to adjust by
    #LOADW   r5, PATCH_STATE_PTR
    #lbz     r6, AUTOSAVE_ENABLED(r5)
    #xori    r6, r6, 1
    #stb     r6, AUTOSAVE_ENABLED(r5)
    LOADWH  r7,  saveData
    LOADBL2 r6,  (saveData+0x10), r7
    xori    r6,  r6,  0x80
    STOREB  r6,  (saveData+0x10), r7
    blr

##########################################################################

drawItem_camPad:
    addi    r4,  r14, (s_camPad - mainLoop)
    addi    r5,  r14, (s_pad3 - mainLoop)
    LOADW   r7,  PATCH_STATE_PTR
    lbz     r6,  CAMERA_OPTIONS(r7)
    andi.   r6,  r6,  CAMERA_OPTION_PAD3
    beq     .drawCamPad_off
    addi    r5,  r14, (s_pad1 - mainLoop)

.drawCamPad_off:
    blr

adjItem_camPad: # r3 = amount to adjust by
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  CAMERA_OPTIONS(r5)
    xori    r6,  r6, CAMERA_OPTION_PAD3
    stb     r6,  CAMERA_OPTIONS(r5)
    LOADWH  r5,  saveData
    STOREB  r6,  SAVEDATA_CAMERA_OPTIONS+saveData, r5
    blr

##########################################################################

drawItem_camInvX:
    addi    r4,  r14, (s_camInvX - mainLoop)
    addi    r5,  r14, (s_Normal - mainLoop)
    LOADW   r7,  PATCH_STATE_PTR
    lbz     r6,  CAMERA_OPTIONS(r7)
    andi.   r6,  r6,  CAMERA_OPTION_INVERTX
    beq     .drawCamInvX_off
    addi    r5,  r14, (s_invert - mainLoop)

.drawCamInvX_off:
    blr

adjItem_camInvX: # r3 = amount to adjust by
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  CAMERA_OPTIONS(r5)
    xori    r6,  r6, CAMERA_OPTION_INVERTX
    stb     r6,  CAMERA_OPTIONS(r5)
    LOADWH  r5,  saveData
    STOREB  r6,  SAVEDATA_CAMERA_OPTIONS+saveData, r5
    blr

##########################################################################

drawItem_camInvY:
    addi    r4,  r14, (s_camInvY - mainLoop)
    addi    r5,  r14, (s_Normal - mainLoop)
    LOADW   r7,  PATCH_STATE_PTR
    lbz     r6,  CAMERA_OPTIONS(r7)
    andi.   r6,  r6,  CAMERA_OPTION_INVERTY
    beq     .drawCamInvY_off
    addi    r5,  r14, (s_invert - mainLoop)

.drawCamInvY_off:
    blr

adjItem_camInvY: # r3 = amount to adjust by
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  CAMERA_OPTIONS(r5)
    xori    r6,  r6, CAMERA_OPTION_INVERTY
    stb     r6,  CAMERA_OPTIONS(r5)
    LOADWH  r5,  saveData
    STOREB  r6,  SAVEDATA_CAMERA_OPTIONS+saveData, r5
    blr

##########################################################################

drawItem_widescreen:
    addi    r4,  r14, (s_widescreen - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r7,  renderFlags
    andi.   r7,  r7,  8 # widescreen
    beqlr
    addi    r5,  r14, (s_on - mainLoop)
    blr

adjItem_widescreen: # r3 = amount to adjust by
    LOADWH  r7,  renderFlags
    LOADWL2 r3,  renderFlags, r7
    srwi    r3,  r3,  3
    xori    r3,  r3,  1
    andi.   r3,  r3,  1

    LOADWH  r5,  saveData
    STOREB  r3,  SAVEDATA_WIDESCREEN+saveData, r5
    JUMP    setWidescreen, r0

##########################################################################

.if 0
# need to figure out how to reload text and fonts.
# may only be feasible on title screen.
# logic for switching language from European version
# doesn't appear to be present in this version.

drawItem_language:
    addi    r4,  r14, (s_language - mainLoop)
    LOADW   r6,  curLanguage
    slwi    r6,  r6,  1
    addi    r5,  r14, .languageNames - mainLoop
    lhzx    r5,  r5,  r6
    add     r5,  r5,  r14
    blr

adjItem_language: # r3 = amount to adjust by (0=A button)
    LOADWH  r5,  curLanguage
    LOADWL2 r4,  curLanguage, r5
    cmpwi   r3,  0
    bne     .adjItem_language_notA
    li      r3,  1 # A button = increase
.adjItem_language_notA:

    # do the adjustment and wrap
    add     r4,  r4,  r3
    cmpwi   r4,  6
    blt     .adjItem_language_notLast
    li      r4,  0
.adjItem_language_notLast:
    cmpwi   r4,  0
    bge     .adjItem_language_notFirst
    li      r4,  5
.adjItem_language_notFirst:
    STOREW  r4,  curLanguage, r5
    #JUMP    0x8001a234, r3
    #CALL    0x8001bbd8
    li      r3, 3
    li      r4, 2
    JUMP    0x8001a66c, r5
    blr

.languageNames:
    .short s_English  - mainLoop
    .short s_French   - mainLoop
    .short s_German   - mainLoop
    .short s_Italian  - mainLoop
    .short s_Japanese - mainLoop
    .short s_Spanish  - mainLoop
.endif
