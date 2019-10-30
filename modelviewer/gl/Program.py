import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
import ctypes
import numpy as np
import re
from importlib import resources
from .Attribute import Attribute
from .Uniform import makeUniform
from .UniformBlock import UniformBlock


class Program:
    """Base class for shader programs.

    You should use `Context.Program()` instead of
    instantiating this class.
    """

    # vtx -> tesselate -> geom -> fragment
    shader_types = {
        'fragment_shader':        gl.GL_FRAGMENT_SHADER,
        'vertex_shader':          gl.GL_VERTEX_SHADER,
        'geometry_shader':        gl.GL_GEOMETRY_SHADER,
        'tess_evaluation_shader': gl.GL_TESS_EVALUATION_SHADER,
        'tess_control_shader':    gl.GL_TESS_CONTROL_SHADER,
    }

    def __init__(self, ctx, separable=False, **files):
        self.ctx = ctx
        self.id  = ctx.glCreateProgram()

        if separable or getattr(self, 'separable', False):
            self.ctx.glProgramParameteri(self.id,
                self.ctx.GL_PROGRAM_SEPARABLE, True)

        # every program should have its own vertex array object,
        # since that stores important state info.
        self.vao = ctx.VertexArray()
        self._shaders      = {}
        self.attribs       = {}
        self.uniforms      = {}
        self.uniformBlocks = {}

        if len(files) > 0:
            self._loadShaders(**files)
            self._link()
        else: # we can also specify these as attributes of subclasses.
            shaders = {}
            for typ in self.shader_types.keys():
                path = getattr(self, typ, None)
                if path is not None:
                    shaders[typ] = path
            if len(shaders) > 0:
                self._loadShaders(**shaders)
                self._link()


    def __del__(self):
        for obj in self._shaders.values():
            self.ctx.glDeleteShader(obj)
        self.ctx.glDeleteProgram(self.id)


    def __enter__(self):
        self.bind()
        return self

    def __exit__(self, type, value, traceback):
        self.unbind()


    def bind(self):
        self.vao.bind()
        self.ctx.glUseProgram(self.id)

    def unbind(self):
        self.ctx.glUseProgram(0)
        self.vao.unbind()


    @property
    def memUsedCpu(self):
        """Estimated amount of CPU-side memory used."""
        return None # amount not known

    @property
    def memUsedGpu(self):
        """Estimated amount of GPU-side memory used."""
        return None # amount not known


    def run(self, *args, **kwargs):
        """Execute the program."""
        raise NotImplementedError


    def setUniforms(self, **values):
        """Set the values of multiple uniform variables."""
        self.bind()
        for name, val in values.items():
            self.uniforms[name].value = val


    def _loadShaders(self, **files):
        """Load shader sources and attach to current program.

        files: paths to shader code files.
        """
        for name, path in files.items():
            shd = self._compileShader(name, path)
            log.debug("Bind shader %d to program %d", shd, self.id)
            self.ctx.glAttachShader(self.id, shd)


    def _getShaderCodeFromFile(self, path):
        """Load shader code from file path.

        Exists so that programs can override it to provide a base path.
        """
        with open(path, 'rt') as file:
            return file.read()


    def _getShaderCode(self, path, forcePath=False):
        """Load shader code from a path.

        path: A file path beginning with '@',
            or a tuple of (package, name) to use importlib.resources,
            or the code itself.
        forcePath: Whether to interpret `path` as a file path even if it
            doesn't begin with '@'. Used for '#include' function since
            including a string isn't very useful.
        """
        if type(path) is tuple:
            code = resources.read_text(*path)
        elif path.startswith('@'):
            code = self._getShaderCodeFromFile(path[1:])
        elif forcePath: # for #include
            code = self._getShaderCodeFromFile(path)
        else:
            code = path

        return self._preprocessShader(code)


    def _compileShader(self, name, path):
        if name not in self.shader_types:
            raise KeyError("Unknown shader type '%s'" % str(name))

        code = self._getShaderCode(path)
        shd = self.ctx.glCreateShader(self.shader_types[name])
        self.ctx.glShaderSource(shd, code)
        self.ctx.glCompileShader(shd)

        r = self.ctx.glGetShaderiv(shd, gl.GL_COMPILE_STATUS)
        if not r:
            raise RuntimeError("Error compiling %s:\n%s" % (
                name, self.ctx.glGetShaderInfoLog(shd).decode('utf-8')))

        self._shaders[name] = shd
        return shd


    def _preprocessShader(self, code):
        """Preprocess shader code, as a hack since we don't have
        glNamedStringARB."""
        rx = r'^\#include\s+"([^"]+)"'
        result = []
        while True:
            match = re.search(rx, code, re.MULTILINE)
            if match is None: break
            st, en, path = match.start(), match.end(), match.group(1)
            result.append(code[0:st])
            result.append("/*** include(%s) ***/\n" % path)
            result.append(self._getShaderCode(path, forcePath=True))
            result.append("\n/*** end include(%s) ***/\n" % path)
            code = code[en:]
        result.append(code)
        result = ''.join(result)
        #if result != code:
        #    log.debug("*** preprocessed code:\n%s", result)
        return result



    def _link(self):
        self.ctx.glLinkProgram(self.id)

        # check if linked OK
        stat = self.ctx.glGetProgramiv(self.id, gl.GL_LINK_STATUS)
        #log.debug("link status = %r", stat)
        if not stat:
            r = self.ctx.glGetProgramInfoLog(self.id).decode('utf-8')
            raise RuntimeError("Program linking failed:\n%s" % r)

        self.ctx.glUseProgram(self.id)
        self._getVariables()


    def _getVariables(self):
        """Get the attribues and uniforms."""
        nAttrs = self.ctx.glGetProgramiv(self.id, gl.GL_ACTIVE_ATTRIBUTES)
        log.debug("Program has %d attrs", nAttrs)
        for i in range(nAttrs):
            a = Attribute(self, i)
            self.attribs[a.name] = a

        nUniforms = self.ctx.glGetProgramiv(self.id, gl.GL_ACTIVE_UNIFORMS)
        log.debug("Program has %d uniforms", nUniforms)
        for i in range(nUniforms):
            u = makeUniform(self, i)
            self.uniforms[u.name] = u

        nBlocks = self.ctx.glGetProgramiv(self.id, gl.GL_ACTIVE_UNIFORM_BLOCKS)
        log.debug("Program has %d uniform blocks", nBlocks)
        for i in range(nBlocks):
            u = UniformBlock(self, i)
            self.uniformBlocks[u.name] = u
