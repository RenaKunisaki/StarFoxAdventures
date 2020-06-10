# pointers to the submodules.
# ENSURE THAT THESE TABLES MATCH.
# each draw function must have a corresponding adjust function,
# and they must be in the same order.
# (I could probably combine the tables into one alternating, but I
# think that would be slightly less efficient as I wouldn't be able
# to use lwzu to index them.)

menuPages:
    .int itemDrawFuncs_page0   - mainLoop
    .int itemAdjustFuncs_page0 - mainLoop
    .int itemDrawFuncs_page1   - mainLoop
    .int itemAdjustFuncs_page1 - mainLoop
    .int itemDrawFuncs_page2   - mainLoop
    .int itemAdjustFuncs_page2 - mainLoop
    .int 0

itemDrawFuncs_page0:
    # first entry is title char*
    # subsequent entries are function pointers
    .int title_page0          - mainLoop
    .int drawItem_PDAHUD      - mainLoop
    .int drawItem_bigMap      - mainLoop
    .int drawItem_mapAlpha    - mainLoop
    .int 0 # drawfuncs table must be null terminated

itemAdjustFuncs_page0:
    .int adjItem_PDAHUD       - mainLoop
    .int adjItem_bigMap       - mainLoop
    .int adjItem_mapAlpha     - mainLoop


itemDrawFuncs_page1:
    .int title_page1          - mainLoop
    .int drawItem_player      - mainLoop
    .int drawItem_FOV         - mainLoop
    .int drawItem_gameSpeed   - mainLoop
    .int drawItem_volMusic    - mainLoop
    .int drawItem_volSFX      - mainLoop
    .int drawItem_volCutScene - mainLoop
    .int 0

itemAdjustFuncs_page1:
    .int adjItem_player       - mainLoop
    .int adjItem_FOV          - mainLoop
    .int adjItem_gameSpeed    - mainLoop
    .int adjItem_volMusic     - mainLoop
    .int adjItem_volSFX       - mainLoop
    .int adjItem_volCutScene  - mainLoop


itemDrawFuncs_page2:
    .int title_page2          - mainLoop
    .int drawItem_debugText   - mainLoop
    .int drawItem_trickyDebug - mainLoop
    .int drawItem_freeMove    - mainLoop
    .int drawItem_reloadMap   - mainLoop
    .int drawItem_sound       - mainLoop
    .int drawItem_music       - mainLoop
    .int drawItem_streamTest  - mainLoop
    .int 0

itemAdjustFuncs_page2:
    .int adjItem_debugText   - mainLoop
    .int adjItem_trickyDebug - mainLoop
    .int adjItem_freeMove    - mainLoop
    .int adjItem_reloadMap   - mainLoop
    .int adjItem_sound       - mainLoop
    .int adjItem_music       - mainLoop
    .int adjItem_streamTest  - mainLoop
