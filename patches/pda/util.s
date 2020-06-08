# some generic code used by multiple submodules.

adjItem_float: # r3=adj, r5=&val, f2=adjStep, f3=min, f4=max
    lfs     f1, 0(r5) # f1 = val
    cmpwi   r3, 0
    bge     .noNegFloat
    fneg    f2, f2
.noNegFloat:
    fadds   f1, f1, f2 # f2 = -adjStep
    fcmpo   0, f1, f3 # f1 < min?
    bge     .float_moreThanMin
    # XXX wraparound doesn't work nicely due to rounding error.
    # sometims you'll jump from 10% to 200% instead of to 0%.
    #fmr     f1, f4 # f1 = max
    fmr     f1, f3 # f1 = min
.float_moreThanMin:
    fcmpo   0, f1, f4 # f1 > max?
    ble     .float_lessThanMax
    #fmr     f1, f3 # f1 = min
    fmr     f1, f4 # f1 = max
.float_lessThanMax:
    stfs    f1, 0(r5)
    blr
