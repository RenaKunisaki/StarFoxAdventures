import logging; log = logging.getLogger(__name__)
import gi
gi.require_version('Gtk', '3.0'); from gi.repository import Gtk, Gdk, GLib
import time
import sys
import gl


class GLArea(Gtk.GLArea):
    """A GTK widget that displays GL rendering results."""
    def __init__(self, renderer=None):
        self.ctx = None
        super().__init__()
        self._lastDrawTime     = 0
        self._peakDrawTime     = 0
        self.renderer = renderer
        self.connect("realize", self.on_realize)
        self.connect("render",  self.on_render)
        self.connect('configure-event', self.on_configure_event)
        self._onSetup = None

        self.set_required_version(4, 0)
        #self.set_debug_enabled(True)
        self.set_use_es(0) # 1=yes 0=no -1=auto
        self.set_has_depth_buffer(True)
        #ctx.realize()
        #ctx.make_current()


    def _setupGlContext(self):
        """Set up GL context."""
        self.ctx = gl.Context(self)
        #self.debugText = DebugText(self.ctx)
        self._updateViewport()
        if self._onSetup is not None:
            self._onSetup(self)


    def _updateViewport(self):
        """Called when widget is created or resized, to update the
        GL viewport to match its dimensions.
        """
        rect = self.get_allocation()
        pos  = self.translate_coordinates(self.get_toplevel(), 0, 0)
        if pos is None: return # widget is not visible

        self.ctx.viewport = (pos[0], pos[1], rect.width, rect.height)
        log.debug("Viewport: %d x %d @ (%d, %d)",
            rect.width, rect.height, pos[0], pos[1])


    def on_realize(self, area) -> None:
        """Called when the widget is being realized (ie initialized).

        area: The widget itself.
        """
        try:
            ctx = self.get_context()
            log.debug("Realized ctx %r", ctx)
            self._gtk_ctx = ctx
            self._gtk_ctx.make_current()

            self._setupGlContext()
            if self.renderer:
                self.renderer._setWidget(self)
                self.renderer.setup()
            else: log.warning("GLArea: no renderer")
        except:
            log.exception("Error in widget.on_realize: %s", self.get_error())


    def on_configure_event(self, widget, event):
        """Callback for GTK configure-event on widget."""
        if self.ctx is None: return
        self._gtk_ctx.make_current()
        self._updateViewport()


    def on_render(self, area, ctx) -> bool:
        """Called when the widget needs to be redrawn.

        area: The widget itself.
        ctx:  The GL context.

        Returns True to stop other handlers from being invoked
        for the event, or False to propagate the event further.
        """
        try:
            if not self.ctx: return False # something went wrong
            self._gtk_ctx.make_current()
            tStart = time.perf_counter()

            #query = self.ctx.query(samples=True, any_samples=False,
            #    time=True, primitives=True)
            #with query:
            #    self.lineDraw.run()

            if self.renderer:
                try:
                    self.renderer.draw()
                except:
                    log.exception("Error in renderer.draw")
                    sys.exit(1)
            else:
                log.warning("no renderer")

            #err = self.ctx.error
            #if err is None: err = ''
            #else: err = "\x1B[38;2;255;0;0m%s\x1B[0m" % err
            #self.debugText.printf(
            #    "Draw time: %4dms (peak %4dms); s:%4dms\n"
            #    "Samples: %6d  Prims: %4d\n"
            #    "\n"
            #    "Buffers (MB)   CPU  GPU\n"
            #    "Lines         %4d %4d",
            #    self._lastDrawTime * 1000, # secs -> msecs
            #    self._peakDrawTime * 1000,
            #    query.elapsed / 1000, # nsecs -> msecs
            #    query.samples, query.primitives, #err,
            #    self.lineDraw.memUsedCpu / 1048576, # bytes -> Mbytes
            #    self.lineDraw.memUsedGpu / 1048576) # (1024 ** 2)
            #self.debugText.run()

            self.ctx.glFinish() # wait for finish, not really needed
            #err = self.ctx.error
            #if err is not None and err != "GL_NO_ERROR":
            #    print("GL ERROR:", err)

            # performance counters update now but display on next draw
            self._lastDrawTime = time.perf_counter() - tStart
            self._peakDrawTime = max(self._peakDrawTime, self._lastDrawTime)

            return True
        except:
            log.exception("Error in widget.on_draw")
            sys.exit(1)
