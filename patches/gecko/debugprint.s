# Restore logPrintf function for displaying on screen
.text
.include "common.s"

# redirect some stubbed print functions to OSReport
# (OSReport is itself a stub, but Dolphin hooks it)
GECKO_CREATE_BRANCH 0x80148B78, 0x8007D6DC # trickyReportError
GECKO_CREATE_BRANCH 0x802510cc, 0x8007D6DC # dspDebugPrint
GECKO_CREATE_BRANCH 0x80246e04, 0x8007D6DC # _osDebugPrint

# patch some other debug print functions to use this logPrintf
GECKO_CREATE_BRANCH 0x801378a8, 0x80137948 # debugPrintf
#GECKO_CREATE_BRANCH 0x80148bc8, 0x80137948 # trickyDebugPrint
#GECKO_CREATE_BRANCH 0x8007d6dc, 0x80137948 # OSReport

GECKO_BEGIN_PATCH 0x8013798c # stw r10,0x24(r1) - logPrintf

stw     r10,0x24(r1) # replaced
mflr    r11
stw     r11, 0x68(r1) # stash lr too
LOADW   r3,  debugLogEnd  # prepare params for sprintf
lwz     r4,  0x08(r1) # must shift all params up one register
lwz     r5,  0x0C(r1)
lwz     r6,  0x10(r1)
lwz     r7,  0x14(r1)
lwz     r8,  0x18(r1)
lwz     r9,  0x1C(r1)
lwz     r10, 0x20(r1)
lwz     r11, 0x24(r1)
CALL    sprintf # returns # characters written excluding null

# update pLogEnd
LOADWH  r4, debugLogEnd
LOADWL2 r5, debugLogEnd, r4
add     r5, r5, r3
addi    r5, r5, 1 # or else it deadlocks displaying
STOREW  r5, debugLogEnd, r4
lwz     r5, 0x68(r1)
mtlr    r5

GECKO_END_PATCH
