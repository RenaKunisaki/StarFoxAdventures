# patches to override various HUD settings.

doHudOverrides:
    # do minimap size/opacity override
    LOADWH  r9, minimapWidth
    LOADBL2 r6, minimapMode, r9
    cmpwi   r6, 0 # map mode?
    bne     .noMapOverride # don't change alpha, use default size

    lbz    r4, (minimapAlphaOverride - mainLoop)(r14)
    cmpwi  r4, 0xFF
    beq    .noMapAlphaOverride
    STOREH r4, minimapAlpha, r9

.noMapAlphaOverride:
    lhz     r6, (minimapSizeOverride - mainLoop)(r14)
    cmpwi   r6, 0 # override enabled?
    beq     .noMapOverride

    # override position.
    li      r4, 0x0002 # X pos
    li      r5, 0x01D8 # Y pos
    lis     r7, 0x4000 # X pos float
    #       r6, r9 are already set
    b       .setMapDimensions

.noMapOverride:
    # use default position/size.
    li      r4, 0x0032 # X pos
    li      r5, 0x01B8 # Y pos
    li      r6, 0x0078 # width and height
    lis     r7, 0x4248 # X pos float
    #       r9 is already set
    b       .setDimensionsNoSize # let it resize itself.


.setMapDimensions:
    # r4 = X position
    # r5 = Y position
    # r6 = width and height
    # r7 = Y position as float
    # r9 = high half of &minimapWidth

    # override size
    STOREW  r6, minimapWidth,  r9
    STOREW  r6, minimapHeight, r9

.setDimensionsNoSize:
    li      r8, 0
    lis     r9, 0x8013

    # additional width override to prevent shuddering.
    # must be done even for normal size to undo the patch
    # for larger size.
    addi    r3, r9, 0x25FA
    sth     r6, 0(r3)
    icbi    r8, r3 # flush instruction cache

    # some instructions to patch for height
    addi    r3, r9, 0x24CA
    sth     r6, 0(r3)
    sth     r6, 8(r3)
    icbi    r8, r3 # flush instruction cache

    # override Y position.
    ori     r3, r9, 0x266A
    sth     r5, 0(r3)
    icbi    r8, r3 # flush instruction cache

    # override X position.
    ori     r3, r9, 0x267A
    sth     r4, 0(r3) # the box
    icbi    r8, r3 # flush instruction cache

    ori     r3, r9, 0x26A6
    sth     r4, 0(r3) # the box clip
    icbi    r8, r3 # flush instruction cache

    ori     r3, r9, 0x2A92
    sth     r4, 0(r3) # texture related
    sth     r4, 4(r3) # texture related
    icbi    r8, r3 # flush instruction cache

    lis     r5, 0x803E
    stw     r7, 0x2210(r5) # the clip offset (float)

.end:
    blr
