import logging; log = logging.getLogger(__name__)
from importlib import resources
import struct
import math
import sys
import gl
import numpy as np
from .SfaProgram import SfaProgram
from . import shaders


class LineShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'line.vert')
    geometry_shader = (shaders, 'line.geom')


class LineRenderer(gl.Pipeline):
    """Subprogram for rendering thick lines."""
    MAX_LINES = 1024
    vtxBufferFmt = "6f" # (x,y,z) head, (x,y,z) tail
    colorBufferFmt = "8f" # (r,g,b,a) per line (head, tail)

    def __init__(self, parent):
        self.parent = parent
        self.shader = LineShader(self.parent.ctx)
        self.lineWidth = 8
        self._nLines = 0

        super().__init__(parent.ctx,
            vertex_shader   = self.shader,
            geometry_shader = self.shader,
            fragment_shader = parent.fragShader,
        )

        self._vtxSize   = struct.calcsize(self.vtxBufferFmt)
        self._colorSize = struct.calcsize(self.colorBufferFmt)
        self.bufVtxs    = self.ctx.Buffer(self._vtxSize   * self.MAX_LINES, True)
        self.bufColors  = self.ctx.Buffer(self._colorSize * self.MAX_LINES, True)
        self.useDepthTest = True


    def addLine(self, pointA, pointB, colorA, colorB):
        """Add a line from pointA to pointB."""
        if self._nLines >= self.MAX_LINES: return

        x1, y1, z1 = pointA
        x2, y2, z2 = pointB
        r1, g1, b1, a1 = colorA
        r2, g2, b2, a2 = colorB

        vtxData = struct.pack(self.vtxBufferFmt, x1, y1, z1, x2, y2, z2)
        colData = struct.pack(self.colorBufferFmt, r1, g1, b1, a1, r2, g2, b2, a2)
        vtxOffs = self._nLines * self._vtxSize
        colOffs = self._nLines * self._colorSize
        self.bufVtxs.write(vtxData, vtxOffs)
        self.bufColors.write(colData, colOffs)
        self._nLines += 1


    def reset(self):
        self._nLines = 0



    def run(self):
        if self._nLines == 0: return
        self.ctx.glBlendFunc(self.ctx.GL_SRC_ALPHA, self.ctx.GL_ONE_MINUS_SRC_ALPHA)
        self.ctx.glEnable(self.ctx.GL_BLEND)

        if self.useDepthTest: self.ctx.glEnable(self.ctx.GL_DEPTH_TEST)
        else: self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)
        self.ctx.glDisable(self.ctx.GL_CULL_FACE)
        self.ctx.glLineWidth(self.lineWidth)

        with self:
            self._bindBuffers()
            self.shader.vao.render(self.ctx.GL_POINTS, count=self._nLines)


    def setMtxs(self, projection, modelView):
        self.shader.setMtxs(projection, modelView)


    def _bindBuffers(self):
        with self.shader as shader:
            # in vec3 head
            shader.attribs['head'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 3,
                stride=6 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in vec3 tail
            shader.attribs['tail'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 3,
                stride=6 * gl.Util.SIZEOF_FLOAT,
                offset=3 * gl.Util.SIZEOF_FLOAT)

            # in vec4 colHead
            shader.attribs['colHead'].bindBuffer(
                self.bufColors, self.ctx.GL_FLOAT, 4,
                stride=8 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in vec4 colTail
            shader.attribs['colTail'].bindBuffer(
                self.bufColors, self.ctx.GL_FLOAT, 4,
                stride=8 * gl.Util.SIZEOF_FLOAT,
                offset=4 * gl.Util.SIZEOF_FLOAT)
