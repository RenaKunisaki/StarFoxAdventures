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
from .Menu import Menu, MainMenu


class FragmentShader(SfaProgram):
    """Subprogram for generic fragment shader."""
    separable       = True
    fragment_shader = (shaders, 'program.frag')


class SfaModelViewer(SfaProgram, EventHandler):
    """Render extracted SFA models."""
    MAX_VTXS = 1024

    def __init__(self, ctx):
        super().__init__(ctx)

        # setup shaders and subprograms
        self.fragShader     = FragmentShader(self.ctx)
        self.boneRenderer   = BoneRenderer(self)
        self.boxRenderer    = BoxRenderer(self)
        self.dlistRenderer  = DlistRenderer(self)

        # temporary until real texture support
        tex = self.ctx.Texture("./texture.png")
        self.dlistRenderer.setTexture('test', tex)

        # set up UI
        self.frame         = 0
        self._mouseButtons = {}
        self._mouseOrigin  = [0, 0]
        self._translate    = [15, -15, -50]
        self._rotate       = [0, 180, 0] # for some reason the models face backward
        self._initT        = self._translate.copy()
        self._initR        = self._rotate.copy()
        self.dlists        = []
        self.menu          = MainMenu(self)
        self._menuStack    = []

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
            for i, dlist in enumerate(loader.dlists):
                self.dlists.append(dlist)
                self.dlistRenderer.addList(dlist)
            self.menu.refresh()

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


    def run(self):
        """Render the scene."""
        #self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)
        #self.ctx.glDepthFunc(self.ctx.GL_LESS)
        #self.ctx.glDepthMask(self.ctx.GL_TRUE)
        #self.ctx.glDepthRange(1,0)

        self._setMtxs()
        self.menu.render()
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
