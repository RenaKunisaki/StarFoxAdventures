.text
.include "common.s"

# Patch modelFileLoad(); patch MODELS.TAB to add the offset of
# the Krystal model we loaded in patch 1.
GECKO_BEGIN_PATCH 0x800291d4 # lwz r3, 0x18(r1)
# just before a call to loadModelsBin
# r3-r12 are free
# r30 = modelIdx

.set MODELS_TAB,0x8035f490
.set MODELS_TAB_H,0x8036
.set MODELS_TAB_L,-(0x10000 - 0xF490)
.set KRYSTAL_MODEL_ID,0x4E8

cmpwi   r30, KRYSTAL_MODEL_ID
bne     skip

# r3 = address of MODELS.tab
lis     r3, MODELS_TAB_H
lwz     r3, MODELS_TAB_L(r3)
lwz     r4, (KRYSTAL_MODEL_ID * 4)(r3) # r4 = offset for Krystal model
rlwinm. r5, r4, 4, 0xF # r5 = (r4 >> 28) & 0xF
cmpwi   r5, 0 # already loaded?
bne     skip
# XXX double check that the textures are loaded, as they sometimes aren't.

# get the ptr to model data, stored by previous code.
lis     r4, 0x8180
lwz     r4, -4(r4)

lis     r5, 0xA000 # set flags: compressed, use local MODELS.BIN
or      r4, r4, r5 # since that's the one we append our data into

# store it into local MODELS.TAB
stw     r4, (KRYSTAL_MODEL_ID * 4)(r3)
stw     r4, 0x18(r1) # return this offset to the caller

skip:
lwz     r3, 0x18(r1) # replaced

GECKO_END_PATCH
