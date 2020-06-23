import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import os
import os.path
import gl
import numpy as np
from . import shaders
from .SfaProgram import SfaProgram
from .EventHandler import EventHandler
from common.sfa.ModelLoader import ModelLoader
from common.gamecube.DlistParser import DlistParser
from programs.Common.BoxRenderer import BoxRenderer
from .DlistRenderer import DlistRenderer
from .Menu import Menu, MainMenu


class FragmentShader(SfaProgram):
    """Subprogram for generic fragment shader."""
    separable       = True
    fragment_shader = (shaders, 'program.frag')


class BoxShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'box.vert')
    geometry_shader = (shaders, 'box.geom')


class SfaMapViewer(SfaProgram, EventHandler):
    """Render SFA maps."""
    MAX_VTXS = 8192

    def __init__(self, ctx):
        super().__init__(ctx)

        # setup shaders and subprograms
        self.fragShader      = FragmentShader(self.ctx)
        self.boxRenderer     = BoxRenderer(self, BoxShader(self.ctx))
        self.dlistRenderer   = DlistRenderer(self)

        # temporary until real texture support
        tex = self.ctx.Texture("./texture.png")
        self.dlistRenderer.setTexture('test', tex)

        # set up UI
        self.frame         = 0
        self._mouseButtons = {}
        self._mouseOrigin  = [0, 0]
        self._translate    = [15, -15, -50]
        self._rotate       = [0, 0, 0]
        self._initT        = self._translate.copy()
        self._initR        = self._rotate.copy()
        self.dlists        = []
        self.menu          = MainMenu(self)
        self._menuStack    = []

        # enable our local logger to print on the screen
        # by using log.dprint()
        log.setLevel('DPRINT')

        if len(sys.argv) > 1:
            path = sys.argv[1]
            if os.path.isdir(path): self.loadDir(path, int(sys.argv[2]))
            else: self.loadModel(path)


    def _getShaderCodeFromFile(self, path):
        try: return resources.read_text(shaders, path)
        except FileNotFoundError: return resources.read_text(Common, path)


    def loadDir(self, path, modelIdx=0):
        modelsTab = {}
        with open(path+'/MODELS.tab', 'rb') as file:
            idx = 0
            while True:
                d = file.read(4)
                if len(d) < 4: break
                s = struct.unpack('>I', d)[0] # grumble
                modelsTab[idx] = {
                    'flags':  s >> 24,
                    'offset': s & 0xFFFFFF,
                }
                idx += 1

        if modelIdx not in modelsTab:
            raise ValueError("Model %d not found in path %s" % (
                modelIdx, path))
        self.loadModel(path+'/MODELS.bin', modelsTab[modelIdx]['offset'])



    def loadModel(self, path, offset=0):
        # read the model
        with open(path, 'rb') as file:
            file.seek(offset)
            loader = ModelLoader()
            self.model = loader.loadFromFile(file)
            self.dlistRenderer.setModel(self.model)
            for i, dlist in enumerate(loader.dlists):
                self.dlists.append(dlist)
                self.dlistRenderer.addList(dlist)
            self.menu.refresh()


    def enterMenu(self, menu):
        self._menuStack.append(self.menu)
        self.menu = menu

    def exitMenu(self):
        if len(self._menuStack) > 0:
            self.menu = self._menuStack.pop()


    def run(self):
        """Render the scene."""
        #self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)
        #self.ctx.glDepthFunc(self.ctx.GL_LESS)
        #self.ctx.glDepthMask(self.ctx.GL_TRUE)
        #self.ctx.glDepthRange(1,0)

        log.dprint("Frame %d", self.frame)
        self._setMtxs()
        self.dlistRenderer.run()
        if (self.frame & 1) == 0:
            self.boxRenderer.run()
        self.menu.render()

        lines = (
            "[1] Face Culling: %s" % ("On" if self.dlistRenderer.useFaceCulling else "Off"),
        )
        for i, line in enumerate(lines):
            log.dprint("\x1B[%d,200H%s", (i*11)+5, line)

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
        log.dprint("t=%+2.1f, %+2.1f, %+2.1f",
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

        self.boxRenderer    .setMtxs(mp, mv)
        self.dlistRenderer  .setMtxs(mp, mv)
