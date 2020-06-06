# better pause menu patch:
# 1) removes confirmation before saving
# 2) removes some voices from the menu
.text
.include "common.s"

# define patches
patchList:
    PATCH_ID   "Pause  " # must be 7 chars
    PATCH_WORD 0x8012A97C, 0x4BFBDD55 # disable save confirmation
    # disable some voices
    PATCH_WORD 0x8012A904, 0x60000000
    PATCH_WORD 0x8012A8C4, 0x60000000
    PATCH_WORD 0x8012A95C, 0x60000000
    PATCH_WORD 0x8012BA94, 0x60000000
    PATCH_WORD 0x8012B8DC, 0x60000000
    PATCH_WORD 0x8012B88C, 0x60000000
    PATCH_WORD 0x8012B8B4, 0x60000000
    PATCH_WORD 0x8012BD78, 0x60000000
    PATCH_END PATCH_FREE_AFTER_RUN

entry: # called as soon as our patch is loaded.
    blr # nothing to do here

# eventually we should put a menu in that pops up with some key combo,
# or replaces the pause menu or even has its own icon (could use Krystal talking head model)
# and has options like toggle debug print, set FOV, game speed,
# cheats, etc.
