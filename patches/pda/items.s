.ascii "pdaItems" # 8 byte file ID for debug
# pointers to the submodules.

.set MENU_PAGE_NONE,           0xFF
.set MENU_PAGE_GAME_SETTINGS,  0x00
.set MENU_PAGE_VIDEO,          0x01
.set MENU_PAGE_CAMERA,         0x02
.set MENU_PAGE_AUDIO,          0x03
.set MENU_PAGE_DEBUG,          0x04
.set MENU_PAGE_DEBUG_TEXT,     0x05
.set MENU_PAGE_DEBUG_MAP,      0x06
.set MENU_PAGE_DEBUG_DATA,     0x07
.set MENU_PAGE_DEBUG_RENDER,   0x08
.set MENU_PAGE_DEBUG_CHEAT,    0x09
.set MENU_PAGE_DEBUG_MISC,     0x0A
.set MENU_PAGE_DEBUG_CAMERA,   0x0B
.set MENU_PAGE_DEBUG_ENV,      0x0C
.set MENU_PAGE_DEBUG_GAMETEXT, 0x0D
.set MENU_PAGE_DEBUG_AUDIO,    0x0E
.set MENU_PAGE_DEBUG_SEQ,      0x0F

menuPages:
    .int itemDrawFuncs_gameSettings    - mainLoop
    .int itemAdjustFuncs_gameSettings  - mainLoop
    .int itemDrawFuncs_video           - mainLoop
    .int itemAdjustFuncs_video         - mainLoop
    .int itemDrawFuncs_camera          - mainLoop
    .int itemAdjustFuncs_camera        - mainLoop
    .int itemDrawFuncs_audio           - mainLoop
    .int itemAdjustFuncs_audio         - mainLoop
    .int itemDrawFuncs_debug           - mainLoop
    .int itemAdjustFuncs_debug         - mainLoop
    .int itemDrawFuncs_debugText       - mainLoop
    .int itemAdjustFuncs_debugText     - mainLoop
    .int itemDrawFuncs_debugMap        - mainLoop
    .int itemAdjustFuncs_debugMap      - mainLoop
    .int itemDrawFuncs_debugData       - mainLoop
    .int itemAdjustFuncs_debugData     - mainLoop
    .int itemDrawFuncs_debugRender     - mainLoop
    .int itemAdjustFuncs_debugRender   - mainLoop
    .int itemDrawFuncs_debugCheat      - mainLoop
    .int itemAdjustFuncs_debugCheat    - mainLoop
    .int itemDrawFuncs_debugMisc       - mainLoop
    .int itemAdjustFuncs_debugMisc     - mainLoop
    .int itemDrawFuncs_debugCamera     - mainLoop
    .int itemAdjustFuncs_debugCamera   - mainLoop
    .int itemDrawFuncs_debugEnv        - mainLoop
    .int itemAdjustFuncs_debugEnv      - mainLoop
    .int itemDrawFuncs_debugGameText   - mainLoop
    .int itemAdjustFuncs_debugGameText - mainLoop
    .int itemDrawFuncs_debugAudio      - mainLoop
    .int itemAdjustFuncs_debugAudio    - mainLoop
    .int itemDrawFuncs_debugSeq        - mainLoop
    .int itemAdjustFuncs_debugSeq      - mainLoop
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
    .byte MENU_PAGE_DEBUG_CAMERA # previous
    .byte MENU_PAGE_DEBUG_MAP    # next
    .byte MENU_PAGE_DEBUG        # parent
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
    .byte MENU_PAGE_DEBUG_DATA   # previous
    .byte MENU_PAGE_DEBUG_CAMERA # next
    .byte MENU_PAGE_DEBUG        # parent
    .byte 0
    # Debug Cheat
    .byte MENU_PAGE_DEBUG_CHEAT  # previous
    .byte MENU_PAGE_DEBUG_CHEAT  # next
    .byte MENU_PAGE_DEBUG_DATA   # parent
    .byte 0
    # Debug Misc
    .byte MENU_PAGE_DEBUG_RENDER # previous
    .byte MENU_PAGE_DEBUG_CAMERA # next
    .byte MENU_PAGE_DEBUG        # parent
    .byte 0
    # Debug Camera
    .byte MENU_PAGE_DEBUG_MISC   # previous
    .byte MENU_PAGE_DEBUG_ENV    # next
    .byte MENU_PAGE_DEBUG        # parent
    .byte 0
    # Debug Environment
    .byte MENU_PAGE_DEBUG_CAMERA   # previous
    .byte MENU_PAGE_DEBUG_GAMETEXT # next
    .byte MENU_PAGE_DEBUG          # parent
    .byte 0
    # Debug GameText
    .byte MENU_PAGE_DEBUG_ENV    # previous
    .byte MENU_PAGE_DEBUG_AUDIO  # next
    .byte MENU_PAGE_DEBUG        # parent
    .byte 0
    # Debug Audio
    .byte MENU_PAGE_DEBUG_GAMETEXT # previous
    .byte MENU_PAGE_DEBUG_SEQ    # next
    .byte MENU_PAGE_DEBUG        # parent
    .byte 0
    # Debug Seq
    .byte MENU_PAGE_DEBUG_AUDIO  # previous
    .byte MENU_PAGE_DEBUG_TEXT   # next
    .byte MENU_PAGE_DEBUG        # parent
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
    .int s_Debug                - mainLoop # title
    .int drawItem_debugText     - mainLoop
    .int drawItem_debugMap      - mainLoop
    .int drawItem_debugCamera   - mainLoop
    .int drawItem_debugData     - mainLoop
    .int drawItem_debugRender   - mainLoop
    .int drawItem_debugEnv      - mainLoop
    .int drawItem_debugGameText - mainLoop
    .int drawItem_debugAudio    - mainLoop
    .int drawItem_debugSeq      - mainLoop
    .int drawItem_frameAdvance  - mainLoop
    .int drawItem_debugMisc     - mainLoop
    .int 0

itemAdjustFuncs_debug:
    .int adjItem_debugText     - mainLoop
    .int adjItem_debugMap      - mainLoop
    .int adjItem_debugCamera   - mainLoop
    .int adjItem_debugData     - mainLoop
    .int adjItem_debugRender   - mainLoop
    .int adjItem_debugEnv      - mainLoop
    .int adjItem_debugGameText - mainLoop
    .int adjItem_debugAudio    - mainLoop
    .int adjItem_debugSeq      - mainLoop
    .int adjItem_frameAdvance  - mainLoop
    .int adjItem_debugMisc     - mainLoop

itemDrawFuncs_debugText:
    .int s_DebugText                     - mainLoop # title
    .int drawItem_debugText_master       - mainLoop
    .int drawItem_debugText_tricky       - mainLoop
    .int drawItem_debugText_playerState  - mainLoop
    .int drawItem_debugText_seqState     - mainLoop
    .int drawItem_debugText_heaps        - mainLoop
    .int drawItem_debugText_restartPoint - mainLoop
    .int 0

itemAdjustFuncs_debugText:
    .int adjItem_debugText_master       - mainLoop
    .int adjItem_debugText_tricky       - mainLoop
    .int adjItem_debugText_playerState  - mainLoop
    .int adjItem_debugText_seqState     - mainLoop
    .int adjItem_debugText_heaps        - mainLoop
    .int adjItem_debugText_restartPoint - mainLoop

itemDrawFuncs_debugMap:
    .int s_Map                 - mainLoop # title
    .int drawItem_freeMove     - mainLoop
    .int drawItem_reloadMap    - mainLoop
    .int drawItem_warp         - mainLoop
    #.int drawItem_debugObjs    - mainLoop
    .int drawItem_objList      - mainLoop
    .int drawItem_spawn        - mainLoop
    .int drawItem_setRestartPoint  - mainLoop
    .int drawItem_clrRestartPoint  - mainLoop
    .int drawItem_gotoRestartPoint - mainLoop
    .int 0

itemAdjustFuncs_debugMap:
    .int adjItem_freeMove     - mainLoop
    .int adjItem_reloadMap    - mainLoop
    .int adjItem_warp         - mainLoop
    #.int adjItem_debugObjs    - mainLoop
    .int adjItem_objList      - mainLoop
    .int adjItem_spawn        - mainLoop
    .int adjItem_setRestartPoint  - mainLoop
    .int adjItem_clrRestartPoint  - mainLoop
    .int adjItem_gotoRestartPoint - mainLoop

itemDrawFuncs_debugData:
    .int s_Data                    - mainLoop # title
    .int drawItem_gameBits         - mainLoop
    .int drawItem_debugCheat       - mainLoop
    .int drawItem_saveGame         - mainLoop
    .int drawItem_heap             - mainLoop
    .int drawItem_hexedit          - mainLoop
    .int 0

itemAdjustFuncs_debugData:
    .int adjItem_gameBits         - mainLoop
    .int adjItem_debugCheat       - mainLoop
    .int adjItem_saveGame         - mainLoop
    .int adjItem_heap             - mainLoop
    .int adjItem_hexedit          - mainLoop

itemDrawFuncs_debugRender:
    .int s_Render              - mainLoop # title
    .int drawItem_textureDebug - mainLoop
    .int drawItem_hitboxes     - mainLoop
    .int drawItem_blurFilter   - mainLoop
    .int drawItem_motionBlur   - mainLoop
    .int drawItem_heatEffect   - mainLoop
    .int drawItem_monochrome   - mainLoop
    .int drawItem_spiritVision - mainLoop
    #.int drawItem_distortion   - mainLoop
    .int drawItem_colorFilter  - mainLoop
    .int drawItem_colorFilterR - mainLoop
    .int drawItem_colorFilterG - mainLoop
    .int drawItem_colorFilterB - mainLoop
    .int 0

itemAdjustFuncs_debugRender:
    .int adjItem_textureDebug - mainLoop
    .int adjItem_hitboxes     - mainLoop
    .int adjItem_blurFilter   - mainLoop
    .int adjItem_motionBlur   - mainLoop
    .int adjItem_heatEffect   - mainLoop
    .int adjItem_monochrome   - mainLoop
    .int adjItem_spiritVision - mainLoop
    #.int adjItem_distortion   - mainLoop
    .int adjItem_colorFilter  - mainLoop
    .int adjItem_colorFilterR - mainLoop
    .int adjItem_colorFilterG - mainLoop
    .int adjItem_colorFilterB - mainLoop

itemDrawFuncs_debugEnv:
    .int s_Environment        - mainLoop # title
    .int drawItem_timeOfDay   - mainLoop
    .int 0

itemAdjustFuncs_debugEnv:
    .int adjItem_timeOfDay    - mainLoop


itemDrawFuncs_debugCheat:
    .int s_EditPlayerState     - mainLoop # title
    .int drawItem_curHP        - mainLoop
    .int drawItem_maxHP        - mainLoop
    .int drawItem_curMP        - mainLoop
    .int drawItem_maxMP        - mainLoop
    .int drawItem_curMoney     - mainLoop
    .int drawItem_curLives     - mainLoop
    .int drawItem_maxLives     - mainLoop
    .int drawItem_unlockAll    - mainLoop
    .int drawItem_killMe       - mainLoop
    .int 0

itemAdjustFuncs_debugCheat:
    .int adjItem_curHP         - mainLoop
    .int adjItem_maxHP         - mainLoop
    .int adjItem_curMP         - mainLoop
    .int adjItem_maxMP         - mainLoop
    .int adjItem_curMoney      - mainLoop
    .int adjItem_curLives      - mainLoop
    .int adjItem_maxLives      - mainLoop
    .int adjItem_unlockAll     - mainLoop
    .int adjItem_killMe        - mainLoop

itemDrawFuncs_debugMisc:
    .int s_Misc                - mainLoop # title
    .int drawItem_crash        - mainLoop
    .int 0

itemAdjustFuncs_debugMisc:
    .int adjItem_crash         - mainLoop

itemDrawFuncs_debugCamera:
    .int s_Camera               - mainLoop # title
    .int drawItem_dbgCamMode    - mainLoop
    #.int drawItem_camInterpMode - mainLoop
    .int 0

itemAdjustFuncs_debugCamera:
    .int adjItem_dbgCamMode    - mainLoop
    #.int adjItem_camInterpMode - mainLoop

itemDrawFuncs_debugGameText:
    .int s_GameText             - mainLoop # title
    .int drawItem_textTest      - mainLoop
    .int drawItem_gameTextBox - mainLoop
    .int 0

itemAdjustFuncs_debugGameText:
    .int adjItem_textTest    - mainLoop

itemDrawFuncs_debugAudio:
    .int s_Audio              - mainLoop # title
    .int drawItem_dbgAudioCmd - mainLoop
    .int 0

itemAdjustFuncs_debugAudio:
    .int adjItem_dbgAudioCmd - mainLoop


itemDrawFuncs_debugSeq:
    .int s_ObjSeq             - mainLoop # title
    .int drawItem_playSeq     - mainLoop
    .int drawItem_stopSeq     - mainLoop
    .int drawItem_editSeq     - mainLoop
    .int drawItem_seqGlobal1  - mainLoop
    .int drawItem_seqGlobal2  - mainLoop
    .int drawItem_seqGlobal3  - mainLoop
    .int drawItem_seqBool     - mainLoop
    .int drawItem_seqVar1     - mainLoop
    .int drawItem_seqVar2     - mainLoop
    .int drawItem_seqVar3     - mainLoop
    .int drawItem_seqFlags    - mainLoop
    .int 0

itemAdjustFuncs_debugSeq:
    .int adjItem_playSeq       - mainLoop
    .int adjItem_stopSeq       - mainLoop
    .int adjItem_editSeq       - mainLoop
    .int adjItem_seqGlobal1    - mainLoop
    .int adjItem_seqGlobal2    - mainLoop
    .int adjItem_seqGlobal3    - mainLoop
    .int adjItem_seqBool       - mainLoop
    .int adjItem_seqVar1       - mainLoop
    .int adjItem_seqVar2       - mainLoop
    .int adjItem_seqVar3       - mainLoop
    .int adjItem_seqFlags      - mainLoop

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
    .int inputFuncDummy       - mainLoop # Debug Cheat
    .int inputFuncDummy       - mainLoop # Debug Misc
    .int inputFuncDummy       - mainLoop # Debug Camera
    .int inputFuncDummy       - mainLoop # Debug Environment
    .int inputFuncDummy       - mainLoop # Debug GameText
    .int inputFuncDebugAudio  - mainLoop # Debug Audio
    .int inputFuncDummy       - mainLoop # Debug Seq


inputFuncDummy: blr
