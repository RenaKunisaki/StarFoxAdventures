import logging; log = logging.getLogger(__name__)

class Renderer:
    """Base class for renderer.

    Your program should inherit from this.
    """

    def _setWidget(self, widget):
        self.widget = widget
        self.ctx = widget.ctx


    def setup(self):
        """Called when GL is being set up."""
        pass


    def draw(self):
        """Called when scene needs to be redrawn."""
        pass
