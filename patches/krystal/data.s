# Strings and constant pools

# let's make the paths short to shave a few more bytes
# off the patch size, since RAM is tight.
modelPath1:   .string "patches/km1"
texturePath1: .string "patches/kt1"
modelPath2:   .string "patches/km2"
texturePath2: .string "patches/kt2"
#titleData:    .byte   0x52,0x75,0x6C,0x65,0x20,0x33,0x34,0x00

.if DEBUG
    msg_loadAsset:       .string "Loading %s"
    msg_loadModel:       .string "Load model to offs %08X sz %08X"
    msg_loadModelFailed: .string "Load model FAIL"
    msg_whichModel:      .string "Using model %d"
    msg_whichTex1Bin:    .string "Using TEX1.bin %s"
    msg_whichTex1Tab:    .string "Using TEX1.tab %d"
    msg_doTex1TabPatch:  .string "Patch TEX1.tab %X -> %X"
    msg_tex1TabNoTex:    .string "New texture not loaded"
    msg_tex1TabNoTab:    .string "TEX1.tab not loaded"
    msg_tex1TabNoBin:    .string "TEX1.bin not loaded"
    msg_tex1PatchDone:   .string "Textures installed OK"
.endif
