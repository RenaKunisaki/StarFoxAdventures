import logging; log = logging.getLogger(__name__)
from importlib import resources
import struct
import math
import sys
import gl
import numpy as np
from .SfaProgram import SfaProgram
from . import shaders
from .DlistRenderer import DlistRenderer

class ModelRenderer:
    """Renders a Model."""

    def __init__(self, program):
        self.program = program
        self.model   = None
        self.pos     = [0, 0, 0]
        self.rot     = [0, 0, 0]
        self.dlistRenderer = DlistRenderer(self.program)


    def setModel(self, model):
        self.model = model
        self.dlistRenderer.setModel(model)
        for list in model.dlists:
            self.dlistRenderer.addList(list)


    def addInstance(self, pos):
        self.dlistRenderer.addInstance(pos)


    def render(self):
        #rx, ry, rz = self.rot
        #t = self.pos
        #mp=self.program.mtxProjection
        #mx=gl.Util.Matrix.rotateX(math.radians(rx))
        #my=gl.Util.Matrix.rotateY(math.radians(ry))
        #mz=gl.Util.Matrix.rotateZ(math.radians(rz))
        #mt=gl.Util.Matrix.translate(t[0], t[1], t[2])
        #mv = mt @ (mz @ my @ mx) # modelview = all rotations
        ##mp = mt @ mp # projection = perspective and translation
        #mv = mv @ self.program.mtxModelView

        #self.dlistRenderer.setMtxs(mp, mv)
        self.dlistRenderer.run()
