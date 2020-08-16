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
from .LineRenderer import LineRenderer
from programs.Common.BoxRenderer import BoxRenderer
from programs.Common.SphereRenderer import SphereRenderer
from .DlistRenderer import DlistRenderer
from .TextureRenderer import TextureRenderer
from .Menu import Menu, MainMenu, BoneMenu, HitboxMenu, AttachPointMenu
from common.BinaryFile import BinaryFile


class FragmentShader(SfaProgram):
    """Subprogram for generic fragment shader."""
    separable       = True
    fragment_shader = (shaders, 'program.frag')

class BoxShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'box.vert')
    geometry_shader = (shaders, 'box.geom')

class SphereShader(SfaProgram):
    separable       = True
    vertex_shader   = (shaders, 'sphere.vert')
    geometry_shader = (shaders, 'sphere.geom')
    #fragment_shader = (shaders, 'sphere.frag')


class SfaModelViewer(SfaProgram, EventHandler):
    """Render extracted SFA models."""
    MAX_VTXS = 1024

    # copied from OBJECTS.bin for now
    attachPoints = (
        #     X        Y        Z  rotX    rotY    rotZ    bone
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x08),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x0E),
        (     0,       0,   1.485, 0x3FFF, 0xDF4B, 0x0000, 0x02),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x02),
        (     0,  44.000, -12.000, 0x0000, 0x0000, 0x0000, 0xFF),
        (     0,       0,   1.980, 0x0000, 0x0000, 0x0000, 0x10),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x1C),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x18),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x06),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x0C),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x11),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x00),
        (-1.944,   0.978,   1.873, 0x0000, 0x0000, 0x0000, 0x02),
        (-0.049,   0.978,   1.873, 0x0000, 0x0000, 0x0000, 0x02),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x0E),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x1A),
        (     0,       0,       0, 0x0000, 0x0000, 0x0000, 0x16),
        (     0,  -0.990,  -3.960, 0x0000, 0x0000, 0x0000, 0x1C),
        (     0,  -0.990,  -3.960, 0x0000, 0x0000, 0x0000, 0x18),
        (     0,       0,  -0.990, 0x0000, 0x0000, 0x0000, 0x11),
    )

    def __init__(self, ctx):
        super().__init__(ctx)
        self.model = None
        self.showHitboxes = True
        self.highlightedHitbox = -1
        self.showAttachPoints = True
        self.highlightedAttachPoint = -1
        self.showBones = True

        # setup shaders and subprograms
        self.fragShader      = FragmentShader(self.ctx)
        self.boneRenderer    = BoneRenderer(self)
        self.lineRenderer    = LineRenderer(self)
        self.boxRenderer     = BoxRenderer(self, BoxShader(self.ctx))
        self.sphereRenderer  = SphereRenderer(self, SphereShader(self.ctx))
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
        #self.menu          = MainMenu(self)
        #self.menu          = BoneMenu(self)
        #self.menu          = HitboxMenu(self)
        self.menu          = AttachPointMenu(self)
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

        log.debug("%d hitboxes", len(self.model.hitboxes))
        self._firstHitboxIdx = self.boxRenderer._nBoxes
        for i, box in enumerate(self.model.hitboxes):
            log.debug("%02X|%04X|%04X|%+7.4f|%+7.4f|%+7.4f|%+7.4f|%04X|%02X|%02X|",
                i, box.bone, box.unk02, box.radius,
                box.pos[0], box.pos[1], box.pos[2],
                box.unk14, box.unk16, box.unk17)

        log.debug("%d bones", len(self.model.bones))
        for i, bone in enumerate(self.model.bones):
            log.debug("%02X|%02X|%02X|%02X|%02X|%+7.4f %+7.4f %+7.4f|%+7.4f %+7.4f %+7.4f|",
                i, bone.parent, bone.unk01, bone.unk02, bone.unk03,
                bone.head.x, bone.head.y, bone.head.z,
                bone.tail.x, bone.tail.y, bone.tail.z)


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
        self.boxRenderer.reset()
        self.lineRenderer.reset()
        self.sphereRenderer.reset()

        if self.showHitboxes: self._renderHitboxes()
        if self.showAttachPoints: self._renderAttachPoints()
        self.dlistRenderer.run()
        if self.showBones: self.boneRenderer.run()
        self.lineRenderer.run()
        self.boxRenderer.run()
        self.sphereRenderer.run()
        #self.textureRenderer.run() # draw texture on screen
        log.dprint('\n\n') # space out menu from keys
        self.menu.render()

        lines = (
            "[1] Face Culling: %s" % ("On" if self.dlistRenderer.useFaceCulling else "Off"),
            "[2] Wireframe: %s" % ("On" if self.dlistRenderer.forceWireFrame else "Off"),
            "[3] Bone X-ray: %s" % (
                "Off" if self.boneRenderer.useDepthTest else "On"),
            "[4] Bones: %s" % ("On" if self.showBones else "Off"),
            "[5] Hit Spheres: %s" % ("On" if self.showHitboxes else "Off"),
            "[6] Attach Points: %s" % ("On" if self.showAttachPoints else "Off"),
        )
        for i, line in enumerate(lines):
            log.dprint("\x1B[%d,200H%s", (i*11)+5, line)

        self.frame += 1


    def _renderHitboxes(self):
        self._firstHitboxIdx = self.sphereRenderer._nSpheres
        for i, box in enumerate(self.model.hitboxes):
            x, y, z = box.pos
            radius  = box.radius
            bone = self.model.bones[box.bone]
            head, tail = self.model.calcBonePos(bone, True)
            x, y, z = x+head[0], y+head[1], z+head[2]

            sc = 1.0
            if i == self.highlightedHitbox and self.frame & 1 == 0:
                r, g, b, a, sc = 1.0, 0.0, 0.0, 0.5, 1.1
            elif self.highlightedHitbox == -1:
                r, g, b, a = box.unk14, box.unk16 / 0x14, box.unk17 / 0x14, 0.2
            else: r, g, b, a = 0, 0, 0, 0.2
            #else: continue

            self.sphereRenderer.addSphere(
                (x, y, z), radius*sc,
                (r, g, b, a),
            )
            #self.boxRenderer.addBox(
            #    (x-radius, y-radius, z-radius),
            #    (x+radius, y+radius, z+radius),
            #    (r, g, b, a),
            #)

    def _renderAttachPoints(self):
        # these are in OBJECTS.bin, and I've just copied them into
        # this script for testing.
        for i, point in enumerate(self.attachPoints):
            x, y, z, rx, ry, rz, bIdx = point
            if bIdx != 0xFF:
                bone = self.model.bones[bIdx]
                head, tail = self.model.calcBonePos(bone, True)
                x1, y1, z1 = x+head[0], y+head[1], z+head[2]
                x2, y2, z2 = x+tail[0], y+tail[1], z+tail[2]
            if i == self.highlightedAttachPoint and self.frame & 1 == 0:
                r, g, b, a, sc = 1.0, 0.0, 0.0, 0.5, 1.1
            elif self.highlightedAttachPoint == -1:
                r, g, b, a, sc = 1.0, 0.0, 0.0, 0.5, 1.0
            else:
                r, g, b, a, sc = 0.0, 0.0, 0.0, 0.2, 1.0
            #self.sphereRenderer.addSphere((x1, y1, z1), sc, (r, g, b, a))

            rx  = (rx / 65536) * math.pi * 2
            ry  = (ry / 65536) * math.pi * 2
            rz  = (rz / 65536) * math.pi * 2
            mrx = gl.Util.Matrix.rotateX(rx)
            mry = gl.Util.Matrix.rotateY(ry)
            mrz = gl.Util.Matrix.rotateZ(rz)
            mr  = mrx @ mry @ mrz
            #s   = 0.1 * sc
            v1  = gl.Util.Matrix.translate(x1, y1, z1) @ mr
            v2  = gl.Util.Matrix.translate(x2, y2, z2) @ mr
            #log.dprint("%2d %+7.2f %+7.2f %+7.2f %+7.2f\n   %+7.2f %+7.2f %+7.2f %+7.2f\n   %+7.2f %+7.2f %+7.2f %+7.2f\n   %+7.2f %+7.2f %+7.2f %+7.2f",
            #    i, *v1[0], *v1[1], *v1[2], *v1[3])
            self.lineRenderer.addLine(
                v1[3][0:3], v2[3][0:3],
                (r, g, b, a), (1, 1, 1, 1),
            )



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
        self.lineRenderer   .setMtxs(mp, mv)
        self.boxRenderer    .setMtxs(mp, mv)
        self.sphereRenderer .setMtxs(mp, mv)
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
