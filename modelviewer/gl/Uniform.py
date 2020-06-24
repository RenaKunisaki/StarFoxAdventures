import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl
import ctypes


_types = {
    '?': {'ctype': ctypes.c_bool,  'gltype': 'i'},
    'f': {'ctype': ctypes.c_float, 'gltype': 'f'},
    'i': {'ctype': ctypes.c_int,   'gltype': 'i'},
}


class Uniform:
    """Uniform variable in a GL shader program.

    You should use `makeUniform` instead of instantiating this class.
    """

    def __init__(self, program, index, name, size, type):
        self.program = program
        self.ctx     = program.ctx
        self.index   = index
        self.name    = name.decode('ascii')
        self.size    = size
        self.type    = type
        self.isArray = '[' in self.name

        # it would make entirely too much sense if the name we got
        # was the actual name.
        #lol = self.name.find('[')
        #if lol >= 0: self.name = self.name[0:lol]
        # this still doesn't work for structs/arrays
        self.id = self.ctx.glGetUniformLocation(program.id, name)

        log.debug("Uniform '%s': idx:%d ID:%d type:%s size:%d",
            self.name, self.index, self.id,
            self.ctx.constantName(self.type), self.size)

        self._makeGetter()
        self._makeSetter()


    def _makeGetter(self):
        dtype = _types[self._dtype]
        func = 'glGetUniform%sv' % dtype['gltype']

        _buf = (dtype['ctype'] * self._count)()
        _get = getattr(self.ctx, func)

        if self._count == 1:
            def get():
                _get(self.program.id, self.id, _buf)
                return _buf[0]
        else:
            def get():
                _get(self.program.id, self.id, _buf)
                return _buf
        self._get = get


    def _makeSetter(self):
        dtype = _types[self._dtype]
        gtype = dtype['gltype']
        if self.isArray: gtype += 'v'
        func = 'glUniform%d%s' % (self._count, gtype)
        _set = getattr(self.ctx, func)
        if self.isArray:
            def set(val):
                log.debug("uniform set %s %s %s", self.id, func, val)
                _set(self.id, len(val), val)
        elif self._count == 1:
            def set(val):
                _set(self.id, val)
        else:
            def set(val):
                _set(self.id, *val)
        self._set = set



    @property
    def value(self):
        return self._get()

    @value.setter
    def value(self, val):
        self._set(val)


class UniformInt(Uniform):
    _dtype = 'i'
    _count = 1

class UniformFloat(Uniform):
    _dtype = 'f'
    _count = 1

class UniformBool(Uniform):
    _dtype = '?'
    _count = 1


class UniformVec2i(Uniform):
    _dtype = 'i'
    _count = 2

class UniformVec3i(Uniform):
    _dtype = 'i'
    _count = 3

class UniformVec4i(Uniform):
    _dtype = 'i'
    _count = 4


class UniformVec2f(Uniform):
    _dtype = 'f'
    _count = 2

class UniformVec3f(Uniform):
    _dtype = 'f'
    _count = 3

class UniformVec4f(Uniform):
    _dtype = 'f'
    _count = 4


class UniformVec2bool(Uniform):
    _dtype = '?'
    _count = 2

class UniformVec3bool(Uniform):
    _dtype = '?'
    _count = 3

class UniformVec4bool(Uniform):
    _dtype = '?'
    _count = 4


class UniformMatrix(Uniform):
    def __init__(self, *args, **kwargs):
        self._count = self._rows * self._columns
        super().__init__(*args, **kwargs)

    def _makeSetter(self):
        if self._rows == self._columns: size = self._rows
        else: size = '%dx%d' % (self._columns, self._rows)
        func = 'glUniformMatrix%s%sv' % (size, self._dtype)
        _set = getattr(self.ctx, func)
        def set(val):
            #log.debug("%s(%s, %s)", func, self.name, val.flatten())
            _set(self.id, 1, False, val.flatten())
        self._set = set

class UniformMatrix2f(UniformMatrix): # 2x2
    _dtype   = 'f'
    _rows    = 2
    _columns = 2

class UniformMatrix2x3f(UniformMatrix):
    _dtype   = 'f'
    _rows    = 3
    _columns = 2

class UniformMatrix2x4f(UniformMatrix):
    _dtype   = 'f'
    _rows    = 4
    _columns = 2

class UniformMatrix3x2f(UniformMatrix):
    _dtype   = 'f'
    _rows    = 2
    _columns = 3

class UniformMatrix3f(UniformMatrix): # 3x3
    _dtype   = 'f'
    _rows    = 3
    _columns = 3

class UniformMatrix3x4f(UniformMatrix):
    _dtype   = 'f'
    _rows    = 4
    _columns = 3

class UniformMatrix4x2f(UniformMatrix):
    _dtype   = 'f'
    _rows    = 2
    _columns = 4

class UniformMatrix4x3f(UniformMatrix):
    _dtype   = 'f'
    _rows    = 3
    _columns = 4

class UniformMatrix4f(UniformMatrix): # 4x4
    _dtype   = 'f'
    _rows    = 4
    _columns = 4

class UniformSampler1d(Uniform): # XXX
    _dtype = 'f'
    _count = 1

class UniformSampler2d(Uniform): # XXX
    _dtype = 'f'
    _count = 2

class UniformSampler3d(Uniform): # XXX
    _dtype = 'f'
    _count = 4


_typeMap = {
    gl.GL_INT:          UniformInt,
    gl.GL_INT_VEC2:     UniformVec2i,
    gl.GL_INT_VEC3:     UniformVec3i,
    gl.GL_INT_VEC4:     UniformVec4i,
    gl.GL_FLOAT:        UniformFloat,
    gl.GL_FLOAT_VEC2:   UniformVec2f,
    gl.GL_FLOAT_VEC3:   UniformVec3f,
    gl.GL_FLOAT_VEC4:   UniformVec4f,
    gl.GL_BOOL:         UniformBool,
    gl.GL_BOOL_VEC2:    UniformVec2bool,
    gl.GL_BOOL_VEC3:    UniformVec3bool,
    gl.GL_BOOL_VEC4:    UniformVec4bool,
    gl.GL_FLOAT_MAT2:   UniformMatrix2f,
    gl.GL_FLOAT_MAT2x3: UniformMatrix2x3f,
    gl.GL_FLOAT_MAT2x4: UniformMatrix2x4f,
    gl.GL_FLOAT_MAT3x2: UniformMatrix3x2f,
    gl.GL_FLOAT_MAT3:   UniformMatrix3f,
    gl.GL_FLOAT_MAT3x4: UniformMatrix3x4f,
    gl.GL_FLOAT_MAT4x2: UniformMatrix4x2f,
    gl.GL_FLOAT_MAT4x3: UniformMatrix4x3f,
    gl.GL_FLOAT_MAT4:   UniformMatrix4f,
    gl.GL_SAMPLER_1D:   UniformSampler1d,
    gl.GL_SAMPLER_2D:   UniformSampler2d,
    gl.GL_SAMPLER_3D:   UniformSampler3d,
}


def makeUniform(program, index):
    name, size, type = \
        program.ctx.glGetActiveUniform(program.id, index)
    if type not in _typeMap:
        raise TypeError("Unsupported uniform type %s",
            program.ctx.constantName(type))

    cls = _typeMap[type]
    return cls(program, index, name, size, type)
