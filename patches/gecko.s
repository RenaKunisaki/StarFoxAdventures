# macros for assembling Gecko codes in binary form.

.macro GECKO_BEGIN_PATCH addr, name=patch
    # Generate an "Insert ASM" Gecko code line.
    # Your code should follow this.
    #.int 0x42000000, 0x80000000 # set base address
    .int 0xC2000000 | ((\addr) & 0xFFFFFF), ((_patch_end_\name - _patch_start_\name)+7) / 8
    _patch_start_\name:
.endm

.macro GECKO_END_PATCH name=patch
    .int 0 # Gecko code must end with this
    _patch_end_\name:
.endm

# write val to addr+(ba or po, depending on use_po), count times
.macro GECKO_WRITE8 addr, val, count=1, use_po=0
    .int 0x00000000 | ((\addr) & 0x01FFFFFF) | ((\use_po) << 28)
    .int (((\count)-1) << 16) | (\val)
.endm

# write val to addr+(ba or po, depending on use_po), count times
.macro GECKO_WRITE16 addr, val, count=1, use_po=0
    .int 0x02000000 | ((\addr) & 0x01FFFFFF) | (\use_po << 28)
    .int (((\count)-1) << 16) | (\val)
.endm

# write val to addr+(ba or po, depending on use_po)
.macro GECKO_WRITE32 addr, val, use_po=0
    .int 0x04000000 | ((\addr) & 0x01FFFFFF) | ((\use_po) << 28)
    .int (\val)
.endm

# execute following codes if (*(u16*)addr) & mask == val.
# endif: if 1, apply an "end if" first
# use_po: use address (addr+po) if 1, (addr+ba) if 0
.macro GECKO_IFEQ_16 addr, val, mask=0xFFFF, endif=0, use_po=0
    .int 0x28000000 | ((\addr) & 0x01FFFFFF) | ((\use_po) << 28) | (\endif)
    .int (((\mask) ^ 0xFFFF) << 16) | (\val)
.endm

# apply count "end if" codes
# ba: if not 0, set `ba` value to ba<<16
# po: if not 0, set `po` value to po<<16
.macro GECKO_ENDIF count=1, ba=0, po=0
    .int 0xE2000000 | (\count)
    .int ((\ba) << 16) | (\po)
.endm

# po = *(addr + (grN if n>=0) + (ba if use_ba) + (po if use_po)) + (po if add)
# use_po and use_ba are mutually exclusive
.macro GECKO_LOAD_PO addr, add=0, gr=-1, use_po=0, use_ba=0
    .if (\gr) >= 0
        .int 0x48001000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16) | (\gr)
    .else
        .int 0x48000000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16)
    .endif
    .int (\addr)
.endm

.macro GECKO_LOAD_BA addr, add=0, gr=-1, use_po=0, use_ba=0
    .if (\gr) >= 0
        .int 0x40001000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16) | (\gr)
    .else
        .int 0x40000000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16)
    .endif
    .int (\addr)
.endm

# if(po >= (lo << 16) && po < (hi << 16))
.macro GECKO_CHECK_PO lo=0x8000, hi=0x8180, endif=0
    .int 0xDE000000 | (\endif)
    .int ((\lo) << 16) | (\hi)
.endm

.macro GECKO_CHECK_BA lo=0x8000, hi=0x8180, endif=0
    .int 0xCE000000 | (\endif)
    .int ((\lo) << 16) | (\hi)
.endm

# 4PTYZ00N XXXXXXXX
# P: what to set: 0=ba, 8=po
# T: 0=set 1=add
# Y: 1=add ba to XXXXXXXX
# Z: 1=add grN to XXXXXXXX
# 48000 : po = [       XXXXXXXX]
# 48001 : po = [   grN+XXXXXXXX]
# 48010 : po = [ba    +XXXXXXXX]
# 48011 : po = [ba+grN+XXXXXXXX]
# 48100 : po+= [       XXXXXXXX]
# 48101 : po+= [   grN+XXXXXXXX]
# 48110 : po+= [ba    +XXXXXXXX]
# 48111 : po+= [ba+grN+XXXXXXXX]
# 58010 : po = [po    +XXXXXXXX]
# 58011 : po = [po+grN+XXXXXXXX]
# 58110 : po+= [po    +XXXXXXXX]
# 58111 : po+= [po+grN+XXXXXXXX]

# po = val + (grN if n>=0) + (ba if use_ba) + (po if use_po) + (po if add)
# use_po and use_ba are mutually exclusive
.macro GECKO_SET_PO val, add=0, gr=-1, use_po=0, use_ba=0
    .if (\gr) >= 0
        .int 0x4A001000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16) | (\gr)
    .else
        .int 0x4A000000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16)
    .endif
    .int (\val)
.endm

.macro GECKO_SET_BA val, add=0, gr=-1, use_po=0, use_ba=0
    .if (\gr) >= 0
        .int 0x42001000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16) | (\gr)
    .else
        .int 0x42000000 | ((\use_po)<<28) | ((\add)<<20) | ((\use_ba)<<16)
    .endif
    .int (\val)
.endm

# 4PTYZ00N XXXXXXXX
# P: what to set: 2=ba, A=po
# T: 0=set 1=add
# Y: 1=add ba to XXXXXXXX
# Z: 1=add grN to XXXXXXXX
# 4A000 : po =        XXXXXXXX
# 4A001 : po =    grN+XXXXXXXX
# 4A010 : po = ba    +XXXXXXXX
# 4A011 : po = ba+grN+XXXXXXXX
# 4A100 : po+=        XXXXXXXX
# 4A101 : po+=    grN+XXXXXXXX
# 4A110 : po+= ba    +XXXXXXXX
# 4A111 : po+= ba+grN+XXXXXXXX
# 5A010 : po = po    +XXXXXXXX
# 5A011 : po = po+grN+XXXXXXXX
# 5A110 : po+= po    +XXXXXXXX
# 5A111 : po+= po+grN+XXXXXXXX

# *(addr + (po or ba) + (grN if n>=0)) = po
# use_po and use_ba are mutually exclusive
.macro GECKO_STORE_PO addr, gr=-1, use_po=0, use_ba=0
    .if (\gr) >= 0
        .int 0x4C001000 | ((\use_po)<<28) | ((\use_ba)<<16) | (\gr)
    .else
        .int 0x4C000000 | ((\use_po)<<28) | ((\use_ba)<<16)
    .endif
    .int (\val)
.endm

.macro GECKO_STORE_BA addr, gr=-1, use_po=0, use_ba=0
    .if (\gr) >= 0
        .int 0x44001000 | ((\use_po)<<28) | ((\use_ba)<<16) | (\gr)
    .else
        .int 0x44000000 | ((\use_po)<<28) | ((\use_ba)<<16)
    .endif
    .int (\val)
.endm

# set po to address of next line plus offset (int16)
.macro GECKO_GET_NEXT_PO offset=0
    .int 0x4E000000 | (\offset)
    .int 0x00000000
.endm

.macro GECKO_GET_NEXT_BA offset=0
    .int 0x46000000 | (\offset)
    .int 0x00000000
.endm

# 4P0YZ00N XXXXXXXX
# P: what to store: 4=ba, C=po
# Y: 1=add po or ba to XXXXXXXX
# Z: 1=add grN to XXXXXXXX
# 4C000 : [XXXXXXXX]        = po
# 4C001 : [XXXXXXXX+grN]    = po
# 4C010 : [XXXXXXXX+ba]     = po
# 4C011 : [XXXXXXXX+ba+grN] = po
# 5C010 : [XXXXXXXX+po]     = po
# 5C011 : [XXXXXXXX+po+grN] = po

# grN = val + (ba or po)
.macro GECKO_SET_GR r, val, use_po=0, use_ba=0
    .int 0x80000000 | (\r) | ((\use_ba) << 16) | ((\use_po) << 28)
    .int (\val)
.endm

.macro GECKO_ADD_GR r, val, use_po=0, use_ba=0
    .int 0x80100000 | (\r) | ((\use_ba) << 16) | ((\use_po) << 28)
    .int (\val)
.endm

# 80SY000N XXXXXXXX
# S: 0=set 1=add
# Y: 1=add ba or po
# 8000 : grN = XXXXXXXX
# 8001 : grN = XXXXXXXX+ba
# 8010 : grN+= XXXXXXXX
# 8011 : grN+= XXXXXXXX+ba
# 9001 : grN = XXXXXXXX+po
# 9011 : grN+= XXXXXXXX+po

.macro GECKO_LOAD_GR_8 r, val, use_po=0, use_ba=0
    .int 0x82000000 | (\r) | ((\use_ba) << 16) | ((\use_po) << 28)
    .int (\val)
.endm
.macro GECKO_LOAD_GR_16 r, val, use_po=0, use_ba=0
    .int 0x82100000 | (\r) | ((\use_ba) << 16) | ((\use_po) << 28)
    .int (\val)
.endm
.macro GECKO_LOAD_GR_32 r, val, use_po=0, use_ba=0
    .int 0x82200000 | (\r) | ((\use_ba) << 16) | ((\use_po) << 28)
    .int (\val)
.endm

# 82UY000N XXXXXXXX
# U: 0=8bit 1=16bit 2=32bit
# YL add po or ba
# 8200 : grN=  8bits at [XXXXXXXX]
# 8210 : grN= 16bits at [XXXXXXXX]
# 8220 : grN= 32bits at [XXXXXXXX]
# 8201 : grN=  8bits at [XXXXXXXX+ba]
# 8211 : grN= 16bits at [XXXXXXXX+ba]
# 8221 : grN= 32bits at [XXXXXXXX+ba]
# 9201 : grN=  8bits at [XXXXXXXX+po]
# 9211 : grN= 16bits at [XXXXXXXX+po]
# 9221 : grN= 32bits at [XXXXXXXX+po]

.macro GECKO_GR_OP r, op, val, regp=0, valp=0
    .int 0x86000000 | (\r) | ((\regp)<<16) | ((\valp)<<17) | ((\op)<<20)
    .int (\val)
.endm
.set GECKO_OP_ADD,   0 # add (+)
.set GECKO_OP_MUL,   1 # mul (*)
.set GECKO_OP_OR,    2 # or  (|)
.set GECKO_OP_AND,   3 # and (&)
.set GECKO_OP_XOR,   4 # xor (^)
.set GECKO_OP_SLW,   5 # slw (<<)
.set GECKO_OP_SRW,   6 # srw (>>)
.set GECKO_OP_ROL,   7 # rol (rotate left)
.set GECKO_OP_ASR,   8 # asr (arithmetic shift right)
.set GECKO_OP_FADDS, 9 # fadds (single float add)
.set GECKO_OP_FMULS,10 # fmuls (single float mul)

# 86TY000N XXXXXXXX
# T: op (the ? below)
# Y: 2 bits
#   bit 0: grN is ptr
#   bit 1: val is ptr
# 86T0 :  grN  = ( grN  ?  XXXXXXXX)
# 86T1 : [grN] = ([grN] ?  XXXXXXXX)
# 86T2 :  grN  = ( grN  ? [XXXXXXXX])
# 86T3 : [grN] = ([grN] ? [XXXXXXXX])

.macro GECKO_STORE_GR r, addr, size, count=1, use_po=0, use_ba=0
    .int 0x84000000 | (((\count)-1)<<4) | (\r) | ((\size)<<20) | ((\use_ba)<<16) | ((\use_po)<<28)
    .int (\addr)
.endm

.macro GECKO_STORE_GR_8 r, addr, count=1, use_po=0, use_ba=0
    GECKO_STORE_GR (\r), (\addr), 0, (\count), (\use_po), (\use_ba)
.endm

.macro GECKO_STORE_GR_16 r, addr, count=1, use_po=0, use_ba=0
    GECKO_STORE_GR (\r), (\addr), 1, (\count), (\use_po), (\use_ba)
.endm

.macro GECKO_STORE_GR_32 r, addr, count=1, use_po=0, use_ba=0
    GECKO_STORE_GR (\r), (\addr), 2, (\count), (\use_po), (\use_ba)
.endm

# 84T0YYYN XXXXXXXX : Starting Address is XXXXXXXX
# 84T1YYYN XXXXXXXX : Starting Address is XXXXXXXX+ba
# 94T1YYYN XXXXXXXX : Starting Address is XXXXXXXX+po
# T = 0 : byte
# T = 1 : halfword
# T = 2 : word-
# Write grN's T to YYY+1 consecutive T-sized addresses.
