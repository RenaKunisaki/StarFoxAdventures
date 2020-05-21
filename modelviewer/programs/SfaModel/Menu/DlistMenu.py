import logging; log = logging.getLogger(__name__)
from .Menu import Menu
from .PolyMenu import PolyMenu

#self.polys.append({
#    'vat':  vat,
#    'mode': mode,
#    'vtxs': vtxs,
#    'list': self.listIdx,
#    'offs': offs,
#    'idx':  len(self.polys),
#    'shader': self.shader,
#})

class DlistMenu(Menu):
    """A menu for examining a display list."""

    drawModes = ('quads', 'quads2', 'tris', 'triStrip',
        'triFan', 'lines', 'lineStrip', 'points')

    def __init__(self, parent, dlist, title):
        super().__init__(parent)
        self.dlist = dlist
        self.title = title
        self.refresh()


    def refresh(self):
        self.items = ["Poly Offset DrawMode  Vtxs Shd VAT"]
        for i, poly in enumerate(self.dlist.polys):
            self.items.append("%4d %06X %-9s %4d %3d %d" % (i,
                poly['offs'], self.drawModes[poly['mode']],
                len(poly['vtxs']),
                poly['shader']._idx, poly['vat']))
        self.cursorPos = 0


    def activate(self):
        selPoly = self.cursorPos - 1
        if selPoly >= 0:
            poly = self.dlist.polys[selPoly]
            menu = PolyMenu(self.parent, poly,
                "Display List %d Poly %d: %s" % (poly['list'], selPoly,
                self.drawModes[poly['mode']],
            ))
            self.parent.enterMenu(menu)


    def render(self):
        super().render()

        #selPoly = self.cursorPos - 1
        #if selPoly >= 0:
        #    poly = self.dlist.polys[selPoly]
        #    log.dprint("\x1B[16,400HPoly %d: %s, %d vtxs", selPoly,
        #        self.drawModes[poly['mode']],
        #        len(poly['vtxs']))


    def _onChange(self):
        param = self.parent.dlistRenderer.getRenderParam(self.dlist.listIdx)
        param.polyIdx = self.cursorPos - 1
