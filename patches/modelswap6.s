# Supplementary debugging patch. Not needed for model swap itself.
# Displays the player animation ID in place of "Scanning for information"
# in the PDA.
.text
.include "common.s"

# patch into gameText_run
GECKO_BEGIN_PATCH 0x80019c48 # lis r3, 0x8034
# free: r0, r3, r26, r27, r28, r31

.set HUD_TEXT_PTR,0x8033afbc

# get HUD texts
lis     r3, 0x8034
lwz     r3, -(0x10000 - 0xafbc)(r3)
cmpwi   r3, 0
beq     end

lwz     r3, 8(r3)
cmpwi   r3, 0
beq     end

lwz     r3, (4 * 79)(r3) # to "Scanning for Information"

# get player
lis     r12, 0x8034
lwz     r12, 0x28f8(r12)
cmpwi   r12, 0
beq     end
lhz     r5,  0xA0(r12)

lis     r4, 0x8030 # set r4 to a random "%x"
ori     r4, r4, 0xe79f

CALL    sprintf

end:
lis     r3, 0x8034 # replaced
GECKO_END_PATCH
