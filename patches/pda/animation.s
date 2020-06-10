doAnimation:
    # expects r19 = boxAddr, f1 = menuAnimTimer
    lfs    f2,  (f_menuWidth   - mainLoop)(r14) # f2 = width
    lfs    f3,  (f_menuHeight  - mainLoop)(r14) # f3 = height
    lfs    f4,  (f_centerX     - mainLoop)(r14) # f4 = X
    lfs    f5,  (f_centerY     - mainLoop)(r14) # f5 = Y
    lfs    f9,  (two           - mainLoop)(r14) # r9 = 2
    lfs    f8,  (twoFiveFive   - mainLoop)(r14) # r8 = 255
    fmuls  f8, f8, f1 # f8 = scaled alpha
    fctiwz f0, f8
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r20, (SP_FLOAT_TMP+4)(r1)

    fmuls  f2, f2, f1 # f2 = scaled width
    fctiwz f0, f2
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x00(r19) # set box width

    fdivs  f2, f2, f9 # f2 /= 2
    fsubs  f6, f4, f2 # f6 = scaled X pos
    fctiwz f0, f6
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x0C(r19) # set box X pos

    fmuls  f3, f3, f1 # f3 = scaled height
    fctiwz f0, f3
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x02(r19) # set box height

    fdivs  f3, f3, f9 # f2 /= 2
    fsubs  f6, f5, f3 # f6 = scaled Y pos
    fctiwz f0, f6
    stfd   f0, SP_FLOAT_TMP(r1)
    lwz    r6, (SP_FLOAT_TMP+4)(r1)
    sth    r6, 0x0E(r19) # set box Y pos
    blr


doOpenAnimation:
    mflr   r23
    lwz    r3, (menuAnimTimer - mainLoop)(r14)
    cmpwi  r3, 0
    bne    .noOpenSound
    li     r4, 0x03E5
    CALL   audioPlaySound

.noOpenSound:
    # do animation.
    lfs    f1, (menuAnimTimer - mainLoop)(r14)
    lfs    f2, (menuAnimSpeed - mainLoop)(r14)
    fadds  f1, f1, f2
    lfs    f3, (one - mainLoop)(r14)
    fcmpo  0, f1, f3
    blt    .animContinue
    fmr    f1, f3 # clamp anim timer to 1
.animContinue:
    lwz    r19, (boxAddr - mainLoop)(r14)
    stfs   f1,  (menuAnimTimer - mainLoop)(r14) # f1 = timer
    mtlr   r23
    b      doAnimation


doCloseAnimation:
    mflr   r23
    lwz    r3, (menuAnimTimer - mainLoop)(r14)
    cmpwi  r3, 0
    beqlr  # menu is fully closed.

    lfs    f1, (menuAnimTimer - mainLoop)(r14)
    lfs    f2, (menuAnimSpeed - mainLoop)(r14)
    fsubs  f1, f1, f2
    lfs    f3, (zero - mainLoop)(r14)
    fcmpo  0, f1, f3
    bgt    .closeAnimContinue
    fmr    f1, f3 # clamp anim timer to 0

    # restore textbox config
    lwz    r19, (boxAddr - mainLoop)(r14)
    li     r4, 0x0186
    sth    r4, 0x00(r19) # width
    li     r4, 0x00C8
    sth    r4, 0x02(r19) # height
    li     r4, 0x0028
    sth    r4, 0x0C(r19) # X
    li     r4, 0x0032
    sth    r4, 0x0E(r19) # Y
.closeAnimContinue:
    li     r3, 1 # menu isn't fully closed, so draw it.
    b      .animContinue


menuAnimTimer: .float 0
menuAnimSpeed: .float 0.125 # 1 / 8
#menuAnimSpeed: .float 0.066666667 # 1 / 15
#menuAnimSpeed: .float 0.0166666667 # 1 / 60
