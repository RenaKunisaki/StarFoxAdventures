import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
import ctypes

class UniformBlock:
    """Uniform block object in a GL shader program."""

    def __init__(self, program, index):
        self.program = program
        self.ctx     = program.ctx
        self.index   = index
        self._buffer = None

        self.size = self.ctx.glGetActiveUniformBlockiv(
            program.id, self.index, gl.GL_UNIFORM_BLOCK_DATA_SIZE)
        self.numUniforms = self.ctx.glGetActiveUniformBlockiv(
            program.id, self.index, gl.GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS)
        self.numIdxs = self.ctx.glGetActiveUniformBlockiv(
            program.id, self.index, gl.GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES)

        name_length = 30 # no idea
        glNameSize  = (gl.constants.GLsizei)()
        glName      = (gl.constants.GLchar * name_length)()

        self.ctx.glGetActiveUniformBlockName(program.id, self.index, name_length, glNameSize, glName)
        self.name = glName.value.decode('ascii')

        log.debug("UniformBlock '%s': idx:%d size:%d uniforms:%d idxs:%d",
            self.name, self.index, self.size, self.numUniforms, self.numIdxs)


    def bindBuffer(self, buffer):
        log.debug("Bind buffer %d to block %d in program %d",
            buffer.id, self.index, self.program.id)
        self.ctx.glUniformBlockBinding(self.program.id,
            self.index, self.index)
        self.ctx.glBindBufferBase(self.ctx.GL_UNIFORM_BUFFER,
            self.index, buffer.id)
        # the third parameter of glUniformBlockBinding, and the
        # second of glBindBufferBase, is a binding point index.
        # as far as I can tell, these can be arbitrary,
        # so you can bind the same buffer to multiple blocks.
        # for simplicity, I'll just use the block's index.
        # the second parameter of glUniformBlockBinding must be
        # the block index, it's not a binding point.
