# patches to override various HUD settings.

doHudOverrides:
    mflr    r20
    bl      doHudElementOverrides # in hudelems.s

    # do FOV override
    LOADW   r5,  0x803dd548
    cmpwi   r5,  0
    bne     .noFovOverride # don't apply in first person
    LOADW   r5,  0x803dd524
    cmpwi   r5,  0
    beq     .noFovOverride
    lwz     r6,  (fovOverride - mainLoop)(r14)
    stw     r6,  0xB4(r5)
    #stw     r6,  0x118(r5)
    STOREW  r6,  0x803dd4d0, r5
    LOADWH  r5,  fovY
    STOREW  r6,  fovY, r5
    LOADW   r5,  0x803dd530
    cmpwi   r5,  0
    beq     .noFovOverride
    stw     r6,  0x70(r5)

.noFovOverride:
    # do minimap size/opacity override
    LOADWH  r9,  minimapWidth
    LOADBL2 r6,  minimapMode, r9
    cmpwi   r6,  0 # map mode?
    bne     .noMapOverride # don't change alpha, use default size

    lbz     r4,  (minimapAlphaOverride - mainLoop)(r14)
    cmpwi   r4,  0xFF
    beq     .noMapAlphaOverride
    STOREH  r4,  minimapAlpha, r9

.noMapAlphaOverride:
    lhz     r6,  (minimapSizeOverride - mainLoop)(r14)
    LOADW   r8,  0x803dd93c # map texture
    cmpwi   r8,  0
    bne     .haveMap

    # hide "no map data" message.
    # this "overrides the override" so to speak.
    # it will shrink the map down to nothing, regardless
    # of whether the map size is set to Normal or Huge.
    # when we do have a map, it'll spring back up again.
    LOADWL2 r6,  minimapWidth, r9
    cmpwi   r6,  2
    blt     .end

    # every frame the game will try to increase the map size
    # by 1, so the shrinkage is actually one unit less than
    # what we do here.
    subi    r6,  r6,  5
    b       .doMapSizeOverride

.haveMap: # we do have a map to show.
    cmpwi   r6,  0 # override enabled?
    beq     .noMapOverride
    cmpwi   r6,  0x100 # no pos override for small map
    blt     .doMapSizeOverride

    # override position.
.doMapPosOverride:
    li      r4,  0x0002 # X pos
    li      r5,  0x01D8 # Y pos
    lis     r7,  0x4000 # X pos float
    b       .setMapDimensions # r6, r9 are already set

.noMapOverride:
    # use default position/size.
    li      r6,  0x0078 # width and height
.doMapSizeOverride:
    li      r4,  0x0032 # X pos
    li      r5,  0x01B8 # Y pos
    lis     r7,  0x4248 # X pos float
    #       r9 is already set
    b       .setDimensionsNoSize # let it resize itself.


.setMapDimensions:
    # r4 = X position
    # r5 = Y position
    # r6 = width and height
    # r7 = Y position as float
    # r9 = high half of &minimapWidth

    # override size
    STOREW  r6,  minimapWidth,  r9
    STOREW  r6,  minimapHeight, r9

.setDimensionsNoSize:
    li      r8,  0
    lis     r9,  0x8013

    # additional width override to prevent shuddering.
    # must be done even for normal size to undo the patch
    # for larger size.
    addi    r3,  r9,  0x25FA
    sth     r6,  0(r3)
    icbi    r8,  r3 # flush instruction cache

    # some instructions to patch for height
    addi    r3,  r9,  0x24CA
    sth     r6,  0(r3)
    sth     r6,  8(r3)
    icbi    r8,  r3 # flush instruction cache

    # override Y position.
    ori     r3,  r9,  0x266A
    sth     r5,  0(r3)
    icbi    r8,  r3 # flush instruction cache

    # override X position.
    ori     r3,  r9,  0x267A
    sth     r4,  0(r3) # the box
    icbi    r8,  r3 # flush instruction cache

    ori     r3,  r9,  0x26A6
    sth     r4,  0(r3) # the box clip
    icbi    r8,  r3 # flush instruction cache

    ori     r3,  r9,  0x2A92
    sth     r4,  0(r3) # texture related
    sth     r4,  4(r3) # texture related
    icbi    r8,  r3 # flush instruction cache

    lis     r5,  0x803E
    stw     r7,  0x2210(r5) # the clip offset (float)

.end:
    mtlr    r20
    blr
