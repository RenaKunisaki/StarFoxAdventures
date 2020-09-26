# uwu patch:
# uwu-ifies text
.text
.include "common.s"
.include "globals.s"

# define patches
patchList:
    PATCH_ID  "uwu    " # must be 7 chars
    PATCH_B   0x800175bc, main
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_GPR_SAVE,0x10

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


main: # called by our hook, from the patch list.
    # replaced a call to 800e7e94: BOOL saveFileStruct:isCheatActive(CheatFlag flag)
    # checking if the "dino language" cheat is on.
    # r26 = the message to translate.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r4,  SP_GPR_SAVE(r1)

    LOADW   r4,  PATCH_STATE_PTR
    lbz     r5,  EXTRA_FEATURE_FLAGS(r4)
    andi.   r5,  r5,  EXTRA_FEATURE_UWU
    #beq     .disabled

    # scan the string and replace letters as appropriate.
    subi    r4,  r26, 1
.nextChar:
    lbzu    r3,  1(r4)
    cmpwi   r3,  0
    beq     .end
    cmpwi   r3, 'l'
    beq     .small
    cmpwi   r3, 'r'
    beq     .small
    cmpwi   r3, 'L'
    beq     .large
    cmpwi   r3, 'R'
    bne     .nextChar

.large: # replace large L, R
    li      r3, 'W'
    b       .storeChar
.small: # replace small l, r
    li      r3,  'w'
.storeChar:
    stb     r3, 0(r4)
    b       .nextChar

.end:
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r4,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800175e4, r4 # skip dino translation

.disabled:
    CALL    0x800e7e94 # replaced function
    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r4,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    JUMP    0x800175c0, r4 # back to original code
