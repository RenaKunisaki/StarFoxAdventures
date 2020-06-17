doHudElementOverrides:
    addi  r4, r14, (hudElementOverrides_addrs - mainLoop) - 4
    addi  r5, r14, (hudElementOverrides_values_off - mainLoop) - 4
    lbz   r3, (hudFullScreen - mainLoop)(r14)
    cmpwi r3, 0
    beq   .doHudElementOverrides_next
    addi  r5, r14, (hudElementOverrides_values_on - mainLoop) - 4

.doHudElementOverrides_next:
    lwzu  r6, 4(r4) # get addr
    cmpwi r6, 0
    beq   .doHudElementOverrides_end
    lwzu  r7, 4(r5)
    stw   r7, 0(r6)
    b     .doHudElementOverrides_next

.doHudElementOverrides_end:
    blr

hudElementOverrides_addrs:
#    .int 0x80121634 # health bar X
#    .int 0x803e1fac # health bar Y
#    .int 0x803dbad8 # magic bar X
#    .int 0x803dbadc # magic bar Y
#    .int 0x803e1f9c # Tricky face X
#    .int 0x803e1fb0 # Tricky face Y
#    .int 0x8012191c # Tricky foods X
#    .int 0x803e1fb4 # Tricky foods Y
#    .int 0x8012173c # spellstone1 X
#    .int 0x80121744 # spellstone2 X
#    # spellstone Y is same as health bar
#    .int 0x803e1fcc # B button X
#    .int 0x803e1fdc # C menu page icon
#    .int 0x803e1fe0 # Y button circle X pos
#    .int 0x803e1fe4 # B button X
#    .int 0x803e1fe8 # partial Y pos
#    .int 0x803e1fec # right half of A button circle X pos
#    .int 0x803e1ff0 # right half of A button circle Y pos
#    .int 0x803e1ff4 # A button icon X pos
#    .int 0x803e1ff8 # A button icon Y pos
#    .int 0x803e1ffc # A button text background Y pos
#    .int 0x803e2000 # A button text background X pos (ignored?)
#    .int 0x803e2004 # B button text background Y pos
#    .int 0x803e2008 # B button text background X pos (ignored?)
#    .int 0x803e200c # B button left half Y pos
#    .int 0x803e2014 # Y button item icon X pos
#    .int 0x80123b0c # A button text background left side X pos
#    #       not real position? subtracted from something
#    .int 0x80124020 # Y button '?' X pos
#    .int 0x80124024 # Y button '?' Y pos
#    .int 0x803dd7f8 # bytes: air meter head Y pos, X pos offset
#    .int 0x8011fa10 # air meter head X pos
#    .int 0x803dbaec # bytes: air meter head Y pos offset, ?
#    .int 0x8011fa6c # air meter bar X pos
#    .int 0x80122834 # all counters Y pos
#    .int 0x801228c4 # all counters Y pos
    .int 0

hudElementOverrides_values_off:
#    .int    30 | 0x38040000 # health bar X
#    .float  31              # health bar Y
#    .int    30              # magic bar X
#    .int    70              # magic bar Y
#    .float  30              # Tricky face X
#    .float  92              # Tricky face Y
#    .int    64 | 0x38040000 # Tricky foods X
#    .float 102              # Tricky foods Y
#    .int   260 | 0x38000000 # spellstone1 X
#    .int   290 | 0x38000000 # spellstone2 X
#    .float 537              # B button X
#    .float 575              # C menu page icon X
#    .float 519              # Y button circle X pos
#    .float 561              # B button X
#    .float  41              # partial Y pos
#    .float 581              # right half of A button circle X pos
#    .float  46              # right half of A button circle Y pos
#    .float 560              # A button icon X pos
#    .float  54              # A button icon Y pos
#    .float  62              # A button text background Y pos
#    .float 508              # A button text background X pos (ignored?)
#    .float  87              # B button text background Y pos
#    .float 525              # B button text background X pos (ignored?)
#    .float  86              # B button left half Y pos
#    .float 518              # Y button item icon X pos
#    .int   525 | 0x20130000 # A button text background left side X pos
#    .int   534 | 0x38A00000 # Y button '?' X pos
#    .int    34 | 0x38C00000 # Y button '?' Y pos
#    .byte   0, 0, 0, 0      # air meter head Y pos, X pos offset, ?, ?
#    .int   181 | 0x38040000 # air meter head X pos
#    .byte   -2 , 3, 4, 2    # air meter head Y pos offset, ?, ?, ?
#    .int   180 | 0x3BC30000 # air meter bar X pos
#    .int   425 | 0x38C00000 # all counters Y pos
#    .int   425 | 0x38C00000 # all counters Y pos

hudElementOverrides_values_on:
#    .int     0 | 0x38040000 # health bar X
#    .float   1              # health bar Y
#    .int     0              # magic bar X
#    .int    32              # magic bar Y
#    .float   0              # Tricky face X
#    .float  52              # Tricky face Y
#    .int    34 | 0x38040000 # Tricky foods X
#    .float  56              # Tricky foods Y
#    .int   260 | 0x38000000 # spellstone1 X
#    .int   290 | 0x38000000 # spellstone2 X
#    .float 537              # B button X
#    .float 575              # C menu page icon X
#    .float 519              # Y button circle X pos
#    .float 561              # B button X
#    .float  11              # buttons partial Y pos
#    .float 581              # right half of A button circle X pos
#    .float  16              # right half of A button circle Y pos
#    .float 560              # A button icon X pos
#    .float  24              # A button icon Y pos
#    .float  32              # A button text background Y pos
#    .float 508              # A button text background X pos (ignored?)
#    .float  27              # B button text background Y pos
#    .float 525              # B button text background X pos (ignored?)
#    .float  26              # B button left half Y pos
#    .float 518              # Y button item icon X pos
#    .int   525 | 0x20130000 # A button text background left side X pos
#    .int   534 | 0x38A00000 # Y button '?' X pos
#    .int     4 | 0x38C00000 # Y button '?' Y pos
#    .byte   0, 0, 0, 0      # air meter head Y pos, X pos offset, ?, ?
#    .int   181 | 0x38040000 # air meter head X pos
#    .byte   -2 , 3, 4, 2    # air meter head Y pos offset, ?, ?, ?
#    .int   180 | 0x3BC30000 # air meter bar X pos
#    .int   455 | 0x38C00000 # all counters Y pos
#    .int   425 | 0x38C00000 # all counters Y pos

    # - Counters (bottom right):
    #   Y: 80122836, 801228c6 (hardcoded, all numbers)
    #   Y: 803e1fb8 (float, all icons)
    #   - Fuel Cells:
    #       X: 803a931c (float, varies)
    #       A: 803a92e8 (float, varies)
    #   - Moon Seeds:
    #       X: 803a9318 (float, varies)
    #       A: 803a92e4 (float, varies)
    #   - Fireflies:
    #       X: 803a9314 (float, varies)
    #       A: 803a92e0 (float, varies)
    #   - Tricky foods (bottom right):
    #       X: 803a92f0 (float, varies)
    #       A: 803a92bc (float, varies)
    #   - Bomb Spores:
    #       X: 803a92fc (float, varies)
    #       A: 803a92c8 (float, varies)
    #   - Scarabs:
    #       X: 803a92f8 (float, varies)
    #       A: 803a92c4 (float, varies)
    # I don't know where those X positions came from but they're wrong.
    # the counters start at X=591 and this increases by 40 for each
    # counter drawn.
    # we would need to override that logic.

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
# - Tricky task icon:
#     X:
#     Y:
# - Spellstone:
#     X: 8012173e, 80121746 (hardcoded, different)
#     Y: same as health
#     80121774 bl hudDrawElement
#     801217cc maybe drawing another one
# - Buttons:
#     803e1fcc, 803e1fe4: float, partial X pos
#     803e1fdc: float, C menu page icon X pos
#     803e1fe0: float, Y button circle X pos
#       - Y pos is same as Tricky face X pos
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
