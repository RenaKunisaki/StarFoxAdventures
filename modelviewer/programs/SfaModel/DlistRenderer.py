import logging; log = logging.getLogger(__name__)
from importlib import resources
import struct
import math
import sys
import gl
import numpy as np
from gl.Util import Matrix
from .SfaProgram import SfaProgram
from . import shaders


class DlistShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'dlist.vert')
    geometry_shader = (shaders, 'dlist.geom')


class DlistRenderParams:
    """Params used by DlistRenderer to track each list."""
    def __init__(self, dlist):
        self.dlist   = dlist
        self.vtxOffs = 0 # offset into vtx buffer
        self.nVtxs   = 0


class DlistRenderer(gl.Pipeline):
    """Subprogram for rendering display lists."""
    MAX_VTXS = 65536
    vtxBufferFmt = "5f" # (x,y,z,s,t)
    colorBufferFmt = "4f" # (r,g,b,a)

    def __init__(self, parent):
        self.parent = parent
        self.shader = DlistShader(self.parent.ctx)
        self.lists  = []
        self.model  = None

        super().__init__(parent.ctx,
            vertex_shader   = self.shader,
            geometry_shader = self.shader,
            fragment_shader = parent.fragShader,
        )
        self.programs['fragment_shader'].setUniforms(
            enableTexture = True,
            #minAlpha      =  0.5,
        )

        self._vtxSize   = struct.calcsize(self.vtxBufferFmt)
        self._colorSize = struct.calcsize(self.colorBufferFmt)
        self.bufVtxs   = self.ctx.Buffer(self._vtxSize * self.MAX_VTXS, True)
        self.bufColors = self.ctx.Buffer(self._colorSize * self.MAX_VTXS, True)
        self._nVtxs   = 0
        self._prevPos = None
        self._polyIdx = []


    def addList(self, dlist):
        """Add a display list to render."""
        param = DlistRenderParams(dlist)
        param.vtxOffs = self._nVtxs
        self.lists.append(param)
        for i, poly in enumerate(dlist.polys):
            mode = poly['mode']
            if   mode == 0 or mode == 1: self._addQuads(poly)
            elif mode == 2: self._addTris(poly)
            elif mode == 3: self._addTriStrips(poly)
            elif mode == 4: self._addTriFans(poly)
            elif mode == 5: self._addLines(poly)
            elif mode == 6: self._addLineStrips(poly)
            else: self._addPoints(poly)
        param.nVtxs = self._nVtxs - param.vtxOffs

        log.debug("List %d generated %d vtxs", dlist.listIdx, self._nVtxs)


    def setModel(self, model):
        # only for debug
        self.model = model


    def _listToColor(self, listIdx):
        # reverse bits
        listIdx = int('{:08b}'.format(listIdx)[::-1], 2)
        cnt = self.parent.model.header.nDlists
        v   = int((listIdx / cnt) * 128) + 127
        return ((v >> 6) / 4, ((v >> 3) & 7) / 8, (v & 7) / 8, 0.9)

    def _matToColor(self, material):
        return (1, 1, 1, 1)
        if material is None: idx = 0
        else: idx = material._idx #idx = material.textureId[1]
        if idx < 0: idx = 255
        # reverse bits
        idx = int('{:08b}'.format(idx)[::-1], 2) | 0x80
        #cnt = self.parent.model.header.nMaterials
        #v   = int((idx / cnt) * 128) + 127
        v = idx
        return ((v >> 6) / 4, ((v >> 3) & 7) / 8, (v & 7) / 8, 0.9)


    def _addQuads(self, poly):
        c0 = self._matToColor(poly['material'])
        for i in range(0, len(poly['vtxs']), 4):
            self._polyIdx.append(poly['idx'])
            # triangulate
            self._addTri(v0, v1, v2, c0, c0, c0)
            self._addTri(v1, v2, v3, c0, c0, c0)

    def _addTris(self, poly):
        c0 = self._matToColor(poly['material'])
        for i in range(0, len(poly['vtxs']), 3):
            self._polyIdx.append(poly['idx'])
            v0, v1, v2 = poly['vtxs'][i:i+3]
            self._addTri(v0, v1, v2, c0, c0, c0)

    def _addTriStrips(self, poly):
        c0 = self._matToColor(poly['material'])
        for i in range(2, len(poly['vtxs'])):
            self._polyIdx.append(poly['idx'])
            v0, v1, v2 = poly['vtxs'][i-2: i+1]
            # for even idxs, swap v0 and v1 so the triangle
            # faces the right way and doesn't get culled.
            # this is how OpenGL does it.
            if (i&1) == 0:
                self._addTri(v1, v0, v2, c0, c0, c0)
            else:
                self._addTri(v0, v1, v2, c0, c0, c0)

    def _addTriFans(self, poly):
        c0 = self._matToColor(poly['material'])
        v0, v1 = poly['vtxs'][0:2]
        for i in range(2, len(poly['vtxs'])):
            self._polyIdx.append(poly['idx'])
            v2 = poly['vtxs'][i]
            self._addTri(v0, v1, v2, c0, c0, c0)
            v1 = v2

    def _addLines(self, poly):
        c0 = self._matToColor(poly['material'])
        for i in range(0, len(poly['vtxs']), 2):
            self._polyIdx.append(poly['idx'])
            v0, v1 = poly['vtxs'][i:i+2]
            self._addTri(v0, v1, v1, c0, c0, c0)

    def _addLineStrips(self, poly):
        c0 = self._matToColor(poly['material'])
        v0 = poly['vtxs'][0]
        for i in range(1, len(poly['vtxs'])):
            self._polyIdx.append(poly['idx'])
            v1 = poly['vtxs'][i]
            self._addTri(v0, v1, v1, c0, c0, c0)
            v0 = v1

    def _addPoints(self, poly):
        c0 = self._matToColor(poly['material'])
        for i in range(0, len(poly['vtxs'])):
            self._polyIdx.append(poly['idx'])
            v0 = poly['vtxs'][i]
            self._addTri(v0, v0, v0, c0, c0, c0)

    def _addTri(self, v0, v1, v2, c0=None, c1=None, c2=None):
        if c0 is None: c0 = (
            v0['TEX0'][1]/24,
            v0['T0MIDX'][1]/24,
            v0['T7MIDX'][1]/24,
            0.8)
        if c1 is None: c1 = (
            v1['TEX0'][1]/24,
            v1['T0MIDX'][1]/24,
            v1['T7MIDX'][1]/24,
            0.8)
        if c2 is None: c2 = (
            v2['TEX0'][1]/24,
            v2['T0MIDX'][1]/24,
            v2['T7MIDX'][1]/24,
            0.8)

        p0, p1, p2 = v0['POS'],  v1['POS'],  v2['POS']
        t0, t1, t2 = v0['TEX0'], v1['TEX0'], v2['TEX0']

        self._addVtx(p0, c0, t0)
        self._addVtx(p1, c1, t1)
        self._addVtx(p2, c2, t2)


    def _addVtx(self, pos, color, texCoord=None):
        x, y, z = pos
        if self._prevPos is not None:
            if (x == self._prevPos[0]
            and y == self._prevPos[1]
            and z == self._prevPos[2]):
                x, y, z = x+0.05, y+0.05, z+0.05 # actually draw
        self._prevPos = (x, y, z)

        s, t = 0, 0
        if texCoord is not None: s, t = texCoord

        r, g, b, a = color

        vtxData = struct.pack(self.vtxBufferFmt, x, y, z, s, t)
        colData = struct.pack(self.colorBufferFmt, r, g, b, a)
        vtxOffs = self._nVtxs * self._vtxSize
        colOffs = self._nVtxs * self._colorSize
        self.bufVtxs  .write(vtxData, vtxOffs)
        self.bufColors.write(colData, colOffs)
        self._nVtxs += 1


    def run(self, minPoly=0, maxPoly=999999):
        #self.ctx.glEnable(self.ctx.GL_DEPTH_TEST)
        #self.ctx.glDepthFunc(self.ctx.GL_LESS)
        #self.ctx.glEnable(self.ctx.GL_BLEND)
        #self.ctx.glBlendFunc(self.ctx.GL_SRC_ALPHA,
        #    self.ctx.GL_ONE_MINUS_SRC_ALPHA)
        #maxP = min(maxPoly, self._polyIdx[-1])
        #minP = min(minPoly, maxP)

        #vStart, vEnd = 0, 99999999
        #for i, v in enumerate(self._polyIdx):
        #    if v < minP: vStart += 1
        #    if v > maxP:
        #        vEnd = i
        #        break
        ##vStart -= 1?
        #vEnd = min(vEnd, self._nVtxs)
        #log.dprint("Polys %4d - %4d: %4d / %4d", minP, maxP,
        #    maxP - minP, self._polyIdx[-1])
        #log.dprint("Tris %5d - %4d: %4d / %4d", vStart, vEnd,
        #    vEnd - vStart, self._nVtxs)
        #if vEnd <= vStart: return

        # nVtxs = # triangles since we use GL_TRIANGLES here,
        # so the division by 3 is done automatically
        with self:
            self._bindBuffers()
            #if minPoly < 0:
            self.shader.vao.render(self.ctx.GL_TRIANGLES,
                count=self._nVtxs)
            #else: self.shader.vao.render(self.ctx.GL_TRIANGLES,
            #    count=(vEnd-vStart)*3, offset=vStart*3)


    def setMtxs(self, projection, modelView):
        self.shader.setMtxs(projection, modelView)


    def _bindBuffers(self):
        with self.shader as shader:
            # in vec3 vert
            shader.attribs['vert'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 3,
                stride=5 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in vec2 texCoord
            shader.attribs['texCoord'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 2,
                stride=5 * gl.Util.SIZEOF_FLOAT,
                offset=3 * gl.Util.SIZEOF_FLOAT)

            # in vec4 vtxColor
            shader.attribs['vtxColor'].bindBuffer(
                self.bufColors, self.ctx.GL_FLOAT, 4,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)
