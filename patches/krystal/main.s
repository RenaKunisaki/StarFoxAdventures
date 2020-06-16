.text
.include "common.s"
.include "globals.s"

constants:
    # model data
    .set KRYSTAL_MODEL_ID,0x4E8

    # texture data
    .set NUM_TEXTURES,7 # how many entries in tex1Tab_data
    .set TEX1_BASE_ID,0x724 # first ID to patch in table
    .set TEX1_LAST_ID,TEX1_BASE_ID+NUM_TEXTURES

    # stack for all patches
    # since gnu `as` has no concept of local labels, these are just
    # shared among all of them. (XXX can we re-set?)
    .set STACK_SIZE,    0x100 # how much to reserve
    .set SP_LR_SAVE,    0x104 # this is what the game does
    .set SP_R0_SAVE,    0x10
    .set SP_GPR_SAVE,   0x14  # for modelsTabPatch and tex1TabPatch

    # for modelsBinPatch and tex1BinPatch
    .set SP_BUFFER,     0x10 # allocated buffer
    .set SP_EXTRA_SIZE, 0x14 # size of our file
    .set SP_ORIG_SIZE,  0x18 # original buffer size (offset to copy to)
    .set SP_FILE_BUFFER,0x1C # file buffer temp
    .set SP_ALLOC_TAG,  0x20 # original alloc tag

    # for tex1BinPatch
    .set SP_DEST_BUFFER,0x24 # destination in allocated buffer
    .set SP_SRC_OFFSET, 0x28 # SOURCE_OFFSET

# define patches
patchList:
    PATCH_ID        "Krystal" # must be 7 chars
    PATCH_BL        0x800453E0, modelsBinPatch
    PATCH_B         0x80043D7C, modelsTabPatch
    PATCH_BL        0x80046164, tex1BinPatch
    PATCH_B         0x80043DB8, tex1TabPatch
    PATCH_B         0x802B6F28, loadSaveGamePatch
    PATCH_B         0x8011C420, settingsPatch
    PATCH_BL        0x8011D4BC, titlePatch
    PATCH_MAIN_LOOP mainLoopPatch
    PATCH_WORD      0x802B08D8, 0x38000001 # let Krystal use staff
    PATCH_WORD      0x80295BE0, 0x38600001 # let Krystal use map, PDA, etc
    #PATCH_WORD      0x8005F5E0, 0x60000000 # HACK: fix a crash caused by
    #    # the previous patch, where some fog parameter is being
    #    # set incorrectly leading to an endless loop on pause screen.
    #    # XXX figure out what's causing that and fix it correctly.
    #    # this workaround causes some flickering.
    # actually, that crash may be caused by Gecko code handler.
    PATCH_END PATCH_KEEP_AFTER_RUN

entry: # called when patch is loaded
    blr # nothing to do
