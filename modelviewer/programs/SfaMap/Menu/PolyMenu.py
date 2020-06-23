import logging; log = logging.getLogger(__name__)
from .Menu import Menu


class PolyMenu(Menu):
    """A menu for examining a polygon."""

    vtxFields = (
        'PNMTXIDX', 'T0MIDX', 'T1MIDX', 'T2MIDX', 'T3MIDX', 'T4MIDX',
        'T5MIDX', 'T6MIDX', 'T7MIDX', 'POS', 'NRM', 'COL0', 'COL1',
        'TEX0', 'TEX1', 'TEX2', 'TEX3', 'TEX4', 'TEX5', 'TEX6',
        'TEX7')

    def __init__(self, parent, poly, title):
        super().__init__(parent)
        self.poly  = poly
        self.title = title
        self.refresh()


    def _fmtIdx(self, vtx, name):
        name += '_idx'
        if name not in vtx: return "----"
        return '%04X' % vtx[name]


    def refresh(self):
        self.items = ["Vtx  Offset Pos# Tex# Mtx# Nrm#"]
        for i, vtx in enumerate(self.poly['vtxs']):
            self.items.append("%4d %06X %s %s %s %s" % (i,
                vtx['offs'],
                self._fmtIdx(vtx, 'POS'),
                self._fmtIdx(vtx, 'TEX0'),
                self._fmtIdx(vtx, 'PNMTXIDX'),
                self._fmtIdx(vtx, 'NRM'),
            ))
        self.cursorPos = 0


    def render(self):
        super().render()

        maxIdx = len(self.poly['vtxs']) - 4
        idx = max(0, min(self.cursorPos-1, maxIdx))
        vtxs = self.poly['vtxs'][idx: idx+3]

        log.dprint("\x1B[450,500H  S    T    X    Y    Z ")
        for i, v in enumerate(vtxs):
            log.dprint("\x1B[%d,500H%04X %04X %04X %04X %04X",
                450 + ((i+1)*12),
                int(v['TEX0'][0] * 1024) & 0xFFFF,
                int(v['TEX0'][1] * 1024) & 0xFFFF,
                int(v['RAWPOS'].x *  256) & 0xFFFF,
                int(v['RAWPOS'].y *  256) & 0xFFFF,
                int(v['RAWPOS'].z *  256) & 0xFFFF,
            )


    def _onChange(self):
        renderer = self.parent.textureRenderer
        renderer.reset()
        maxIdx = len(self.poly['vtxs']) - 4
        idx  = max(0, min(self.cursorPos-1, maxIdx))
        vtxs = self.poly['vtxs'][idx: idx+3]

        vtx = self.poly['vtxs'][max(0, self.cursorPos-1)]
        x, y = vtx['TEX0']
        dx, dy = 0.001, 0.002
        xa, xb, xc, xd = (0, y-dy), (1, y-dy), (1, y+dy), (0, y+dy)
        ya, yb, yc, yd = (x-dx, 0), (x-dx, 1), (x+dx, 1), (x+dx, 0)

        renderer.addTri(xa, xb, xc, (1, 0, 0, 0.5))
        renderer.addTri(xb, xc, xd, (1, 0, 0, 0.5))
        renderer.addTri(ya, yb, yc, (0, 1, 0, 0.5))
        renderer.addTri(yb, yc, yd, (0, 1, 0, 0.5))
        renderer.addTri(vtxs[0]['TEX0'], vtxs[1]['TEX0'], vtxs[2]['TEX0'],
            (1, 1, 1, 1),
        )

        x, y, z = vtx['POS'][0:3]
        self.parent.boxRenderer.reset()
        self.parent.boxRenderer.addBox(
            (x-0.25, y-0.25, z-0.25),
            (x+0.25, y+0.25, z+0.25),
            (1.0, 1.0, 1.0, 1.0)
        )
