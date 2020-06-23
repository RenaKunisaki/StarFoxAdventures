import logging; log = logging.getLogger(__name__)
from .Menu import Menu
from .DlistMenu import DlistMenu

def dump(d):
    return ' '.join(map(lambda b: '%02X'%b, d))


class MainMenu(Menu):
    """The main menu."""
    title = "Objects"

    def __init__(self, parent):
        super().__init__(parent)
        self.refresh()


    def refresh(self):
        self.items = ["Obj# Type UniqueID ObjName Pos"]
        for i, obj in enumerate(self.parent.curMap.objects):
            self.items.append(
                "%04X %04X %08X %11s %+7.2f %+7.2f %+7.2f" % (i,
                    obj.type, obj.uniqueId, obj.name,
                    obj.pos.x, obj.pos.y, obj.pos.z,
            ))
        self.cursorPos = 0

    def render(self):
        super().render()

        #selList = self.cursorPos - 1
        #if selList >= 0:
        #    dlist = self.parent.dlists[selList]
        #    log.dprint("\nDlist %d: %d polys", selList, len(dlist.polys))


    def activate(self):
        if self.cursorPos > 0:
            obj = self.parent.curMap.objects[self.cursorPos - 1]
            self.parent._translate[0] = obj.pos.x
            self.parent._translate[1] = obj.pos.y + 2
            self.parent._translate[2] = obj.pos.z


    def _onChange(self):
        pass
        #renderer = self.parent.dlistRenderer
        #sel = self.cursorPos
        #for i in range(1, len(self.items)):
        #    param = renderer.getRenderParam(i-1)
        #    if sel == i or sel == 0: param.enableFill = True
        #    else: param.enableFill = False
