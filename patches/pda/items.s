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
    .int itemDrawFuncs_page3   - mainLoop
    .int itemAdjustFuncs_page3 - mainLoop
    .int 0

itemDrawFuncs_page0: # PDA Menu
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


itemDrawFuncs_page1: # Game Settings
    .int title_page1          - mainLoop
    .int drawItem_player      - mainLoop
    .int drawItem_FOV         - mainLoop
    .int drawItem_gameSpeed   - mainLoop
    .int 0

itemAdjustFuncs_page1:
    .int adjItem_player       - mainLoop
    .int adjItem_FOV          - mainLoop
    .int adjItem_gameSpeed    - mainLoop



itemDrawFuncs_page2: # Audio
    .int title_page2          - mainLoop
    .int drawItem_volMusic    - mainLoop
    .int drawItem_volSFX      - mainLoop
    .int drawItem_volCutScene - mainLoop
    .int drawItem_sound       - mainLoop
    .int drawItem_music       - mainLoop
    .int drawItem_streamTest  - mainLoop
    .int 0

itemAdjustFuncs_page2:
    .int adjItem_volMusic     - mainLoop
    .int adjItem_volSFX       - mainLoop
    .int adjItem_volCutScene  - mainLoop
    .int adjItem_sound        - mainLoop
    .int adjItem_music        - mainLoop
    .int adjItem_streamTest   - mainLoop


itemDrawFuncs_page3: # Debug
    .int title_page3           - mainLoop
    .int drawItem_debugText    - mainLoop
    .int drawItem_trickyDebug  - mainLoop
    .int drawItem_freeMove     - mainLoop
    .int drawItem_reloadMap    - mainLoop
    .int drawItem_warp         - mainLoop
    .int drawItem_objects      - mainLoop
    .int drawItem_gameBits     - mainLoop
    .int drawItem_saveGame     - mainLoop
    .int drawItem_heap         - mainLoop
    .int drawItem_textTest     - mainLoop
    .int drawItem_frameAdvance - mainLoop
    #.int drawItem_Save         - mainLoop
    .int 0

itemAdjustFuncs_page3:
    .int adjItem_debugText    - mainLoop
    .int adjItem_trickyDebug  - mainLoop
    .int adjItem_freeMove     - mainLoop
    .int adjItem_reloadMap    - mainLoop
    .int adjItem_warp         - mainLoop
    .int adjItem_objects      - mainLoop
    .int adjItem_gameBits     - mainLoop
    .int adjItem_saveGame     - mainLoop
    .int adjItem_heap         - mainLoop
    .int adjItem_textTest     - mainLoop
    .int adjItem_frameAdvance - mainLoop
    #.int adjItem_Save         - mainLoop
