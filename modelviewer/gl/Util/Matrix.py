import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
import numpy as np
import struct
import math


IDENTIY_2x2 = np.array([
    [1.0, 0.0],
    [0.0, 1.0],
], dtype=np.float)

IDENTIY_3x3 = np.array([
    [1.0, 0.0, 0.0],
    [0.0, 1.0, 0.0],
    [0.0, 0.0, 1.0],
], dtype=np.float)

IDENTIY_4x4 = np.array([
    [1.0, 0.0, 0.0, 0.0],
    [0.0, 1.0, 0.0, 0.0],
    [0.0, 0.0, 1.0, 0.0],
    [0.0, 0.0, 0.0, 1.0],
], dtype=np.float)


def frustum(left, right, bottom, top, near, far):
    #names from glFrustum doc
    W =  (2*near)      / (right-left)
    H =  (2*near)      / (top-bottom)
    A =  (right+left)  / (right-left)
    B =  (top+bottom)  / (top-bottom)
    C =  ((far+near)   / (far-near))
    D =  ((2*far*near) / (far-near))
    return np.array([
        [W,  0,  0,  0],
        [0,  H,  0,  0],
        [A,  B,  C, -1],
        [0,  0,  D,  0],
        #[W,  0,  A,  0],
        #[0,  H,  B,  0],
        #[0,  0,  C,  D],
        #[0,  0, -1,  0],
    ], dtype=np.float)


def perspective(fovy, aspect, zNear, zFar):
    A=(zFar+zNear)/(zNear-zFar)
    B=(2*zFar*zNear)/(zNear-zFar)
    F=1 / math.tan(math.radians(fovy)/2)
    S=aspect
    return np.array([
        #[F/S, 0,  0,  0],
        #[0,   F,  0,  0],
        #[0,   0,  A,  B],
        #[0,   0, -1,  0],
        [F/S, 0,  0,  0],
        [0,   F,  0,  0],
        [0,   0,  A, -1],
        [0,   0,  B,  0],
    ], dtype=np.float)


def ortho(left, right, bottom, top, near, far):
    dx = right - left
    dy = top   - bottom
    dz = far   - near
    # note, glOrtho does NOT negate tx, ty, tz
    tx = (right + left)   / -dx
    ty = (top   + bottom) / -dy
    tz = (far   + near)   / -dz
    return np.array([
        [2/dx, 0,    0,    0],
        [0,    2/dy, 0,    0],
        [0,    0,   -2/dz, 0],
        [tx,   ty,   tz,   1]], dtype=np.float)


def rotateX(r):
    s, c = math.sin(r), math.cos(r)
    return np.array([
        #[1,  0,  0,  0],
        #[0,  c,  s,  0],
        #[0, -s,  c,  0],
        #[0,  0,  0,  1],
        [1, 0,  0, 0],
        [0, c, -s, 0],
        [0, s,  c, 0],
        [0, 0,  0, 1],
    ], dtype=np.float)

def rotateY(r):
    s, c = math.sin(r), math.cos(r)
    return np.array([
        #[c,  0, -s,  0],
        #[0,  1,  0,  0],
        #[s,  0,  c,  0],
        #[0,  0,  0,  1],
        [ c, 0, s, 0],
        [ 0, 1, 0, 0],
        [-s, 0, c, 0],
        [ 0, 0, 0, 1],
    ], dtype=np.float)

def rotateZ(r):
    s, c = math.sin(r), math.cos(r)
    return np.array([
        #[ c,  s,  0,  0],
        #[-s,  c,  0,  0],
        #[ 0,  0,  1,  0],
        #[ 0,  0,  0,  1],
        [c,-s, 0, 0],
        [s, c, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1],
    ], dtype=np.float)

def translate(x, y, z):
    return np.array([
        #[1, 0, 0, x],
        #[0, 1, 0, y],
        #[0, 0, 1, z],
        #[0, 0, 0, 1],
        [1, 0, 0, 0],
        [0, 1, 0, 0],
        [0, 0, 1, 0],
        [x, y, z, 1],
    ], dtype=np.float)


def rotate(rx, ry, rz):
    # I don't think this works
    # since order of rotations matters
    cx, sx = math.cos(rx), math.sin(rx)
    cy, sy = math.cos(ry), math.sin(ry)
    cz, sz = math.cos(rz), math.sin(rz)
    return np.array([
        [ cy*cz,  sz,    -sy,    0],
        [-sz,     cx*cz,  sx,    0],
        [ sy,    -sx,     cx*cy, 0],
        [ 0,      0,      0,     1]], dtype=np.float)
