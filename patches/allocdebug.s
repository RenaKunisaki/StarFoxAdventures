.text
.include "common.s"

# patch heapAlloc() to immediately stop when given a bad size,
# so that we can trap it.
GECKO_BEGIN_PATCH 0x80023864 # or r23, r3, r3

srwi    r23, r4, 16
cmpwi   r23, 0x0180
blt     end

# deliberately crash
lis     r23, 0xD06F
ori     r23, r23, 0xECE5
lis     r8, 0
stw     r23, 0(r8)

end:
or      r23, r3, r3 # replaced
GECKO_END_PATCH
