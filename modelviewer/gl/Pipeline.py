import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
import ctypes
import numpy as np

class Pipeline:
    """A shader pipeline that combines several separate programs."""

    program_types = {
        'fragment_shader':        gl.GL_FRAGMENT_SHADER_BIT,
        'vertex_shader':          gl.GL_VERTEX_SHADER_BIT,
        'geometry_shader':        gl.GL_GEOMETRY_SHADER_BIT,
        'tess_evaluation_shader': gl.GL_TESS_EVALUATION_SHADER_BIT,
        'tess_control_shader':    gl.GL_TESS_CONTROL_SHADER_BIT,
    }

    def __init__(self, ctx, **programs):
        self.ctx = ctx
        self.id  = self.ctx.glGenProgramPipelines(1)

        self.programs = {}
        bits = {}
        for name, prg in programs.items():
            if name not in self.program_types:
                raise KeyError("Unknown shader type '%s'" % str(name))
            self.programs[name] = prg
            bits[prg] = bits.get(prg, 0) | self.program_types[name]

        for prg, val in bits.items():
            self.ctx.glUseProgramStages(self.id, val, prg.id)


    def __enter__(self):
        self.bind()
        return self

    def __exit__(self, type, value, traceback):
        self.unbind()


    def bind(self):
        self.ctx.glBindProgramPipeline(self.id)

    def unbind(self):
        self.ctx.glBindProgramPipeline(0)
