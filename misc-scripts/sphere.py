#!/usr/bin/env python3
import math

nStacks=4
nSlices=4
for stack in range(nStacks):
    for slice in range(nSlices):
        y = 2.0 * stack / nStacks - 1.0
        r = math.sqrt(1 - (y * y))
        x = r * math.sin(2.0 * math.pi * slice / nSlices)
        z = r * math.cos(2.0 * math.pi * slice / nSlices)
        #drawVtx((radius * vec3(x, y, z)) + pos, color);
        #vtxs[i++] = (radius * vec3(x, y, z)) + pos;
        print(".float %8.5f, %8.5f, %8.5f" % (x, y, z))
