import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import gl
import numpy as np
from . import shaders
from .SfaProgram import SfaProgram
from .EventHandler import EventHandler
from .Parser import Model
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

        # used during model loading
        self._vtxFmt       = {'pos':0, 'nrm':0, 'col':0, 'tex':0}
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
        self.curMaterial   = None
        self.mtxLut        = {}

        self.dlistRenderer.setLut('matrix', self.mtxLut)

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
            self.model = Model(file)
            self.dlistRenderer.setLut('vtx', self.model.vtxs)
            self.dlistRenderer.setLut('texCoord', self.model.texCoords)
            self._doRenderInstrs()

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


    def _doRenderInstrs(self):
        """Follow previously parsed render instructions for loaded model."""
        for instr in self.model.renderInstrs:
            op = instr[0]
            if op == 'TEX':
                log.debug("Set material %d", instr[1])
                self.curMaterial = self.model.materials[instr[1]]

            elif op == 'CALL':
                self._callDlist(instr[1])

            elif op == 'VFMT':
                self._vtxFmt['pos'] = instr[1]
                self._vtxFmt['nrm'] = instr[2]
                self._vtxFmt['col'] = instr[3]
                self._vtxFmt['tex'] = instr[4]
                log.debug("Set vtx fmt: %s", self._vtxFmt)

            elif op == 'MTX':
                # not sure why the game does this
                tbl = (0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 0, 0,
                      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 0)
                mtxs = instr[1]
                for i, mtx in enumerate(mtxs):
                    n = tbl[i]
                    self.mtxLut[n] = self.model.xlates[mtx]

            else:
                log.error("BUG: RenderInstrParser generated unknown opcode %s", op)
                raise RuntimeError("RenderInstrParser failure")


    def _callDlist(self, idx):
        parser = DlistParser(self.model, idx)

        attrs = {
            # XXX col and tex apply to all enabled attrs
            'POS':  self._vtxFmt['pos'],
            'NRM':  self._vtxFmt['nrm'],
            'COL0': self._vtxFmt['col'],
            #'COL1' : self._vtxFmt['col'],
            'TEX0': self._vtxFmt['tex'],
        }

        if self.curMaterial:
            log.debug("Material Texture ID is %d, %d", *self.curMaterial.textureId)
        else:
            log.debug("No material")
        if self.curMaterial and self.curMaterial.textureId[0] >= 0:
            attrs['TEX1'] = self._vtxFmt['tex']

        if idx == 24: # HACK
            attrs['PNMTXIDX'] = 1
            attrs['T0MIDX'] = 1
            attrs['COL0'] = 1
            attrs['COL1'] = 1

        parser.setVtxFmt(6, **attrs) # XXX always 6?
        parser.setMaterial(self.curMaterial)
        dlist = parser.parse()
        log.debug("Dlist %d has %d polys", idx, len(parser.polys))
        self.dlistRenderer.addList(dlist)


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
