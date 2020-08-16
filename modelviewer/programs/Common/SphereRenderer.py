import logging; log = logging.getLogger(__name__)
from importlib import resources
import struct
import math
import sys
import gl
import numpy as np


class SphereRenderer(gl.Pipeline):
    """Subprogram for rendering spheres."""
    MAX_SPHERES = 8192
    vtxBufferFmt = "4f" # x, y, z, radius
    colorBufferFmt = "4f" # (r,g,b,a) per sphere

    def __init__(self, parent, shader):
        self.parent = parent
        self.shader = shader

        super().__init__(parent.ctx,
            vertex_shader   = self.shader,
            geometry_shader = self.shader,
            fragment_shader = parent.fragShader,
        )

        self._vtxSize   = struct.calcsize(self.vtxBufferFmt)
        self._colorSize = struct.calcsize(self.colorBufferFmt)
        self.bufVtxs   = self.ctx.Buffer(self._vtxSize   * self.MAX_SPHERES, True)
        self.bufColors = self.ctx.Buffer(self._colorSize * self.MAX_SPHERES, True)
        self._nSpheres = 0


    def addSphere(self, pos, radius, color):
        """Add a sphere."""
        if self._nSpheres >= self.MAX_SPHERES: return

        x, y, z    = pos
        r, g, b, a = color

        vtxData = struct.pack(self.vtxBufferFmt,   x, y, z, radius)
        colData = struct.pack(self.colorBufferFmt, r, g, b, a)
        vtxOffs = self._nSpheres * self._vtxSize
        colOffs = self._nSpheres * self._colorSize
        self.bufVtxs  .write(vtxData, vtxOffs)
        self.bufColors.write(colData, colOffs)
        self._nSpheres += 1


    def reset(self):
        self._nSpheres = 0


    def run(self):
        if self._nSpheres == 0: return
        self.ctx.glBlendFunc(self.ctx.GL_SRC_ALPHA, self.ctx.GL_ONE_MINUS_SRC_ALPHA)
        self.ctx.glEnable(self.ctx.GL_BLEND)
        with self:
            self._bindBuffers()
            self.shader.vao.render(self.ctx.GL_POINTS, count=self._nSpheres)


    def setMtxs(self, projection, modelView):
        self.shader.setMtxs(projection, modelView)


    def _bindBuffers(self):
        with self.shader as shader:
            # in vec3 pos
            shader.attribs['pos'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 3,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in float radius
            shader.attribs['radius'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 1,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=3 * gl.Util.SIZEOF_FLOAT)

            # in vec4 vtxColor
            shader.attribs['color'].bindBuffer(
                self.bufColors, self.ctx.GL_FLOAT, 4,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)
