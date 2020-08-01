# ybutton patch:
# saves the Y button equip to the save file.
.text
.include "common.s"
.include "globals.s"

.set saveYbuttonItem, saveData+0xE4+0x6EA

# define patches
patchList:
    PATCH_ID        "Ybutton" # must be 7 chars
    PATCH_BL        0x800e9c30, onLoad
    PATCH_MAIN_LOOP mainLoop
    PATCH_END       PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_GPR_SAVE,0x20

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr

mainLoop: # called from main loop. r3 = mainLoop
    LOADWH  r3,  saveYbuttonItem
    LOADWH  r4,  yButtonItem
    LOADHL2 r4,  yButtonItem, r4
    STOREH  r4,  saveYbuttonItem,  r3
    blr

onLoad:
    # replace a call to resetYButton()
    LOADWH  r3,  saveYbuttonItem
    LOADHL2 r3,  saveYbuttonItem,  r3
    LOADWH  r4,  yButtonItem
    STOREW  r3,  yButtonItem, r4
    # XXX we should set yButtonItemTextureId, yButtonDisableBit,
    # yButtonHideBit, yButtonState...
    # in fact this just plain doesn't work as-is.
    # of course there's no simple "set Y button item" function...
    # simple way would be to just save all of those, if we can find room.
    blr
