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
# C6aaaaaa tttttttt: insert branch to `tttttttt` at `80aaaaaa`.
# C7aaaaaa tttttttt: insert branch to `tttttttt` at `81aaaaaa`.
# Works like Gecko, but if the lowest bit of `tttttttt` is set, it will
# insert a `blr` instead of a `b`. The target is relative to the beginning
# of the patch file.
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

.text
.include "common.s"
#81681f8c

constants:
    .set STACK_SIZE,0x40 # how much to reserve
    .set SP_LR_SAVE,0x10
    .set SP_R0_SAVE,0x14
    .set SP_PATCH_ADDR,0x18
    .set SP_FLAGS_ADDR,0x1C

start:
    # entry point of this file.
    # here we install hooks as needed.
    stwu r1, -STACK_SIZE(r1) # get some stack space
    mflr r3
    stw  r3,  SP_LR_SAVE(r1)
    stw  r0,  SP_R0_SAVE(r1) # XXX needed?

    # get debugMsg address
    bl .getpc
    .getpc:
        mflr r14
        addi r3, r14, (.debugMsg - .getpc)@l

    # say something to show we're alive.
    CALL OSReport

    # load patches
    # since we don't have DVDOpenDir or other dir functions,
    # we'll number them.
    li    r15, 0 # patch idx

.nextPatch:
    addi    r3, r14, (.patchPath - .getpc)@l + 8 # offset of "000000"
    addi    r4, r14, (.patchFmt - .getpc) # format
    mr      r5, r15 # idx
    addi    r15, r15, 1
    CALL    sprintf # now patchPath = path to load
    addi    r3, r14, (.patchPath - .getpc)@l
    li      r4, 0 # we don't need size
    CALL    loadFileByPath
    cmpwi   r3, 0
    beq     .done # file doesn't exist

    # r3 = patch file
    # interpret the patch list
    stw     r3, SP_PATCH_ADDR(r1) # save to free later
    subi    r3, r3, 8
.nextListItem:
    lwzu    r4, 8(r3) # get first word and advance to next item
    cmpwi   r4, 0
    beq     .endList
    # get command in r5
    # rotate left 8 to move high byte to low byte
    # then mask off low bit and high bytes
    # remember PPC has bit 0 = MSB
    rlwinm  r5, r4, 8, 24, 30 # shift, mask start, mask end

    # convert r4 to address
    rlwinm  r4, r4, 0, 7, 31 # mask out command
    lis     r6, 0x8000 # set high bit
    or      r4, r4, r6 # r4 = addr

    # check command
    cmpwi   r5, 0x00
    beq     .write8
    cmpwi   r5, 0x02
    beq     .write16
    cmpwi   r5, 0x04
    beq     .write32
    cmpwi   r5, 0xC6
    beq     .makeBl

    # unsupported command...
    #b       .nextListItem
    tw       2, r0, r0 # trap if r0 == r0 (ie always)

.write8: # r4 = address; r3 -> patch list entry
    lhz     r9, 4(r3) # get count
    lbz     r6, 7(r3) # get value
    addi    r9, r9, 1
    mr      r5, r9 # copy count

.write8_next: # write bytes in loop
    subi    r5, r5, 1
    stbx    r6, r4, r5
    cmpwi   r5, 0
    bne     .write8_next
    b       .write_flush


.write16: # r4 = address; r3 -> patch list entry
    lhz     r9, 4(r3) # get count
    lhz     r6, 6(r3) # get value
    addi    r9, r9, 1
    slwi    r9, r9, 1 # double count
    mr      r5, r9 # copy count

.write16_next: # write halfwords in loop
    subi    r5, r5, 2
    sthx    r6, r4, r5
    cmpwi   r5, 0
    bne     .write16_next
    b       .write_flush


.write32: # r4 = address; r3 -> patch list entry
    lwz     r6, 4(r3) # get value
    stw     r6, 0(r4) # store value
    li      r9, 4 # number of bytes to flush

.write_flush:
    # flush cache
    # this is also necessary to tell Dolphin to recompile changed code
    mr     r28, r3
    mr     r3,  r4 # addr
    mr     r4,  r9 # count
    CALL   iCacheFlush
    mr     r3,  r28
    b      .nextListItem



.makeBl: # r4 = address; r3 -> patch list entry
    # low bit of address = whether to make a branch (0) or bl (1)
    lwz    r5, 4(r3) # get value
    lwz    r6, SP_PATCH_ADDR(r1) # get patch base address
    add    r5, r5, r6 # convert patch offset to absolute address
    mr     r26, r5 # for debug log

    sub    r7, r5, r4 # r7 = relative destination addr
    rlwinm r7, r7, 0, 1, 29 # r7 = r7 & 0x07FFFFFC
    lis    r6, 0x4800
    or     r7, r7, r6 # make it a branch
    andi.  r5, r5, 1
    or     r7, r7, r5 # optionally make it a bl
    lis    r6, 0x8000
    or     r4, r4, r6 # map address to 8xxxxxxx
    stw    r7, 0(r4)

    # debug log
    mr     r28, r3
    addi   r3,  r14, (.s_makeBl - .getpc)@l
    mr     r5,  r26
    mr     r6,  r7
    CALL   OSReport
    mr     r3,  r28 # addr
    li     r9,  4   # count
    b      .write_flush


.endList:
    # r3 now points to the terminating entry of the patch list
    # which is int 0, int flags. grab the flags
    lwz    r4, 4(r3)
    stw    r4, SP_FLAGS_ADDR(r1)

    # now execute the patch
    addi   r17, r3, 8
    addi   r4,  r15, -1
    addi   r5,  r3, 8
    addi   r3,  r14, (.s_run - .getpc)@l
    CALL   OSReport

    lwz    r3, SP_PATCH_ADDR(r1) # pass base address to patch code.
    mtspr  CTR, r17
    bctrl

    # now handle the flags, of which there's only one.
    lwz    r4, SP_FLAGS_ADDR(r1)
    andi.  r4, r4, 1
    beq    .noFree

    # flag 0x01: free this patch after executing it.
    addi   r3, r14, (.s_free - .getpc)@l
    addi   r4, r15, -1
    CALL   OSReport
    lwz    r3, SP_PATCH_ADDR(r1)
    CALL   mm_free
    # 81681d08


.noFree:
    b .nextPatch

.done:
    lwz  r0, SP_R0_SAVE(r1)
    lwz  r3, SP_LR_SAVE(r1)
    mtlr r3 # restore LR
    addi r1, r1, STACK_SIZE # restore stack ptr
    blr

.debugMsg:
    .string "hello, cruel world"

.s_run:
    .string "run patch %d @%08X"

.s_free:
    .string "free patch %d"

.patchPath:
    .string "patches/0000"

.patchFmt:
    .string "%04d"

.s_makeBl:
    .string "branch %08X -> %08X: %08X"
