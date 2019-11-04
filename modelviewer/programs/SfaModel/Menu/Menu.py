import logging; log = logging.getLogger(__name__)

class Menu:
    """Base class for UI menus."""
    title = ""
    items = []


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
        for i, item in enumerate(self.items):
            log.dprint("%s%s", '>' if i == self.cursorPos else ' ', item)
