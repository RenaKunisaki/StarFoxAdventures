# a few patches need to use global variables so that others
# (mainly the PDA Menu) know where to find them.

.set PATCH_STATE_PTR,0x8000000C # address to store pointer to our state
.set PATCH_STATE_SIZE,0x110 # bytes

# offsets within state
# state always starts at an address aligned to 4 bytes.
.set ENABLE_FREE_MOVE,   0x00
.set CUR_CHAR_ADDR,      0x01 # which character model to use (0=Krystal 1=Fox)
.set SAVE_TEXT_COUNTDOWN,0x02
.set AUTOSAVE_ENABLED,   0x03
.set SHOW_DEBUG_OBJS,    0x04
.set FORCE_TEXT_WIDTH,   0x05
.set CAMERA_OPTIONS,     0x06
.set PDA_MENU_OPEN,      0x07
.set DEBUG_TEXT_FLAGS,   0x08 # which debug texts are enabled
.set EXTRA_FEATURE_FLAGS,0x09
.set DEBUG_RENDER_FLAGS, 0x0A
.set TEMP_FEATURE_FLAGS, 0x0B # extra features that don't get saved
.set DEBUG_CAM_MODE,     0x0C
# 0x0D - 0x0E: reserved
.set ALLOC_FAIL_MAX,     32 # max alloc fails to keep in log (must be power of 2)
.set ALLOC_FAIL_POS,     0x0F # position to write next log entry
.set ALLOC_FAIL_LOG,     0x10 # log of (caller/tag, size) of alloc fails
# 0x24 + (32 * 8) bytes = 0x110

# camera option bits
.set CAMERA_OPTION_PAD3,    0x01 # use controller 3 to move
.set CAMERA_OPTION_INVERTX, 0x02 # invert X axis movement
.set CAMERA_OPTION_INVERTY, 0x04 # invert Y axis movement

# debug text flag bits
.set DEBUG_TEXT_PLAYER_STATE, 0x01
.set DEBUG_TEXT_SEQ_STATE,    0x02
.set DEBUG_TEXT_HEAP_STATE,   0x04
.set DEBUG_TEXT_RESTART_POINT,0x08

# extra feature flag bits
.set EXTRA_FEATURE_RUMBLE_BLUR, 0x01

# temp feature flag bits
.set TEMP_FEATURE_APRIL1,       0x01

# debug render flag bits
.set DEBUG_RENDER_HITBOXES,  0x01

# offsets into saveData
.set SAVEDATA_EXTRA_OPTIONS,  0x01 # unused
.set SAVEDATA_SUBTITLES,      0x02
.set SAVEDATA_CAMERA_OPTIONS, 0x03 # replace unused HUD setting
.set SAVEDATA_WIDESCREEN,     0x06
.set SAVEDATA_CUR_CHAR,       0x07 # unused field
.set SAVEDATA_RUMBLE,         0x08
.set SAVEDATA_SOUND_MODE,     0x09
.set SAVEDATA_MUSIC_VOL,      0x0A
.set SAVEDATA_SFX_VOL,        0x0B
.set SAVEDATA_CUTSCENE_VOL,   0x0C
.set SAVEDATA_OPTIONS,        0x0D # unused field
.set SAVEDATA_FOV,            0x0E # unused field
.set SAVEDATA_MAP_OPACITY,    0x0F # unused field
# XXX: game speed
# autosave is handled elsewhere because I did it earlier

.set SAVEDATA_OPTION_MAP_SIZE,0x03
.set SAVEDATA_OPTION_PDA_MODE,0x0C
.set SAVEDATA_OPTION_FUR_FX,  0x10
.set SAVEDATA_OPTION_BACKPACK,0x60

# debug camera modes
.set DEBUG_CAM_NORMAL, 0x00
.set DEBUG_CAM_STAY,   0x01
.set DEBUG_CAM_FREE,   0x02
.set DEBUG_CAM_MAX,    0x03 # number of modes
