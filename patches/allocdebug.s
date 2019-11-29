.text
.include "common.s"

# patch heapAlloc() to immediately stop when given a bad size,
# so that we can trap it.
GECKO_BEGIN_PATCH 0x80023864 # or r23, r3, r3

srwi    r23, r4, 16
cmpwi   r23, 0x0180
blt     end
.byte   0, 0, 0, 1 # an invalid opcode

end:
or      r23, r3, r3 # replaced
GECKO_END_PATCH
