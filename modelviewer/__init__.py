#!/usr/bin/env python
import logger; logger.setup("python-glsl")
log = logger.logging.getLogger(__name__)

import gi
gi.require_version('Gtk', '3.0'); from gi.repository import Gtk, GLib



from app import MainWindow
from MyRenderer import MyRenderer


win = MainWindow(MyRenderer())

def redraw():
    win.queue_draw()
    return True
GLib.timeout_add(100, redraw)

Gtk.main() # won't return until quit event (ie window is closed)
print("Bye!")
