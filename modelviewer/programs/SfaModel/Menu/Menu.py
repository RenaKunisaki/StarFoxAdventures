import logging; log = logging.getLogger(__name__)

class Menu:
    """Base class for UI menus."""
    title = ""
    items = []
    itemsPerPage = 40


    def __init__(self, parent):
        self.parent = parent
        self.cursorPos = 0


    def _onChange(self):
        pass


    def moveCursor(self, amount):
        self.cursorPos += amount
        if self.cursorPos < 0: self.cursorPos = len(self.items) - 1
        if self.cursorPos >= len(self.items): self.cursorPos = 0
        self._onChange()


    def activate(self):
        pass


    def render(self):
        log.dprint("\n  [%s]", self.title)
        start = max(0, self.cursorPos - (self.itemsPerPage // 2))
        if len(self.items) <= self.itemsPerPage: start = 0

        for i in range(start, start+self.itemsPerPage):
            if i >= len(self.items): break
            item = self.items[i]
            log.dprint("%s%s", '>' if i == self.cursorPos else ' ', item)
