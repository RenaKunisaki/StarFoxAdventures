# The game has the non_x versions of these, but not the _x versions.
# and we can't use those anyway because of relocation.
.text
.include "common.s"

# Routines for saving floating point registers, called by the compiler.
# Called with r11 pointing to the stack header word of the caller of the
# function, just beyond the end of the floating point save area.

	.globl	_savefpr_14
	.globl	_savefpr_15
	.globl	_savefpr_16
	.globl	_savefpr_17
	.globl	_savefpr_18
	.globl	_savefpr_19
	.globl	_savefpr_20
	.globl	_savefpr_21
	.globl	_savefpr_22
	.globl	_savefpr_23
	.globl	_savefpr_24
	.globl	_savefpr_25
	.globl	_savefpr_26
	.globl	_savefpr_27
	.globl	_savefpr_28
	.globl	_savefpr_29
	.globl	_savefpr_30
	.globl	_savefpr_31

		.long	0x00400000	# traceback tag
_savefpr_14:	stfd	14,-144(11)	# save fp registers
_savefpr_15:	stfd	15,-136(11)
_savefpr_16:	stfd	16,-128(11)
_savefpr_17:	stfd	17,-120(11)
_savefpr_18:	stfd	18,-112(11)
_savefpr_19:	stfd	19,-104(11)
_savefpr_20:	stfd	20,-96(11)
_savefpr_21:	stfd	21,-88(11)
_savefpr_22:	stfd	22,-80(11)
_savefpr_23:	stfd	23,-72(11)
_savefpr_24:	stfd	24,-64(11)
_savefpr_25:	stfd	25,-56(11)
_savefpr_26:	stfd	26,-48(11)
_savefpr_27:	stfd	27,-40(11)
_savefpr_28:	stfd	28,-32(11)
_savefpr_29:	stfd	29,-24(11)
_savefpr_30:	stfd	30,-16(11)
_savefpr_31:	stfd	31,-8(11)
		blr

# Routines for saving integer registers, called by the compiler.
# Called with r11 pointing to the stack header word of the caller of the
# function, just beyond the end of the integer save area.

	.globl	_savegpr_14
	.globl	_savegpr_15
	.globl	_savegpr_16
	.globl	_savegpr_17
	.globl	_savegpr_18
	.globl	_savegpr_19
	.globl	_savegpr_20
	.globl	_savegpr_21
	.globl	_savegpr_22
	.globl	_savegpr_23
	.globl	_savegpr_24
	.globl	_savegpr_25
	.globl	_savegpr_26
	.globl	_savegpr_27
	.globl	_savegpr_28
	.globl	_savegpr_29
	.globl	_savegpr_30
	.globl	_savegpr_31

		.long	0x00400000	# traceback tag
_savegpr_14:	stw	14,-72(11)	# save gp registers
_savegpr_15:	stw	15,-68(11)
_savegpr_16:	stw	16,-64(11)
_savegpr_17:	stw	17,-60(11)
_savegpr_18:	stw	18,-56(11)
_savegpr_19:	stw	19,-52(11)
_savegpr_20:	stw	20,-48(11)
_savegpr_21:	stw	21,-44(11)
_savegpr_22:	stw	22,-40(11)
_savegpr_23:	stw	23,-36(11)
_savegpr_24:	stw	24,-32(11)
_savegpr_25:	stw	25,-28(11)
_savegpr_26:	stw	26,-24(11)
_savegpr_27:	stw	27,-20(11)
_savegpr_28:	stw	28,-16(11)
_savegpr_29:	stw	29,-12(11)
_savegpr_30:	stw	30,-8(11)
_savegpr_31:	stw	31,-4(11)
		blr

# Routines for restoring floating point registers, called by the compiler.
# Called with r11 pointing to the stack header word of the caller of the
# function, just beyond the end of the floating point save area.

	.globl	_restfpr_14
	.globl	_restfpr_15
	.globl	_restfpr_16
	.globl	_restfpr_17
	.globl	_restfpr_18
	.globl	_restfpr_19
	.globl	_restfpr_20
	.globl	_restfpr_21
	.globl	_restfpr_22
	.globl	_restfpr_23
	.globl	_restfpr_24
	.globl	_restfpr_25
	.globl	_restfpr_26
	.globl	_restfpr_27
	.globl	_restfpr_28
	.globl	_restfpr_29
	.globl	_restfpr_30
	.globl	_restfpr_31

		.long	0x00600000	# traceback tag
_restfpr_14:	lfd	14,-144(11)	# restore fp registers
_restfpr_15:	lfd	15,-136(11)
_restfpr_16:	lfd	16,-128(11)
_restfpr_17:	lfd	17,-120(11)
_restfpr_18:	lfd	18,-112(11)
_restfpr_19:	lfd	19,-104(11)
_restfpr_20:	lfd	20,-96(11)
_restfpr_21:	lfd	21,-88(11)
_restfpr_22:	lfd	22,-80(11)
_restfpr_23:	lfd	23,-72(11)
_restfpr_24:	lfd	24,-64(11)
_restfpr_25:	lfd	25,-56(11)
_restfpr_26:	lfd	26,-48(11)
_restfpr_27:	lfd	27,-40(11)
_restfpr_28:	lfd	28,-32(11)
_restfpr_29:	lfd	29,-24(11)
_restfpr_30:	lfd	30,-16(11)
_restfpr_31:	lfd	31,-8(11)
		blr

# Routines for restoring integer registers, called by the compiler.
# Called with r11 pointing to the stack header word of the caller of the
# function, just beyond the end of the integer restore area.

	.globl	_restgpr_14
	.globl	_restgpr_15
	.globl	_restgpr_16
	.globl	_restgpr_17
	.globl	_restgpr_18
	.globl	_restgpr_19
	.globl	_restgpr_20
	.globl	_restgpr_21
	.globl	_restgpr_22
	.globl	_restgpr_23
	.globl	_restgpr_24
	.globl	_restgpr_25
	.globl	_restgpr_26
	.globl	_restgpr_27
	.globl	_restgpr_28
	.globl	_restgpr_29
	.globl	_restgpr_30
	.globl	_restgpr_31

		.long	0x00600000	# traceback tag
_restgpr_14:	lwz	14,-72(11)	# rest gp registers
_restgpr_15:	lwz	15,-68(11)
_restgpr_16:	lwz	16,-64(11)
_restgpr_17:	lwz	17,-60(11)
_restgpr_18:	lwz	18,-56(11)
_restgpr_19:	lwz	19,-52(11)
_restgpr_20:	lwz	20,-48(11)
_restgpr_21:	lwz	21,-44(11)
_restgpr_22:	lwz	22,-40(11)
_restgpr_23:	lwz	23,-36(11)
_restgpr_24:	lwz	24,-32(11)
_restgpr_25:	lwz	25,-28(11)
_restgpr_26:	lwz	26,-24(11)
_restgpr_27:	lwz	27,-20(11)
_restgpr_28:	lwz	28,-16(11)
_restgpr_29:	lwz	29,-12(11)
_restgpr_30:	lwz	30,-8(11)
_restgpr_31:	lwz	31,-4(11)
		blr

# Routines for restoring floating point registers, called by the compiler.
# Called with r11 pointing to the stack header word of the caller of the
# function, just beyond the end of the floating point save area.
# In addition to restoring the fp registers, it will return to the caller's
# caller

	.globl	_restfpr_14_x
	.globl	_restfpr_15_x
	.globl	_restfpr_16_x
	.globl	_restfpr_17_x
	.globl	_restfpr_18_x
	.globl	_restfpr_19_x
	.globl	_restfpr_20_x
	.globl	_restfpr_21_x
	.globl	_restfpr_22_x
	.globl	_restfpr_23_x
	.globl	_restfpr_24_x
	.globl	_restfpr_25_x
	.globl	_restfpr_26_x
	.globl	_restfpr_27_x
	.globl	_restfpr_28_x
	.globl	_restfpr_29_x
	.globl	_restfpr_30_x
	.globl	_restfpr_31_x

		.long	0x00600000	# traceback tag
_restfpr_14_x:	lfd	14,-144(11)	# restore fp registers
_restfpr_15_x:	lfd	15,-136(11)
_restfpr_16_x:	lfd	16,-128(11)
_restfpr_17_x:	lfd	17,-120(11)
_restfpr_18_x:	lfd	18,-112(11)
_restfpr_19_x:	lfd	19,-104(11)
_restfpr_20_x:	lfd	20,-96(11)
_restfpr_21_x:	lfd	21,-88(11)
_restfpr_22_x:	lfd	22,-80(11)
_restfpr_23_x:	lfd	23,-72(11)
_restfpr_24_x:	lfd	24,-64(11)
_restfpr_25_x:	lfd	25,-56(11)
_restfpr_26_x:	lfd	26,-48(11)
_restfpr_27_x:	lfd	27,-40(11)
_restfpr_28_x:	lfd	28,-32(11)
_restfpr_29_x:	lfd	29,-24(11)
_restfpr_30_x:	lfd	30,-16(11)
_restfpr_31_x:	lwz	0,4(11)
		lfd	31,-8(11)
		mtlr	0
		mr	1,11
		blr

# Routines for restoring integer registers, called by the compiler.
# Called with r11 pointing to the stack header word of the caller of the
# function, just beyond the end of the integer restore area.

	.globl	_restgpr_14_x
	.globl	_restgpr_15_x
	.globl	_restgpr_16_x
	.globl	_restgpr_17_x
	.globl	_restgpr_18_x
	.globl	_restgpr_19_x
	.globl	_restgpr_20_x
	.globl	_restgpr_21_x
	.globl	_restgpr_22_x
	.globl	_restgpr_23_x
	.globl	_restgpr_24_x
	.globl	_restgpr_25_x
	.globl	_restgpr_26_x
	.globl	_restgpr_27_x
	.globl	_restgpr_28_x
	.globl	_restgpr_29_x
	.globl	_restgpr_30_x
	.globl	_restgpr_31_x

		.long	0x00600000	# traceback tag
_restgpr_14_x:	lwz	14,-72(11)	# rest gp registers
_restgpr_15_x:	lwz	15,-68(11)
_restgpr_16_x:	lwz	16,-64(11)
_restgpr_17_x:	lwz	17,-60(11)
_restgpr_18_x:	lwz	18,-56(11)
_restgpr_19_x:	lwz	19,-52(11)
_restgpr_20_x:	lwz	20,-48(11)
_restgpr_21_x:	lwz	21,-44(11)
_restgpr_22_x:	lwz	22,-40(11)
_restgpr_23_x:	lwz	23,-36(11)
_restgpr_24_x:	lwz	24,-32(11)
_restgpr_25_x:	lwz	25,-28(11)
_restgpr_26_x:	lwz	26,-24(11)
_restgpr_27_x:	lwz	27,-20(11)
_restgpr_28_x:	lwz	28,-16(11)
_restgpr_29_x:	lwz	29,-12(11)
_restgpr_30_x:	lwz	30,-8(11)
_restgpr_31_x:	lwz	0,4(11)
		lwz	31,-4(11)
		mtlr	0
		mr	1,11
		blr
