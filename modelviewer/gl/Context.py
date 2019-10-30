import logging; log = logging.getLogger(__name__)
import numpy as np
import OpenGL
import OpenGL.GL as gl
from .Buffer import Buffer
from .Program import Program
from .Sampler import Sampler
from .Texture import Texture
from .VertexArray import VertexArray


def fmt_arg(val):
    """Used for debug messages."""
    s = repr(val)
    if len(s) > 64: s = s[0:64] + '...'
    return "%s %s" % (type(val).__name__, s)


class Context:
    """OpenGL context."""

    def __init__(self, debug=True):
        self._viewport = (0, 0, 640, 480) # x, y, w, h
        self._debug    = debug
        self._makeConstantMap()
        self._setupFrameBuffer()


    def __del__(self):
        #if self.frameBuffer:
        #    self.glDeleteFramebuffers(self.frameBuffer)
        #if self.renderBufferC:
        #    self.glDeleteRenderbuffers(self.renderBufferC)
        #if self.renderBufferD:
        #    self.glDeleteRenderbuffers(self.renderBufferD)
        pass


    def __getattr__(self, name):
        r = getattr(gl, name)
        if callable(r) and self._debug:
            r = self._makeDebugWrapper(r, name)
        setattr(self, name, r)
        return r


    def constantName(self, val):
        """Return the name of a GL enum value."""
        val = int(val)
        try: return self._glConstants[val]
        except KeyError: return "<Unknown constant %r>" % val


    def Buffer(self, *args, **kwargs):
        return Buffer(self, *args, **kwargs)

    def Program(self, **files):
        return Program(self, **files)

    def Sampler(self, *args, **kwargs):
        return Sampler(self, *args, **kwargs)

    def Texture(self, *args, **kwargs):
        return Texture(self, *args, **kwargs)

    def VertexArray(self, *args, **kwargs):
        return VertexArray(self, *args, **kwargs)


    @property
    def error(self):
        """Last error code reported by GL."""
        e = gl.glGetError()
        if e == gl.GL_NO_ERROR: return None
        return self.constantName(e)


    @property
    def viewport(self):
        return self._viewport

    @viewport.setter
    def viewport(self, port): # (x, y, w, h)
        self._viewport = port
        self.glViewport(*self._viewport)


    def clear(self, color=(0.0, 0.0, 0.0, 0.0), depth=0.0):
        """Clear the color and/or depth buffers.

        color: Color (r, g, b, a) to clear to, or None to not clear.
            Each value is in range 0 to 1.
        depth: Value to clear depth buffer to, or None to not clear.
        """
        flag = 0
        if color is not None:
            self.glClearColor(*color)
            flag |= gl.GL_COLOR_BUFFER_BIT
        if depth is not None:
            self.glClearDepth(depth)
            flag |= gl.GL_DEPTH_BUFFER_BIT

        if flag: self.glClear(flag)


    def _makeDebugWrapper(self, func, name):
        """Used by __getattr__ to make a wrapper for a GL function,
        which logs its args and return value and checks for errors.
        """
        def f(*args, **kwargs):
            a = list(map(fmt_arg, args))

            for k, v in kwargs.items():
                a.append('%s=%s' % (k, fmt_arg(v)))
            sig = "%s(%s)" % (name, ', '.join(a))

            try:
                res = func(*args, **kwargs)
            except Exception:
                log.exception("%s => EXC", sig)
                raise

            err = self.error
            if err:
                log.error("%s => ERROR: %s", sig, err)
                raise RuntimeError(err)

            #log.debug("%s => %s", sig, fmt_arg(res))
            return res
        f.__name__ = "_wrapper_"+name
        setattr(self, name, f)
        return f


    def _makeConstantMap(self):
        # map value => constant name
        self._glConstants = {}
        for name in dir(gl):
            val = getattr(gl, name)
            if type(val) is OpenGL.constant.IntConstant: # and name.startswith('GL_'):
                self._glConstants[int(val)] = name


    def _setupFrameBuffer(self):
        buf = self.glGetIntegerv(gl.GL_DRAW_FRAMEBUFFER_BINDING)
        log.debug("Current frame buffer is %r", buf)


    def _createFrameBuffer(self):
        # this isn't used....
        # kept for reference

        # create color and depth buffers
        self.renderBufferC, self.renderBufferD = self.glGenRenderbuffers(2)
        self.glBindRenderbuffer(gl.GL_RENDERBUFFER, self.renderBufferC)
        self.glRenderbufferStorage(gl.GL_RENDERBUFFER, gl.GL_RGBA8UI, 1920, 1080)
        self.glBindRenderbuffer(gl.GL_RENDERBUFFER, self.renderBufferD)
        self.glRenderbufferStorage(gl.GL_RENDERBUFFER, gl.GL_DEPTH_COMPONENT32F, 1920, 1080)

        # create frame buffer and bind to render buffers
        self.frameBuffer = self.glGenFramebuffers(1)
        self.glBindFramebuffer(gl.GL_FRAMEBUFFER, self.frameBuffer)

        self.glFramebufferRenderbuffer(gl.GL_FRAMEBUFFER,
            gl.GL_COLOR_ATTACHMENT0, gl.GL_RENDERBUFFER,
            self.renderBufferC)
        self.glFramebufferRenderbuffer(gl.GL_FRAMEBUFFER,
            gl.GL_DEPTH_ATTACHMENT, gl.GL_RENDERBUFFER,
            self.renderBufferD)

        # check if it worked
        stat = self.glCheckFramebufferStatus(gl.GL_FRAMEBUFFER)
        if stat != gl.GL_FRAMEBUFFER_COMPLETE:
            raise RuntimeError("Error creating GL frame buffer: %s" %
                self.constantName(stat))

        # enable the buffer
        self.glDrawBuffers(1, [gl.GL_COLOR_ATTACHMENT0])
