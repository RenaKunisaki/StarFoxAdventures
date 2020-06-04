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
lhz     r5,  0xA0(r12) # get anim ID

LOAD    r4, 0x8030e79f # set r4 to a random "%x"

# or show coords instead (but ugly)
# in fact this is too long and overwrites other strings, whoops
#LOAD     r4, 0x8031D621 # "%f %f %f"
#lfs      f1, 0x0C(r12)
#lfs      f2, 0x10(r12)
#lfs      f3, 0x14(r12)
#creqv    4*cr1+eq,4*cr1+eq,4*cr1+eq
CALL    sprintf

end:
lis     r3, 0x8034 # replaced
GECKO_END_PATCH
