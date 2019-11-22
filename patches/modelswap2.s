.text
.include "common.s"

# Patch MODELS.tab loading; set Krystal model offset that we loaded in patch 1
GECKO_BEGIN_PATCH 0x80045670 # lwz r3, -0x6554(r13)
# just before a call to stackPush
# r3, r4 are free

.set MODELS_TAB,0x8035f490
.set KRYSTAL_MODEL_ID,0x4E8

# r3 = address of MODELS.tab
lis     r3, MODELS_TAB@h
ori     r3, r3, MODELS_TAB@l
lwz     r3, 0(r3)

# get the ptr to model data, stored by previous code.
lis     r4, 0x8180
lwz     r4, -4(r4)
lis     r5, 0xA000
or      r4, r4, r5 # set compressed flag
stw     r4, (KRYSTAL_MODEL_ID * 4)(r3)

lwz     r3, -0x6554(r13) # replaced

GECKO_END_PATCH
