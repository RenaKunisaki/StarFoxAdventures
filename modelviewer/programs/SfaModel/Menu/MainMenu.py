import logging; log = logging.getLogger(__name__)
from .Menu import Menu
from .DlistMenu import DlistMenu

def dump(d):
    return ' '.join(map(lambda b: '%02X'%b, d))


class MainMenu(Menu):
    """The main menu."""
    title = "Display Lists"

    def __init__(self, parent):
        super().__init__(parent)
        self.refresh()


    def refresh(self):
        self.items = ["List Offset Polys Sh Spec Unk"]
        for i, dlist in enumerate(self.parent.dlists):
            bbox = dlist.list.bbox
            sc = 1024
            bb = "%+7.2f %+7.2f %+7.2f  %+7.2f %+7.2f %+7.2f" % (
                bbox[0]/sc, bbox[1]/sc, bbox[2]/sc,
                bbox[3]/sc, bbox[4]/sc, bbox[5]/sc,
            )
            self.items.append(
                "%4d %06X %5d %02X %04X %02X %04X %08X" % (
                    i, dlist.offset, len(dlist.polys),
                    #bb,
                    dlist.list.shaderId, dlist.list.specialBitAddr,
                    dlist.list.unk12, dlist.list.unk16, dlist.list.unk18,
            ))
        self.cursorPos = 0

    def render(self):
        super().render()

        #selList = self.cursorPos - 1
        #if selList >= 0:
        #    dlist = self.parent.dlists[selList]
        #    log.dprint("\nDlist %d: %d polys", selList, len(dlist.polys))


    def activate(self):
        selList = self.cursorPos - 1
        if selList >= 0:
            dlist = self.parent.dlists[selList]
            menu = DlistMenu(self.parent, dlist,
                "Display List %d" % selList)
            self.parent.enterMenu(menu)


    def _onChange(self):
        renderer = self.parent.dlistRenderer
        sel = self.cursorPos
        for i in range(1, len(self.items)):
            param = renderer.getRenderParam(i-1)
            if sel == i or sel == 0: param.enableFill = True
            else: param.enableFill = False
