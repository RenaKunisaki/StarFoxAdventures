import logging; log = logging.getLogger(__name__)
from gi.repository import Gdk


class EventHandler:
    """Mixin class for handling UI events."""

    def on_button_press(self, event):
        self._mouseButtons[event.button] = True
        # track the starting point for relative movement.
        # this will break if you press multiple buttons at once
        # but oh well
        self._mouseOrigin = [event.x, event.y]
        self._rotOrigin = self._rotate.copy()
        self._trnOrigin = self._translate.copy()


    def on_button_release(self, event):
        self._mouseButtons[event.button] = False


    def on_motion_notify(self, event):
        dx = event.x - self._mouseOrigin[0]
        dy = event.y - self._mouseOrigin[1]

        if self._mouseButtons.get(3, False):
            # right: move
            self._translate[0] = self._trnOrigin[0] + ( dx / 10)
            self._translate[1] = self._trnOrigin[1] + (-dy / 10)

        elif self._mouseButtons.get(1, False):
            # left: rotate
            self._rotate[0] = self._rotOrigin[0] + (dy / 10)
            self._rotate[1] = self._rotOrigin[1] - (dx / 10)

        elif self._mouseButtons.get(2, False):
            # middle: rotate Z
            # doesn't work because I guess my laptop doesn't
            # generate a press event for this button? only release.
            self._rotate[2] = self._rotOrigin[2] + (dy / 10)


    def on_key_press(self, event):
        key = Gdk.keyval_name(event.keyval)

        #print(key)
        if   key == 'Up':        self.menu.moveCursor( -1)
        elif key == 'Down':      self.menu.moveCursor(  1)
        elif key == 'Page_Up':   self.menu.moveCursor(-10)
        elif key == 'Page_Down': self.menu.moveCursor( 10)
        elif key == 'Return':    self.menu.activate()
        elif key == 'BackSpace': self.exitMenu()

        elif key == 'space': # reset
            self._translate = self._initT.copy()
            self._rotate    = self._initR.copy()

        # w/s: move forward/backward
        elif key == 'w': self._translate[2] += 10.0
        elif key == 's': self._translate[2] -= 10.0
        # a/d: move left/right
        elif key == 'a': self._translate[0] += 10.0
        elif key == 'd': self._translate[0] -= 10.0
        # q/e: rotate Z
        elif key == 'q': self._rotate[2] -= 10
        elif key == 'e': self._rotate[2] += 10
