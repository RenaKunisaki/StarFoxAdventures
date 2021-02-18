# Contents of debug.bin.
# This file is loaded at startup by our modified main.dol and executed.
# No header, just code. This code must be position-independent.
# This program loads and executes files from the `patches` directory.
# Each patch must be named sequentially, eg "0000", "0001",
# ... "0009", "0010"... (no extensions)
# (having more than 10,000 patches may break something, but you'll run
# out of RAM long before that.)
# All other files are ignored. Loading stops once a patch is not found.
#
# Each patch file begins with a patch list, which is similar to a Gecko
# code (but with very few supported types). The first byte (with the low)
# bit masked off, as it's actually part of the address) is the opcode,
# and the next 7 bytes are the address and data.
#
# 00aaaaaa cccc00vv: write byte `vv` at `80aaaaaa` to `80aaaaaa+cccc`.
# 01aaaaaa cccc00vv: write byte `vv` at `81aaaaaa` to `81aaaaaa+cccc`.
# 02aaaaaa ccccvvvv: write halfword `vvvv` at `80aaaaaa` to `80aaaaaa+cccc`.
# 03aaaaaa ccccvvvv: write halfword `vvvv` at `81aaaaaa` to `81aaaaaa+cccc`.
# 04aaaaaa vvvvvvvv: write word `vvvvvvvv` at `80aaaaaa`.
# 05aaaaaa vvvvvvvv: write word `vvvvvvvv` at `81aaaaaa`.
# for 00,01,02,03, cccc is inclusive, so cccc=1 will write to
# both aaaaaa and aaaaaa+1 (for 00,01) or aaaaaa+2 (for 02,03).
#
# C0000000 aaaaaaaa: call `aaaaaaaa` from the game main loop.
# C0000001 aaaaaaaa: call `aaaaaaaa` from late in the game's init routine.
# C0000002 aaaaaaaa: call `aaaaaaaa` just after reading controller input.
# This should be an address relative to the start of the patch file.
# The code is called with r3 = its own address, to help with writing
# position-independent code.
# (in Gecko this calls code immediately following, but that's of
# little use here.)
#
# C6aaaaaa tttttttt: insert branch to `tttttttt` at `80aaaaaa`.
# C7aaaaaa tttttttt: insert branch to `tttttttt` at `81aaaaaa`.
# Works like Gecko, but if the lowest bit of `tttttttt` is set, it will
# insert a `blr` instead of a `b`. The target is relative to the beginning
# of the patch file.
#
# FExxxxxx xxxxxxxx: does nothing. Useful to insert the patch's name
# at the beginning of the file for quick identification, so long as
# it's padded to exactly 7 characters.
# This type is unused in Gecko. It's not 0xFF because the lowest bit
# isn't part of the type.
#
# 00000000 xxxxxxxx: End patch list.
#  xxxxxxxx: flags:
#  00000001: free this patch after executing it.
#
# After this is code that's executed as soon as the patch is loaded.
# You can just stick a `blr` here if you want.
# This code is called with the register states:
#  r3 = memory address of patch file
# The patch should follow the usual ABI rules, ie clobber r3-r12,
# and save the rest.
# 816820e8
# 80003ebc

# patchinfo.bin format:
# 0000 u32 magic;     //0x52656E61
# 0004 u32 numFiles;  //number of patch files to load
# 0008 u32 totalSize; //total size to allocate for patch files
# 000C u8  verMajor;  //major version number
# 000D u8  verMinor;  //minor version number
# 000E u16 verRev;    //revision number
# 0010 u32 buildNo;   //build number

.set DEBUG,0
.text
.include "common.s"
.include "globals.s"

constants:
    .set MAX_MAIN_LOOP_HOOKS,16
    .set MAX_STARTUP_HOOKS,  16
    .set MAX_PAD_HOOKS,      16
    .set MAIN_LOOP_HOOK_ADDR,0x80020D4C # bl doNothing_80014F3C
    .set STARTUP_HOOK_ADDR,  0x80021038 # bl installBsodHandlers
    .set PAD_HOOK_ADDR,      0x80014f90 # 482398D5 bl padReadControllers

    .set OP_WRITE8,  0x00
    .set OP_WRITE16, 0x02
    .set OP_WRITE32, 0x04
    .set OP_ADD_HOOK,0xC0
    .set OP_BRANCH,  0xC6
    .set OP_NOP,     0xFE

    .set STACK_SIZE,   0xC0 # how much to reserve
    .set SP_LR_SAVE,   0xC4
    .set SP_TMP1,      0x10
    .set SP_PATCH_ADDR,0x14
    .set SP_FLAGS_ADDR,0x18
    .set SP_GPR_SAVE,  0x1C

start:
    # entry point of this file.
    # here we install hooks as needed.
    # this is called very early in init, so we may not want to run
    # most patches yet.

    mr      r14, r3 # save address of this code
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)

    # say something to show we're alive.
    .if DEBUG
        addi r3, r14, (.debugMsg - start)@l
        CALL OSReport
    .endif

    # set our state pointer
    addi    r15, r14, patchStateVars - start
    LOADWH  r4,  PATCH_STATE_PTR
    STOREW  r15, PATCH_STATE_PTR, r4

    # install startup hook.
    addi    r5,  r14, (startHook - start)@l # absolute destination address
    LOAD    r4,  STARTUP_HOOK_ADDR
    sub     r7,  r5,  r4 # r7 = relative destination addr
    rlwinm  r7,  r7,  0, 1, 29 # r7 = r7 & 0x07FFFFFC
    oris    r7,  r7,  0x4800
    ori     r7,  r7,  1 # make it a bl
    stw     r7,  0(r4)
    li      r7,  0
    icbi    r7,  r4

    # install main loop hook.
    addi    r5,  r14, (mainLoop - start)@l # absolute destination address
    LOAD    r4,  MAIN_LOOP_HOOK_ADDR
    sub     r7,  r5,  r4 # r7 = relative destination addr
    rlwinm  r7,  r7,  0, 1, 29 # r7 = r7 & 0x07FFFFFC
    oris    r7,  r7,  0x4800
    ori     r7,  r7,  1 # make it a bl
    stw     r7,  0(r4)
    li      r7,  0
    icbi    r7,  r4

    # install pad hook.
    addi    r5,  r14, (padHook - start)@l # absolute destination address
    LOAD    r4,  PAD_HOOK_ADDR
    sub     r7,  r5,  r4 # r7 = relative destination addr
    rlwinm  r7,  r7,  0, 1, 29 # r7 = r7 & 0x07FFFFFC
    oris    r7,  r7,  0x4800
    ori     r7,  r7,  1 # make it a bl
    stw     r7,  0(r4)
    li      r7,  0
    icbi    r7,  r4

    # load patch info file
    addi    r3,  r14, s_patchInfoPath - start
    li      r4,  0 # we don't need size
    CALL    loadFileByPath

    # check magic
    LOAD    r4,  0x52656E61
    lwz     r5,  0x00(r3)
    cmpw    r4,  r5
    bnea    0xFFFFFFFFFFFFBAD0 # mismatch

    # 0000 u32 magic;     //0x52656E61
    # 0004 u32 numFiles;  //number of patch files to load
    # 0008 u32 totalSize; //total size to allocate for patch files
    # 000C u8  verMajor;  //major version number
    # 000D u8  verMinor;  //minor version number
    # 000E u16 verRev;    //revision number
    # 0010 u32 buildNo;   //build number

    # copy some of that into the state struct
    lwz     r4,  0x0C(r3) # version
    stw     r4,  PATCH_INFO_VERSION(r15)
    lwz     r4,  0x10(r3) # build number
    stw     r4,  PATCH_INFO_BUILD_NO(r15)

    # copy the rest into registers
    lwz     r16, 0x04(r3) # number of files
    lwz     r17, 0x08(r3) # alloc size

    # now we can free it
    CALL    mm_free

    # alloc space for the patch files
    mr      r3,  r17    # size
    li      r4,  0x6502 # arbitrary tag
    li      r5,  0      # name
    CALL    allocTagged

    # load patches
    # since we don't have DVDOpenDir or other dir functions,
    # we'll number them.
    li      r15, 0 # patch idx
    mr      r17, r3 # buffer
.nextPatch:
    cmpw    r15, r16
    bge     .done # no files left to load

    addi    r3,  r14, (.patchPath - start)@l + 8 # offset of "000000"
    addi    r4,  r14, (.patchFmt - start)@l # format
    mr      r5,  r15 # idx
    addi    r15, r15, 1
    CALL    sprintf # now patchPath = path to load
    addi    r3,  r14, (.patchPath - start)@l
    addi    r4,  r1,  SP_TMP1 # out size
    CALL    loadFileByPath
    cmpwi   r3,  0
    beq     .done # file doesn't exist

.if DEBUG
    mr      r29, r3
    addi    r3,  r14, .s_patchSize - start
    subi    r4,  r15, 1
    lwz     r5,  SP_TMP1(r1) # size
    CALL    OSReport
    mr      r3,  r29
.endif

    # copy the file into the buffer.
    # this is a bit inefficient but it's what the game does in a bunch
    # of places, and it's much easier than reimplementing loadFileByPath().
    mr      r29, r3 # save src
    mr      r4,  r3 # src
    lwz     r5,  SP_TMP1(r1) # size
    mr      r3,  r17 # dest
    CALL    memcpy # returns dest
    mr      r3,  r29 # restore file buffer
    CALL    mm_free  # and free it
    mr      r3,  r17
    # XXX this means we can't have "free patch after running" unless we were to
    # later sutract size from r17.
    # in that case also we'd still be using memory for that patch.
    # also, need to ensure every patch file size is a multiple of 4.
    # it would vastly simplify things if we did this by padding them at
    # creation time.
    # but do we even use this feature? at least once, for pausemenu
    # but the code to deal with it might well take up more space than the patch.

    # flush patch code
    # r3 is already addr
    lwz     r4,  SP_TMP1(r1) # count
    CALL    iCacheFlush
    mr      r3,  r17
    lwz     r5,  SP_TMP1(r1) # size
    add     r17, r17, r5 # next buffer space

    # r3 = patch file
    # interpret the patch list
    stw     r3,  SP_PATCH_ADDR(r1) # save to free later
    subi    r3,  r3,  8
.nextListItem:
    lwzu    r4,  8(r3) # get first word and advance to next item
    cmpwi   r4,  0
    beq     .endList
    # get command in r5
    # rotate left 8 to move high byte to low byte
    # then mask off low bit and high bytes
    # remember PPC has bit 0 = MSB
    rlwinm  r5,  r4,  8, 24, 30 # shift, mask start, mask end

    # convert r4 to address
    rlwinm  r4,  r4,  0, 7, 31 # mask out command
    oris    r4,  r4,  0x8000 # r4 = addr (set high bit)

    # check command
    # XXX make a table
    cmpwi   r5,  OP_WRITE8
    beq     .write8
    cmpwi   r5,  OP_WRITE16
    beq     .write16
    cmpwi   r5,  OP_WRITE32
    beq     .write32
    cmpwi   r5,  OP_BRANCH
    beq     .makeBl
    cmpwi   r5,  OP_ADD_HOOK
    beq     .addHook
    cmpwi   r5,  OP_NOP
    beq     .nextListItem

    # unsupported command...
    #b       .nextListItem
    ba       0xFFFFFFFFFFFFBAD4

.write8: # r4 = address; r3 -> patch list entry
    lhz     r9,  4(r3) # get count
    lbz     r6,  7(r3) # get value
    addi    r9,  r9,  1
    mr      r5,  r9 # copy count

.write8_next: # write bytes in loop
    subi    r5,  r5,  1
    stbx    r6,  r4,  r5
    cmpwi   r5,  0
    bne     .write8_next
    b       .write_flush

.write16: # r4 = address; r3 -> patch list entry
    lhz     r9,  4(r3) # get count
    lhz     r6,  6(r3) # get value
    addi    r9,  r9,  1
    slwi    r9,  r9,  1 # double count
    mr      r5,  r9 # copy count

.write16_next: # write halfwords in loop
    subi    r5,  r5,  2
    sthx    r6,  r4,  r5
    cmpwi   r5,  0
    bne     .write16_next
    b       .write_flush

.write32: # r4 = address; r3 -> patch list entry
    lwz     r6,  4(r3) # get value
    stw     r6,  0(r4) # store value
    li      r9,  4 # number of bytes to flush

.write_flush:
    # flush cache
    # this is also necessary to tell Dolphin to recompile changed code
    mr      r28, r3
    mr      r3,  r4 # addr
    mr      r4,  r9 # count
    CALL    iCacheFlush
    mr      r3,  r28
    b       .nextListItem


.makeBl: # r4 = address; r3 -> patch list entry
    # low bit of address = whether to make a branch (0) or bl (1)
    lwz     r5,  4(r3) # get value
    lwz     r6,  SP_PATCH_ADDR(r1) # get patch base address
    add     r5,  r5,  r6 # convert patch offset to absolute address
    mr      r26, r5 # for debug log

    sub     r7,  r5,  r4 # r7 = relative destination addr
    rlwinm  r7,  r7,  0, 1, 29 # r7 = r7 & 0x07FFFFFC
    oris    r7,  r7,  0x4800 # make it a branch
    andi.   r5,  r5,  1
    or      r7,  r7,  r5 # optionally make it a bl
    oris    r4,  r4,  0x8000 # map patched address to 8xxxxxxx
    stw     r7,  0(r4)

    # debug log
    .if DEBUG
        mr     r28, r3
        addi   r3,  r14, (.s_makeBl - start)@l
        mr     r5,  r26
        mr     r6,  r7
        CALL   OSReport
        mr     r3,  r28 # addr
    .endif
    li     r9,  4   # count
    b      .write_flush


.addHook: # r4 = address; r3 -> patch list entry
    addi    r7,  r14, ((mainLoopHooks - start) - 4)@l
    lwz     r5,  0(r3) # get opcode
    andi.   r6,  r5, 3
    cmpwi   r6,  0
    beq     .addMainLoopHook
    cmpwi   r6,  2
    beq     .addPadHook

    # add startup hook
    addi    r7,  r14, ((startupHooks - start) - 4)@l

.addMainLoopHook: # r4 = address; r3 -> patch list entry
    lwz     r5,  4(r3) # get value
    lwz     r6,  SP_PATCH_ADDR(r1) # get patch base address
    add     r5,  r5,  r6 # convert patch offset to absolute address

.tryNextHook:
    lwzu    r6,  4(r7)
    cmpwi   r6,  0
    bne     .tryNextHook
    stw     r5,  0(r7)
    b       .nextListItem
    # XXX ensure we don't run out of hooks

.addPadHook:
    addi    r7,  r14, ((padHooks - start) - 4)@l
    b       .addMainLoopHook


.endList:
    # r3 now points to the terminating entry of the patch list
    # which is int 0, int flags. grab the flags
    lwz     r4,  4(r3)
    stw     r4,  SP_FLAGS_ADDR(r1)

    # now execute the patch
    addi   r18, r3, 8
    .if DEBUG
        addi   r4,  r15, -1
        addi   r5,  r3, 8
        lwz    r6,  0(r5)
        addi   r3,  r14, (.s_run - start)@l
        CALL   OSReport
    .endif

    lwz     r3,  SP_PATCH_ADDR(r1) # pass base address to patch code.
    mtspr   CTR, r18
    bctrl

    # now handle the flags, of which there's only one.
    lwz     r4,  SP_FLAGS_ADDR(r1)
    andi.   r4,  r4,  1
    beq     .noFree

    # flag 0x01: free this patch after executing it.
    .if DEBUG
        addi   r3,  r14, (.s_free - start)@l
        addi   r4,  r15, -1
        CALL   OSReport
    .endif
    #lwz     r3,  SP_PATCH_ADDR(r1)
    #CALL    mm_free


.noFree:
    b       .nextPatch

.done:
endSub:
    lwz     r3,  SP_LR_SAVE(r1)
    mtlr    r3   # restore LR
    lmw     r3,  SP_GPR_SAVE(r1)
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    blr


startHook: # called from startup
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)
    CALL    0x80137d28 # installBsodHandlers() (replaced instr)
    li      r15, (startupHooks - .mainLoop_getpc) - 4
    b       .mainLoopDoHooks

padHook: # called from pad update
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r4
    stw     r4,  SP_LR_SAVE(r1)
    CALL    0x8024e864 # padReadControllers() (replaced instr)
    lwz     r4,  SP_GPR_SAVE(r1) # pass output pointer as param
    stw     r3,  SP_TMP1(r1) # save result
    li      r15, (padHooks - .mainLoop_getpc) - 4
    bl      .mainLoopDoHooks

    lwz     r4,  SP_LR_SAVE(r1)
    mtlr    r4   # restore LR
    lwz     r3,  SP_TMP1(r1) # restore result
    addi    r1,  r1,  STACK_SIZE # restore stack ptr
    blr


mainLoop: # called from main loop
    stwu    r1,  -STACK_SIZE(r1) # get some stack space
    stmw    r3,  SP_GPR_SAVE(r1)
    mflr    r3
    stw     r3,  SP_LR_SAVE(r1)
    li      r15, (mainLoopHooks - .mainLoop_getpc) - 4

.mainLoopDoHooks:
    bl      .mainLoop_getpc
    .mainLoop_getpc:
        mflr r14
    add     r15, r15, r14

.callNextHook:
    lwzu    r3,  4(r15)
    cmpwi   r3,  0
    beq     .mainLoopDone
    mtspr   CTR, r3
    bctrl
    b       .callNextHook

.mainLoopDone:
    b       endSub


startupHooks:
    .rept MAX_STARTUP_HOOKS
    .int 0
    .endr

mainLoopHooks:
    .rept MAX_MAIN_LOOP_HOOKS
    .int 0
    .endr

padHooks:
    .rept MAX_PAD_HOOKS
    .int 0
    .endr

patchStateVars:
    .rept PATCH_STATE_SIZE
    .byte 0
    .endr

s_patchInfoPath: .string "patches/patchinfo.bin"
.patchPath: .string "patches/0000"
.patchFmt:  .string "%04d"
.if DEBUG
    .debugMsg:  .string "hello, cruel world"
    .s_patchSize: .string "loaded patch %d size %d"
    .s_run:     .string "run patch %d @ %08X: %08X"
    .s_free:    .string "free patch %d"
    .s_makeBl:  .string "branch %08X -> %08X: %08X"
    .s_startup: .string "run startup hooks"
.endif
