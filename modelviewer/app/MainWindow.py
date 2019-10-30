import logging; log = logging.getLogger(__name__)
import gi
gi.require_version('Gtk', '3.0'); from gi.repository import Gtk, Gdk, GLib

from .GLArea import GLArea


class MainWindow(Gtk.Window):
    """Main application window."""

    def __init__(self, renderer=None):
        super().__init__()
        self.connect('delete-event', Gtk.main_quit) # exit when window closed

        #screen = Gdk.Screen.get_default()
        #visual = Gdk.Screen.get_rgba_visual(screen)
        #self.window = Gtk.Window()
        #Gtk.Widget.set_visual(self.window, visual)

        def setup(glArea):
            # we don't actually use the glArea...
            r = renderer
            win = self.get_window()
            win.set_events(win.get_events() |
                Gdk.EventMask.BUTTON_PRESS_MASK |
                Gdk.EventMask.BUTTON_RELEASE_MASK |
                Gdk.EventMask.POINTER_MOTION_MASK)
            # XXX don't assume renderer has these (or add them to base)
            self.connect('button-press-event',   r.on_button_press)
            self.connect('button-release-event', r.on_button_release)
            self.connect('motion-notify-event',  r.on_motion_notify)
            self.connect('key-press-event',      r.on_key_press)
            self.connect('key-release-event',    r.on_key_release)

        self.glArea = GLArea(renderer)
        self.glArea._onSetup = setup
        self.add(self.glArea)

        # trigger a redraw after the window is visible
        def redraw():
            self.queue_draw()
            return False # don't call again
        GLib.timeout_add(500, redraw)

        self.set_default_size(1473, 900) # arbitrary default size
        self.show_all()
