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
from common.BinaryFile import BinaryFile
from common.sfa.zlb import Zlb
from programs.Common.BoxRenderer import BoxRenderer
from .DlistRenderer import DlistRenderer
from .Menu import Menu, MainMenu
from .Map import Map


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

        self.curMap = Map(self)

        # setup shaders and subprograms
        self.fragShader      = FragmentShader(self.ctx)
        self.boxRenderer     = BoxRenderer(self, BoxShader(self.ctx))
        self.dlistRenderer   = DlistRenderer(self)

        # temporary until real texture support
        #tex = self.ctx.Texture("./texture.png")
        #self.dlistRenderer.setTexture('test', tex)

        # set up UI
        self.frame         = 0
        self._mouseButtons = {}
        self._mouseOrigin  = [0, 0]
        self._translate    = [15, -15, -50]
        self._rotate       = [0, 240, 0]
        self._initT        = self._translate.copy()
        self._initR        = self._rotate.copy()
        self.dlists        = []
        self.menu          = MainMenu(self)
        self._menuStack    = []

        # enable our local logger to print on the screen
        # by using log.dprint()
        log.setLevel('DPRINT')

        self.discRoot = 'files/'
        if len(sys.argv) > 1: self.discRoot = sys.argv[1]

        self.loadObjIndex()
        self.loadObjectsTab()
        self.loadObjectsBin()

        if len(sys.argv) > 2: self.loadMap(sys.argv[2])
        self.menu.refresh()


    def loadObjIndex(self):
        """Load OBJINDEX.bin"""
        self.objIndex = []
        with open(self.discRoot+'/OBJINDEX.bin', 'rb') as objIdxFile:
            entries = objIdxFile.read()
            for i in range(0, len(entries), 2):
                it = struct.unpack_from('>H', entries, i)[0] # grumble
                self.objIndex.append(it)

    def loadObjectsTab(self):
        """Load OBJECTS.tab"""
        self.objsTab = []
        with open(self.discRoot+'/OBJECTS.tab', 'rb') as objTabFile:
            entries = objTabFile.read()
            for i in range(0, len(entries), 4):
                it = struct.unpack_from('>I', entries, i)[0] # grumble
                self.objsTab.append(it)

    def loadObjectsBin(self):
        """Load OBJECTS.bin"""
        self.objDefs = []
        with open(self.discRoot+'/OBJECTS.bin', 'rb') as objBinFile:
            for offs in self.objsTab:
                try:
                    objBinFile.seek(offs + 0x04)
                    scale = struct.unpack('>f', objBinFile.read(4))[0] # grumble
                    objBinFile.seek(offs + 0x91)
                    name = objBinFile.read(11).decode('utf-8').replace('\0', '')
                except struct.error:
                    break
                self.objDefs.append({
                    'scale': scale,
                    'name':  name,
                })


    def loadMap(self, name):
        """Load given map."""
        self.boxRenderer.reset()

        #s = 1 # test box
        #pointA = (0 - s, 0 - s, 0 - s)
        #pointB = (0 + s, 0 + s, 0 + s)
        #color  = (1, 1, 0, 0.5)
        #self.boxRenderer.addBox(pointA, pointB, color)

        self.curMap.load(name)
        for obj in self.curMap.objects: obj.render(self)


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
        self.ctx.glEnable(self.ctx.GL_DEPTH_TEST)
        self.ctx.glDepthFunc(self.ctx.GL_LESS)
        #self.ctx.glDepthMask(self.ctx.GL_TRUE)
        #self.ctx.glDepthRange(1,0)
        self.ctx.glEnable(self.ctx.GL_BLEND)
        self.ctx.glBlendFunc(self.ctx.GL_SRC_ALPHA,
            self.ctx.GL_ONE_MINUS_SRC_ALPHA)
        self.ctx.glDisable(self.ctx.GL_CULL_FACE)

        self.boxRenderer.programs['fragment_shader'].setUniforms(
            enableTexture = False,
            #minAlpha      =  0.5,
        )

        log.dprint("Frame %d; objs %d", self.frame,
            len(self.curMap.objects))
        self._setMtxs()
        #self.dlistRenderer.run()
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
        t = self._translate
        mx=gl.Util.Matrix.rotateX(math.radians(rx))
        my=gl.Util.Matrix.rotateY(math.radians(ry))
        mz=gl.Util.Matrix.rotateZ(math.radians(rz))
        mt=gl.Util.Matrix.translate(-t[0], -t[1], -t[2])
        mp=gl.Util.Matrix.perspective(60, a, 0.1, 10000)
        mv = mt @ (mz @ my @ mx) # modelview = all rotations
        #mp = mt @ mp # projection = perspective and translation

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
