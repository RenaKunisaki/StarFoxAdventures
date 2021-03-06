import logging; log = logging.getLogger(__name__)
from importlib import resources
import struct
import math
import sys
import gl
import numpy as np
from .SfaProgram import SfaProgram
from . import shaders


class BoneShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'bone.vert')
    geometry_shader = (shaders, 'bone.geom')


class BoneRenderer(gl.Pipeline):
    """Subprogram for rendering bones."""
    MAX_BONES = 1024
    vtxBufferFmt = "6f" # (x,y,z) head, (x,y,z) tail
    colorBufferFmt = "8f" # (r,g,b,a) per bone (head, tail)

    def __init__(self, parent):
        self.parent = parent
        self.shader = BoneShader(self.parent.ctx)
        self.model  = None
        self._highlighted = -1
        self._highlightFlash = 0

        super().__init__(parent.ctx,
            vertex_shader   = self.shader,
            geometry_shader = self.shader,
            fragment_shader = parent.fragShader,
        )

        self._vtxSize   = struct.calcsize(self.vtxBufferFmt)
        self._colorSize = struct.calcsize(self.colorBufferFmt)
        self.bufVtxs   = self.ctx.Buffer(self._vtxSize * self.MAX_BONES, True)
        self.bufColors = self.ctx.Buffer(self._colorSize * self.MAX_BONES, True)
        self.useDepthTest = True


    def setModel(self, model):
        self.model = model
        self._nBones = len(self.model.bones)

        vtxData, vtxColors = [], []
        for i, bone in enumerate(self.model.bones):
            head, tail = self.model.calcBonePos(bone, True)
            vtxData.append(struct.pack('6f', # x, y, z - head, tail
                head[0], head[1], head[2],
                tail[0], tail[1], tail[2],
            ))
            vtxColors.append(struct.pack('8f', 1, 1, 1, 1,  1, 0, 0, 1))

        vtxData   = b''.join(vtxData)
        vtxColors = b''.join(vtxColors)
        self.bufVtxs.write(vtxData)
        self.bufColors.write(vtxColors)


    def highlightBone(self, idx):
        self._highlighted = idx



    def run(self):
        if self.model is None: return


        vtxColors = []
        for i, bone in enumerate(self.model.bones):
            if i == self._highlighted and self._highlightFlash == 0:
                vtxColors.append(struct.pack('8f', 0, 1, 0, 1,  0, 1, 0, 1))
            else:
                vtxColors.append(struct.pack('8f', 1, 1, 1, 1,  1, 0, 0, 1))

        vtxColors = b''.join(vtxColors)
        self.bufColors.write(vtxColors)
        self._highlightFlash ^= 1

        if self.useDepthTest: self.ctx.glEnable(self.ctx.GL_DEPTH_TEST)
        else: self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)
        self.ctx.glDisable(self.ctx.GL_CULL_FACE)
        self.ctx.glLineWidth(2)
        with self:
            self._bindBuffers()
            self.shader.vao.render(self.ctx.GL_POINTS, count=self._nBones)


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
