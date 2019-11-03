import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import gl
import numpy as np
from . import Model
from .DlistParser import DlistParser


class ModelLoader:
    """Load a model extracted from SFA."""

    def __init__(self):
        self.model       = None
        self._vtxFmt     = {'pos':0, 'nrm':0, 'col':0, 'tex':0}
        self.curMaterial = None
        self.mtxLut      = {}
        self.dlists      = []


    def loadFromFile(self, file):
        self.model = Model(file)
        self._doRenderInstrs()
        return self.model


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
        parser.setMtxLut(self.mtxLut)
        dlist = parser.parse()
        log.debug("Dlist %d has %d polys", idx, len(parser.polys))
        self.dlists.append(dlist)
