# uwu patch:
# uwu-ifies text.
# XXX this suffers from the same bug as the dino cheat: if the letter w graphic
# isn't present, it just gets skipped over.
# this unfortunately is very noticeable in the opening scene.
# really ought to rewrite that system to just load all letters.
.text
.include "common.s"
.include "globals.s"
.set UWU_DEBUG,1

# define patches
patchList:
    PATCH_ID  "uwu    " # must be 7 chars
    PATCH_B   0x800175bc, main
    PATCH_MAIN_LOOP mainLoop
    PATCH_END PATCH_KEEP_AFTER_RUN

constants:
    .set STACK_SIZE,0x100 # how much to reserve
    .set SP_LR_SAVE,0x104 # this is what the game does
    .set SP_GPR_SAVE,0x10
    .set SP_FLOAT_TMP,0x18 # temporary storage for float conversion (8 bytes)
    .set SP_CHR_SIZE,0x18 # reuse for char size in shiftJisGetNextChar

entry: # called as soon as our patch is loaded.
    # nothing to do
    blr


mainLoop: # main loop hook
    # here we just check the date.
    # we could probably do this less often but I doubt it's
    # enough of a performance penalty to matter at all.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r4,  SP_GPR_SAVE(r1)
    mr      r14, r3 # r14 = mainLoop

    # get time
    CALL    0x80246c70 # __OSGetSystemTime -> u64 ticks in r3, r4

    # set up magic floats
    lfd     f3,  (FLOAT_MAGIC - mainLoop)(r14)
    lwz     r5,  (FLOAT_MAGIC - mainLoop)(r14)
    stw     r5,  SP_FLOAT_TMP(r1)

    # convert low half to double
    stw     r4,  (SP_FLOAT_TMP+4)(r1)
    lfd     f2,  SP_FLOAT_TMP(r1)
    fsub    f2,  f2,  f3

    # convert high half to double
    stw     r3,  (SP_FLOAT_TMP+4)(r1)
    lfd     f1,  SP_FLOAT_TMP(r1)
    fsub    f1,  f1,  f3

    # combine
    lfs     f3,  (f_2p32 - mainLoop)(r14)
    fmadd   f1,  f1,  f3,  f2 # f1 = (f1 * f3) + f2

    # scale to seconds
    lfd     f2,  (TIMER_SCALE - mainLoop)(r14)
    fdiv    f1,  f1,  f2 # f1 = seconds since 2000-01-01
    frsp    f1,  f1 # double to single (could do this later but eh)

    # compute year
    lfs     f2,  (SECONDS_PER_YEAR - mainLoop)(r14)
    fdivs   f3,  f1,  f2 # f3 = years since 2000-01-01
    fctiw   f2,  f3 # single to int
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r4,  (SP_FLOAT_TMP+4)(r1) # r4 = (int)years since 2000-01-01

    # now convert r4 BACK to float, because this is apparently
    # the only way to truncate it to integer.
    mr      r3,  r4
    lfd     f9,  (FLOAT_MAGIC2 - mainLoop)(r14) # load constant into f9
    lis     r0,  0x4330
    stw     r0,  SP_FLOAT_TMP(r1)   # store exponent part for integer
    xoris   r3,  r3,  0x8000        # invert sign of integer
    stw     r3,  SP_FLOAT_TMP+4(r1) # store fraction part for integer
    lfd     f5,  SP_FLOAT_TMP(r1)   # load integer in double format into f5
    fsub    f5,  f5,  f9            # f5 contains converted integer
    frsp    f5,  f5                 # double to single
    # now f5 = (int)years since 2000-01-01

    # compute day
    lfs     f2,  (SECONDS_PER_DAY - mainLoop)(r14)
    fdivs   f4,  f1,  f2 # f4 = days since 2000-01-01
    # day = f4 - (f5 * 365)
    lfs     f2,  (DAYS_PER_YEAR - mainLoop)(r14)
    fmuls   f6,  f5,  f2
    fsubs   f5,  f4,  f6

    # convert to integers
    #addi    r4,  r4,  2000 # r4 = year
    fctiw   f2,  f5
    stfd    f2,  SP_FLOAT_TMP(r1)
    lwz     r5,  (SP_FLOAT_TMP+4)(r1) # r5 = day of year
    subi    r5,  r5,  1 # compensate for 2000 not being leap year.

    # we don't actually need the day of month or the month itself.
    # we just need to know if it's the 91st day of the year,
    # or the 92nd day of a leap year.
    li      r9,  90 # minus 1 because we count from 0
    andi.   r8,  r4,  3 # is year divisible by 4?
    bne     .notLeapYear
    # XXX test if divisible by 100 or 400.
    # this needs to be fixed before year 2100.
    #addi    r9,  r9,  1
    # I don't know why the day of year conversion is wrong for leap years
    # and I don't care. Disabling this works well enough.

.notLeapYear:
    li      r6,  0
    cmpw    r5,  r9
    bne     .notApril1
    li      r6,  TEMP_FEATURE_APRIL1
.notApril1:
    LOADW   r7,  PATCH_STATE_PTR
    lbz     r8,  TEMP_FEATURE_FLAGS(r7)
    andi.   r8,  r8,  (~TEMP_FEATURE_APRIL1) & 0x7FFF
    or      r8,  r8,  r6
    stb     r8,  TEMP_FEATURE_FLAGS(r7)

.if UWU_DEBUG
    addi    r5,  r5,  1 # for display
    addi    r3,  r14, s_time - mainLoop
    creqv   4*cr1+eq,4*cr1+eq,4*cr1+eq
    CALL    debugPrintf
.endif

    lwz     r5,  SP_LR_SAVE(r1)
    mtlr    r5   # restore LR
    lmw     r4,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    blr

FLOAT_MAGIC:  .int   0x43300000, 0
FLOAT_MAGIC2: .int   0x43300000,0x80000000
f_2p32:       .float 4294967296.0
TIMER_SCALE:  .double 81000000.0 # everything says this should be 162m / 4,
# but I only seem to get anything sensible with 162m / 2.
SECONDS_PER_YEAR: .float 31556908.8 # approximate average
SECONDS_PER_DAY:  .float 86400 # non-leap days
DAYS_PER_YEAR:    .float 365.242 # average

main: # called by our hook, from the patch list.
    # replaced a call to 800e7e94: BOOL saveFileStruct:isCheatActive(CheatFlag flag)
    # checking if the "dino language" cheat is on.
    # r26 = the message to translate.
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    mflr    r5
    stw     r5,  SP_LR_SAVE(r1)
    stmw    r4,  SP_GPR_SAVE(r1)

    LOADW   r4,  PATCH_STATE_PTR
    lbz     r5,  TEMP_FEATURE_FLAGS(r4)
    andi.   r5,  r5,  TEMP_FEATURE_APRIL1
    beq     .disabled

    # scan the string and replace letters as appropriate.
    li      r15, 0 # char idx
    LOAD    r16, controlCharLength
.getChar:
    add     r3,  r26, r15 # char*
    addi    r4,  r1,  SP_CHR_SIZE # out size
    CALL    shiftJisGetNextChar # r3 = char

    cmpwi   r3,  0
    beq     .end
    cmpwi   r3, 'l'
    beq     .small
    cmpwi   r3, 'r'
    beq     .small
    cmpwi   r3, 'L'
    beq     .large
    cmpwi   r3, 'R'
    beq     .large

    # of course this can't be simple.
    li      r4,  46 # num entries in table
    mr      r5,  r16
.nextControlChar:
    lwz     r6,  0(r5)
    cmpw    r6,  r3
    beq     .foundControl
    addi    r5,  r5,  8 # to next entry
    subi    r4,  r4,  1
    cmpwi   r4,  0
    bge     .nextControlChar
    b       .nextChar

.foundControl:
    lwz     r6,  4(r5) # get length
    slwi    r6,  r6,  1 # times 2 for some reason
    add     r15, r15, r6
    b       .nextChar

.large: # replace large L, R
    li      r3, 'W'
    b       .storeChar
.small: # replace small l, r
    li      r3,  'w'
.storeChar:
    stbx    r3,  r26, r15
.nextChar:
    lwz     r3,  SP_CHR_SIZE(r1)
    add     r15, r15, r3
    b       .getChar

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

.if UWU_DEBUG
    s_time: .string "T:\x84%d%d (%d)\x83\n"
    .align 4
.endif
