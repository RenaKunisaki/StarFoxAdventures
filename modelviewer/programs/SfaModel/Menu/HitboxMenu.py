import logging; log = logging.getLogger(__name__)
from .Menu import Menu

class HitboxMenu(Menu):
    """A menu for examining a hitbox."""

    def __init__(self, parent):
        super().__init__(parent)
        self.title = "Hitboxes"
        self.refresh()


    def refresh(self):
        if self.parent.model is None:
            self.items = ["No Model"]
            self.cursorPos = 0
            return

        self.items = ["Box Bone 02  14   1617 Radius  X      Y      Z"]
        for i, box in enumerate(self.parent.model.hitboxes):
            self.items.append("%3d %04X %04X %04X %02X%02X %+7.2f %+7.2f %+7.2f %+7.2f" % (
                i, box.bone,
                box.unk02, box.unk14, box.unk16, box.unk17, box.radius,
                box.pos[0], box.pos[1], box.pos[2],
                ))
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
        self.parent.highlightedHitbox = sel
