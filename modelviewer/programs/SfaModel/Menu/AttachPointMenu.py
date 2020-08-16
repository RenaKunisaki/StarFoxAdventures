import logging; log = logging.getLogger(__name__)
from .Menu import Menu

class AttachPointMenu(Menu):
    """A menu for examining an attach point."""

    def __init__(self, parent):
        super().__init__(parent)
        self.title = "Attach Points"
        self.refresh()


    def refresh(self):
        if self.parent.model is None:
            self.items = ["No Model"]
            self.cursorPos = 0
            return

        self.items = [" #    Pos                     Rot            Bone"]
        for i, point in enumerate(self.parent.attachPoints):
            self.items.append("%02X %+7.2f %+7.2f %+7.2f %5d %5d %5d %02X" % (i, *point))
        self.cursorPos = 0


    #def activate(self):
    #    selPoly = self.cursorPos - 1
    #    if selPoly >= 0:
    #        poly = self.dlist.polys[selPoly]
    #        menu = PolyMenu(self.parent, poly,
    #            "Display List %d Poly %d: %s" % (poly['list'], selPoly,
    #            self.drawModes[poly['mode']],
    #        ))
    #        self.parent.enterMenu(menu)


    def render(self):
        super().render()

        #selPoly = self.cursorPos - 1
        #if selPoly >= 0:
        #    poly = self.dlist.polys[selPoly]
        #    log.dprint("\x1B[16,400HPoly %d: %s, %d vtxs", selPoly,
        #        self.drawModes[poly['mode']],
        #        len(poly['vtxs']))


    def _onChange(self):
        sel = self.cursorPos - 1
        self.parent.highlightedAttachPoint = sel
