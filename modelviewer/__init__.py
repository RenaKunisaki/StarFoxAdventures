#!/usr/bin/env python
import logger; logger.setup("python-glsl")
log = logger.logging.getLogger(__name__)

import gi
try: gi.require_version('Gtk', '3.0')
except AttributeError:
    raise RuntimeError("python-gobject is not installed")
from gi.repository import Gtk, GLib

from app import MainWindow
from MyRenderer import MyRenderer

win = MainWindow(MyRenderer())

def redraw():
    win.queue_draw()
    return True
GLib.timeout_add(100, redraw)

Gtk.main() # won't return until quit event (ie window is closed)
print("Bye!")
