import logging; log = logging.getLogger(__name__)
from importlib import resources
import struct
import math
import sys
import gl
import numpy as np
from .SfaProgram import SfaProgram
from . import shaders


class BoxShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'box.vert')
    geometry_shader = (shaders, 'box.geom')


class BoxRenderer(gl.Pipeline):
    """Subprogram for rendering boxes."""
    MAX_BOXES = 8192
    vtxBufferFmt = "6f" # (x,y,z) A, (x,y,z) B
    colorBufferFmt = "4f" # (r,g,b,a) per box

    def __init__(self, parent):
        self.parent = parent
        self.shader  = BoxShader(self.parent.ctx)

        super().__init__(parent.ctx,
            vertex_shader   = self.shader,
            geometry_shader = self.shader,
            fragment_shader = parent.fragShader,
        )

        self._vtxSize   = struct.calcsize(self.vtxBufferFmt)
        self._colorSize = struct.calcsize(self.colorBufferFmt)
        self.bufVtxs   = self.ctx.Buffer(self._vtxSize * self.MAX_BOXES, True)
        self.bufColors = self.ctx.Buffer(self._colorSize * self.MAX_BOXES, True)
        self._nBoxes = 0


    def addBox(self, pointA, pointB, color):
        """Add a box with one corner at pointA and the other at pointB."""
        x1, y1, z1 = pointA
        x2, y2, z2 = pointB
        r, g, b, a = color

        vtxData = struct.pack(self.vtxBufferFmt, x1, y1, z1, x2, y2, z2)
        colData = struct.pack(self.colorBufferFmt, r, g, b, a)
        vtxOffs = self._nBoxes * self._vtxSize
        colOffs = self._nBoxes * self._colorSize
        self.bufVtxs.write(vtxData, vtxOffs)
        self.bufColors.write(colData, colOffs)
        self._nBoxes += 1


    def reset(self):
        self._nBoxes = 0


    def run(self):
        self.ctx.glEnable(self.ctx.GL_BLEND)
        self.ctx.glBlendFunc(self.ctx.GL_SRC_ALPHA,
            self.ctx.GL_ONE_MINUS_SRC_ALPHA)
        with self:
            self._bindBuffers()
            self.shader.vao.render(self.ctx.GL_POINTS, count=self._nBoxes)


    def setMtxs(self, projection, modelView):
        self.shader.setMtxs(projection, modelView)


    def _bindBuffers(self):
        with self.shader as shader:
            # in vec3 vertA
            shader.attribs['vertA'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 3,
                stride=6 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in vec3 vertB
            shader.attribs['vertB'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 3,
                stride=6 * gl.Util.SIZEOF_FLOAT,
                offset=3 * gl.Util.SIZEOF_FLOAT)

            # in vec4 vtxColor
            shader.attribs['vtxColor'].bindBuffer(
                self.bufColors, self.ctx.GL_FLOAT, 4,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)
