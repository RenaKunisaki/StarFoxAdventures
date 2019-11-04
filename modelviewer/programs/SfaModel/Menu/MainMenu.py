import logging; log = logging.getLogger(__name__)
from .Menu import Menu


class MainMenu(Menu):
    """The main menu."""
    title = "Display Lists"

    def __init__(self, parent):
        super().__init__(parent)
        self.refresh()


    def refresh(self):
        self.items = ["All Lists"]
        for i, dlist in enumerate(self.parent.dlists):
            self.items.append("%3d" % i)
        self.cursorPos = 0


    def render(self):
        super().render()

        selList = self.cursorPos - 1
        if selList >= 0:
            dlist = self.parent.dlists[selList]
            log.dprint("\nDlist %d: %d polys", selList, len(dlist.polys))


    def _onChange(self):
        """Handle change in "Browse Display Lists" menu."""
        renderer = self.parent.dlistRenderer
        sel = self.cursorPos
        for i in range(1, len(self.items)):
            param = renderer.getRenderParam(i-1)
            if sel == i or sel == 0: param.enableFill = True
            else: param.enableFill = False
