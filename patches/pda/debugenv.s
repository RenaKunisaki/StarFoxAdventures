.ascii "debugenv" # 8 byte file ID for debug

drawItem_debugEnv:
    addi    r4,  r14, s_Environment - mainLoop
    blr

adjItem_debugEnv: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_PAGE_DEBUG_ENV
    b       .debugSetPage

#######################################################################

drawItem_timeOfDay:
    addi    r4,  r14, (s_timeOfDay - mainLoop)
    LOADW   r8,  0x803dd12c # pSkyStruct
    cmpwi   r8,  0
    beq     .drawItem_timeOfDay_none
    lfs     f8,  0x020C(r8)

    lfs     f2,  (f_3600 - mainLoop)(r14)
    fdivs   f1,  f8,  f2 # hours
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r5,  (SP_FLOAT_TMP+4)(r1)

    lfs     f2,  (f_60 - mainLoop)(r14)
    fdivs   f1,  f8,  f2 # minutes
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r6,  (SP_FLOAT_TMP+4)(r1)

    li      r9,  60
    divwu   r7,  r6,  r9 # r7 (c) = minutes (a) // 60 (b)
    mulli   r8,  r7,  60 # r8 (d) = (minutes // 60) (c) * 60 (b)
    sub     r6,  r6,  r8 # r6 = minutes (a) - d

    fctiwz  f1,  f8 # seconds
    stfd    f1,  SP_FLOAT_TMP(r1)

    lwz     r7,  (SP_FLOAT_TMP+4)(r1)
    divwu   r8,  r7,  r9 # r8 (c) = seconds (a) // 60 (b)
    mulli   r9,  r8,  60 # r9 (d) = (seconds // 60) (c) * 60 (b)
    sub     r7,  r7,  r9 # r7 = seconds (a) - d

    # return r4=str, r5=hours, r6=minutes, r7=seconds
    blr

.drawItem_timeOfDay_none:
    addi   r4,  r14, s_timeOfDayNA - mainLoop
    blr

adjItem_timeOfDay: # r3 = amount to adjust by (0=A button)
    LOADW   r4,  0x803dd12c # pSkyStruct
    cmpwi   r4,  0
    beqlr

    mulli   r3,  r3,  60
    mflr    r20
    bl      intToFloat
    lfs     f2,  0x020C(r4)
    fadds   f2,  f2,  f1
    stfs    f2,  0x020C(r4)
    mtlr    r20
    blr
