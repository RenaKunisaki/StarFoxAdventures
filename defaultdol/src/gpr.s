.global __save_gpr
.global _savegpr_14
.global _savegpr_15
.global _savegpr_16
.global _savegpr_17
.global _savegpr_18
.global _savegpr_19
.global _savegpr_20
.global _savegpr_21
.global _savegpr_22
.global _savegpr_23
.global _savegpr_24
.global _savegpr_25
.global _savegpr_26
.global _savegpr_27
.global _savegpr_28
.global _savegpr_29
.global _savegpr_30
.global _savegpr_31
.global __restore_gpr
.global _restgpr_14
.global _restgpr_14_x
.global _restgpr_15
.global _restgpr_15_x
.global _restgpr_16
.global _restgpr_16_x
.global _restgpr_17
.global _restgpr_17_x
.global _restgpr_18
.global _restgpr_18_x
.global _restgpr_19
.global _restgpr_19_x
.global _restgpr_20
.global _restgpr_20_x
.global _restgpr_21
.global _restgpr_21_x
.global _restgpr_22
.global _restgpr_22_x
.global _restgpr_23
.global _restgpr_23_x
.global _restgpr_24
.global _restgpr_24_x
.global _restgpr_25
.global _restgpr_25_x
.global _restgpr_26
.global _restgpr_26_x
.global _restgpr_27
.global _restgpr_27_x
.global _restgpr_28
.global _restgpr_28_x
.global _restgpr_29
.global _restgpr_29_x
.global _restgpr_30
.global _restgpr_30_x
.global _restgpr_31
.global _restgpr_31_x
.global __save_fpr
.global _savefpr_14
.global _savefpr_15
.global _savefpr_16
.global _savefpr_17
.global _savefpr_18
.global _savefpr_19
.global _savefpr_20
.global _savefpr_21
.global _savefpr_22
.global _savefpr_23
.global _savefpr_24
.global _savefpr_25
.global _savefpr_26
.global _savefpr_27
.global _savefpr_28
.global _savefpr_29
.global _savefpr_30
.global _savefpr_31
.global __restore_fpr
.global _restfpr_14
.global _restfpr_14_x
.global _restfpr_15
.global _restfpr_15_x
.global _restfpr_16
.global _restfpr_16_x
.global _restfpr_17
.global _restfpr_17_x
.global _restfpr_18
.global _restfpr_18_x
.global _restfpr_19
.global _restfpr_19_x
.global _restfpr_20
.global _restfpr_20_x
.global _restfpr_21
.global _restfpr_21_x
.global _restfpr_22
.global _restfpr_22_x
.global _restfpr_23
.global _restfpr_23_x
.global _restfpr_24
.global _restfpr_24_x
.global _restfpr_25
.global _restfpr_25_x
.global _restfpr_26
.global _restfpr_26_x
.global _restfpr_27
.global _restfpr_27_x
.global _restfpr_28
.global _restfpr_28_x
.global _restfpr_29
.global _restfpr_29_x
.global _restfpr_30
.global _restfpr_30_x
.global _restfpr_31
.global _restfpr_31_x

# there are copies of these (except the _x versiosn) in the game,
# but because we don't know what address our code will load at,
# it would take more instructions to call those than to just
# duplicate them here. these are used by -Os and that still saves
# enough space to be worth it.
__save_gpr:
_savegpr_14: stw 14, -0x48(11)
_savegpr_15: stw 15, -0x44(11)
_savegpr_16: stw 16, -0x40(11)
_savegpr_17: stw 17, -0x3C(11)
_savegpr_18: stw 18, -0x38(11)
_savegpr_19: stw 19, -0x34(11)
_savegpr_20: stw 20, -0x30(11)
_savegpr_21: stw 21, -0x2C(11)
_savegpr_22: stw 22, -0x28(11)
_savegpr_23: stw 23, -0x24(11)
_savegpr_24: stw 24, -0x20(11)
_savegpr_25: stw 25, -0x1C(11)
_savegpr_26: stw 26, -0x18(11)
_savegpr_27: stw 27, -0x14(11)
_savegpr_28: stw 28, -0x10(11)
_savegpr_29: stw 29, -0x0C(11)
_savegpr_30: stw 30, -0x08(11)
_savegpr_31: stw 31, -0x04(11)
blr

__restore_gpr:
_restgpr_14: lwz 14, -0x48(11)
_restgpr_15: lwz 15, -0x44(11)
_restgpr_16: lwz 16, -0x40(11)
_restgpr_17: lwz 17, -0x3C(11)
_restgpr_18: lwz 18, -0x38(11)
_restgpr_19: lwz 19, -0x34(11)
_restgpr_20: lwz 20, -0x30(11)
_restgpr_21: lwz 21, -0x2C(11)
_restgpr_22: lwz 22, -0x28(11)
_restgpr_23: lwz 23, -0x24(11)
_restgpr_24: lwz 24, -0x20(11)
_restgpr_25: lwz 25, -0x1C(11)
_restgpr_26: lwz 26, -0x18(11)
_restgpr_27: lwz 27, -0x14(11)
_restgpr_28: lwz 28, -0x10(11)
_restgpr_29: lwz 29, -0x0C(11)
_restgpr_30: lwz 30, -0x08(11)
_restgpr_31: lwz 31, -0x04(11)
blr

_restgpr_14_x: lwz 14, -0x48(11)
_restgpr_15_x: lwz 15, -0x44(11)
_restgpr_16_x: lwz 16, -0x40(11)
_restgpr_17_x: lwz 17, -0x3C(11)
_restgpr_18_x: lwz 18, -0x38(11)
_restgpr_19_x: lwz 19, -0x34(11)
_restgpr_20_x: lwz 20, -0x30(11)
_restgpr_21_x: lwz 21, -0x2C(11)
_restgpr_22_x: lwz 22, -0x28(11)
_restgpr_23_x: lwz 23, -0x24(11)
_restgpr_24_x: lwz 24, -0x20(11)
_restgpr_25_x: lwz 25, -0x1C(11)
_restgpr_26_x: lwz 26, -0x18(11)
_restgpr_27_x: lwz 27, -0x14(11)
_restgpr_28_x: lwz 28, -0x10(11)
_restgpr_29_x: lwz 29, -0x0C(11)
_restgpr_30_x: lwz 30, -0x08(11)
_restgpr_31_x: lwz 31, -0x04(11)
lwz	0,4(11)
mtlr	0
mr	1,11
blr

__save_fpr:
_savefpr_14: stfd 14, -0x90(11)
_savefpr_15: stfd 15, -0x88(11)
_savefpr_16: stfd 16, -0x80(11)
_savefpr_17: stfd 17, -0x78(11)
_savefpr_18: stfd 18, -0x70(11)
_savefpr_19: stfd 19, -0x68(11)
_savefpr_20: stfd 20, -0x60(11)
_savefpr_21: stfd 21, -0x58(11)
_savefpr_22: stfd 22, -0x50(11)
_savefpr_23: stfd 23, -0x48(11)
_savefpr_24: stfd 24, -0x40(11)
_savefpr_25: stfd 25, -0x38(11)
_savefpr_26: stfd 26, -0x30(11)
_savefpr_27: stfd 27, -0x28(11)
_savefpr_28: stfd 28, -0x20(11)
_savefpr_29: stfd 29, -0x18(11)
_savefpr_30: stfd 30, -0x10(11)
_savefpr_31: stfd 31, -0x08(11)
blr

__restore_fpr:
_restfpr_14: lfd 14, -0x90(11)
_restfpr_15: lfd 15, -0x88(11)
_restfpr_16: lfd 16, -0x80(11)
_restfpr_17: lfd 17, -0x78(11)
_restfpr_18: lfd 18, -0x70(11)
_restfpr_19: lfd 19, -0x68(11)
_restfpr_20: lfd 20, -0x60(11)
_restfpr_21: lfd 21, -0x58(11)
_restfpr_22: lfd 22, -0x50(11)
_restfpr_23: lfd 23, -0x48(11)
_restfpr_24: lfd 24, -0x40(11)
_restfpr_25: lfd 25, -0x38(11)
_restfpr_26: lfd 26, -0x30(11)
_restfpr_27: lfd 27, -0x28(11)
_restfpr_28: lfd 28, -0x20(11)
_restfpr_29: lfd 29, -0x18(11)
_restfpr_30: lfd 30, -0x10(11)
_restfpr_31: lfd 31, -0x08(11)
blr

_restfpr_14_x: lfd 14, -0x90(11)
_restfpr_15_x: lfd 15, -0x88(11)
_restfpr_16_x: lfd 16, -0x80(11)
_restfpr_17_x: lfd 17, -0x78(11)
_restfpr_18_x: lfd 18, -0x70(11)
_restfpr_19_x: lfd 19, -0x68(11)
_restfpr_20_x: lfd 20, -0x60(11)
_restfpr_21_x: lfd 21, -0x58(11)
_restfpr_22_x: lfd 22, -0x50(11)
_restfpr_23_x: lfd 23, -0x48(11)
_restfpr_24_x: lfd 24, -0x40(11)
_restfpr_25_x: lfd 25, -0x38(11)
_restfpr_26_x: lfd 26, -0x30(11)
_restfpr_27_x: lfd 27, -0x28(11)
_restfpr_28_x: lfd 28, -0x20(11)
_restfpr_29_x: lfd 29, -0x18(11)
_restfpr_30_x: lfd 30, -0x10(11)
_restfpr_31_x: lfd 31, -0x08(11)
lwz	0,4(11)
mtlr	0
mr	1,11
blr
