# patches to override various HUD settings.

doHudOverrides:
    # do FOV override
    LOADW   r5, 0x803dd548
    cmpwi   r5, 0
    bne     .noFovOverride # don't apply in first person
    LOADW   r5, 0x803dd524
    cmpwi   r5, 0
    beq     .noFovOverride
    lwz     r6, (fovOverride - mainLoop)(r14)
    stw     r6, 0xB4(r5)
    #stw     r6, 0x118(r5)
    STOREW  r6, 0x803dd4d0, r5
    LOADWH  r5, fovY
    STOREW  r6, fovY, r5
    LOADW   r5, 0x803dd530
    cmpwi   r5, 0
    beq     .noFovOverride
    stw     r6, 0x70(r5)

.noFovOverride:
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

# HUD elements:
# - Health bar:
#     X: 80121636 (hardcoded)
#     Y: 803e1fac (float)
# - Magic bar:
#     X: 803dbad8 (int)
#     Y: 803dbadc (int)
# - Tricky face:
#     X: 803e1f9c (float)
#     Y: 803e1fb0 (float)
# - Tricky foods meter:
#     X: 8012191e (hardcoded)
#     Y: 803e1fb4 (float)
# - Spellstone:
#     X: 8012173e, 80121746 (hardcoded, different)
#     Y: same as health
#     80121774 bl hudDrawElement
#     801217cc maybe drawing another one
# - Buttons:
#     803e1fcc, 803e1fe4, 803e1fcc: float, partial X pos
#     803e1fdc: float, C menu page icon X pos
#     803e1fe0: float, Y button circle X pos
#     803e1fe8: float, partial Y pos
#     803e1fec: float, right half of A button circle X pos
#     803e1ff0: float, right half of A button circle Y pos
#     803e1ff4: float, A button icon X pos
#     803e1ff8: float, A button icon Y pos
#     803e1ffc: float, A button text background Y pos
#     803e2000: float, A button text background X pos (ignored?)
#     803e2004: float, B button text background Y pos
#     803e2008: float, B button text background X pos (ignored?)
#     803e200c: float, B button left half Y pos
#     803e2014: float, Y button item icon X pos
#     80123b0e: hardcoded, A button text background left side X pos
#       not real position? subtracted from something
#     80124022: hardcoded, Y button '?' X pos
#     80124026: hardcoded, Y button '?' Y pos
#     might involve GameText boxes
#     Tricky face Y pos is also used
#     probably other hardcoded positions for texts and shit here
# - Timer:
#     X:
#     Y:
# - Air meter:
#     803dd7f8: (byte) head Y pos
#     803dd7f9: (byte) head X pos offset
#     8011fa12: (hardcoded) head X pos
#     803dbaec: (byte) head Y pos offset
#     8011fa6e: (hardcoded) bar X pos
# - Timing meter (eg for test of fear, tied up by lightfoots)
# - Counters (bottom right):
#   Y: 80122836, 801228c6 (hardcoded, all numbers)
#   Y: 803e1fb8 (float, all icons)
#   - Fuel Cells:
#       X: 803a931c (float, varies)
#   - Moon Seeds:
#       X: 803a9318 (float, varies)
#   - Fireflies:
#       X: 803a9314 (float, varies)
#   - Tricky foods (inventory):
#       X: 803a92f0 (float, varies)
#   - Bomb Spores:
#       X: 803a92fc (float, varies)
#   - Scarabs:
#       X: 803a92f8 (float, varies)

# misc:
# 803e1e70 X, used to align counters to their icons (0.5)
# 803e2018 X, ??? (140.0)
# 803e2038 Y, ??? (90.0)
# 803e203c Y, ??? (94.0)

# 80396880 HUD matrix
#  80396880 width (0x3b4ccccd) -> 0x3b5fcccd
#  8039688c X pos (-1.0) -> 0xBF8B0000
#  80396894 height (0xBB888889) -> 0xBB9B8889
#  8039689c Y pos ( 1.0) -> 0x3F900000
#  with these changes it reaches the extreme edges, but the map isn't
#  positioned properly, and it's stretched.
#  also, changing Y pos this way breaks text for buttons.

# during combat the map changes to "no map data" and actually hides.
# with our override we can still see it.
# should try to only apply the override if there is in fact a map
# to display, and maybe hide/shrink the map if there isn't, or at least
# show something more useful in the box instead of "no map data".
# this also happens when feeding Tricky
