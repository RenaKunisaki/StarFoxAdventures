# patches to override various HUD settings.

doHudOverrides:
    # do minimap size/opacity override
    LOADWH r5, minimapWidth
    lbz    r4, (minimapAlphaOverride - mainLoop)(r14)
    cmpwi  r4, 0xFF
    beq    .noAlphaOverride
    STOREH r4, minimapAlpha, r5

.noAlphaOverride:
    li      r4, 100 # default height
    LOADBL2 r6, minimapMode, r5
    cmpwi   r6, 0 # map mode?
    bne     .noMapOverride # don't change size
    lhz     r6, (minimapSizeOverride - mainLoop)(r14)
    cmpwi   r6, 0
    beq     .noMapOverride
    mr      r4, r6
    STOREW  r4, minimapWidth, r5

.noMapOverride:
    STOREW r4, minimapHeight, r5
    LOAD   r5, 0x801324CA # some instructions to patch for height
    sth    r4, 0(r5)
    sth    r4, 8(r5)
    li     r4, 0
    icbi   r4, r5 # flush instruction cache

    blr

    # 8013266A = 01B8, -> 01D8 to move map down
    # to move left, all following must be changed:
    # 8013267A = 0032 (the box)
    # 801326A6 = 0032 (the box clip)
    # 80132A92, 80132A96 (some texture shit)
    # 803e2210 (float) the clip offset
    # 80132A94 could just be changed to "add r5, r28, r3" unless a branch points directly at it
    # this also doesn't affect the D-pad icon and arrows.
    # also with huge map, it shudders sometimes, trying to shrink
    # back to normal size.
