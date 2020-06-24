import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
import ctypes
from . import Util

class Attribute:
    """Attribute in a GL shader program."""

    def __init__(self, program, index):
        self.program = program
        self.ctx     = program.ctx
        self.index   = index

        # this should work exactly the same as glGetActiveUniform,
        # since it has the same signature, right? lol no
        name_length = 30 # no idea
        glNameSize  = (gl.constants.GLsizei)()
        glSize      = (gl.constants.GLint)()
        glType      = (gl.constants.GLenum)()
        glName      = (gl.constants.GLchar * name_length)()

        self.ctx.glGetActiveAttrib(program.id, self.index, name_length,
            glNameSize, glSize, glType, glName)

        self.name = glName.value.decode('ascii')
        self.size, self.type = glSize.value, glType.value

        self.id = self.ctx.glGetAttribLocation(program.id, self.name)
        #log.debug("Attribute '%s': idx:%d ID:%d type:%s size:%d",
        #    self.name, self.index, self.id,
        #    self.ctx.constantName(self.type), self.size)


    def bindBuffer(self, buffer, type, size, normalized=False, stride=0, offset=0, convert=False, divisor=None):
        """Bind a Buffer to this attribute.

        buffer: Buffer to bind.
        type: Data type.
        size: Number of elements per vertex (1, 2, 3, 4).
        normalized: Whether fixed-point values should be normalized.
        stride: Bytes between elements.
        offset: Offset within buffer.
        convert: Whether to convert to fixed point.

        A vertex array must be bound before calling this! Otherwise,
        it raises GL_INVALID_OPERATION.
        """

        dSize = Util.typeSize[self.type]
        with buffer.bound():
            # for an array (size > 1) each element is its own index.
            for i in range(self.size):
                offsetP = ctypes.c_void_p(offset + (i * dSize))
                self.ctx.glEnableVertexAttribArray(self.index + i)
                # XXX this is ugly
                if type in (gl.GL_BYTE, gl.GL_UNSIGNED_BYTE, gl.GL_SHORT, gl.GL_UNSIGNED_SHORT, gl.GL_INT, gl.GL_UNSIGNED_INT) and not convert:
                    self.ctx.glVertexAttribIPointer(self.index + i,
                        size, type, stride, offsetP)
                else: self.ctx.glVertexAttribPointer(self.index + i,
                    size, type, normalized, stride, offsetP)
                if divisor is not None:
                    self.ctx.glVertexAttribDivisor(self.index+i, divisor)
