# a few patches need to use global variables so that others
# (mainly the PDA Menu) kno where to find them.

.set PATCH_STATE_PTR,0x8000000C # address to store our state
.set PATCH_STATE_SIZE,0x08 # bytes

# offsets within state
# state always starts at an address aligned to 4 bytes.
.set ENABLE_FREE_MOVE,0x00
.set CUR_CHAR_ADDR,0x01 # which character model to use
.set SAVE_TEXT_COUNTDOWN,0x02
.set AUTOSAVE_ENABLED,0x03
.set SHOW_DEBUG_OBJS,0x04
