import gl
from importlib import resources
from . import shaders
from .. import Common

class SfaMapProgram(gl.Program):
    """Base for Programs used by this renderer."""
    separable = True

    def __init__(self, ctx):
        super().__init__(ctx)
        #self._translate    = [-1, -2, -300]
        self._translate    = [-0.1, -0.3, 20]
        self._rotate       = [0, 0, 0]
        self._initT        = self._translate.copy()
        self._initR        = self._rotate.copy()


    def _getShaderCodeFromFile(self, path):
        # this lets us specify where to #include from.
        try: return resources.read_text(shaders, path)
        except FileNotFoundError: return resources.read_text(Common, path)


    def setMtxs(self, projection, modelView):
        # used by geometry shaders
        with self:
            self.uniforms['matProjection'].value = projection
            self.uniforms['matModelview'].value  = modelView
