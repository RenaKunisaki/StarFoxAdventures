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

        log.dprint("\x1B[330,330H  S    T    X    Y    Z ")
        for i, v in enumerate(vtxs):
            log.dprint("\x1B[%d,330H%04X %04X %04X %04X %04X",
                330 + ((i+1)*12),
                int(v['TEX0'][0] * 1024) & 0xFFFF,
                int(v['TEX0'][1] * 1024) & 0xFFFF,
                int(v['POS'] [0] *  256) & 0xFFFF,
                int(v['POS'] [1] *  256) & 0xFFFF,
                int(v['POS'] [2] *  256) & 0xFFFF,
            )


    def _onChange(self):
        renderer = self.parent.textureRenderer
        renderer.reset()
        maxIdx = len(self.poly['vtxs']) - 4
        idx = max(0, min(self.cursorPos-1, maxIdx))
        vtxs = self.poly['vtxs'][idx: idx+3]
        renderer.addTri(
            vtxs[0]['TEX0'],
            vtxs[1]['TEX0'],
            vtxs[2]['TEX0'],
            (1, 1, 1, 0.5),
        )
