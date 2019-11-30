.text
.include "common.s"

# Patch modelFileLoad(); patch MODELS.TAB to add the offset of
# the Krystal model we loaded in patch 1.
GECKO_BEGIN_PATCH 0x800291d4 # lwz r3, 0x18(r1)
# just before a call to loadModelsBin
# r3-r12 are free
# r30 = modelIdx

.set MODELS_TAB,0x8035f490
.set KRYSTAL_MODEL_ID,0x4E8

cmpwi   r30, KRYSTAL_MODEL_ID # is this Krystal?
bne     end # nope, don't touch it.

# get the ptr to model data, stored by previous code.
lis     r4, 0x8180
lwz     r4, -4(r4)

lis     r5, 0xB000 # set flags: compressed, use either MODELS.BIN
or      r6, r4, r5

LOADW   r3, MODELS_TAB # get the address of MODELS.TAB in memory
lwz     r4, (KRYSTAL_MODEL_ID * 4)(r3) # r4 = offset for Krystal model
rlwinm. r5, r4, 4, 0xF # r5 = (r4 >> 28) & 0xF (the flag bits)
cmpwi   r5, 0 # already loaded?
bne     end # then don't change it.

# store it into local MODELS.TAB
stw     r6, (KRYSTAL_MODEL_ID * 4)(r3)
stw     r6, 0x18(r1) # return this offset to the caller

end:
lwz     r3, 0x18(r1) # replaced

GECKO_END_PATCH
