# trippy visuals
.text
.include "common.s"

#803dce54 float possibly filter related

GECKO_WRITE32 0x8000FC4C, 0x60000000 # NOP an FOV write
GECKO_WRITE32 0x8000FC2C, 0x60000000 # NOP an aspect write
GECKO_BEGIN_PATCH 0x80020C78 # li r3, 0
# free: r3-r12
# we use:
# r12: base to store to game vars
# r11: player
# r10: constants

b start

constants:
    # how quickly the colors will cycle
    .set CONST_COLOR_MASK,0x00
    .int 0x01010101

    # force these bits in the color values
    .set CONST_COLOR_OR,0x04
    .int 0xC0C0C0C0

    .set CONST_COLOR1_MASK,0x08
    .int 0x7F7F7F7F

    .set CONST_BLUR_BASE,0x0C
    .float 2.0

    .set CONST_ASPECT_BASE,0x10
    .float 1.7777

    .set CONST_MTX_BASE,0x14
    .float 0.5

    .set CONST_MTX_MUL,0x18
    .float 0.1

    .set CONST_TIME_BASE,0x1C
    .float 30.0

    .set CONST_TIME_SCALE,0x20
    .float 60.0

start:
    # get player, check if it's null
    LOAD    r11, pPlayer
    lwz     r11, 0(r11)
    cmpwi   r11, 0
    beq     end

    # get our constants in r10
    bl   .getpc
    .getpc:
        mflr r10
        #mtlr r7 # restore LR
        addi r10, r10, (constants - .getpc)@l

    # enable some filters
    LOADWH  r12, bEnableBlurFilter # base address
    li      r4, 1
    STOREB  r4, bEnableMotionBlur, r12
    STOREB  r4, bEnableSpiritVision, r12

    # fuck with colors
    lbz     r3, 0x9A(r11) # various player vars
    lbz     r4, 0xA6(r11)
    lbz     r5, 0x22(r11)
    lbz     r6, 0x00(r11)
    lbz     r7, 0x05(r11)
    xor     r6, r6, r7
    rlwimi  r3,r4,8,16,23 # r3 |= ((r4 <<  8) & 0x0000FF00)
    rlwimi  r3,r5,16,8,15 # r3 |= ((r5 << 16) & 0x00FF0000)
    rlwimi  r3,r6,24,0,7  # r3 |= ((r6 << 24) & 0xFF000000)
    lwz     r6, CONST_COLOR_MASK(r10)
    lwz     r5, CONST_COLOR_OR(r10)
    and     r3, r3, r6

    LOADWL2 r4, 0x803db6d0, r12 # spirit vision color 1
    # this one is a signed offset
    add     r4, r4, r3
    #rotlwi  r4, r4, 1
    #or      r4, r4, r5
    lwz     r6, CONST_COLOR1_MASK(r10)
    and     r4, r4, r6
    STOREW  r4, 0x803db6d0, r12 # spirit vision color 1

    LOADWL2 r4, 0x803db6d4, r12 # spirit vision color 2
    rotlwi  r3, r3, 8
    add     r4, r4, r3
    #or      r4, r4, r5
    STOREW  r4, 0x803db6d4, r12 # spirit vision color 2

    LOADWL2 r4, 0x803db6d8, r12 # spirit vision color 3
    rotlwi  r3, r3, 8
    add     r4, r4, r3
    #and     r4, r4, r6
    STOREW  r4, 0x803db6d8, r12 # spirit vision color 3

    # this one makes things dark
    LOADWL2 r4, 0x803db6dc, r12 # spirit vision color 4
    rotlwi  r3, r3, 8
    add     r4, r4, r3
    #or      r4, r4, r5
    and     r4, r4, r6
    STOREW  r4, 0x803db6dc, r12 # spirit vision color 4

    LOADWL2 r4, 0x803deed4, r12 # color scale
    rotlwi  r3, r3, 8
    add     r4, r4, r3
    #or      r4, r4, r5
    #STOREW  r4, 0x803deed4, r12 # color scale

    #LOADWL2 r4, colorFilterColor, r12
    #add     r4, r4, r3
    ##rotlwi  r4, r4, 8
    ##or      r4, r4, r5
    #and     r4, r4, r6
    #STOREW  r4, colorFilterColor, r12

    # fuck with motion blur
    lfs     f1, 0x2c(r11) # player->vel.z
    #lfs     f1, 0x98(r11) # player->animTimer
    lfs     f2, CONST_BLUR_BASE(r10)
    fmuls   f1, f1, f2
    #fadds   f1, f1, f2
    fabs    f1, f1
    STOREF  f1, motionBlurIntensity, r12

    # fuck with aspect
    lfs     f1, 0x2C(r11) # player->vel.z
    lfs     f4, CONST_ASPECT_BASE(r10)
    fadds   f1, f1, f4 # base around original aspect
    fabs    f1, f1
    STOREF  f1, viewportAspect, f12
    #STOREF  f2, 0x803dc890, f12 # ortho var

    # fuck with TIME ITSELF
    lfs     f1, 0x98(r11) # player->animTimer
    lfs     f2, CONST_TIME_BASE(r10)
    lfs     f3, CONST_TIME_SCALE(r10)
    #fmuls   f1, f1, f3
    #fadds   f1, f1, f2
    fmadds  f1, f1, f3, f2
    STOREF  f1, physicsTimeScale, f12

    # fuck with perspective
    LOADWH  r12, 0x803e7640 # perspective mtx constant
    lfs     f1, 0x28(r11) # player->vel.y
    lfs     f2, CONST_MTX_BASE(r10)
    lfs     f3, CONST_MTX_MUL(r10)
    #fmuls   f1, f1, f3
    #fabs    f1, f1
    #fadds   f1, f1, f2
    fmadds  f1, f1, f3, f2
    fabs    f1, f1
    STOREF  f1, 0x803e7640, f12 # perspective

end:
    li r3, 0
GECKO_END_PATCH
