.ascii "dbgrendr" # 8 byte file ID for debug

drawItem_textureDebug:
    addi    r4,  r14, (s_Textures - mainLoop)
    blr

adjItem_textureDebug: # r3 = amount to adjust by (0=A button)
    li      r4,  MENU_ID_TEXTURES
    b       .debug_setMenu

#######################################################################

drawItem_hitboxes:
    addi    r4,  r14, (s_Hitboxes - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r6,  PATCH_STATE_PTR
    lbz     r6,  DEBUG_RENDER_FLAGS(r6)
    andi.   r6,  r6,  DEBUG_RENDER_HITBOXES
    beq     .drawItem_hitboxes_off
    addi    r5,  r14, (s_on - mainLoop)

.drawItem_hitboxes_off:
    blr

adjItem_hitboxes: # r3 = amount to adjust by (0=A button)
    LOADW   r3,  PATCH_STATE_PTR
    lbz     r4,  DEBUG_RENDER_FLAGS(r3)
    xori    r4,  r4,  DEBUG_RENDER_HITBOXES
    stb     r4,  DEBUG_RENDER_FLAGS(r3)
    blr

#######################################################################

drawItem_blurFilter:
    addi    r4,  r14, (s_BlurFilter - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADB   r6,  bEnableBlurFilter
    cmpwi   r6,  0
    b       debugText_printOffOn

adjItem_blurFilter: # r3 = amount to adjust by (0=A button)
    LOADWH  r3,  bEnableBlurFilter
    LOADBL2 r4,  bEnableBlurFilter, r3
    xori    r4,  r4,  1
    STOREB  r4,  bEnableBlurFilter, r3
    blr

#######################################################################

drawItem_motionBlur:
    addi    r4,  r14, (s_MotionBlur - mainLoop)
    li      r5,  0
    LOADWH  r7,  motionBlurIntensity
    LOADBL2 r6,  bEnableMotionBlur, r7
    cmpwi   r6,  0
    beqlr
    LOADFL2 f1,  motionBlurIntensity, r7
    fctiwz  f1,  f1
    stfd    f1,  SP_FLOAT_TMP(r1)
    lwz     r5,  (SP_FLOAT_TMP+4)(r1)
    blr

adjItem_motionBlur: # r3 = amount to adjust by (0=A button)
    #neg     r3,  r3
    mr      r8,  r9
    mflr    r7
    bl      intToFloat
    mtlr    r7
    mr      r9,  r8

    li      r3,  1 # enable blur
    LOADWH  r5,  motionBlurIntensity
    LOADFL2 f2,  motionBlurIntensity, r5
    fadds   f2,  f2,  f1
    lfs     f3,  (f_zero - mainLoop)(r14)
    lfs     f4,  (twoFiveFive - mainLoop)(r14)
    fcmpo   0,   f2,  f3
    ble     .adjItem_motionBlur_lz
    fcmpo   0,   f2,  f4
    ble     .adjItem_motionBlur_store
    fmr     f2,  f4 # if(blur > 255) blur = 255
    b       .adjItem_motionBlur_store
.adjItem_motionBlur_lz:
    fmr     f2,  f3 # if(blur < 0) blur = 0
    li      r3,  0  # disable blur
.adjItem_motionBlur_store:
    STOREB  r3,  bEnableMotionBlur, r5
    STOREF  f2,  motionBlurIntensity, r5
    blr

#######################################################################

drawItem_heatEffect:
    addi    r4,  r14, (s_HeatEffect - mainLoop)
    LOADWH  r6,  heatEffectIntensity
    LOADWL2 r5,  heatEffectIntensity, r6
    blr

adjItem_heatEffect: # r3 = amount to adjust by
    LOADWH  r6,  heatEffectIntensity
    LOADWL2 r5,  heatEffectIntensity, r6
    add     r5,  r5,  r3
    andi.   r5,  r5,  0xFF
    STOREW  r5,  heatEffectIntensity, r6
    blr

#######################################################################

drawItem_monochrome:
    addi    r4,  r14, (s_Monochrome - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADB   r6,  bEnableMonochromeFilter
    cmpwi   r6,  0
    b       debugText_printOffOn

adjItem_monochrome: # r3 = amount to adjust by (0=A button)
    LOADWH  r3,  bEnableMonochromeFilter
    LOADBL2 r4,  bEnableMonochromeFilter, r3
    xori    r4,  r4,  1
    STOREB  r4,  bEnableMonochromeFilter, r3
    blr

#######################################################################

drawItem_spiritVision:
    addi    r4,  r14, (s_SpiritVision - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADB   r6,  bEnableSpiritVision
    cmpwi   r6,  0
    b       debugText_printOffOn

adjItem_spiritVision: # r3 = amount to adjust by (0=A button)
    LOADWH  r3,  bEnableSpiritVision
    LOADBL2 r4,  bEnableSpiritVision, r3
    xori    r4,  r4,  1
    STOREB  r4,  bEnableSpiritVision, r3
    blr

#######################################################################

# not really useful
#drawItem_distortion:
#    addi    r4,  r14, (s_Distortion - mainLoop)
#    addi    r5,  r14, (s_off - mainLoop)
#    LOADB   r6,  bEnableDistortionFilter
#    cmpwi   r6,  0
#    b       debugText_printOffOn
#
#adjItem_distortion: # r3 = amount to adjust by (0=A button)
#    LOADWH  r3,  bEnableDistortionFilter
#    LOADBL2 r4,  bEnableDistortionFilter, r3
#    xori    r4,  r4,  1
#    STOREB  r4,  bEnableDistortionFilter, r3
#    blr

#######################################################################

drawItem_colorFilter:
    addi    r4,  r14, (s_ColorFilter - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADB   r6,  bEnableColorFilter
    cmpwi   r6,  0
    b       debugText_printOffOn

adjItem_colorFilter: # r3 = amount to adjust by (0=A button)
    LOADWH  r3,  bEnableColorFilter
    LOADBL2 r4,  bEnableColorFilter, r3
    xori    r4,  r4,  1
    STOREB  r4,  bEnableColorFilter, r3
    blr

#######################################################################

drawItem_colorFilterR:
    addi    r4,  r14, (s_Red - mainLoop)
    LOADWH  r6,  colorFilterColor+0
    LOADBL2 r5,  colorFilterColor+0, r6
    blr

adjItem_colorFilterR: # r3 = amount to adjust by
    LOADWH  r6,  colorFilterColor+0
    LOADBL2 r5,  colorFilterColor+0, r6
    add     r5,  r5,  r3
    STOREB  r5,  colorFilterColor+0, r6
    blr

#######################################################################

drawItem_colorFilterG:
    addi    r4,  r14, (s_Green - mainLoop)
    LOADWH  r6,  colorFilterColor+1
    LOADBL2 r5,  colorFilterColor+1, r6
    blr

adjItem_colorFilterG: # r3 = amount to adjust by
    LOADWH  r6,  colorFilterColor+1
    LOADBL2 r5,  colorFilterColor+1, r6
    add     r5,  r5,  r3
    STOREB  r5,  colorFilterColor+1, r6
    blr

#######################################################################

drawItem_colorFilterB:
    addi    r4,  r14, (s_Blue - mainLoop)
    LOADWH  r6,  colorFilterColor+2
    LOADBL2 r5,  colorFilterColor+2, r6
    blr

adjItem_colorFilterB: # r3 = amount to adjust by
    LOADWH  r6,  colorFilterColor+2
    LOADBL2 r5,  colorFilterColor+2, r6
    add     r5,  r5,  r3
    STOREB  r5,  colorFilterColor+2, r6
    blr

# XXX move these color filters to another menu,
# and also there's a "colorScale" value...
