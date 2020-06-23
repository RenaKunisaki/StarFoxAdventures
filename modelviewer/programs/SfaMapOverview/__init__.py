"""Program to display an overview of the global map grid.

This is incomplete. Mouse coordinate translation doesn't work.
"""
import logging; log = logging.getLogger(__name__)
import gl
import sys
import math
import struct
import numpy as np
from importlib import resources
from . import shaders
from .EventHandler import EventHandler
from .SfaMapProgram import SfaMapProgram
from .MapRenderer import MapRenderer


class FragmentShader(SfaMapProgram):
    """Subprogram for generic fragment shader."""
    separable       = True
    fragment_shader = (shaders, 'generic.frag')


class SfaMapOverview(SfaMapProgram, EventHandler):
    def __init__(self, ctx):
        SfaMapProgram.__init__(self, ctx)
        EventHandler.__init__(self)
        self.ctx   = ctx
        self.frame = 0
        self.fragShader  = FragmentShader(self.ctx)
        self.mapRenderer = MapRenderer(self)

        self.mapRenderer.loadMapData(sys.argv[1])

        # enable our local logger to print on the screen
        # by using log.dprint()
        log.setLevel('DPRINT')


    def run(self):
        log.dprint("Frame %d", self.frame)
        self._setMtxs()
        self.mapRenderer.run()
        self.frame += 1


    def _setMtxs(self):
        # update projection matrix to current viewport
        x, y, width, height = self.ctx.viewport
        a = width/height
        rx, ry, rz = self._rotate
        tx, ty, tz = self._translate
        mx=gl.Util.Matrix.rotateX(math.radians(rx))
        my=gl.Util.Matrix.rotateY(math.radians(ry))
        mz=gl.Util.Matrix.rotateZ(math.radians(rz))
        mt=gl.Util.Matrix.translate(tx, ty, 0)
        #mp=gl.Util.Matrix.perspective(60, a, 0.1, 1000)
        mv = (mx @ my @ mz) # modelview = all rotations
        #mj = mt @ mp # projection = perspective and translation
        #self.mapRenderer.setMtxs(mj, mv)
        self.mapRenderer.setMtxs(
            gl.Util.Matrix.scale(tz/width, tz/height) @
            gl.Util.Matrix.translate(tx, ty, 0),
            mv)
        # biggest layer size: 63x86

        # debug print
        log.dprint("t=%+2.1f, %+2.1f, %+2.1f",
            self._translate[0], self._translate[1], self._translate[2])
        #log.dprint("MR=%d, %d", *self._mousePosition)

        # unproject mouse
        scale = tz * self.mapRenderer.BOX_SIZE
        sx, sy = 63, 86
        view = self.ctx.viewport
        log.dprint("V=%d, %d, %d, %d, a=%f", *view, a)
        wx, wy = self._mousePosition[0], self._mousePosition[1]
        log.dprint("MR=%d, %d", wx, wy)

        # this gives us (-1..1, -1..1) with (0,0) at screen center
        wHalf, hHalf = width/2, height/2
        mouse = (
            ((wx - wHalf) / wHalf) * sx,
            ((wy - hHalf) / hHalf) * sy,
        )
        log.dprint("MP=%+7.2f, %+7.2f", *mouse)

        try:
            cell = self.mapRenderer.layer[0].get(int(mouse[0]), int(mouse[1]))
        except:
            cell = None
        if cell is not None:
            map   = cell['map']
            block = cell['block']
            log.dprint("Map %02X Block %d.%d", map['map'], block['mod'], block['sub'])
        else: log.dprint("no map")
