#!/usr/bin/env python3
import numpy as np


#input is a RGB numpy array with shape (height,width,3), can be uint,int, float or double, values expected in the range 0..255
#output is a double YUV numpy array with shape (height,width,3), values in the range 0..255
def RGB2YUV( rgb ):

    m = np.array([[ 0.29900, -0.16874,  0.50000],
                 [  0.58700, -0.33126, -0.41869],
                 [  0.11400,  0.50000, -0.08131]])

    yuv = np.dot(rgb,m)
    yuv[:,:,1:]+=128.0
    return yuv

print(RGB2YUV(np.array([
    [[0, 0, 255]]
])))
