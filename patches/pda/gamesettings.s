drawItem_FOV:
    mr      r7, r3
    lfs     f1, (fovOverride - mainLoop)(r14)
    fctiwz  f1, f1
    stfd    f1, SP_FLOAT_TMP(r1)
    lwz     r5, (SP_FLOAT_TMP+4)(r1)
    mr      r3, r7
    addi    r4, r14, (s_FOV - mainLoop)
    blr

adjItem_FOV: # r3 = amount to adjust by
    #LOAD    r5, fovY
    addi    r5, r14, fovOverride - mainLoop
    lfs     f2, (five   - mainLoop)(r14) # f2 = step
    lfs     f3, (five   - mainLoop)(r14) # f3 = min
    lfs     f4, (fovMax - mainLoop)(r14) # f4 = max
    b       adjItem_float

fovOverride: .float 60

##########################################################################

drawItem_player:
    addi    r4, r14, (s_Player - mainLoop)
    addi    r5, r14, (s_Krystal - mainLoop)
    LOADW   r6, PATCH_STATE_PTR
    lbz     r6, CUR_CHAR_ADDR(r6)
    andi.   r6, r6, 0x7F
    cmpwi   r6, 0
    beq     .drawPlayer_krystal
    addi    r5, r14, (s_Fox - mainLoop)
.drawPlayer_krystal:
    blr

adjItem_player: # r3 = amount to adjust by
    LOADW   r5, PATCH_STATE_PTR
    lbz     r6, CUR_CHAR_ADDR(r5)
    xori    r6, r6, 1
    stb     r6, CUR_CHAR_ADDR(r5)
    blr

##########################################################################

drawItem_gameSpeed:
    addi    r4, r14, (s_gameSpeed - mainLoop)
    LOADWH  r5, physicsTimeScale
    LOADFL2 f1, physicsTimeScale, r5 # f1 = speed
    lfs     f4, (gameSpeedDiv - mainLoop)(r14) # f4 = 60/100
    fdivs   f1, f1, f4 # f1 = speed%
    fctiw   f1, f1
    stfd    f1, SP_FLOAT_TMP(r1)
    lwz     r5, (SP_FLOAT_TMP+4)(r1)
    blr

adjItem_gameSpeed: # r3 = amount to adjust by
    LOAD    r5, physicsTimeScale
    lfs     f2, (gameSpeedMin - mainLoop)(r14) # f2 = 15
    fmr     f3, f2 # f3 = 15
    lfs     f4, (gameSpeedMax - mainLoop)(r14) # f4 = 240
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
