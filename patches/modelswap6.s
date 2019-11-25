# Supplementary debugging patch. Not needed for model swap itself.
# Displays the player animation ID in place of "Scanning for information"
# in the PDA.
.text
.include "common.s"

# patch into gameText_run
GECKO_BEGIN_PATCH 0x80019c48 # lis r3, 0x8034

.set HUD_TEXT_PTR,0x8033afbc

# get HUD texts
LOADW   r3, HUD_TEXT_PTR
cmpwi   r3, 0
beq     end

lwz     r3, 8(r3)
cmpwi   r3, 0
beq     end

lis     r4, mapInfo@h
ori     r4, r4, mapInfo@l
stw     r4, (80 * 4)(r3) # change "Information" to map name
lwz     r3, (79 * 4)(r3) # addr of "Scanning for"

# get player
LOADW   r12, pPlayer
cmpwi   r12, 0
beq     end
lhz     r5,  0xA0(r12)

lis     r4, 0x8030 # set r4 to a random "%x"
ori     r4, r4, 0xe79f

CALL    sprintf

end:
lis     r3, 0x8034 # replaced
GECKO_END_PATCH
