# set register names
.set r0,0;   .set r1,1;   .set r2,2; .set r3,3;   .set r4,4
.set r5,5;   .set r6,6;   .set r7,7;   .set r8,8;   .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31; .set f0,0; .set f2,2; .set f3,3

# SFA functions
.set __restore_gpr,0x802860f4
.set __save_gpr,0x802860a8
.set allocTagged,0x80023cc8 #void* (uint size,AllocTag tag,char *name)
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
.set pPlayer,0x803428f8

.macro GECKO_BEGIN_PATCH addr
    # Generate an "Insert ASM" Gecko code line.
    # Your code should follow this.
    #.int 0x42000000, 0x80000000 # set base address
    .int 0xC2000000 | (\addr & 0xFFFFFF), ((_end - _start)+7) / 8
    _start:
.endm

.macro GECKO_END_PATCH
    .int 0 # Gecko code must end with this
    _end:
.endm

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
