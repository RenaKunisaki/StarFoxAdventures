import logging; log = logging.getLogger(__name__)


class Page:
    title = ""
    items = []

    def __init__(self, title, *items):
        self.title = title
        self.items = list(items) # make mutable
        self.cursorPos = 0


class Menu:
    """Handles the UI menus."""

    pages = {
        'dlists': Page("Browse Display Lists",
            "..."),

        'test': Page("Other Page", "Foo", "Bar"),
    }
    pageOrder = ('dlists', 'test',)


    def __init__(self, parent):
        self.parent = parent
        self.curPageIdx = 0
        self.changePage(0)


    def changePage(self, amount):
        self.curPageIdx += amount
        if self.curPageIdx < 0: self.curPageIdx = len(self.pageOrder) - 1
        if self.curPageIdx >= len(self.pageOrder): self.curPageIdx = 0
        idx = self.pageOrder[self.curPageIdx]
        self.curPage = self.pages[idx]
        if idx == 'dlists': self._onDlistChange()


    def moveCursor(self, amount):
        page = self.curPage
        page.cursorPos += amount
        if page.cursorPos < 0: page.cursorPos = len(page.items) - 1
        if page.cursorPos >= len(page.items): page.cursorPos = 0

        idx = self.pageOrder[self.curPageIdx]
        if idx == 'dlists': self._onDlistChange()


    def activate(self):
        page = self.curPage


    def render(self):
        page = self.curPage
        log.dprint("\n  [%s]", page.title)
        for i, item in enumerate(page.items):
            log.dprint("%s%s", '>' if i == page.cursorPos else ' ', item)


    def _onDlistChange(self):
        """Handle change in "Browse Display Lists" menu."""
        renderer = self.parent.dlistRenderer
        sel = self.curPage.cursorPos
        for i in range(1, len(self.curPage.items)):
            param = renderer.getRenderParam(i-1)
            if sel == i or sel == 0: param.enableFill = True
            else: param.enableFill = False
