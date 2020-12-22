.ascii "dbgcamer" # 8 byte file ID for debug

drawItem_dbgCamMode:
    addi    r4,  r14, (s_Mode - mainLoop)
    LOADW   r5,  PATCH_STATE_PTR
    lbz     r5,  DEBUG_CAM_MODE(r5)
    slwi    r5,  r5,  2
    addi    r5,  r5,  dbgCamStrs - mainLoop
    lwzx    r5,  r5,  r14
    add     r5,  r5,  r14
    blr

dbgCamStrs:
    .int    s_Normal - mainLoop
    .int    s_Stay   - mainLoop
    .int    s_Free   - mainLoop

adjItem_dbgCamMode: # r3 = amount to adjust by (0=A button)
    # r3 = amount to adjust by (0 = A button)
    # r9 = sound effect to play after calling this

    LOADW   r5,  PATCH_STATE_PTR
    lbz     r6,  DEBUG_CAM_MODE(r5)
    add     r6,  r6,  r3
    cmpwi   r6,  0
    blt     .adjItem_dbgCamMode_wrapNeg
    cmpwi   r6,  DEBUG_CAM_MAX
    blt     .adjItem_dbgCamMode_store
    li      r6,  0

.adjItem_dbgCamMode_store:
    stb     r6,  DEBUG_CAM_MODE(r5)
    blr

.adjItem_dbgCamMode_wrapNeg:
    li      r6,  DEBUG_CAM_MAX - 1
    b       .adjItem_dbgCamMode_store

#######################################################################

# seems to do not much of anything
#drawItem_camInterpMode:
#    addi    r4,  r14, (s_InterpMode - mainLoop)
#    li      r5,  -1
#    LOADW   r6,  pCamera
#    cmpwi   r6,  0
#    beqlr
#    lbz     r5,  0x0139(r6)
#    blr
#
#adjItem_camInterpMode: # r3 = amount to adjust by
#    LOADW   r6,  pCamera
#    cmpwi   r6,  0
#    beqlr
#    lbz     r4,  0x0139(r6)
#    add     r4,  r4,  r3
#    stb     r4,  0x0139(r6)
#    blr
