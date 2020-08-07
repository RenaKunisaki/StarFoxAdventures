drawItem_PDAHUD:
    addi    r4,  r14, (s_PDAHUD - mainLoop)
    LOADWH  r7,  pdaOn
    LOADBL2 r6,  pdaOn, r7
    cmpwi   r6,  0
    beq     .draw_pda_off
    LOADBL2 r6,  minimapMode, r7
    b       .draw_pda

.draw_pda_off:
    li      r6,  3

.draw_pda:
    slwi    r6,  r6,  1 # r6 = offs into pdaModeStrs
    addi    r6,  r6,  .pdaModeStrs - mainLoop # r6 = addr of str offs
    lhzx    r6,  r6,  r14 # r6 = str offs from mainLoop
    add     r5,  r6,  r14 # r5 = str
    blr

.pdaModeStrs:
    .short s_map     - mainLoop
    .short s_compass - mainLoop
    .short s_info    - mainLoop
    .short s_off     - mainLoop


adjItem_PDAHUD:
    LOADWH  r7,  pdaOn
    LOADBL2 r6,  minimapMode, r7 # shares same upper half addr
    LOADBL2 r8,  pdaOn, r7
    cmpwi   r8,  0
    bne     .adjPDA
    li      r6,  3

.adjPDA:
    add     r6,  r6,  r3 # r3 = adjust amount
    andi.   r6,  r6,  3
    cmpwi   r6,  3
    beq     .pdaOff
    STOREB  r6,  minimapMode, r7
    li      r8,  1
    b       .setPDA
.pdaOff:
    li      r8,  0

.setPDA:
    STOREB  r8,  pdaOn, r7
    stb     r8,  (menuWasPdaOn - mainLoop)(r14) # don't reset

    LOADWH  r5,  saveData
    LOADBL2 r7,  SAVEDATA_OPTIONS+saveData, r5
    andi.   r7,  r7,  (~SAVEDATA_OPTION_PDA_MODE) & 0xFFFF
    slwi    r6,  r6,  2
    or      r7,  r7,  r6
    STOREB  r7,  SAVEDATA_OPTIONS+saveData, r5
    blr

##########################################################################

# putting these here because Game Settings is crowded
# and PDA Menu is a reasonable place for UI settings.

drawItem_subtitles:
    addi    r4,  r14, (s_subtitles - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r6,  bSubtitlesEnabled
    cmpwi   r6,  0
    beqlr
    addi    r5,  r14, (s_on - mainLoop)
    blr

adjItem_subtitles: # r3 = amount to adjust by
    LOADW   r3,  bSubtitlesEnabled
    xori    r3,  r3,  1
    LOADWH  r5,  SAVEDATA_SUBTITLES+saveData
    STOREB  r3,  SAVEDATA_SUBTITLES+saveData, r5
    JUMP    setSubtitlesEnabled, r0

##########################################################################

drawItem_rumble:
    addi    r4,  r14, (s_rumble - mainLoop)
    addi    r5,  r14, (s_off - mainLoop)
    LOADWH  r6,  enableRumble
    LOADBL2 r6,  enableRumble, r6
    cmpwi   r6,  0
    beqlr
    addi    r5,  r14, (s_on - mainLoop)
    blr

adjItem_rumble: # r3 = amount to adjust by
    LOADWH  r6,  enableRumble
    LOADBL2 r3,  enableRumble, r6
    xori    r3,  r3,  1
    STOREB  r3,  enableRumble, r6
    LOADWH  r5,  SAVEDATA_RUMBLE+saveData
    STOREB  r3,  SAVEDATA_RUMBLE+saveData, r5
    blr

##########################################################################

drawItem_rumbleBlur:
    addi    r4,  r14, (s_rumbleBlur - mainLoop)
    li      r8,  EXTRA_FEATURE_RUMBLE_BLUR
drawExtraFeatureItem:
    addi    r5,  r14, (s_off - mainLoop)
    LOADW   r7,  PATCH_STATE_PTR
    lbz     r6,  EXTRA_FEATURE_FLAGS(r7)
    and.    r6,  r6,  r8
    b       debugText_printOffOn

adjItem_rumbleBlur: # r3 = amount to adjust by
    li      r7,  EXTRA_FEATURE_RUMBLE_BLUR
toggleExtraFeatureItem:
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  EXTRA_FEATURE_FLAGS(r5)
    xor     r6,  r6,  r7
    stb     r6,  EXTRA_FEATURE_FLAGS(r5)
    LOADWH  r5,  SAVEDATA_EXTRA_OPTIONS+saveData
    STOREB  r6,  SAVEDATA_EXTRA_OPTIONS+saveData, r5
    blr
