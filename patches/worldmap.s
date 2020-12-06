# worldmap patch:
# lets you press Z to skip arwing sections.
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID  "wrldmap" # must be 7 chars
    PATCH_BL  0x8020d31c, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_GPR_SAVE,0x10
    .set SP_FLOAT_TMP,0x18 # temporary storage for float conversion (8 bytes)

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


main: # called every frame on the "go/cancel" screen of world map.
    # r29 = buttons pressed
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r4,  SP_GPR_SAVE(r1)

    bl .getpc
    .getpc:
        mflr r14

    # we replaced a bl to a function that just does this.
    li      r0,  60
    stb     r0,  -0x5a58(r13) # pauseMenuFrameCounter

    # draw message
    addi    r3,  r14,  s_skip - .getpc
    li      r4,    2 # box type
    li      r5,    0 # X that we can't actually set
    li      r6,  352 # Y
    CALL    gameTextShowStr

    # is Z pressed?
    andi.   r3,  r29, PAD_BUTTON_Z
    beq     .end

    # Z pressed, do warp
    # get selected map (MapDirIdx32)
    # note we map MapDirIdx32 to WarpEnum here
    LOADW   r4,  0x803ddd28
    cmpwi   r4,  0x38
    beq     .warpToPlanet
    cmpwi   r4,  0x39
    beq     .warpToDarkIce
    cmpwi   r4,  0x3A
    beq     .warpToCloudRunner
    cmpwi   r4,  0x3B
    beq     .warpToWallCity
    # else must be dragon rock
    li      r3,  0x79
    b       .doWarp

.warpToPlanet:
    li      r3,  0x6C
    b       .doWarp
.warpToDarkIce:
    li      r3,  0x77
    b       .doWarp
.warpToCloudRunner:
    li      r3,  0x63
    b       .doWarp
.warpToWallCity:
    li      r3,  0x78

.doWarp:
    li      r4,  0
    CALL    warpToMap

.end:
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r4,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    blr

s_skip: .string "Z: Skip   " # pad with spaces to center
