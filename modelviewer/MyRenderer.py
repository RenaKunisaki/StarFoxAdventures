import logging; log = logging.getLogger(__name__)
import struct
import sys
import gl
import numpy as np
from app import Renderer
from programs.DebugText import DebugText
#from programs.LineDraw import LineDraw
from programs.SfaModel import SfaModelViewer


class MyRenderer(Renderer):
    def setup(self):
        """Called when we're ready to initialize our scene."""
        try:
            self.bgColor  = (0.0, 0.5, 0.5, 0.0) # r, g, b, a
            self.programs = {
                #'test': TestProgram(self.ctx),
                'debugText': DebugText(self.ctx),
                'sfa': SfaModelViewer(self.ctx),
            }

            log.debug("Init OK")
        except:
            log.exception("Error in setup")
            sys.exit(1)


    def draw(self):
        """Called when we need to redraw the scene."""
        #log.debug("Drawing")
        self.programs['debugText'].reset()
        self.ctx.clear(color=self.bgColor, depth=100)
        #self.programs['test'].run()
        self.programs['sfa'].run()
        #self.programs['debugText'].printf("Howdy thar")
        self.programs['debugText'].run()

        #log.debug("Draw OK")

    # these are set up in MainWindow

    def _dispatchEvent(self, name, event):
        for prg in self.programs.values():
            f = getattr(prg, 'on_'+name, None)
            if f is not None: f(event)

    def on_button_press(self, widget, event):
        self._dispatchEvent('button_press', event)

    def on_button_release(self, widget, event):
        self._dispatchEvent('button_release', event)

    def on_motion_notify(self, widget, event):
        self._dispatchEvent('motion_notify', event)

    def on_key_press(self, widget, event):
        self._dispatchEvent('key_press', event)

    def on_key_release(self, widget, event):
        self._dispatchEvent('key_release', event)
