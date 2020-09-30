.ascii "items   " # 8 byte file ID for debug
# pointers to the submodules.

.set MENU_PAGE_NONE,          0xFF
.set MENU_PAGE_GAME_SETTINGS, 0
.set MENU_PAGE_VIDEO,         1
.set MENU_PAGE_CAMERA,        2
.set MENU_PAGE_AUDIO,         3
.set MENU_PAGE_DEBUG,         4
.set MENU_PAGE_DEBUG_TEXT,    5
.set MENU_PAGE_DEBUG_MAP,     6
.set MENU_PAGE_DEBUG_DATA,    7
.set MENU_PAGE_DEBUG_RENDER,  8

menuPages:
    .int itemDrawFuncs_gameSettings   - mainLoop
    .int itemAdjustFuncs_gameSettings - mainLoop
    .int itemDrawFuncs_video          - mainLoop
    .int itemAdjustFuncs_video        - mainLoop
    .int itemDrawFuncs_camera         - mainLoop
    .int itemAdjustFuncs_camera       - mainLoop
    .int itemDrawFuncs_audio          - mainLoop
    .int itemAdjustFuncs_audio        - mainLoop
    .int itemDrawFuncs_debug          - mainLoop
    .int itemAdjustFuncs_debug        - mainLoop
    .int itemDrawFuncs_debugText      - mainLoop
    .int itemAdjustFuncs_debugText    - mainLoop
    .int itemDrawFuncs_debugMap       - mainLoop
    .int itemAdjustFuncs_debugMap     - mainLoop
    .int itemDrawFuncs_debugData      - mainLoop
    .int itemAdjustFuncs_debugData    - mainLoop
    .int itemDrawFuncs_debugRender    - mainLoop
    .int itemAdjustFuncs_debugRender  - mainLoop
    .int 0

menuPageStructure:
    # 4 items per page: previous page, next page, parent, unused.
    # previous/next are for cycling with C stick.
    # parent is for B button.

    # Game Settings
    .byte MENU_PAGE_DEBUG # previous
    .byte MENU_PAGE_VIDEO # next
    .byte MENU_PAGE_NONE  # parent
    .byte 0
    # Video Settings
    .byte MENU_PAGE_GAME_SETTINGS # previous
    .byte MENU_PAGE_CAMERA        # next
    .byte MENU_PAGE_NONE          # parent
    .byte 0
    # Camera Settings
    .byte MENU_PAGE_VIDEO # previous
    .byte MENU_PAGE_AUDIO # next
    .byte MENU_PAGE_NONE  # parent
    .byte 0
    # Audio Settings
    .byte MENU_PAGE_CAMERA # previous
    .byte MENU_PAGE_DEBUG  # next
    .byte MENU_PAGE_NONE   # parent
    .byte 0
    # Debug
    .byte MENU_PAGE_AUDIO         # previous
    .byte MENU_PAGE_GAME_SETTINGS # next
    .byte MENU_PAGE_NONE          # parent
    .byte 0
    # Debug Text
    .byte MENU_PAGE_DEBUG_DATA # previous
    .byte MENU_PAGE_DEBUG_MAP  # next
    .byte MENU_PAGE_DEBUG      # parent
    .byte 0
    # Debug Map
    .byte MENU_PAGE_DEBUG_TEXT # previous
    .byte MENU_PAGE_DEBUG_DATA # next
    .byte MENU_PAGE_DEBUG      # parent
    .byte 0
    # Debug Data
    .byte MENU_PAGE_DEBUG_MAP    # previous
    .byte MENU_PAGE_DEBUG_RENDER # next
    .byte MENU_PAGE_DEBUG        # parent
    .byte 0
    # Debug Render
    .byte MENU_PAGE_DEBUG_DATA # previous
    .byte MENU_PAGE_DEBUG_TEXT # next
    .byte MENU_PAGE_DEBUG      # parent
    .byte 0

# ENSURE THAT THESE TABLES MATCH.
# each draw function must have a corresponding adjust function,
# and they must be in the same order.
# (I could probably combine the tables into one alternating, but I
# think that would be slightly less efficient as I wouldn't be able
# to use lwzu to index them.)

itemDrawFuncs_gameSettings:
    # first entry is title char*
    # subsequent entries are function pointers
    .int s_GameSettings       - mainLoop # title
    .int drawItem_player      - mainLoop
    .int drawItem_gameSpeed   - mainLoop
    .int drawItem_autoSave    - mainLoop
    #.int drawItem_language    - mainLoop
    .int drawItem_subtitles   - mainLoop
    .int drawItem_rumble      - mainLoop
    .int drawItem_rumbleBlur  - mainLoop
    .int drawItem_backpack    - mainLoop
    .int 0 # drawfuncs table must be null terminated

itemAdjustFuncs_gameSettings:
    .int adjItem_player       - mainLoop
    .int adjItem_gameSpeed    - mainLoop
    .int adjItem_autoSave     - mainLoop
    #.int adjItem_language     - mainLoop
    .int adjItem_subtitles    - mainLoop
    .int adjItem_rumble       - mainLoop
    .int adjItem_rumbleBlur   - mainLoop
    .int adjItem_backpack     - mainLoop


itemDrawFuncs_video:
    .int s_VideoSettings      - mainLoop # title
    .int drawItem_PDAHUD      - mainLoop
    .int drawItem_mapSize     - mainLoop
    .int drawItem_mapAlpha    - mainLoop
    #.int drawItem_mapZoom     - mainLoop
    .int drawItem_furEffect   - mainLoop
    .int 0

itemAdjustFuncs_video:
    .int adjItem_PDAHUD       - mainLoop
    .int adjItem_mapSize      - mainLoop
    .int adjItem_mapAlpha     - mainLoop
    #.int adjItem_mapZoom      - mainLoop
    .int adjItem_furEffect    - mainLoop


itemDrawFuncs_camera:
    .int s_CameraSettings     - mainLoop # title
    .int drawItem_FOV         - mainLoop
    #.int drawItem_LOD         - mainLoop
    .int drawItem_widescreen  - mainLoop
    .int drawItem_camPad      - mainLoop
    .int drawItem_camInvX     - mainLoop
    .int drawItem_camInvY     - mainLoop
    .int 0

itemAdjustFuncs_camera:
    .int adjItem_FOV          - mainLoop
    #.int adjItem_LOD          - mainLoop
    .int adjItem_widescreen   - mainLoop
    .int adjItem_camPad       - mainLoop
    .int adjItem_camInvX      - mainLoop
    .int adjItem_camInvY      - mainLoop


itemDrawFuncs_audio:
    .int s_AudioSettings      - mainLoop # title
    .int drawItem_volMusic    - mainLoop
    .int drawItem_volSFX      - mainLoop
    .int drawItem_volCutScene - mainLoop
    .int drawItem_soundMode   - mainLoop
    .int drawItem_sound       - mainLoop
    .int drawItem_music       - mainLoop
    .int drawItem_streamTest  - mainLoop
    .int 0

itemAdjustFuncs_audio:
    .int adjItem_volMusic     - mainLoop
    .int adjItem_volSFX       - mainLoop
    .int adjItem_volCutScene  - mainLoop
    .int adjItem_soundMode    - mainLoop
    .int adjItem_sound        - mainLoop
    .int adjItem_music        - mainLoop
    .int adjItem_streamTest   - mainLoop


itemDrawFuncs_debug:
    .int s_Debug               - mainLoop # title
    .int drawItem_debugText    - mainLoop
    .int drawItem_debugMap     - mainLoop
    .int drawItem_debugData    - mainLoop
    .int drawItem_debugRender  - mainLoop
    .int drawItem_textTest     - mainLoop
    .int drawItem_frameAdvance - mainLoop
    .int 0

itemAdjustFuncs_debug:
    .int adjItem_debugText    - mainLoop
    .int adjItem_debugMap     - mainLoop
    .int adjItem_debugData    - mainLoop
    .int adjItem_debugRender  - mainLoop
    .int adjItem_textTest     - mainLoop
    .int adjItem_frameAdvance - mainLoop

itemDrawFuncs_debugText:
    .int s_DebugText                    - mainLoop # title
    .int drawItem_debugText_master      - mainLoop
    .int drawItem_debugText_tricky      - mainLoop
    .int drawItem_debugText_playerState - mainLoop
    .int drawItem_debugText_seqState    - mainLoop
    .int 0

itemAdjustFuncs_debugText:
    .int adjItem_debugText_master      - mainLoop
    .int adjItem_debugText_tricky      - mainLoop
    .int adjItem_debugText_playerState - mainLoop
    .int adjItem_debugText_seqState    - mainLoop

itemDrawFuncs_debugMap:
    .int s_Map                 - mainLoop # title
    .int drawItem_freeMove     - mainLoop
    .int drawItem_reloadMap    - mainLoop
    .int drawItem_warp         - mainLoop
    #.int drawItem_debugObjs    - mainLoop
    .int drawItem_objList      - mainLoop
    .int drawItem_spawn        - mainLoop
    .int 0

itemAdjustFuncs_debugMap:
    .int adjItem_freeMove     - mainLoop
    .int adjItem_reloadMap    - mainLoop
    .int adjItem_warp         - mainLoop
    #.int adjItem_debugObjs    - mainLoop
    .int adjItem_objList      - mainLoop
    .int adjItem_spawn        - mainLoop

itemDrawFuncs_debugData:
    .int s_Data                - mainLoop # title
    .int drawItem_gameBits     - mainLoop
    .int drawItem_saveGame     - mainLoop
    .int drawItem_heap         - mainLoop
    .int 0

itemAdjustFuncs_debugData:
    .int adjItem_gameBits     - mainLoop
    .int adjItem_saveGame     - mainLoop
    .int adjItem_heap         - mainLoop

itemDrawFuncs_debugRender:
    .int s_Render              - mainLoop # title
    .int drawItem_textureDebug - mainLoop
    .int drawItem_hitboxes     - mainLoop
    .int 0

itemAdjustFuncs_debugRender:
    .int adjItem_textureDebug - mainLoop
    .int adjItem_hitboxes     - mainLoop


pageInputFuncs:
    .int inputFuncDummy       - mainLoop # Game Settings
    .int inputFuncDummy       - mainLoop # Video
    .int inputFuncDummy       - mainLoop # Camera
    .int inputFuncAudio       - mainLoop # Audio
    .int inputFuncDummy       - mainLoop # Debug
    .int inputFuncDummy       - mainLoop # Debug Text
    .int inputFuncDummy       - mainLoop # Debug Map
    .int inputFuncDummy       - mainLoop # Debug Data
    .int inputFuncDummy       - mainLoop # Debug Render


inputFuncDummy: blr
