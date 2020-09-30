.ascii "util    " # 8 byte file ID for debug
# some generic code used by multiple submodules.

intToFloat: # convert r3 from int to float.
    # stores result in f1.
    # clobbers f9, r0, r3, SP_FLOAT_TMP
    lfd     f9,  (floatMagic - mainLoop)(r14) # load constant into f9
    lis     r0,  0x4330
    stw     r0,  SP_FLOAT_TMP(r1)   # store exponent part for integer
    xoris   r3,  r3,  0x8000        # invert sign of integer
    stw     r3,  SP_FLOAT_TMP+4(r1) # store fraction part for integer
    lfd     f1,  SP_FLOAT_TMP(r1)   # load integer in double format into f1
    fsub    f1,  f1,  f9            # f1 contains converted integer
    frsp    f1,  f1                 # double to single
    blr

floatMagic:    .int 0x43300000,0x80000000


adjItem_float: # r3=adj, r5=&val, f2=adjStep, f3=min, f4=max
    lfs     f1,  0(r5) # f1 = val
    cmpwi   r3,  0
    bge     .noNegFloat
    fneg    f2,  f2
.noNegFloat:
    fadds   f1,  f1,  f2 # f2 = -adjStep
    fcmpo   0,   f1,  f3 # f1 < min?
    bge     .float_moreThanMin
    # XXX wraparound doesn't work nicely due to rounding error.
    # sometims you'll jump from 10% to 200% instead of to 0%.
    # (fun fact: Super Smash Bros Melee's debug menus have this
    # exact same bug.)
    #fmr     f1,  f4 # f1 = max
    fmr     f1,  f3 # f1 = min
.float_moreThanMin:
    fcmpo   0,   f1,  f4 # f1 > max?
    ble     .float_lessThanMax
    #fmr     f1,  f3 # f1 = min
    fmr     f1,  f4 # f1 = max
.float_lessThanMax:
    stfs    f1,  0(r5)
    blr


menuSetFixedWidth:
    LOADW   r3,  PATCH_STATE_PTR
    li      r4,  11
    stb     r4,  FORCE_TEXT_WIDTH(r3)
    blr

menuHideHud:
    li      r3,  1
    LOADWH  r4,  hudHidden
    STOREB  r3,  hudHidden, r4
    li      r3,  0
    LOADWH  r4,  pdaOn
    STOREB  r3,  pdaOn, r4
    blr


menuSetTextColor: # r3 = RRGGBBAA
    rlwinm  r4,  r3,  16, 0xFF # G
    rlwinm  r5,  r3,  24, 0xFF # B
    rlwinm  r6,  r3,   0, 0xFF # A
    rlwinm  r3,  r3,   8, 0xFF # R
    JUMP    gameTextSetColor, r0


menuPrintf: # r19:X, r20:Y, r4:fmt, r5...:args
    mflr    r31
    addi    r3,  r1, SP_STR_BUF
    CALL    sprintf
    addi    r3,  r1, SP_STR_BUF
    mtlr    r31
menuDrawStr2:
    mr      r5,  r19 # X pos
    mr      r6,  r20 # Y pos
menuDrawStr: # r3:str, r5:X, r6:Y
    li      r4,  MENU_TEXTBOX_ID # box type
    JUMP    gameTextShowStr, r0
