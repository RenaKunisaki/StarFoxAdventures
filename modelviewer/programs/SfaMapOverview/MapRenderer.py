import logging; log = logging.getLogger(__name__)
import gl
import struct
from importlib import resources
from .EventHandler import EventHandler
from .SfaMapProgram import SfaMapProgram
from .MapReader import MapReader
from . import shaders
from .. import Common


class MapShader(SfaMapProgram):
    separable = True
    vertex_shader   = (shaders, 'map.vert')
    geometry_shader = (shaders, 'map.geom')


class MapRenderer(gl.Pipeline):
    # layers seem to max out at 64x64, and there are 5 layers
    NUM_LAYERS      = 5
    MAX_BOXES       = 64 * 64 * NUM_LAYERS
    BOX_SIZE        = 1
    vtxBufferFmt    = "4f" # x1, y1, x2, y2
    colorBufferFmt  = "4f" # (r,g,b,a) per box
    zBufferFmt      = "f"  # z per box

    def __init__(self, parent):
        self.parent = parent
        self.shader = MapShader(self.parent.ctx)
        super().__init__(parent.ctx,
            vertex_shader   = self.shader,
            geometry_shader = self.shader,
            fragment_shader = parent.fragShader,
        )

        self.enabledLayers = [False, False, True, False, False]
        self._layerBoxes   = []
        self._nBoxes       = 0
        self._vtxSize      = struct.calcsize(self.vtxBufferFmt)
        self._colorSize    = struct.calcsize(self.colorBufferFmt)
        self._zSize        = struct.calcsize(self.zBufferFmt)

        self.bufVtxs   = self.ctx.Buffer(self._vtxSize   * self.MAX_BOXES, True)
        self.bufColors = self.ctx.Buffer(self._colorSize * self.MAX_BOXES, True)
        self.bufZ      = self.ctx.Buffer(self._zSize     * self.MAX_BOXES, True)


    def _mapIdToColor(self, id):
        n = id + 1
        id = 0
        for i in range(8): id |= ((n >> i) & 1) << i
        r = ( id       & 3) / 3
        g = ((id >> 2) & 7) / 7
        b = ((id >> 5) & 3) / 3
        return r, g, b


    def loadMapData(self, discroot:str):
        reader = MapReader(discroot)
        reader.run()
        self.layer = reader.layer
        for i, layer in reader.layer.items():
            bStart = self._nBoxes
            for y in range(layer.yMin, layer.yMax+1):
                for x in range(layer.xMin, layer.xMax+1):
                    cell = layer.get(x, y)
                    if cell is not None:
                        map   = cell['map']
                        block = cell['block']
                        if block['mod'] == 0xFF: a = 0.25
                        else: a = 1
                        r, g, b = self._mapIdToColor(map['map'])
                        self.addBox(x, y, 0, (r, g, b, a))
                    else:
                        self.addBox(x, y, 0, (0, 0, 0, 0.25))

            # add warps
            for iWarp, warp in enumerate(reader.warps):
                if warp['layer'] == i:
                    x, z = warp['x'] / 640, warp['z'] / 640
                    self.addBox(x, z, 0, (255, 0, 0, 255))

            # add origin
            self.addBox(0, 0, 0, (255, 255, 255, 128))

            bEnd = self._nBoxes
            self._layerBoxes.append((bStart, bEnd))


    def addBox(self, x, y, z, col):
        if self._nBoxes >= self.MAX_BOXES: return

        vtxOffs = self._nBoxes * self._vtxSize
        colOffs = self._nBoxes * self._colorSize
        zOffs   = self._nBoxes * self._zSize

        vtxData = struct.pack(self.vtxBufferFmt,
            x * self.BOX_SIZE, y * self.BOX_SIZE,
            (x+1) * self.BOX_SIZE, (y+1) * self.BOX_SIZE)
        colData = struct.pack(self.colorBufferFmt, *col)
        zData   = struct.pack(self.zBufferFmt, z)
        self.bufVtxs  .write(vtxData, vtxOffs)
        self.bufColors.write(colData, colOffs)
        self.bufZ     .write(zData,   zOffs)

        self._nBoxes += 1


    def run(self):
        self.ctx.glDisable(self.ctx.GL_DEPTH_TEST)
        self.ctx.glDisable(self.ctx.GL_CULL_FACE)
        self.ctx.glEnable(self.ctx.GL_BLEND)
        self.ctx.glBlendFunc(self.ctx.GL_SRC_ALPHA,
            self.ctx.GL_ONE_MINUS_SRC_ALPHA)
        with self:
            self._bindBuffers()
            for i, ly in enumerate(self._layerBoxes):
                cnt = ly[1] - ly[0]
                if self.enabledLayers[i]:
                    self.shader.vao.render(self.ctx.GL_POINTS,
                        count=cnt, offset=ly[0])
                log.dprint("\x1B[%d,8H[%d] Layer %2d: %s", (i*11)+128,
                    i+1, i-2, "On" if self.enabledLayers[i] else "Off")


    def setMtxs(self, projection, modelView):
        self.shader.setMtxs(projection, modelView)


    def _bindBuffers(self):
        with self.shader as shader:
            # in vec2 corner1
            shader.attribs['corner1'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 2,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in vec2 corner2
            shader.attribs['corner2'].bindBuffer(
                self.bufVtxs, self.ctx.GL_FLOAT, 2,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=2 * gl.Util.SIZEOF_FLOAT)

            # in vec4 color
            shader.attribs['color'].bindBuffer(
                self.bufColors, self.ctx.GL_FLOAT, 4,
                stride=4 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)

            # in float z
            shader.attribs['z'].bindBuffer(
                self.bufZ, self.ctx.GL_FLOAT, 1,
                stride=1 * gl.Util.SIZEOF_FLOAT,
                offset=0 * gl.Util.SIZEOF_FLOAT)
