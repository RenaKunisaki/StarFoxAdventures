# Change PDA "Scanning for information" message to map name and coords.
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

LOADW   r4, pCurMapInfo
stw     r4, (80 * 4)(r3) # change "Information" to map name
lwz     r3, (79 * 4)(r3) # addr of "Scanning for"

# get player
LOADW   r12, pPlayer
cmpwi   r12, 0
beq     end

LOAD    r4, 0x8032CC05 # "%%04x%04x"
lis     r5, 0x803e
lhz     r6, -(0x10000 - 0xcdce)(r5) # cell Z
lhz     r5, -(0x10000 - 0xcdca)(r5) # cell X
CALL    sprintf

end:
lis     r3, 0x8034 # replaced
GECKO_END_PATCH
