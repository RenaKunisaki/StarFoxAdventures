import gi
gi.require_version('Gtk', '3.0')
from gi.repository import GObject, Gtk, Gdk, GLib, GdkPixbuf

class DrawingArea(Gtk.DrawingArea):
    def __init__(self):
        super().__init__()
        self.cairo  = None
        self.width  = 0
        self.height = 0
        self.connect('draw', self._on_draw_get_cairo)


    def redraw(self):
        self.queue_draw()


    def draw_rect(self, rect, fill=None, outline=None):
        """Draw a rectangle.

        x, y: Where to draw.
        w, h: Size, in pixels.
        fill: Color to fill with; None for no fill.
        outline: Color to outline with; None for no outline.
        """
        self.cairo.rectangle(rect.x, rect.y, rect.w, rect.h)
        self._fillAndStroke(fill, outline)


    def clear(self, color):
        """Fill the drawing area with the given color."""
        self._setColor(color)
        self.cairo.paint()

    def _fillAndStroke(self, fill=None, stroke=None):
        if fill:
            self._setColor(fill)
            if stroke: self.cairo.fill_preserve()
            else: self.cairo.fill()

        if stroke:
            self._setColor(stroke)
            self.cairo.stroke()


    def _setColor(self, c):
        if not isinstance(c, Color): c = Color(c)
        self.cairo.set_source_rgba(c.r, c.g, c.b, c.a)

    def _on_draw_get_cairo(self, widget, cr):
        """Get the Cairo context and other info on redraw."""
        self.cairo  = cr
        self.width  = self.get_allocated_width()
        self.height = self.get_allocated_height()
        return False # don't eat event
