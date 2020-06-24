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
from .BoneRenderer import BoneRenderer
from programs.Common.BoxRenderer import BoxRenderer
from .DlistRenderer import DlistRenderer
from .TextureRenderer import TextureRenderer
from .Menu import Menu, MainMenu
from common.BinaryFile import BinaryFile


class FragmentShader(SfaProgram):
    """Subprogram for generic fragment shader."""
    separable       = True
    fragment_shader = (shaders, 'program.frag')

class BoxShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'box.vert')
    geometry_shader = (shaders, 'box.geom')


class SfaModelViewer(SfaProgram, EventHandler):
    """Render extracted SFA models."""
    MAX_VTXS = 1024

    def __init__(self, ctx):
        super().__init__(ctx)

        # setup shaders and subprograms
        self.fragShader      = FragmentShader(self.ctx)
        self.boneRenderer    = BoneRenderer(self)
        self.boxRenderer     = BoxRenderer(self, BoxShader(self.ctx))
        self.dlistRenderer   = DlistRenderer(self)
        self.textureRenderer = TextureRenderer(self)

        # temporary until real texture support
        tex = self.ctx.Texture("./texture.png")
        self.dlistRenderer.setTexture('test', tex)
        self.textureRenderer.setTexture(tex)

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

        #self.loadModel('../dump/krystal.bin')
        #self.loadModel(sys.argv[1])
        if len(sys.argv) > 1:
            path = sys.argv[1]
            if os.path.isdir(path): self.loadDir(path, int(sys.argv[2], 0))
            else: self.loadModel(path)


    def _getShaderCodeFromFile(self, path):
        try: return resources.read_text(shaders, path)
        except FileNotFoundError: return resources.read_text(Common, path)


    def loadDir(self, path, modelIdx=0):
        modelsTab = {}
        with BinaryFile(path+'/MODELS.tab', 'rb') as file:
            idx = 0
            while True:
                try: s = file.readu32()
                except struct.error: break
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
        with BinaryFile(path, 'rb', offset=offset) as file:
            loader = ModelLoader()
            self.model = loader.loadFromFile(file)
            self.dlistRenderer.setModel(self.model)
            for i, dlist in enumerate(loader.dlists):
                self.dlists.append(dlist)
                self.dlistRenderer.addList(dlist)
                #bbox = dlist.list.bbox
                #b1, b2 = bbox[0:3], bbox[3:]
                #sc = 1024
                #col = self.dlistRenderer._listToColor(i)
                #self.boxRenderer.addBox(
                #    (b1[0] / sc, b1[1] / sc, b1[2] / sc),
                #    (b2[0] / sc, b2[1] / sc, b2[2] / sc),
                #    (col[0], col[1], col[2], 0.2),
                #)
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


    def enterMenu(self, menu):
        self._menuStack.append(self.menu)
        self.menu = menu

    def exitMenu(self):
        if len(self._menuStack) > 0:
            self.menu = self._menuStack.pop()


    def run(self):
        """Render the scene."""
        log.dprint("Frame %d", self.frame)
        self._setMtxs()
        self.dlistRenderer.run()
        self.boneRenderer.run()
        if (self.frame & 1) == 0:
            self.boxRenderer.run() # highlight boxes
        #self.textureRenderer.run() # draw texture on screen
        self.menu.render()

        lines = (
            "[1] Face Culling: %s" % ("On" if self.dlistRenderer.useFaceCulling else "Off"),
            "[2] Bone X-ray: %s" % (
                "Off" if self.boneRenderer.useDepthTest else "On"),
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
        mp=gl.Util.Matrix.perspective(60, a, 0.1, 10000)
        mv = (mx @ my @ mz) # modelview = all rotations
        mp = mt @ mp # projection = perspective and translation
        self.mtxProjection, self.mtxModelView = mp, mv

        # debug print
        #log.dprint("w=%d h=%d f=%d a=%f", width, height, self.frame,
        #    width/height)
        log.dprint("r=%d, %d, %d",
            self._rotate[0], self._rotate[1], self._rotate[2])
        log.dprint("t=%+2.1f, %+2.1f, %+2.1f",
            self._translate[0], self._translate[1], self._translate[2])
        #log.dprint("B=%s", self._mouseButtons)
        #self._printMtxs()


        #for i, bone in enumerate(self.model.bones):
        #    head, tail = self._calcBonePos(self.model, bone)
        #    log.dprint("%2d %+7.2f %+7.2f %+7.2f | %+7.2f %+7.2f %+7.2f",
        #        i, head[0], head[1], head[2], tail[0], tail[1], tail[2])

        self.boneRenderer   .setMtxs(mp, mv)
        self.boxRenderer    .setMtxs(mp, mv)
        self.dlistRenderer  .setMtxs(mp, mv)
        self.textureRenderer.setMtxs(
            gl.Util.Matrix.scale(3/width, 3/height) @
            gl.Util.Matrix.translate(-0.9, -0.9, 0),
            mv)

    def _printMtxs(self):
        log.dprint("\nPROJ\n" +
            "%+7.2f %+7.2f %+7.2f %+7.2f\n"
            "%+7.2f %+7.2f %+7.2f %+7.2f\n"
            "%+7.2f %+7.2f %+7.2f %+7.2f\n"
            "%+7.2f %+7.2f %+7.2f %+7.2f", *self.mtxProjection.flatten())
        log.dprint("MV\n" +
            "%+7.2f %+7.2f %+7.2f %+7.2f\n"
            "%+7.2f %+7.2f %+7.2f %+7.2f\n"
            "%+7.2f %+7.2f %+7.2f %+7.2f\n"
            "%+7.2f %+7.2f %+7.2f %+7.2f\n", *self.mtxModelView.flatten())
        #"P=\n%sM=\n%s", p, mt @ (mx @ my @ mz))
