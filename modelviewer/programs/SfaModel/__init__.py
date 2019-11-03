import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import gl
import numpy as np
from . import shaders
from .SfaProgram import SfaProgram
from .EventHandler import EventHandler
from .Parser.ModelLoader import ModelLoader
from .Parser.DlistParser import DlistParser
from .BoneRenderer import BoneRenderer
from .BoxRenderer import BoxRenderer
from .DlistRenderer import DlistRenderer


class FragmentShader(SfaProgram):
    """Subprogram for generic fragment shader."""
    separable       = True
    fragment_shader = (shaders, 'program.frag')


class SfaModelViewer(SfaProgram, EventHandler):
    """Render extracted SFA models."""
    MAX_VTXS = 1024

    def __init__(self, ctx):
        super().__init__(ctx)

        # temporary until real texture support
        self.texture = self.ctx.Texture("./texture.png")

        # setup shaders and subprograms
        self.fragShader     = FragmentShader(self.ctx)
        self.boneRenderer   = BoneRenderer(self)
        self.boxRenderer    = BoxRenderer(self)
        self.dlistRenderer  = DlistRenderer(self)

        # set up UI
        self.frame         = 0
        self._mouseButtons = {}
        self._mouseOrigin  = [0, 0]
        self._translate    = [15, -15, -50]
        self._rotate       = [0, 180, 0] # for some reason the models face backward
        self._initT        = self._translate.copy()
        self._initR        = self._rotate.copy()
        self.curList       = -1 # all
        self.minPoly       = -1 # all
        self.maxPoly       = 9999

        # enable our local logger to print on the screen
        # by using log.dprint()
        log.setLevel('DPRINT')

        self.loadModel('../dump/krystal.bin')


    def _getShaderCodeFromFile(self, path):
        try: return resources.read_text(shaders, path)
        except FileNotFoundError: return resources.read_text(Common, path)


    def loadModel(self, path):
        # read the model
        with open(path, 'rb') as file:
            loader = ModelLoader()
            self.model = loader.loadFromFile(file)
            self.dlistRenderer.setModel(self.model)
            for dlist in loader.dlists:
                self.dlistRenderer.addList(dlist)

        # draw each vertex as a point
        #d=0.04
        #for i, vtx in enumerate(self.model.vtxs):
        #    self.boxRenderer.addBox(
        #        (vtx.x-d, vtx.y-d, vtx.z-d),
        #        (vtx.x+d, vtx.y+d, vtx.z+d),
        #        (0, 0, 0, 0.5),
        #    )

        self.boneRenderer.setModel(self.model)
        #self._renderHitboxes()


    def _renderHitboxes(self):
        for i, box in enumerate(self.model.hitboxes):
            # no idea the actual format, just guessing here...
            # unk00 looks like 2 words, first might be a matrix idx?
            # unk14 is always? 0000nnnn, where both bytes of nn == i,
            # eg 00001212
            #log.debug("Hitbox %3d: %08X, %08X, %+5.2f, %+5.2f, %+5.2f, %+5.2f", i,
            #    box.unk00, box.unk14, *box.unk04)
            w, x, y, z = box.unk04
            x *= 10
            y *= 10
            z *= 10
            w *= 0.5
            self.boxRenderer.addBox(
                (x-w, y-w, z-w),
                (x+w, y+w, z+w),
                (
                    (box.unk00 & 0xFF) / 16.0,
                    ((box.unk00 >> 8) & 0xFF) / 16.0,
                    (box.unk14 & 0xFF) / 16.0,
                    0.25)
            )


    def _makeAxes(self, vtxData, vtxColors):
        """Generate some axis lines."""
        vtxData.append(struct.pack('18f',
            10, 0, 0,    0,  0,  0,
            0, 10, 0,    0,  0,  0,
            0, 0, 10,    0,  0,  0,
        ))
        vtxColors.append(struct.pack('24f',
            1, 0, 0, 1,   1, 0, 0, 1,
            0, 1, 0, 1,   0, 1, 0, 1,
            0, 0, 1, 1,   0, 0, 1, 1,
        ))


    def _makeCube(self, vtxData, vtxColors):
        """Generate a test cube."""
        # cube
        #    a______b
        #    /     /|
        #  c.-----/d|
        #   |     | |h
        #   | g   |/
        #  e'-----'f
        va = (-1, -1, -1)
        vb = ( 1, -1, -1)
        vc = (-1, -1,  1)
        vd = ( 1, -1,  1)
        ve = (-1,  1,  1)
        vf = ( 1,  1,  1)
        vg = (-1,  1, -1)
        vh = ( 1,  1, -1)
        faces = np.array([
        #    A   B   C    B   C   D
            va, vc, ve,  vc, ve, vg, # -X
            vb, vd, vf,  vd, vf, vh, # +X
            va, vb, vc,  vb, vc, vd, # -Y
            vg, vh, ve,  vh, ve, vf, # +Y
            va, vg, vb,  vg, vb, vh, # -Z
            vc, ve, vd,  ve, vd, vf, # +Z
        ], dtype=np.float)

        vtxData.append(struct.pack('108f', *faces.flatten()))
        vtxColors.append(struct.pack('144f',
            1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1, #-X
            1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1, #-X

            0, 1, 1, 1,   0, 1, 1, 1,   0, 1, 1, 1, #+X
            0, 1, 1, 1,   0, 1, 1, 1,   0, 1, 1, 1, #+X

            0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1, #-Y
            0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1, #-Y

            1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1, #+Y
            1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1, #+Y

            0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1, #-Z
            0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1, #-Z

            1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1, #+Z
            1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1, #+Z
        ))


    def run(self):
        """Render the scene."""
        #self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)
        #self.ctx.glDepthFunc(self.ctx.GL_LESS)
        #self.ctx.glDepthMask(self.ctx.GL_TRUE)
        #self.ctx.glDepthRange(1,0)

        self._setMtxs()
        self.texture.bind()
        self.dlistRenderer.run()
        self.boneRenderer.run()
        self.boxRenderer.run()

        self.frame += 1


    def _setMtxs(self):
        # update projection matrix to current viewport
        x, y, width, height = self.ctx.viewport
        a = width/height
        rx, ry, rz = self._rotate
        mx=gl.Util.Matrix.rotateX(math.radians(rx))
        my=gl.Util.Matrix.rotateY(math.radians(ry))
        mz=gl.Util.Matrix.rotateZ(math.radians(rz))
        mt=gl.Util.Matrix.translate(*self._translate)
        mp=gl.Util.Matrix.perspective(60, a, 0.1, 100)
        mv = (mx @ my @ mz) # modelview = all rotations
        mp = mt @ mp # projection = perspective and translation

        # debug print
        #log.dprint("w=%d h=%d f=%d a=%f", width, height, self.frame,
        #    width/height)
        log.dprint("r=%d, %d, %d",
            self._rotate[0], self._rotate[1], self._rotate[2])
        log.dprint("t=%f, %f, %f",
            self._translate[0], self._translate[1], self._translate[2])
        #log.dprint("B=%s", self._mouseButtons)
        #log.dprint("\nPROJ\n" +
        #    "%+7.2f %+7.2f %+7.2f %+7.2f\n"
        #    "%+7.2f %+7.2f %+7.2f %+7.2f\n"
        #    "%+7.2f %+7.2f %+7.2f %+7.2f\n"
        #    "%+7.2f %+7.2f %+7.2f %+7.2f", *mp.flatten())
        #log.dprint("MV\n" +
        #    "%+7.2f %+7.2f %+7.2f %+7.2f\n"
        #    "%+7.2f %+7.2f %+7.2f %+7.2f\n"
        #    "%+7.2f %+7.2f %+7.2f %+7.2f\n"
        #    "%+7.2f %+7.2f %+7.2f %+7.2f\n", *mv.flatten())
        #"P=\n%sM=\n%s", p, mt @ (mx @ my @ mz))

        #for i, bone in enumerate(self.model.bones):
        #    head, tail = self._calcBonePos(self.model, bone)
        #    log.dprint("%2d %+7.2f %+7.2f %+7.2f | %+7.2f %+7.2f %+7.2f",
        #        i, head[0], head[1], head[2], tail[0], tail[1], tail[2])

        self.boneRenderer.setMtxs(mp, mv)
        self.boxRenderer .setMtxs(mp, mv)
        self.dlistRenderer.setMtxs(mp, mv)
