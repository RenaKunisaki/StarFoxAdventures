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

        selVts = self.cursorPos
        #poly = self.dlist.polys[selPoly]
        #log.dprint("\x1B[16,400HPoly %d: %s, %d vtxs", selPoly,
        #    self.drawModes[poly['mode']],
        #    len(poly['vtxs']))
        #for i, vtx in enumerate(poly['vtxs']):
        #    log.dprint("%d", i)


    def _onChange(self):
        pass
