import logging; log = logging.getLogger(__name__)
from .Menu import Menu

class BoneMenu(Menu):
    """A menu for examining a bone."""

    def __init__(self, parent):
        super().__init__(parent)
        self.title = "Bones"
        self.refresh()


    def refresh(self):
        if self.parent.model is None:
            self.items = ["No Model"]
            self.cursorPos = 0
            return

        self.items = ["Bone Prnt 01 02 03 Head                       Tail"]
        for i, bone in enumerate(self.parent.model.bones):
            self.items.append("%4d %4d %02X %02X %02X %+7.3f %+7.3f %+7.3f %+7.3f %+7.3f %+7.3f" % (i,
                bone.parent, bone.unk01, bone.unk02, bone.unk03,
                bone.head.x, bone.head.y, bone.head.z,
                bone.tail.x, bone.tail.y, bone.tail.z))
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
        self.parent.boneRenderer.highlightBone(sel)
