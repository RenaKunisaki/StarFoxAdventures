import OpenGL.GL as gl
import struct
from . import Matrix, Data

SIZEOF_FLOAT = struct.calcsize('f')
SIZEOF_INT   = struct.calcsize('i')

# XXX there must be a way to get this from PyOpenGL
typeFmt = {
    gl.GL_BYTE:                'b',
    gl.GL_UNSIGNED_BYTE:       'B',
    gl.GL_SHORT:               'h',
    gl.GL_UNSIGNED_SHORT:      'H',
    gl.GL_INT:                 'i',
    gl.GL_INT_VEC2:            '2i',
    gl.GL_INT_VEC3:            '3i',
    gl.GL_INT_VEC4:            '4i',
    gl.GL_UNSIGNED_INT:        'I',
    gl.GL_UNSIGNED_INT_VEC2:   '2I',
    gl.GL_UNSIGNED_INT_VEC3:   '3I',
    gl.GL_UNSIGNED_INT_VEC4:   '4I',
    gl.GL_FLOAT:               'f',
    gl.GL_FLOAT_VEC2:          '2f',
    gl.GL_FLOAT_VEC3:          '3f',
    gl.GL_FLOAT_VEC4:          '4f',
    int:                       'i',
    float:                     'f',
}
typeSize = {}
for typ, fmt in typeFmt.items():
    typeSize[typ] = struct.calcsize(fmt)
