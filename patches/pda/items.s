# pointers to the submodules.
# ENSURE THAT THESE TABLES MATCH.
# each draw function must have a corresponding adjust function,
# and they must be in the same order.
# (I could probably combine the tables into one alternating, but I
# think that would be slightly less efficient as I wouldn't be able
# to use lwzu to index them.)
itemDrawFuncs:
    .int drawItem_player    - mainLoop
    .int drawItem_PDAHUD    - mainLoop
    .int drawItem_bigMap    - mainLoop
    .int drawItem_mapAlpha  - mainLoop
    #.int drawItem_sound     - mainLoop
    .int drawItem_FOV       - mainLoop
    .int drawItem_volMusic  - mainLoop
    .int drawItem_volSFX    - mainLoop
    .int drawItem_gameSpeed - mainLoop
    .int drawItem_debugText - mainLoop
    # this table must end with a null entry to mark the end of the menu.
    # other tables don't need a null entry.
    .int 0

itemAdjustFuncs:
    .int adjItem_player    - mainLoop
    .int adjItem_PDAHUD    - mainLoop
    .int adjItem_bigMap    - mainLoop
    .int adjItem_mapAlpha  - mainLoop
    #.int adjItem_sound     - mainLoop
    .int adjItem_FOV       - mainLoop
    .int adjItem_volMusic  - mainLoop
    .int adjItem_volSFX    - mainLoop
    .int adjItem_gameSpeed - mainLoop
    .int adjItem_debugText - mainLoop
