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


class TextureShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'texturemap.vert')
    geometry_shader = (shaders, 'texturemap.geom')
    fragment_shader = (shaders, 'texturemap.frag')


class TextureRenderer(gl.Pipeline):
    """Subprogram for displaying texture coordinates."""
    MAX_VTXS = 65536
    vtxBufferFmt = "3f" # (x,y,useTexture)
    colorBufferFmt = "4f" # (r,g,b,a)

    def __init__(self, parent):
        self.parent   = parent
        self.shader   = TextureShader(self.parent.ctx)
        self.texture  = None

        super().__init__(parent.ctx,
            vertex_shader   = self.shader,
            geometry_shader = self.shader,
            fragment_shader = self.shader,
        )
        #self.programs['fragment_shader'].setUniforms(
        #    minAlpha      =  0.5,
        #)

        self._vtxSize   = struct.calcsize(self.vtxBufferFmt)
        self._colorSize = struct.calcsize(self.colorBufferFmt)
        self.bufVtxs   = self.ctx.Buffer(self._vtxSize * self.MAX_VTXS, True)
        self.bufColors = self.ctx.Buffer(self._colorSize * self.MAX_VTXS, True)
        self.reset()



    def reset(self):
        self._nVtxs = 0

        # the actual texture image
        col = (1, 1, 1, 0.5)
        self.addTri((0, 0), (0, 1), (1, 0), col, useTexture=True)
        self.addTri((0, 1), (1, 0), (1, 1), col, useTexture=True)


    def addTri(self, a, b, c, color=(1, 1, 1, 1), useTexture=False):
        self.addVtx(a, color, useTexture)
        self.addVtx(b, color, useTexture)
        self.addVtx(c, color, useTexture)


    def addVtx(self, pos, color=(1, 1, 1, 1), useTexture=False):
        tex = 1 if useTexture else 0
        vtxData = struct.pack(self.vtxBufferFmt, pos[0], pos[1], tex)
        colData = struct.pack(self.colorBufferFmt, *color)
        vtxOffs = self._nVtxs * self._vtxSize
        colOffs = self._nVtxs * self._colorSize
        self.bufVtxs  .write(vtxData, vtxOffs)
        self.bufColors.write(colData, colOffs)
        self._nVtxs += 1


    def run(self):
        self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)

        # nVtxs = # triangles since we use GL_TRIANGLES here,
        # so the division by 3 is done automatically
        with self:
            self.texture.bind()
            self._bindBuffers()
            self.shader.vao.render(self.ctx.GL_TRIANGLES,
                count=self._nVtxs)


    def setMtxs(self, projection, modelView):
        with self.shader:
            self.shader.uniforms['matProjection'].value = projection

    def setTexture(self, tex):
        self.texture = tex


    def _bindBuffers(self):
        with self.shader as shader:
            # in ivec3 texCoord
            shader.attribs['texCoord'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 3,
                stride=3 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in vec4 vtxColor
            shader.attribs['color'].bindBuffer(
                self.bufColors, self.ctx.GL_FLOAT, 4,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)
