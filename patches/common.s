# set register names
.set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
.set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31; .set f0,0; .set f2,2; .set f3,3
.set f0,0;   .set f1,1;   .set f2,2; .set f3,3;   .set f4,4
.set f5,5;   .set f6,6;   .set f7,7;   .set f8,8;   .set f9,9
.set f10,10; .set f11,11; .set f12,12; .set f13,13; .set f14,14
.set f15,15; .set f16,16; .set f17,17; .set f18,18; .set f19,19
.set f20,20; .set f21,21; .set f22,22; .set f23,23; .set f24,24
.set f25,25; .set f26,26; .set f27,27; .set f28,28; .set f29,29
.set f30,30; .set f31,31; .set f0,0; .set f2,2; .set f3,3

.set PAD_BUTTON_LEFT, 0x0001
.set PAD_BUTTON_RIGHT,0x0002
.set PAD_BUTTON_DOWN, 0x0004
.set PAD_BUTTON_UP,   0x0008
.set PAD_BUTTON_Z,    0x0010
.set PAD_BUTTON_R,    0x0020
.set PAD_BUTTON_L,    0x0040
.set PAD_BUTTON_A,    0x0100
.set PAD_BUTTON_B,    0x0200
.set PAD_BUTTON_X,    0x0400
.set PAD_BUTTON_Y,    0x0800
.set PAD_BUTTON_MENU, 0x1000 # Start

# SFA functions
.set __restore_gpr,0x802860f4
.set __save_gpr,0x802860a8
.set allocTagged,0x80023cc8 #void* (uint size,AllocTag tag,char *name)
.set DVDReadAsyncPrio,0x80248eac
.set free,0x800233e8
.set loadFileByPath,0x80015ab4 #void* (char *path,uint *outSize)
    # Returns pointer to allocated buffer of contents.
    # outSize: if not NULL, receives file size.
.set memcpy,0x80003494 # clobbers: r0, r6
.set memset,0x800033D8 # clobbers: r0, r6, r7
.set modelLoad,0x80029570 # ModelFileHeader* (
    # int modelNum,ModelFlags_loadCharacter flags,uint *outSize)
    # if modelNum is negative, don't use MODELIND.bin
.set sprintf,0x8028f688
.set storeRegs26,0x802860D8
.set zlbDecompress,0x8004B658 # void *data,uint compLen,void *out
    # returns with r5 = out + rawLen


# SFA globals
.set fileBuffers,0x8035f3e8 # void*[fileIdx]
.set mapInfo,0x816a8ba0 # char[28] name
.set buttonsJustPressed,0x803398d0
.set pPlayer,0x803428f8
.set playerId,0x803a32c8 # 0=Krystal, 1=Fox

# we can't use `bl` instruction because it's relative and we don't
# know where our code will end up. so we use this instead.
# this clobbers lr and r0
.macro CALL addr
    lis  r0,\addr@h
    ori  r0,r0,\addr@l
    mtlr r0
    blrl  # branch link to the address in the link register
.endm

# jump to absolute address, clobber specified register
.macro JUMP addr, reg
    lis  \reg, \addr@h
    ori  \reg, \reg, \addr@l
    mtlr \reg
    blr
.endm

# expands to:
# lis reg, X
# lwz reg, Y(reg)
# where X and Y are the upper and lower halves of addr,
# adjusted as necessary when Y >= 0x8000.
.macro LOADW reg, addr
    lis \reg, (\addr >> 16) + ((\addr & 0x8000) >> 15)
    lwz \reg, -(((\addr & 0x8000) << 1) - (\addr & 0xFFFF))(\reg)
.endm

.macro LOAD reg, val
    lis \reg, \val@h
    ori \reg, \reg, \val@l
.endm

.include "gecko.s"
