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
#from common.gamecube.DlistParser import DlistParser
from common.BinaryFile import BinaryFile
from common.sfa.zlb import Zlb
from programs.Common.BoxRenderer import BoxRenderer
from .SfaRoot import SfaRoot
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

        # temporary until real texture support
        #tex = self.ctx.Texture("./texture.png")
        #self.dlistRenderer.setTexture('test', tex)

        # set up UI
        self.frame         = 0
        self._mouseButtons = {}
        self._mouseOrigin  = [0, 0]
        self._translate    = [-75, -15, -50]
        self._rotate       = [0, 240, 0]
        self._initT        = self._translate.copy()
        self._initR        = self._rotate.copy()
        self.dlists        = []
        self._menuStack    = []

        # enable our local logger to print on the screen
        # by using log.dprint()
        log.setLevel('DPRINT')

        root = 'files/'
        if len(sys.argv) > 1: root = sys.argv[1]
        self.game = SfaRoot(self, root)
        self.game.loadGlobalAssets()

        if len(sys.argv) > 2: self.loadMap(sys.argv[2])
        self.menu = MainMenu(self)
        self.menu.refresh()


    def loadMap(self, name):
        self.boxRenderer.reset()
        self.game.loadMap(name)


    def _getShaderCodeFromFile(self, path):
        try: return resources.read_text(shaders, path)
        except FileNotFoundError: return resources.read_text(Common, path)


    def enterMenu(self, menu):
        self._menuStack.append(self.menu)
        self.menu = menu

    def exitMenu(self):
        if len(self._menuStack) > 0:
            self.menu = self._menuStack.pop()


    def run(self):
        """Render the scene."""
        log.dprint("Frame %d; objs %d", self.frame,
            len(self.game.map.objects))
        self._setMtxs()

        self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)
        self.ctx.glDepthFunc(self.ctx.GL_LESS)
        self.ctx.glDisable(self.ctx.GL_BLEND)
        self.ctx.glDisable(self.ctx.GL_CULL_FACE)
        self.game.map.render()

        self.ctx.glEnable(self.ctx.GL_DEPTH_TEST)
        self.ctx.glDepthFunc(self.ctx.GL_LESS)
        self.ctx.glEnable(self.ctx.GL_BLEND)
        self.ctx.glBlendFunc(self.ctx.GL_SRC_ALPHA,
            self.ctx.GL_ONE_MINUS_SRC_ALPHA)
        self.ctx.glDisable(self.ctx.GL_CULL_FACE)
        self.boxRenderer.programs['fragment_shader'].setUniforms(
            enableTexture = False,
            #minAlpha      =  0.5,
        )
        self.boxRenderer.run()
        self.menu.render()

        #lines = (
        #    "[1] Face Culling: %s" % ("On" if self.dlistRenderer.useFaceCulling else "Off"),
        #)
        #for i, line in enumerate(lines):
        #    log.dprint("\x1B[%d,200H%s", (i*11)+5, line)

        self.frame += 1


    def _setMtxs(self):
        # update projection matrix to current viewport
        x, y, width, height = self.ctx.viewport
        a = width/height
        rx, ry, rz = self._rotate
        t = self._translate
        mx=gl.Util.Matrix.rotateX(math.radians(rx))
        my=gl.Util.Matrix.rotateY(math.radians(ry))
        mz=gl.Util.Matrix.rotateZ(math.radians(rz))
        mt=gl.Util.Matrix.translate(-t[0], -t[1], -t[2])
        mp=gl.Util.Matrix.perspective(60, a, 0.1, 10000)
        mv = mt @ (mz @ my @ mx) # modelview = all rotations
        #mp = mt @ mp # projection = perspective and translation
        self.mtxModelView  = mv
        self.mtxProjection = mp

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

        self.boxRenderer.setMtxs(mp, mv)
        self.game.setMtxs(mp, mv)
