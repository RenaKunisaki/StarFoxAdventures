import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
import math

class Buffer:
    """GL memory buffer."""

    class _Binding:
        """Object returned by Buffer.bound()."""

        def __init__(self, buffer, target):
            self.buffer = buffer
            self.target = target

        def __enter__(self):
            self.buffer.bind(self.target)
            return self.buffer

        def __exit__(self, type, value, traceback):
            self.buffer.unbind(self.target)


    def __init__(self, ctx, size_or_data, dynamic=False, target=gl.GL_ARRAY_BUFFER):
        """Create a buffer.

        ctx: GL context.
        size_or_data: Number of bytes to allocate (int), or data to
            populate with (bytes).
        dynamic: If True, use GL_DYNAMIC_DRAW, else GL_STATIC_DRAW.
        target: Default binding target.
        """
        self.ctx    = ctx
        self.id     = ctx.glGenBuffers(1)
        self.target = target

        # allocate the buffer
        if type(size_or_data) is int:
            # just create it with undefined contents
            self.size = size_or_data
            data = None

        elif type(size_or_data) in (bytes, bytearray):
            # create it with specified data
            self.size = len(size_or_data)
            data = size_or_data
            if len(data) == 0: raise ValueError("Data cannot be empty")

        else:
            raise TypeError(size_or_data)

        # XXX use glNamedBufferData when possible?
        drawMode = gl.GL_DYNAMIC_DRAW if dynamic else gl.GL_STATIC_DRAW
        self.bind()
        self.ctx.glBufferData(self.target, self.size, data, drawMode)

        log.debug("Created buffer %d, size %d", self.id, self.size)


    def bind(self, target=None):
        """Bind to GL target object."""
        if target is None: target = self.target
        self.ctx.glBindBuffer(target, self.id)

    def unbind(self, target=None):
        """Unbind from GL target object."""
        if target is None: target = self.target
        self.ctx.glBindBuffer(target, 0)


    def bound(self, target=None):
        """Create a temporary binding: `with myBuffer.bound() ...`"""
        if target is None: target = self.target
        return self._Binding(self, target)


    def write(self, data, offset=0, length=None):
        """Write into the buffer.

        data: Bytes to write.
        offset: Offset (in bytes) to write to.
        length: Number of bytes to write. Default is all of `data`.
        """
        if length is None: length = len(data)
        if length < 1: return
        self.bind()
        # we need to cast to bytes, because bytearray gives weird.
        self.ctx.glBufferSubData(self.target, offset, length, bytes(data))


    def read(self, offset=0, size=None):
        """Read from the buffer.

        offset: offset to read from.
        size: number of bytes to read. None = entire bufer.
        """
        if size is None: size = self.size - offset
        self.bind()
        return self.ctx.glGetBufferSubData(self.target, offset, size)


    def clear(self, data=b'\0', offset=0, size=None):
        """Clear the buffer.

        data: Pattern to fill with.
        offset: Offset to clear.
        size: Number of bytes to clear. None = entire buffer.
        """
        if len(data) == 0: raise ValueError("Data cannot be empty")
        if size is None: size = self.size - offset
        data *= math.ceil(size / len(data))
        self.bind()
        self.ctx.glBufferSubData(self.target, offset, size, data)
