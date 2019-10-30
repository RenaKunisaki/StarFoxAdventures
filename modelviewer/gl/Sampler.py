import logging; log = logging.getLogger(__name__)
import OpenGL.GL as gl


class Sampler:
    """GL texture sampler object."""

    _props = {
        'wrap_s':       gl.GL_TEXTURE_WRAP_S,
        'wrap_t':       gl.GL_TEXTURE_WRAP_T,
        'wrap_r':       gl.GL_TEXTURE_WRAP_R,
        'min_filter':   gl.GL_TEXTURE_MIN_FILTER,
        'mag_filter':   gl.GL_TEXTURE_MAG_FILTER,
        'min_lod':      gl.GL_TEXTURE_MIN_LOD,
        'max_lod':      gl.GL_TEXTURE_MAX_LOD,
        'lod_bias':     gl.GL_TEXTURE_LOD_BIAS,
        'compare_mode': gl.GL_TEXTURE_COMPARE_MODE,
        'compare_func': gl.GL_TEXTURE_COMPARE_FUNC,
    }

    def __init__(self, ctx):
        self.ctx = ctx
        self.id  = ctx.glGenSamplers(1)


    def __getattr__(self, name):
        if name == '_props': return object.__getattr__(self, name)
        if name in self._props:
            return self.ctx.glGetSamplerParameteriv(self.id, self._props[name])
        raise AttributeError(name)


    def __setattr__(self, name, val):
        if name in self._props:
            return self.ctx.glSetSamplerParameteri(self.id,
                self._props[name], val)
        else: object.__setattr__(self, name, val)
