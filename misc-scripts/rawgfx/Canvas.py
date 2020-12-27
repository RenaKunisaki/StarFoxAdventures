import gi
gi.require_version('Gtk', '3.0')
from gi.repository import GObject, Gtk, Gdk, GLib, GdkPixbuf
from DrawingArea import DrawingArea

class Rect:
    def __init__(self, x, y, w, h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h

class Color:
    def __init__(self, r, g, b, a):
        self.r = r
        self.g = g
        self.b = b
        self.a = a


class Canvas(DrawingArea):
    def __init__(self, parent):
        super().__init__()
        self.pixbuf = None
        self.parent = parent
        self.connect('draw', self._on_draw)


    def reread(self):
        """Re-read the pixel data."""
        width   = self.parent.width
        height  = int(self.height / self.parent.scale)
        nPixels = width * height
        nBytes  = nPixels * self.parent.format.bits * 8
        if height < 1: return

        #buf = bytearray(width * height)
        self.parent.file.seek(self.parent.offset)
        data = self.parent.file.read(nBytes)
        if len(data) < nBytes: # pad with zeros
            data += bytes(nBytes - len(data))
        self.pixbuf = self.parent.format.pixbufFromData(data, width, height)
        self.redraw()


    def drawImage(self, image, width, height, scale=1):
        self.cairo.scale(scale, scale)
        height = int(height / scale)
        Gdk.cairo_set_source_pixbuf(self.cairo, image, 0, 0)
        self.draw_rect(Rect(0, 0, width, height))
        self.cairo.fill()


    def _on_draw(self, widget, cr):
        #self.cairo = cr
        #self.clear((0, 0, 0))
        if self.pixbuf is None:
            self.reread()
            # reread triggers a redraw, so avoid looping
            return False # don't eat event
        self.drawImage(self.pixbuf, self.parent.width,
            self.height, self.parent.scale)

        return False # don't eat event
